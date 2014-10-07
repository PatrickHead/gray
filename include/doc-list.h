/*!
    @file doc-list.h

    @brief Header file for splitting and tagging STDIO data stream

    @timestamp Tue, 07 Oct 2014 04:43:34 +0000

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

    @file doc-list.h

    Header file for routine that splits and tags desired parts of STDIO stream

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

#ifndef DOC_LIST_H
#define DOC_LIST_H

#include "strapp.h"

  /*!
    @brief Document List data structure
  */

typedef struct
{
    /*! @brief Regular Expression for document matching (splitter) */
  char *mpat;
    /*! @brief Number of parts in document list */
  int nlist;
    /*! @brief Parts in document list */
  char **list;
    /*! @brief Regular Expression for marked (keeper) parts */
  char *kpat;
    /*! @brief Number of parts in keeper list */
  int nkeep;
    /*! @brief Keeper list */
  char **keep;
} doc_list_s;

  // Document List API function prototypes

doc_list_s *doc_list_create(FILE *f, char *mpat, char *kpat);
void doc_list_destroy(doc_list_s *dl);
void doc_list_produce(doc_list_s *dl, char *doc);
void doc_list_consume(doc_list_s *dl, char *doc);

#endif // DOC_LIST_H
