/*!
    @file doc-list.c

    @brief Source file for splitting and tagging STDIO data stream routines

    @timestamp Tue, 07 Oct 2014 07:32:14 +0000

    @author Patrick Head  mailto:patrickhead@gmail.com

    @copyright Copyright (C) 2014  Patrick Head

    @license
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.@n
    @n
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.@n
    @n
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

  /*!

    @file doc-list.c

    Source file for routine that splits and tags desired parts of STDIO stream

    A document list (doc-list) can be created from any STDIO stream (FILE *)
    by supplying a single (can be complex) Regular Expression that is used
    as a marker to split the stream into separate parts.  The resulting doc-list
    will contain a list of these text parts.  In addition, a second distinct
    Regular Expression can be supplied to create a list of pointers into the
    doc-list that mark any "interesting" parts.

    For example, a stream may contain many XML documents.  An RE can be
    supplied that will split the stream into individual XML document texts.  In
    addition, a second RE can be supplied that will identify all XML documents
    with an XML element tag name of "customer" or "vendor" for later use by
    a calling program.  This is just an example, as the REs can split and tag
    most any text that can be imagined or needed by the caller.

    Also, utility functions are provided that allow the user to produce new
    parts to the doc-list, and to consume (remove) marked parts from the
    doc-list.

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>
#include <assert.h>

  // Project related headers

#include "doc-list.h"

  // Internal helper functions

static void keeper(doc_list_s *dl, char *doc);
static char *find_doc(doc_list_s *dl, char *doc);

  /*!

     @brief Create doc-list structure

     Creates a doc-list structure, and populates the two contained lists from
     the open STDIO input stream.  The user supplies an open "FILE *" to any
     STDIO file stream, and two regular expressions.   The first RE is used
     to find any matching patterns that are used to mark (or start) a new
     document from the stream.  All text that follows the matching RE is saved
     as a document, up to the next RE match, which in turn starts a new
     document.  The second RE is used to match any documents that are to be
     marked as 'keeper' documents in a second list.

     @param f    "FILE *" of open input stream
     @param mpat "char *" containing string for document matching RE
     @param kpat "char *" containing string for document keeping RE

     @retval "doc_list_s *" success
     @retval NULL failure

  */

doc_list_s *doc_list_create(FILE *f, char *mpat, char *kpat)
{
  doc_list_s *dl = NULL;
  char b[2048];
  char *def_pat = ".*";
  regex_t mre;
  int i;

    // Sanity check parameters.
  assert(f);

    // If reading from a TTY, then do nothing

  if (isatty(fileno(f))) return NULL;
  
    // Set default RE patterns, if not supplied by user
    // default pattern is ".*", which matches everything

  if (!mpat) mpat = def_pat;
  if (!kpat) kpat = def_pat;

    // Create base doc_list_s structure

  dl = (doc_list_s *)malloc(sizeof(doc_list_s));
  memset(dl, 0, sizeof(doc_list_s));

    // Save RE patterns

  dl->mpat = strdup(mpat);
  dl->kpat = strdup(kpat);

    // Compile the mpat RE

  if (regcomp(&mre, dl->mpat, REG_NOSUB)) return NULL;

    // Find and separate documents

  while (fgets(b, 2048, f))
  {
    if (!regexec(&mre, b, 0, NULL, 0))
    {
      ++dl->nlist;
      if (!dl->list)
        dl->list = (char **)malloc(sizeof(char *) * dl->nlist);
      else
        dl->list = (char **)realloc(dl->list, sizeof(char *) * dl->nlist);
      dl->list[dl->nlist-1] = strapp(NULL, "");
    }
    if (dl->list) dl->list[dl->nlist-1] = strapp(dl->list[dl->nlist-1], b);
  }

    // Find the keepers

  for (i = 0; i < dl->nlist; i++)
    keeper(dl, dl->list[i]);

    // Clean up compiled mpat

  regfree(&mre);

    // Return "doc_list_s *"

  return dl;
}

  /*!

     @brief Free up all allocated memory associated with doc-list

     Frees doc-list structure and all associated allocated memory.

     @param dl    "doc_list_s *"

     @retval NONE

  */

