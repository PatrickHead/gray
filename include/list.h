#ifndef LIST_H
#define LIST_H

typedef void (*list_payload_free)(void * const payload);
typedef int (*list_payload_compare)(void * const pl1, void * const pl2);

typedef enum
{
  HEAD = 0,
  CURR,
  TAIL
} list_whence_t;

typedef struct
{
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
