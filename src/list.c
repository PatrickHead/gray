/*!
    @file list.c

    @brief Source file for list data

    @timestamp Mon, 06 Oct 2014 13:42:48 +0000

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

    Source file for linked list structure data management

    This module provides a generic management interface for the classic double
    linked list data structure.

    In addition to the global list management functions, functions are also
    provided to insert/delete/change list elements, search for list items by
    reference and data value.  Also, functions are provided for queue and stack
    operations.

  */

  // Required system headers

#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "list.h"

  /*!
    @brief INTERNAL: list object(item) structure
  */

typedef struct _list_obj
{
    /*! @brief previous item in list */
  struct _list_obj *p;
    /*! @brief next item in list */
  struct _list_obj *n;
    /*! @brief data payload */
  void *_pl;
} _list_obj;

  /*!
    @brief INTERNAL: list internals structure
  */

typedef struct
{
    /*! @brief head of list */
  _list_obj *h;
    /*! @brief cursor, current in list */
  _list_obj *c;
    /*! @brief tail of list */
  _list_obj *t;
    /*! @brief number of items in list */
  int len;
    /*! @brief list object data payload de-allocation function */
  list_payload_free list_pl_free;
} _list_internals;

  // INTERNAL: utility function prototypes for module

static _list_obj *_list_obj_create(void * const pl);
static void *_list_obj_free(_list_obj *const lo);
static void _list_obj_destroy(_list_obj *const lo, list_payload_free fpl);

static list_payload_free _list_get_pl_free(list_s * const l);
static _list_internals* _list_get_internals(list_s * const l);
static void *_list_get_payload(_list_obj *const lo);
static _list_obj *_list_find_by_reference(list_s * const l, void * const pl);

  /*!

     @brief Create a new list

     Allocates memory for a new list structure, creating all necessary
     components, and setting reasonable defaults.

     @retval "list_s *" success
     @retval NULL    failure

  */

list_s *list_create(void)
{
  list_s *l;

  l = malloc(sizeof(list_s));
  if (!l) return NULL;
  memset(l, 0, sizeof(list_s));

  l->internals = (void*)malloc(sizeof(_list_internals));
  if (!l->internals)
  {
    free(l);
    return NULL;
  }
  memset(l->internals, 0, sizeof(_list_internals));

    // Set initial payload data free function to system free()
  list_set_free(l, free);

    // Return "list_s *"
  return l;
}

  /*!

     @brief Destroy a list

     De-allocate all allocated memory associated with a list, including
     the payload data.  If the list payload data free function is set to NULL,
     then the payload data is left intact.

     @param list    pointer to existing list

     @retval NONE

  */

void list_destroy(list_s * const list)
{
  _list_internals* lin;
  _list_obj *lo;
  _list_obj *next;
  list_payload_free fpl;

  if (!list) return;

  lin = _list_get_internals(list);
  if (!lin) return;

  fpl = _list_get_pl_free(list);
  
  lo = lin->h;
  while (lo)
  {
    next = lo->n;
    if (fpl)
      _list_obj_destroy(lo, fpl);
    else
      _list_obj_free(lo);
    lo = next;
  }

  free(lin);
  free(list);
}

  /*!

     @brief Free a list

     De-allocate all allocated memory associated with a list, leaving
     the payload data intact.

     @param list    pointer to existing list

     @retval NONE

  */

void list_free(list_s * const list)
{
  _list_internals* lin;
  _list_obj *lo;
  _list_obj *next;

    // Sanity check parameters.
  assert(list);

  lin = _list_get_internals(list);
  if (!lin) return;

  lo = lin->h;
  while (lo)
  {
    next = lo->n;
    _list_obj_free(lo);
    lo = next;
  }

  free(lin);
  free(list);
}

  /*!

     @brief Set payload data free function

     Set payload data free function for list to user defined function.

     @param list    pointer to existing list
     @param func    pointer to user defined function

     @retval NONE

  */

void list_set_free(list_s * const list, list_payload_free func)
{
  _list_internals *lin;

    // Sanity check parameters.
  assert(list);
  assert(func);

  lin = _list_get_internals(list);
  if (lin) lin->list_pl_free = func;
}

  /*!

     @brief Get count of items in list

     Return the count of items in a list.

     @param list    pointer to existing list

     @retval "int" always

  */

