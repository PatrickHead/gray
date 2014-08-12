#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

typedef struct _list_obj
{
  struct _list_obj *p;  // Previous item in list
  struct _list_obj *n;  // Next item in list
  void *_pl;          // Payload
} _list_obj;

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
list_s *list_create(void)
{
  list_s *l;
  l = malloc(sizeof(list_s));
  memset(l, 0, sizeof(list_s));
  l->internals = (void*)malloc(sizeof(_list_internals));
  memset(l->internals, 0, sizeof(_list_internals));
  list_set_free(l, free);
  return l;
}

  // Destroy removes alist items from the list AND de-alistocates the payloads.
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
void list_free(list_s * const list)
{
  _list_internals* lin;
  _list_obj *lo;
  _list_obj *next;

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

void list_set_free(list_s * const list, list_payload_free func)
{
  _list_internals *lin;

  assert(list);
  assert(func);

  lin = _list_get_internals(list);
  if (lin) lin->list_pl_free = func;

  return;
}

int list_len(list_s * const list)
{
  _list_internals *lin;

  if (!list) return 0;

  lin = _list_get_internals(list);
  if (lin) return lin->len;

  return 0;
}

  // Insert adds a new item to the list.
  // whence:
  //   HEAD - ALWAYS before head, ALWAYS becomes new head
  //   CURR - ALWAYS before current, MAY become new head
  //   TAIL - ALWAYS after tail
void list_insert(list_s * const list, void * const payload, void * const whence)
{
  _list_internals* lin;
  _list_obj *lo;
  _list_obj *new;

  if (!list) return;
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

  return;
}

  // Delete removes an item from the list AND de-alistocates the payload.
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

  return;
}

  // Replace puts a new payload in place of an existing payload.  No memory
  // de-alistocation occurs on the original payload.
void list_replace(list_s * const list, void * const payload, void * const whence)
{
  _list_internals* lin = NULL;
  _list_obj *lo = NULL;

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

  return;
}

  // Remove removes an item from the list, but does NOT de-allocate the payload.
void *list_remove(list_s * const list, void * const whence)
{
  _list_internals* lin = NULL;
  _list_obj *lo = NULL;
  void *pl = NULL;

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
  return pl;
}

void *list_find_by_reference(list_s * const list, void * const reference)
{
  _list_obj *lo;

  assert(list);
  assert(reference);

  lo = _list_find_by_reference(list, reference);
  if (lo) return lo->_pl;

  return NULL;
}

void *list_find_by_value(list_s * const list,
                       void * const value,
                       list_payload_compare func)
{
  void *pl;

  assert(list);
  assert(value);
  assert(func);

  for (pl = list_head(list); pl; pl = list_next(list))
    if (!func(value, pl)) return pl;

  return NULL;
}

void *list_head(list_s * const list)
{
  _list_internals *lin;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin)
  {
    lin->c = lin->h;
    if (lin->c)
      return _list_get_payload(lin->c);
  }

  return NULL;
}

void *list_curr(list_s * const list)
{
  _list_internals *lin;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin)
  {
    if (lin->c)
      return _list_get_payload(lin->c);
  }

  return NULL;
}

void *list_tail(list_s * const list)
{
  _list_internals *lin;

  if (!list) return NULL;

  lin = _list_get_internals(list);
  if (lin)
  {
    lin->c = lin->t;
    if (lin->c)
      return _list_get_payload(lin->c);
  }

  return NULL;
}

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
      return pl;
    }
  }

  return NULL;
}

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

static _list_obj *_list_obj_create(void * const pl)
{
  _list_obj *new;

  new = (_list_obj*)malloc(sizeof(_list_obj));
  memset(new, 0, sizeof(_list_obj));

  new->_pl = pl;

  return new;
}

static void *_list_obj_free(_list_obj *const lo)
{
  void *pl;

  assert(lo);

  if (lo->_pl) pl = lo->_pl;

  free(lo);

  return pl;
}

static void _list_obj_destroy(_list_obj *const lo, list_payload_free fpl)
{
  void *pl;

  assert(lo);
  assert(fpl);

  pl = _list_obj_free(lo);

  if (pl) fpl(pl);
}

static list_payload_free _list_get_pl_free(list_s * const l)
{
  _list_internals *lin;

  assert(l);

  lin = _list_get_internals(l);
  if (lin) return lin->list_pl_free;

  return NULL;
}

static _list_internals* _list_get_internals(list_s * const l)
{
  assert(l);
  return l->internals;
}

static void *_list_get_payload(_list_obj *const lo)
{
  assert(lo);
  return lo->_pl;
}

static _list_obj *_list_find_by_reference(list_s * const l, void * const pl)
{
  _list_internals *lin;
  _list_obj *lo;

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

void list_push(list_s * const list, void * const payload)
{
  assert(list);
  assert(payload);

  list_insert(list, payload, (void*)HEAD);
}

void list_queue(list_s * const list, void * const payload)
{
  assert(list);
  assert(payload);

  list_insert(list, payload, (void*)TAIL);
}

void *list_dequeue(list_s * const list)
{
  void *pl;

  assert(list);

  pl = list_head(list);
  list_remove(list, (void*)HEAD);

  return pl;
}

