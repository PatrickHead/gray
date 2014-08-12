#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

void print_list(list_s *list);
void print_reverse(list_s *list);

int main(int argc, char **argv)
{
  list_s *list;
  char *s1 = strdup("alice");
  char *s2 = strdup("betty");
  char *s3 = strdup("connie");
  char *s4 = strdup("donna");
  char *s5 = strdup("eloise");

  list = list_create();

  list_insert(list, s1, (void*)HEAD);
  print_list(list);

  list_remove(list, s1);
  print_list(list);

  list_insert(list, s1, (void*)HEAD);
  print_list(list);

  list_delete(list, s1);
  print_list(list);

  list_insert(list, s2, (void*)TAIL);
  list_insert(list, s3, (void*)TAIL);
  list_insert(list, s4, (void*)TAIL);
  list_insert(list, s5, (void*)TAIL);
  print_list(list);

  list_free(list);

  list = list_create();
  print_list(list);

  list_insert(list, s2, (void*)HEAD);
  list_insert(list, s3, (void*)CURR);
  list_insert(list, s4, (void*)TAIL);
  list_insert(list, s5, (void*)CURR);
  print_list(list);
  printf("current: '%s'\n", (char*)list_curr(list));

  printf("backwards:\n");
  print_reverse(list);
  printf("current: '%s'\n", (char*)list_curr(list));

  list_destroy(list);

  return 0;
}

void print_list(list_s *list)
{
  char *s;

  assert(list);

  printf("#: %d\n", list_len(list));

  for (s = list_head(list); s; s = list_next(list))
    printf("'%s'\n", s);
}

void print_reverse(list_s *list)
{
  char *s;

  assert(list);

  printf("#: %d\n", list_len(list));

  for (s = list_tail(list); s; s = list_prev(list))
    printf("'%s'\n", s);
}
