#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vertex.h"

vertex_s *vertex_create(void)
{
  vertex_s *v;

  v = (vertex_s *)malloc(sizeof(vertex_s));
  memset(v, 0, sizeof(vertex_s));

  vertex_set_tag(v, "AUTO");

  return v;
}

void vertex_destroy_void(void *v)
{
  vertex_destroy((vertex_s *)v);
}

void vertex_destroy(vertex_s *v)
{
  assert(v);

  if (v->tag) free(v->tag);
  free(v);
}

vertex_s *vertex_copy(vertex_s *v)
{
  vertex_s *nv;

  assert(v);

  nv = vertex_create();
  memcpy(nv, v, sizeof(vertex_s));

  if (v->tag) nv->tag = strdup(v->tag);

  return nv;
}

void vertex_set(vertex_s *v, char *tag, double x, double y, double z)
{
  assert(v);
  assert(tag);

  vertex_set_tag(v, tag);
  vertex_set_x(v, x);
  vertex_set_y(v, y);
  vertex_set_z(v, z);
}

void vertex_get(vertex_s *v, char **tag, double *x, double *y, double *z)
{
  assert(v);
  assert(tag);
  assert(x);
  assert(y);
  assert(z);

  *tag = vertex_get_tag(v);
  *x = vertex_get_x(v);
  *y = vertex_get_y(v);
  *z = vertex_get_z(v);
}

void vertex_set_tag(vertex_s *v, char *tag)
{
  assert(v);
  assert(tag);

  if (v->tag) free(v->tag);
  v->tag = strdup(tag);
}

char *vertex_get_tag(vertex_s *v)
{
  assert(v);
  return v->tag;
}

void vertex_set_x(vertex_s *v, double x)
{
  assert(v);
  v->x = x;
}

double vertex_get_x(vertex_s *v)
{
  assert(v);
  return v->x;
}

void vertex_set_y(vertex_s *v, double y)
{
  assert(v);
  v->y = y;
}

double vertex_get_y(vertex_s *v)
{
  assert(v);
  return v->y;
}

void vertex_set_z(vertex_s *v, double z)
{
  assert(v);
  v->z = z;
}

double vertex_get_z(vertex_s *v)
{
  assert(v);
  return v->z;
}

vertex_s *str2vertex(char *s)
{
  vertex_s *v;

  assert(s);

  v = vertex_create();
  if (!v) return NULL;

  vertex_set_tag(v, "AUTO");

  sscanf(s, "[%lf,%lf,%lf]", &v->x, &v->y, &v->z);

  return v;
}

  // NOTE: returned string is statically allocated, and this function is NOT
  //       re-entrant friendly.
char *vertex2str(vertex_s *v)
{
  static char s[40];

  assert(v);

  memset(s, 0, 40);
  snprintf(s, 40, "[%f,%f,%f]\n", v->x, v->y, v->z);

  return s;
}

