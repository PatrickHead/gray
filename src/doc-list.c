/*!
    @file doc-list.c

    @brief SOURCE_BRIEF

    @timestamp Mon, 06 Jan 2014 15:17:36 +0000

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

    SOURCE_BRIEF

    SOURCE_DETAILS

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

static void keeper(doc_list_s *dl, char *doc);
static char *find_doc(doc_list_s *dl, char *doc);

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

  if (isatty(fileno(f))) return NULL;
  
  if (!mpat) mpat = def_pat;
  if (!kpat) kpat = def_pat;

  dl = (doc_list_s *)malloc(sizeof(doc_list_s));
  memset(dl, 0, sizeof(doc_list_s));

  dl->mpat = strdup(mpat);
  dl->kpat = strdup(kpat);

  if (regcomp(&mre, dl->mpat, REG_NOSUB)) return NULL;

    // find and separate documents

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

    // find the keepers

  for (i = 0; i < dl->nlist; i++)
    keeper(dl, dl->list[i]);

  regfree(&mre);

    // Return RETVAL
  return dl;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

static void keeper(doc_list_s *dl, char *doc)
{
  regex_t kre;

    // Sanity check parameters.
  assert(dl);
  assert(doc);

  if (!find_doc(dl, doc)) return;

  if (regcomp(&kre, dl->kpat, REG_NOSUB|REG_EXTENDED)) return;

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

static char *find_doc(doc_list_s *dl, char *doc)
{
  int i;

    // Sanity check parameters.
  assert(dl);
  assert(doc);

  for (i = 0; i < dl->nlist; i++)
    if (dl->list[i] == doc) return doc;

    // Return RETVAL
  return NULL;
}

