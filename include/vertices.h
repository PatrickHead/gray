#ifndef VERTICES_H
#define VERTICES_H

#include "list.h"
#include "vertex.h"

typedef struct
{
  list_s *vertices;
} vertices_s;

vertices_s *vertices_create(void);
void vertices_destroy(vertices_s *vs);
vertices_s *vertices_copy(vertices_s *vs);
void vertices_add_vertex(vertices_s *vs, vertex_s *v);

#endif // VERTICES_H
