#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#include "color.h"

color_s *color_create(void)
{
  color_s *c;

  c = (color_s *)malloc(sizeof(color_s));
  memset(c, 0, sizeof(color_s));

  color_set_tag(c, "AUTO");

  return c;
}

void color_destroy(color_s *c)
{
  assert(c);

  if (c->tag) free(c->tag);
  free(c);
}

color_s *color_copy(color_s *c)
{
  color_s *nc;

  assert(c);

  nc = color_create();
  memcpy(nc, c, sizeof(color_s));

  if (c->tag) nc->tag = strdup(c->tag);

  return nc;
}

void color_set(color_s *c, char *tag, double r, double g, double b, double a)
{
  assert(c);
  assert(tag);

  color_set_tag(c, tag);
  color_set_rgba(c, r, g, b, a);
}

void color_get(color_s *c,
               char **tag,
               double *r,
               double *g,
               double *b,
               double *a)
{
  assert(c);
  assert(tag);
  assert(r);
  assert(g);
  assert(b);
  assert(a);

  *tag = color_get_tag(c);
  color_get_rgba(c, r, g, b, a);
}

void color_set_rgba(color_s *c, double r, double g, double b, double a)
{
  assert(c);

  color_set_r(c, r);
  color_set_g(c, g);
  color_set_b(c, b);
  color_set_a(c, a);
}

void color_get_rgba(color_s *c, double *r, double *g, double *b, double *a)
{
  assert(c);
  assert(r);
  assert(g);
  assert(b);
  assert(a);

  *r = color_get_r(c);
  *g = color_get_g(c);
  *b = color_get_b(c);
  *a = color_get_a(c);
}

void color_set_tag(color_s *c, char *tag)
{
  assert(c);
  assert(tag);

  if (c->tag) free(c->tag);
  c->tag = strdup(tag);
}

char *color_get_tag(color_s *c)
{
  assert(c);
  return c->tag;
}

void color_set_r(color_s *c, double r)
{
  assert(c);
  c->r = r;
}

double color_get_r(color_s *c)
{
  assert(c);
  return c->r;
}

void color_set_g(color_s *c, double g)
{
  assert(c);
  c->g = g;
}

double color_get_g(color_s *c)
{
  assert(c);
  return c->g;
}

void color_set_b(color_s *c, double b)
{
  assert(c);
  c->b = b;
}

double color_get_b(color_s *c)
{
  assert(c);
  return c->b;
}

void color_set_a(color_s *c, double a)
{
  assert(c);
  c->a = a;
}

double color_get_a(color_s *c)
{
  assert(c);
  return c->a;
}

color_s *str2color(char *s)
{
  color_s *c;

  assert(s);

  c = color_create();
  if (!c) return NULL;

  color_set_tag(c, "AUTO");

  sscanf(s, "[%lf,%lf,%lf,%lf]", &c->r, &c->g, &c->b, &c->a);

  return c;
}

  // NOTE: returned string is statically allocated, and this function is NOT
  //       re-entrant friendly.
char *color2str(color_s c)
{
  static char s[80];

  memset(s, 0, 80);
  snprintf(s, 80, "[%f,%f,%f,%f]\n", c.r, c.g, c.b, c.a);

  return s;
}

