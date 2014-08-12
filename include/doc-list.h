#ifndef DOC_LIST_H
#define DOC_LIST_H

#include "strapp.h"

typedef struct
{
  char *mpat;
  int nlist;
  char **list;
  char *kpat;
  int nkeep;
  char **keep;
} doc_list_s;

doc_list_s *doc_list_create(FILE *f, char *mpat, char *kpat);
void doc_list_destroy(doc_list_s *dl);
void doc_list_produce(doc_list_s *dl, char *doc);
void doc_list_consume(doc_list_s *dl, char *doc);

#endif // DOC_LIST_H