int list_len(list_s * const list)
{
  _list_internals *lin;

  if (!list) return 0;

  lin = _list_get_internals(list);
  if (lin) return lin->len;

  return 0;
}

  /*!

     @brief Add item to list

     Add a new element to a list.  User must supply a pointer to the payload
     data, and a value for the whence parameter.

     The following table list the possible values for whence:

     Value              Meaning
     -----              ----------------------------------

     HEAD               ALWAYS before head of list
     CURR               ALWAYS before current item in list
     TAIL               ALWAYS after last item in list

     @param list    pointer to existing list
     @param payload    pointer to payload data to add
     @param whence    (see table above)

     @retval NONE

  */

void list_insert(list_s * const list, void * const payload, void * const whence)
{
  _list_internals* lin;
  _list_obj *lo;
  _list_obj *new;

    // Sanity check parameters.
  assert(list);
  assert(payload);

  lin = _list_get_internals(list);
  if (!lin) return;

  new = _list_obj_create(payload);
  if (!new) return;

  switch ((list_whence_t)whence)
  {
    case HEAD:
      new->n = lin->h;
      new->p = NULL;
      lin->h = new;
      if (!lin->t) lin->t = new;
      if (new->n) new->n->p = new;
      ++lin->len;
      lin->c = new;
      break;

    case CURR:
      if (!lin->c)
      {
        _list_obj_free(new);
        list_insert(list, payload, HEAD);
        return;
      }
      new->n = lin->c;
      new->p = lin->c->p;
      lin->c->p = new;
      if (new->p) new->p->n = new;
      if (!new->p) lin->h = new;
      ++lin->len;
      lin->c = new;
      break;

    case TAIL:
      new->n = NULL;
      new->p = lin->t;
      lin->t = new;
      if (!lin->h) lin->h = new;
      if (new->p) new->p->n = new;
      ++lin->len;
      lin->c = new;
      break;

    default:
      lo = _list_find_by_reference(list, whence);
      if (!lo)
      {
        _list_obj_free(new);
        return;
      }
      lin->c = lo;
      list_insert(list, payload, (void*)CURR);
      break;
  }
}

  /*!

     @brief Delete item from a list

     Remove item from a list and de-allocate the payload data.  Must supply
     a value for whence.

     The following table list the possible values for whence:

     Value              Meaning
     -----              ----------------------------------

     HEAD               ALWAYS before head of list
     CURR               ALWAYS before current item in list
     TAIL               ALWAYS after last item in list
     (other)            ALWAYS before the list item that "other" points to

     @param list    pointer to existing list
     @param whence    (see table above)

     @retval NONE

  */

void list_delete(list_s * const list, void * const whence)
{
  void *pl = NULL;
  list_payload_free fpl = NULL;

  assert(list);

  pl = list_remove(list, whence);
  if (pl)
  {
    fpl = _list_get_pl_free(list);
    if (fpl) fpl(pl);
  }
}

  /*!

     @brief Replace payload data of list item

     Replace the payload data of a list item.  User must supply a pointer to
     the payload data, and a value for the whence parameter.

     NOTE:  The original payload data is NOT de-allocated.

     @param list    pointer to existing list
     @param payload    pointer to replacement payload data
     @param whence    ( see list_delete() )

     @retval NONE

  */

void list_replace(list_s * const list, void * const payload, void * const whence)
{
  _list_internals* lin = NULL;
  _list_obj *lo = NULL;

    // Sanity check parameters.
  assert(list);
  assert(payload);

  lin = _list_get_internals(list);
  if (!lin) return;

  switch ((list_whence_t)whence)
  {
    case HEAD:
      lo = lin->h;
      break;
    case CURR:
      lo = lin->c;
      break;
    case TAIL:
      lo = lin->t;
      break;
    default:
      lo = _list_find_by_reference(list, whence);
      break;
  }

  if (lo)
  {
    lo->_pl = payload;
    lin->c = lo;
  }
}

  /*!

     @brief Remove item from a list, leave payload data intact

     Remove item from a list.  Do NOT de-allocate the payload data.

     @param list    pointer to existing list
     @param whence    ( see list_delete() )

     @retval NONE

  */

