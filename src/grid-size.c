#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "grid-size.h"

grid_size_s *grid_size_create(void)
{
  grid_size_s *gs;

  gs = (grid_size_s *)malloc(sizeof(grid_size_s));
  memset(gs, 0, sizeof(grid_size_s));

  return gs;
}

void grid_size_destroy_void(void *v)
{
  assert(v);
  grid_size_destroy((grid_size_s *)v);
}

void grid_size_destroy(grid_size_s *gs)
{
  assert(gs);
  free(gs);
}

grid_size_s *grid_size_copy(grid_size_s *gs)
{
  grid_size_s *ngs;

  assert(gs);

  ngs = grid_size_create();
  if (!ngs) return NULL;

  grid_size_set_width(ngs, grid_size_get_width(gs));
  grid_size_set_height(ngs, grid_size_get_height(gs));

  return ngs;
}

void grid_size_set(grid_size_s *gs, int width, int height)
{
  assert(gs);

  grid_size_set_width(gs, width);
  grid_size_set_height(gs, height);
}

void grid_size_get(grid_size_s *gs, int *width, int *height)
{
  assert(gs);

  *width = grid_size_get_width(gs);
  *height = grid_size_get_height(gs);
}

void grid_size_set_width(grid_size_s *gs, int width)
{
  assert(gs);

  if (width < 0) width = 0;

  gs->width = width;
}

int grid_size_get_width(grid_size_s *gs)
{
  assert(gs);
  return gs->width;
}

void grid_size_set_height(grid_size_s *gs, int height)
{
  assert(gs);

  if (height < 0) height = 0;

  gs->height = height;
}

int grid_size_get_height(grid_size_s *gs)
{
  assert(gs);
  return gs->height;
}

grid_size_s *str2grid_size(char *s)
{
  grid_size_s *gs;

  assert(s);

  gs = grid_size_create();
  if (!gs) return NULL;

  sscanf(s, "[%d,%d]", &gs->width, &gs->height);

  return gs;
}

char *grid_size2str(grid_size_s *gs)
{
  static char s[45];

  assert(gs);

  sprintf(s, "[%d,%d]\n",
    grid_size_get_width(gs),
    grid_size_get_height(gs));

  return s;
}