void doc_list_destroy(doc_list_s *dl)
{
  int i;

    // Sanity check parameters.
  assert(dl);

  if (dl->list)
  {
    for (i = 0; i < dl->nlist; i++)
      free(dl->list[i]);
    free(dl->list);
  }

  if (dl->mpat) free(dl->mpat);
  if (dl->kpat) free(dl->kpat);

  free(dl);
}

  /*!

     @brief Add a document to doc-list

     Adds a new document to an existing doc-list

     @param dl    "doc_list_s *" pointer to existing document list
     @param doc   "char *" pointer to new document to add to list

     @retval NULL

  */

void doc_list_produce(doc_list_s *dl, char *doc)
{
    // Sanity check parameters.
  assert(dl);
  assert(doc);

  ++dl->nlist;
  if (!dl->list)
    dl->list = (char **)malloc(sizeof(char *) * dl->nlist);
  else
    dl->list = (char **)realloc(dl->list, sizeof(char *) * dl->nlist);
  dl->list[dl->nlist-1] = NULL;
  dl->list[dl->nlist-1] = strapp(dl->list[dl->nlist-1], doc);
}

  /*!

     @brief Remove a document from doc-list

     Removes a document from and existing document list

     @param dl    "doc_list_s *" pointer to existing document list
     @param doc   "char *" pointer to document already in list

     @retval NULL

  */

void doc_list_consume(doc_list_s *dl, char *doc)
{
  int i;

    // Sanity check parameters.
  assert(dl);
  assert(doc);

    // is doc in keeper list

  for (i = 0; i < dl->nkeep; i++)
    if (dl->keep[i] == doc) break;
  if (i >= dl->nkeep) return;

    // remove reference from keeper list

  dl->keep[i] = NULL;

    // remove document from main list

  for (i = 0; i < dl->nlist; i++)
  {
    if (dl->list[i] == doc)
    {
      free(dl->list[i]);
      dl->list[i] = NULL;
      break;
    }
  }
}

  /*!

     @brief INTERNAL: Check and mark a document in keeper list

     Checks a document for inclusion in keeper list, and marks it as a keeper
     if it matches the doc-list keeper RE pattern.

     @param dl    "doc_list_s *" pointer to existing document list
     @param doc   "char *" pointer to document already in list

     @retval NULL

  */

static void keeper(doc_list_s *dl, char *doc)
{
  regex_t kre;

    // Sanity check parameters.
  assert(dl);
  assert(doc);

    // Locate the document in list, return if not found
  if (!find_doc(dl, doc)) return;

    // Compile the keeper RE pattern
  if (regcomp(&kre, dl->kpat, REG_NOSUB|REG_EXTENDED)) return;

    // Is document a keeper?  If yes, add it to keeper list
  if (!regexec(&kre, doc, 0, NULL, 0))
  {
    ++dl->nkeep;
    if (!dl->keep)
      dl->keep = (char **)malloc(sizeof(char *) * dl->nkeep);
    else
      dl->keep = (char **)realloc(dl->keep, sizeof(char *) * dl->nkeep);
    dl->keep[dl->nkeep-1] = doc;
  }
}

  /*!

     @brief INTERNAL: Find a document in doc-list

     Locates a document in document list, based on passed document pointer.

     @param dl    "doc_list_s *" pointer to existing document list
     @param doc   "char *" pointer to document to find in list

     @retval "char *"  success
     @retval NULL  failure

  */

static char *find_doc(doc_list_s *dl, char *doc)
{
  int i;

    // Sanity check parameters.
  assert(dl);
  assert(doc);

    // Cycle through all documents in list, return pointer if found in list
  for (i = 0; i < dl->nlist; i++)
    if (dl->list[i] == doc) return doc;

    // Return NULL (not found)
  return NULL;
}