void *list_remove(list_s * const list, void * const whence)
{
  _list_internals* lin = NULL;
  _list_obj *lo = NULL;
  void *pl = NULL;

    // Sanity check parameters.
  assert(list);

  lin = _list_get_internals(list);
  if (!lin) return NULL;

  switch ((list_whence_t)whence)
  {
    case HEAD:
      lo = lin->h;
      break;
    case CURR:
      lo = lin->c;
      break;
    case TAIL:
      lo = lin->t;
      break;
    default:
      lo = _list_find_by_reference(list, whence);
      break;
  }

  if (!lo) return NULL;

    // Adjust internal pointers, if needed

  if (lo == lin->h)
    lin->h = lo->n;
  if (lo == lin->c)
    lin->c = lo->n;
  if (lo == lin->t)
    lin->t = lo->p;

    // Adjust internal statistics

  --lin->len;

    // Unlink the object

  if (lo->p)
    lo->p->n = lo->n;
  if (lo->n)
    lo->n->p = lo->p;

    // Save the payload
  pl = lo->_pl;

    // Unalistocate the object
  _list_obj_free(lo);

    // Return "void *"
  return pl;
}

  /*!

     @brief Find a list item by reference

     Find an item in a list.  A match exists when the user supplied
     "reference" parameter is equal to the address of the list item's
     payload data.  The payload data pointer is returned on a match.

     @param list    pointer to exising list
     @param reference    pointer to search for in list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_find_by_reference(list_s * const list, void * const reference)
{
  _list_obj *lo;

    // Sanity check parameters.
  assert(list);
  assert(reference);

  lo = _list_find_by_reference(list, reference);
  if (lo) return lo->_pl;

  return NULL;
}

  /*!

     @brief Find a list item by value

     Find an item in a list.  A match exists when contents of the user
     supplied "value" parameter is equal to the contents of a list item's
     payload data.  The payload data is returned on a match.  The user must
     supply a payload data comparison function.

     @param list    pointer to exising list
     @param value    pointer to value to search for in list
     @param func    user supplied payload data comparison function

     @retval "void *" success
     @retval NULL    failure

  */

void *list_find_by_value(list_s * const list,
                       void * const value,
                       list_payload_compare func)
{
  void *pl;

    // Sanity check parameters.
  assert(list);
  assert(value);
  assert(func);

  for (pl = list_head(list); pl; pl = list_next(list))
    if (!func(value, pl)) return pl;

  return NULL;
}

  /*!

     @brief Get head of list

     Return payload data for list item at head of list.

     @param list    pointer to exising list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_head(list_s * const list)
{
  _list_internals *lin;

  assert(list);

  lin = _list_get_internals(list);
  if (lin)
  {
    lin->c = lin->h;
    if (lin->c)
      return _list_get_payload(lin->c);
  }

  return NULL;
}

  /*!

     @brief Get current list item

     Return payload data for current item in a list.

     @param list    pointer to existing list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_curr(list_s * const list)
{
  _list_internals *lin;

  assert(list);

  lin = _list_get_internals(list);
  if (lin)
  {
    if (lin->c)
      return _list_get_payload(lin->c);
  }

  return NULL;
}

  /*!

     @brief Get tail of list

     Return the payload data of the last item in a list.

     @param list    pointer to existing list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_tail(list_s * const list)
{
  _list_internals *lin;

  assert(list);

  lin = _list_get_internals(list);
  if (lin)
  {
    lin->c = lin->t;
    if (lin->c)
      return _list_get_payload(lin->c);
  }

  return NULL;
}

  /*!

     @brief Get next item of list

     Return the payload data for the next item in a list.  The cursor, or
     current location, of the list is also moved to the next item.

     @param list    pointer to existing list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_next(list_s * const list)
{
  _list_internals *lin;
  void *pl;

  assert(list);

  lin = _list_get_internals(list);
  if (lin->c)
  {
    if (lin->c->n)
    {
      lin->c = lin->c->n;
      pl = _list_get_payload(lin->c);
        return pl;
    }
  }

  return NULL;
}

  /*!

     @brief Get previous item of list

     Return the payload data for the previous item in a list.  The cursor, or
     current location, of the list is also moved to the previous item.

     @param list    pointer to existing list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_prev(list_s * const list)
{
  _list_internals *lin;
  void *pl;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin->c)
  {
    if (lin->c->p)
    {
      lin->c = lin->c->p;
      pl = _list_get_payload(lin->c);
        return pl;
    }
  }

  return NULL;
}

  /*!

     @brief Push item onto head of list

     Push a new list item onto the head of a list.

     NOTE:  This is a wrapper for list_insert

     @param list    pointer to exising list
     @param payload    pointer to payload data to insert into list

     @retval NONE

  */

