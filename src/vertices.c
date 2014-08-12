#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vertices.h"

vertices_s *vertices_create(void)
{
  vertices_s *vs;

  vs = (vertices_s *)malloc(sizeof(vertices_s));
  memset(vs, 0, sizeof(vertices_s));

  vs->vertices = list_create();
  list_set_free(vs->vertices, vertex_destroy_void);

  return vs;
}

void vertices_destroy(vertices_s *vs)
{
  assert(vs);

  if (vs->vertices) list_destroy(vs->vertices);

  free(vs);
}

vertices_s *vertices_copy(vertices_s *vs)
{
  vertices_s *nvs;
  vertex_s *v;
  vertex_s *nv;

  assert(vs);

  nvs = vertices_create();
  memcpy(nvs, vs, sizeof(vertices_s));

  if (vs->vertices)
  {
    nvs->vertices = list_create();
    for (v = (vertex_s *)list_head(vs->vertices);
         v;
         v = (vertex_s *)list_next(vs->vertices))
    {
      nv = vertex_copy(v);
      if (nv) list_insert(nvs->vertices, (void *)nv, (void *)TAIL);
    }
  }

  return nvs;
}

void vertices_add_vertex(vertices_s *vs, vertex_s *v)
{
  vertex_s *nv;

  assert(vs);
  assert(v);

  nv = vertex_copy(v);
  if (nv) list_insert(vs->vertices, nv, (void *)TAIL);
}

