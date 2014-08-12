#ifndef GRID_H
#define GRID_H

#include "grid-size.h"
#include "vertex.h"

typedef void (*grid_payload_free)(void *payload);
typedef int (*grid_payload_compare)(void *pl1, void *pl2);

typedef struct
{
  void *_internals;
} grid_s;

grid_s *grid_create(void);
void grid_destroy(grid_s *g);
void grid_free(grid_s *g);
void grid_set_free(grid_s *g, grid_payload_free func);

void grid_set_size(grid_s *g, grid_size_s *gs);
void grid_set_size_free_only(grid_s *g, grid_size_s *gs);
grid_size_s *grid_get_size(grid_s *g);

vertex_s *grid_get_location(grid_s *g);

void grid_create_row(grid_s *g, int row);
void grid_create_column(grid_s *g, int column);
void grid_free_row(grid_s *g, int row);
void grid_free_column(grid_s *g, int column);
void grid_destroy_row(grid_s *g, int row);
void grid_destroy_column(grid_s *g, int column);

void grid_clear_cell(grid_s *g);
void *grid_get_cell(grid_s *g);
void grid_set_cell(grid_s *g, void *payload);

void *grid_find_by_reference(grid_s *g, void *reference);
void *grid_find_by_value(grid_s *g,
                       void *value,
                       grid_payload_compare func);

void *grid_origin(grid_s *g);
void *grid_current(grid_s *g);
void *grid_end(grid_s *g);
void *grid_left(grid_s *g);
void *grid_right(grid_s *g);
void *grid_up(grid_s *g);
void *grid_down(grid_s *g);
void *grid_goto(grid_s *g, int row, int column);

#endif // GRID_H
