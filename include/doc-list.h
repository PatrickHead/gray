/*!
    @file doc-list.h

    @brief HEADER_BRIEF

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

    @file doc-list.h

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef DOC_LIST_H
#define DOC_LIST_H

#include "strapp.h"

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct
{
    /*! brief ELEMENT_BRIEF */
  char *mpat;
    /*! brief ELEMENT_BRIEF */
  int nlist;
    /*! brief ELEMENT_BRIEF */
  char **list;
    /*! brief ELEMENT_BRIEF */
  char *kpat;
    /*! brief ELEMENT_BRIEF */
  int nkeep;
    /*! brief ELEMENT_BRIEF */
  char **keep;
} doc_list_s;

doc_list_s *doc_list_create(FILE *f, char *mpat, char *kpat);
void doc_list_destroy(doc_list_s *dl);
void doc_list_produce(doc_list_s *dl, char *doc);
void doc_list_consume(doc_list_s *dl, char *doc);

#endif // DOC_LIST_H
