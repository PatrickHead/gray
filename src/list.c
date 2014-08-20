/*!
    @file list.c

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

    @file list.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

  // Required system headers

#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "list.h"

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct _list_obj
{
  struct _list_obj *p;  // Previous item in list
  struct _list_obj *n;  // Next item in list
  void *_pl;          // Payload
} _list_obj;

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct
{
  _list_obj *h;                  // Head of list
  _list_obj *c;                  // Cursor (current item in list)
  _list_obj *t;                  // Tail of list
  int len;                     // Number of items in the list
  list_payload_free list_pl_free;  // Memory de-alistocation function.  Can be
                               //   payload specific.
} _list_internals;

static _list_obj *_list_obj_create(void * const pl);
static void *_list_obj_free(_list_obj *const lo);
static void _list_obj_destroy(_list_obj *const lo, list_payload_free fpl);

static list_payload_free _list_get_pl_free(list_s * const l);
static _list_internals* _list_get_internals(list_s * const l);
static void *_list_get_payload(_list_obj *const lo);
static _list_obj *_list_find_by_reference(list_s * const l, void * const pl);

  // New creates a new list
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

list_s *list_create(void)
{
  list_s *l;
  l = malloc(sizeof(list_s));
  memset(l, 0, sizeof(list_s));
  l->internals = (void*)malloc(sizeof(_list_internals));
  memset(l->internals, 0, sizeof(_list_internals));
  list_set_free(l, free);
    // Return RETVAL
  return l;
}

  // Destroy removes alist items from the list AND de-alistocates the payloads.
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

  // Free removes alist the items from the list, but does NOT de-alistocate the
  // payloads.
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void list_set_free(list_s * const list, list_payload_free func)
{
  _list_internals *lin;

    // Sanity check parameters.
  assert(list);
  assert(func);

  lin = _list_get_internals(list);
  if (lin) lin->list_pl_free = func;

    // Return RETVAL
  return;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

int list_len(list_s * const list)
{
  _list_internals *lin;

  if (!list) return 0;

  lin = _list_get_internals(list);
  if (lin) return lin->len;

    // Return RETVAL
  return 0;
}

  // Insert adds a new item to the list.
  // whence:
  //   HEAD - ALWAYS before head, ALWAYS becomes new head
  //   CURR - ALWAYS before current, MAY become new head
  //   TAIL - ALWAYS after tail
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void list_insert(list_s * const list, void * const payload, void * const whence)
{
  _list_internals* lin;
  _list_obj *lo;
  _list_obj *new;

  if (!list) return;
    // Sanity check parameters.
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
          // Return RETVAL
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
          // Return RETVAL
  return;
      }
      lin->c = lo;
      list_insert(list, payload, (void*)CURR);
      break;
  }

    // Return RETVAL
  return;
}

  // Delete removes an item from the list AND de-alistocates the payload.
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void list_delete(list_s * const list, void * const whence)
{
  void *pl = NULL;
  list_payload_free fpl = NULL;

  if (!list) return;

  pl = list_remove(list, whence);
  if (pl)
  {
    fpl = _list_get_pl_free(list);
    if (fpl) fpl(pl);
  }

    // Return RETVAL
  return;
}

  // Replace puts a new payload in place of an existing payload.  No memory
  // de-alistocation occurs on the original payload.
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

    // Return RETVAL
  return;
}

  // Remove removes an item from the list, but does NOT de-allocate the payload.
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

    // Return the payload
    // Return RETVAL
  return pl;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void *list_find_by_reference(list_s * const list, void * const reference)
{
  _list_obj *lo;

    // Sanity check parameters.
  assert(list);
  assert(reference);

  lo = _list_find_by_reference(list, reference);
  if (lo) return lo->_pl;

    // Return RETVAL
  return NULL;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

    // Return RETVAL
  return NULL;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void *list_head(list_s * const list)
{
  _list_internals *lin;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin)
  {
    lin->c = lin->h;
    if (lin->c)
        // Return RETVAL
  return _list_get_payload(lin->c);
  }

    // Return RETVAL
  return NULL;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void *list_curr(list_s * const list)
{
  _list_internals *lin;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin)
  {
    if (lin->c)
        // Return RETVAL
  return _list_get_payload(lin->c);
  }

    // Return RETVAL
  return NULL;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void *list_tail(list_s * const list)
{
  _list_internals *lin;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin)
  {
    lin->c = lin->t;
    if (lin->c)
        // Return RETVAL
  return _list_get_payload(lin->c);
  }

    // Return RETVAL
  return NULL;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void *list_next(list_s * const list)
{
  _list_internals *lin;
  void *pl;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin->c)
  {
    if (lin->c->n)
    {
      lin->c = lin->c->n;
      pl = _list_get_payload(lin->c);
        // Return RETVAL
  return pl;
    }
  }

    // Return RETVAL
  return NULL;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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
        // Return RETVAL
  return pl;
    }
  }

    // Return RETVAL
  return NULL;
}

static _list_obj *_list_obj_create(void * const pl)
{
  _list_obj *new;

  new = (_list_obj*)malloc(sizeof(_list_obj));
  memset(new, 0, sizeof(_list_obj));

  new->_pl = pl;

    // Return RETVAL
  return new;
}

static void *_list_obj_free(_list_obj *const lo)
{
  void *pl;

    // Sanity check parameters.
  assert(lo);

  if (lo->_pl) pl = lo->_pl;

  free(lo);

    // Return RETVAL
  return pl;
}

static void _list_obj_destroy(_list_obj *const lo, list_payload_free fpl)
{
  void *pl;

    // Sanity check parameters.
  assert(lo);
  assert(fpl);

  pl = _list_obj_free(lo);

  if (pl) fpl(pl);
}

static list_payload_free _list_get_pl_free(list_s * const l)
{
  _list_internals *lin;

    // Sanity check parameters.
  assert(l);

  lin = _list_get_internals(l);
  if (lin) return lin->list_pl_free;

    // Return RETVAL
  return NULL;
}

static _list_internals* _list_get_internals(list_s * const l)
{
    // Sanity check parameters.
  assert(l);
    // Return RETVAL
  return l->internals;
}

static void *_list_get_payload(_list_obj *const lo)
{
    // Sanity check parameters.
  assert(lo);
    // Return RETVAL
  return lo->_pl;
}

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

    // Return RETVAL
  return NULL;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void list_push(list_s * const list, void * const payload)
{
    // Sanity check parameters.
  assert(list);
  assert(payload);

  list_insert(list, payload, (void*)HEAD);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void list_queue(list_s * const list, void * const payload)
{
    // Sanity check parameters.
  assert(list);
  assert(payload);

  list_insert(list, payload, (void*)TAIL);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void *list_dequeue(list_s * const list)
{
  void *pl;

    // Sanity check parameters.
  assert(list);

  pl = list_head(list);
  list_remove(list, (void*)HEAD);

    // Return RETVAL
  return pl;
}

