/*!
    @file list.h

    @brief HEADER_BRIEF

    @timestamp Tue, 19 Aug 2014 05:12:38 +0000

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

    @file list.h

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef LIST_H
#define LIST_H

  /*!
    brief TYPEDEF_BRIEF
  */

typedef void (*list_payload_free)(void * const payload);
  /*!
    brief TYPEDEF_BRIEF
  */

typedef int (*list_payload_compare)(void * const pl1, void * const pl2);

  /*!
    brief TYPEDEF_BRIEF
  */

typedef enum
{
  HEAD = 0,
  CURR,
  TAIL
} list_whence_t;

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct
{
    /*! brief ELEMENT_BRIEF */
  void *internals;
} list_s;

list_s *list_create(void);
void list_destroy(list_s * const list);
void list_free(list_s * const list);
void list_set_free(list_s * const list, list_payload_free func);
int list_len(list_s * const list);

void list_insert(list_s * const list, void * const payload, void * const whence);
void list_delete(list_s * const list, void * const whence);
void *list_remove(list_s * const list, void * const whence);
void list_replace(list_s * const list, void * const payload, void * const whence);

void *list_find_by_reference(list_s * const list, void * const reference);
void *list_find_by_value(list_s * const list,
                         void * const value,
                         list_payload_compare func);

void *list_head(list_s * const list);
void *list_curr(list_s * const list);
void *list_tail(list_s * const list);
void *list_next(list_s * const list);
void *list_prev(list_s * const list);

void list_queue(list_s * const list, void * const payload);
void *list_dequeue(list_s * const list);
void list_push(list_s * const list, void * const payload);
#define list_pop(l) list_dequeue(l)

#endif // LIST_H