void list_push(list_s * const list, void * const payload)
{
    // Sanity check parameters.
  assert(list);
  assert(payload);

  list_insert(list, payload, (void*)HEAD);
}

  /*!

     @brief Get and remove item from head of list

     Return the payload data and remove the list item from the head of a list.

     NOTE:  This is a wrapper for list_dequeue

     @param list    pointer to existing list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_pop(list_s * const list)
{
  return list_dequeue(list);
}

  /*!

     @brief Add item to tail of list

     Append a new list item to end of a list.

     NOTE:  This is a wrapper for list_insert

     @param list    pointer to exising list
     @param payload    pointer to payload data to insert into list

     @retval NONE

  */

void list_queue(list_s * const list, void * const payload)
{
    // Sanity check parameters.
  assert(list);
  assert(payload);

  list_insert(list, payload, (void*)TAIL);
}

  /*!

     @brief Get and remove item from head of list

     Return the payload data and remove the list item from the head of a list.

     @param list    pointer to existing list

     @retval "void *" success
     @retval NULL    failure

  */

void *list_dequeue(list_s * const list)
{
  void *pl;

    // Sanity check parameters.
  assert(list);

  pl = list_head(list);
  list_remove(list, (void*)HEAD);

    // Return "void *"
  return pl;
}

  /*!

     @brief INTERNAL:  Create a new list object

     Allocates and initializes all components of a list object structure.

     @param pl    pointer to payload data

     @retval "_list_obj *" success
     @retval NULL    failure

  */

static _list_obj *_list_obj_create(void * const pl)
{
  _list_obj *new;

  new = (_list_obj*)malloc(sizeof(_list_obj));
  if (!new) return NULL;
  memset(new, 0, sizeof(_list_obj));

  new->_pl = pl;

    // Return "_list_obj *"
  return new;
}

  /*!

     @brief INTERNAL:  Free a list object, leaving payload intact

     De-allocates memory associated with a list object structure.  Payload data
     is left intact, and returned to user.

     @param lo    pointer to list object structure

     @retval "void *" success
     @retval NULL    failure

  */

static void *_list_obj_free(_list_obj *const lo)
{
  void *pl;

    // Sanity check parameters.
  assert(lo);

  if (lo->_pl) pl = lo->_pl;

  free(lo);

    // Return "void *"
  return pl;
}

  /*!

     @brief INTERNAL:  Destroy a list object

     De-allocates memory associated with a list object structure, including
     payload data.

     @param lo    pointer to list object structure
     @param fpl    payload data free function

     @retval NONE

  */

static void _list_obj_destroy(_list_obj *const lo, list_payload_free fpl)
{
  void *pl;

    // Sanity check parameters.
  assert(lo);
  assert(fpl);

  pl = _list_obj_free(lo);

  if (pl) fpl(pl);
}

  /*!

     @brief INTERNAL:  Get payload data free function

     Return the payload data free function from a list.

     @param l    pointer to existing list

     @retval "list_payload_free *" success
     @retval NULL    failure

  */

static list_payload_free _list_get_pl_free(list_s * const l)
{
  _list_internals *lin;

    // Sanity check parameters.
  assert(l);

  lin = _list_get_internals(l);
  if (lin) return lin->list_pl_free;

  return NULL;
}

  /*!

     @brief INTERNAL:  Get list internals

     Return pointer to internals structure from a list.

     @param l    pointer to existing list

     @retval "_list_internals *" success
     @retval NULL    failure

  */

static _list_internals *_list_get_internals(list_s * const l)
{
    // Sanity check parameters.
  assert(l);

    // Return "_list_internals *"
  return l->internals;
}

  /*!

     @brief INTERNAL:  Get payload data from list object

     Return pointer payload data from list object structure.

     @param lo    pointer to list object

     @retval "void *" success
     @retval NULL    failure

  */

static void *_list_get_payload(_list_obj *const lo)
{
    // Sanity check parameters.
  assert(lo);

    // Return "void *"
  return lo->_pl;
}

  /*!

     @brief INTERNAL:  Find a list object by reference

     Find an object in a list.  A match exists when the user supplied
     "reference" parameter is equal to the address of the list item's
     payload data.  The list object pointer is returned on a match.

     @param list    pointer to exising list
     @param reference    pointer to search for in list

     @retval "void *" success
     @retval NULL    failure

  */

static _list_obj *_list_find_by_reference(list_s * const l, void * const pl)
{
  _list_internals *lin;
  _list_obj *lo;

    // Sanity check parameters.
  assert(l);
  assert(pl);

  lin = _list_get_internals(l);
  if (!lin) return NULL;

  lo = lin->h;
  while (lo)
  {
    if (lo->_pl == pl) return lo;
    lo = lo->n;
  }

  return NULL;
}

