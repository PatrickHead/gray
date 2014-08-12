#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "grid.h"

typedef struct _cell
{
  struct _cell *up;     // Up
  struct _cell *down;   // Down
  struct _cell *left;   // Left
  struct _cell *right;  // Right
  void *payload;        // Payload
} _cell;

typedef struct
{
  _cell *origin;               // Origin
  _cell *current;              // Current
  _cell *end;                  // End
  grid_size_s *size;
  vertex_s *location;
  grid_payload_free grid_pl_free;  // Memory de-allocation function.  Can be
                                   //   payload specific.
} _grid_internals;

static _cell *_cell_new(void *pl);
static void _cell_free(_cell *c, grid_payload_free fpl);

static grid_payload_free _grid_get_pl_free(grid_s *gs);
static _grid_internals *_grid_get_internals(grid_s *gs);
static void *_grid_get_payload(_cell *c);
static _cell *_grid_find_by_reference(grid_s *gs, void *pl);
static _cell *_grid_find_by_value(grid_s *gs,
                                  void *pl,
                                  grid_payload_compare cf);
static _cell *_grid_get_cell(grid_s *grid);

  // New creates a new grid
grid_s *grid_create(void)
{
  grid_s *g;
  grid_size_s *gs;

  g = malloc(sizeof(grid_s));
  memset(g, 0, sizeof(grid_s));
  g->_internals = (void*)malloc(sizeof(_grid_internals));
  memset(g->_internals, 0, sizeof(_grid_internals));
  ((_grid_internals *)(g->_internals))->location = vertex_create();
  ((_grid_internals *)(g->_internals))->size = grid_size_create();
  grid_set_free(g, free);

  gs = grid_size_create();
  grid_size_set(gs, 1, 1);
  grid_set_size(g, gs);

  vertex_set_x(((_grid_internals *)(g->_internals))->location, 0);
  vertex_set_y(((_grid_internals *)(g->_internals))->location, 0);

  return g;
}

void grid_free(grid_s *grid)
{
  _grid_internals *gin;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  if (gin->size)
    while (grid_size_get_height(gin->size))
      grid_free_row(grid, 0);

  if (gin->size) grid_size_destroy(gin->size);
  if (gin->location) vertex_destroy(gin->location);

  free(gin);
  free(grid);
}

  // Free removes all the items from the grid, and destroys all the cell
  // payloads.
void grid_destroy(grid_s *grid)
{
  _grid_internals *gin;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  if (gin->size)
    while (grid_size_get_height(gin->size))
      grid_destroy_row(grid, 0);

  if (gin->size) grid_size_destroy(gin->size);
  if (gin->location) vertex_destroy(gin->location);

  free(gin);
  free(grid);
}

void grid_set_free(grid_s *grid, grid_payload_free func)
{
  _grid_internals *gin;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (gin) gin->grid_pl_free = func;

  return;
}

void grid_set_size(grid_s *grid, grid_size_s *gs)
{
  grid_size_s *size;
  int rows, cols;
  int width, height;
  int i;

  assert(grid);
  assert(gs);

  size = grid_get_size(grid);
  if (!size) return;

  cols = grid_size_get_width(gs);
  rows = grid_size_get_height(gs);

  width = grid_size_get_width(size);
  height = grid_size_get_height(size);

    // If rows is greater than current size, insert needed rows at end
  for (i = height; i < rows; ++i)
    grid_create_row(grid, -1);

    // If rows is less than current size, delete rows from end
  for (i = height - 1; rows < (i + 1); --i)
    grid_destroy_row(grid, -1);

    // If cols is greater than current size, insert cols at end
  for (i = width; i < cols; ++i)
    grid_create_column(grid, -1);

    // If cols is less than current size, delete cols from end
  for (i = width - 1; cols < (i + 1); --i)
    grid_destroy_column(grid, -1);

  return;
}

void grid_set_size_free_only(grid_s *grid, grid_size_s *gs)
{
  grid_size_s *size;
  int rows, cols;
  int width, height;
  int i;

  assert(grid);
  assert(gs);

  size = grid_get_size(grid);
  if (!size) return;

  cols = grid_size_get_width(gs);
  rows = grid_size_get_height(gs);

  width = grid_size_get_width(size);
  height = grid_size_get_height(size);

    // If rows is greater than current size, insert needed rows at end
  for (i = height; i < rows; ++i)
    grid_create_row(grid, -1);

    // If rows is less than current size, delete rows from end
  for (i = height - 1; rows < (i + 1); --i)
    grid_free_row(grid, -1);

    // If cols is greater than current size, insert cols at end
  for (i = width; i < cols; ++i)
    grid_create_column(grid, -1);

    // If cols is less than current size, delete cols from end
  for (i = width - 1; cols < (i + 1); --i)
    grid_free_column(grid, -1);

  return;
}

grid_size_s *grid_get_size(grid_s *grid)
{
  _grid_internals *gin;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  return gin->size;
}

vertex_s *grid_get_location(grid_s *grid)
{
  _grid_internals *gin;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  return gin->location;
}

void grid_create_row(grid_s *grid, int row)
{
  _grid_internals *gin;
  int chgt, cwid;
  _cell *urow;  // Up row, row before new row
  _cell *drow;  // Down row, row after new row
  int i;
  _cell *nrow;  // New row
  _cell *ncell; // A new cell
  _cell *pcell; // Tracks most previous cell created

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

    // Get width and height of grid
  chgt = grid_size_get_height(gin->size);
  cwid = grid_size_get_width(gin->size);

    // Adding row outside of grid boundry plus one row is not allowed
  if (row > chgt) return;

    // Any negative row, indicates to add row to end
  if (row < 0) row = chgt;

    // Fix width, if none
  if (cwid < 1) cwid = 1;

    // Create new row with number of cells equal to width of grid
  for (nrow = NULL, i = 0; i < cwid; i++)
  {
    ncell = _cell_new(NULL);
    if (!nrow)
      nrow = ncell;
    else
    {
      pcell->right = ncell;
      ncell->left = pcell;
    }
    pcell = ncell;
  }

    // If this is the first row in the grid
  if (!chgt)
  {
    gin->origin = nrow;
    gin->current = nrow;
    gin->end = ncell;
    vertex_set_y(gin->location, 0);
    vertex_set_x(gin->location, 0);
    grid_size_set_height(gin->size, 1);
    grid_size_set_width(gin->size, cwid);
  }
  else
  {
    if (!row) // Insert before all rows
    {
      drow = gin->origin;
      for (pcell = nrow; drow && pcell; drow = drow->right, pcell = pcell->right)
      {
        drow->up = pcell;
        pcell->down = drow;
      }

      gin->origin = nrow;
      gin->current = nrow;
      vertex_set_y(gin->location, 0);
      vertex_set_x(gin->location, 0);
      grid_size_set_height(gin->size, chgt + 1);
    }
    else if (row == chgt) // Append after all rows
    {
      for (urow = gin->origin; urow->down; urow = urow->down) ;
      for (pcell = nrow; urow && pcell; urow = urow->right, pcell = pcell->right)
      {
        urow->down = pcell;
        pcell->up = urow;
      }

      gin->end = ncell;
      grid_size_set_height(gin->size, chgt + 1);
    }
    else // Inserting in middle of rows somewhere
    {
      for (i = 0, drow = gin->origin;
           (i < row) && drow->down;
           i++, drow = drow->down) ;
      urow = drow->up;
      for (pcell = nrow;
           drow && urow && pcell;
           drow = drow->right, urow = urow->right, pcell = pcell->right)
      {
        urow->down = pcell;
        drow->up = pcell;
        pcell->up = urow;
        pcell->down = drow;
      }

      grid_size_set_height(gin->size, chgt + 1);
    }
  }

  return;
}

void grid_create_column(grid_s *grid, int col)
{
  _grid_internals *gin;
  int chgt, cwid;
  _cell *lcol;  // Left col, col before new col
  _cell *rcol;  // Righ col, col after new col
  int i;
  _cell *ncol;  // New row
  _cell *ncell; // A new cell
  _cell *pcell; // Tracks most previous cell created

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

    // Get width and height of grid
  chgt = grid_size_get_height(gin->size);
  cwid = grid_size_get_width(gin->size);

    // Adding col outside of grid boundry plus one col is not allowed
  if (col > cwid) return;

    // Any negative col, indicates to add col to end
  if (col < 0) col = cwid;

    // Fix height, if none
  if (chgt < 1) chgt = 1;

    // Create new col with number of cells equal to height of grid
  for (ncol = NULL, i = 0; i < chgt; i++)
  {
    ncell = _cell_new(NULL);
    if (!ncol)
      ncol = ncell;
    else
    {
      pcell->down = ncell;
      ncell->up = pcell;
    }
    pcell = ncell;
  }

    // If this is the first col in the grid
  if (!cwid)
  {
    gin->origin = ncol;
    gin->current = ncol;
    gin->end = ncell;
    vertex_set_y(gin->location, 0);
    vertex_set_x(gin->location, 0);
    grid_size_set_height(gin->size, chgt);
    grid_size_set_width(gin->size, 1);
  }
  else
  {
    if (!col) // Insert before all cols
    {
      rcol = gin->origin;
      for (pcell = ncol; rcol && pcell; rcol = rcol->down, pcell = pcell->down)
      {
        rcol->left = pcell;
        pcell->right = rcol;
      }

      gin->origin = ncol;
      gin->current = ncol;
      vertex_set_y(gin->location, 0);
      vertex_set_x(gin->location, 0);
      grid_size_set_width(gin->size, cwid + 1);
    }
    else if (col == cwid) // Append after all cols
    {
      for (lcol = gin->origin; lcol->right; lcol = lcol->right) ;
      for (pcell = ncol; lcol && pcell; lcol = lcol->down, pcell = pcell->down)
      {
        lcol->right = pcell;
        pcell->left = lcol;
      }

      gin->end = ncell;
      grid_size_set_width(gin->size, cwid + 1);
    }
    else // Inserting in middle of rows somewhere
    {
      for (i = 0, rcol = gin->origin;
           (i < col) && rcol->right;
           i++, rcol = rcol->right) ;
      lcol = rcol->left;
      for (pcell = ncol;
           rcol && lcol && pcell;
           rcol = rcol->down, lcol = lcol->down, pcell = pcell->down)
      {
        lcol->right = pcell;
        rcol->left = pcell;
        pcell->left = lcol;
        pcell->right = rcol;
      }

      grid_size_set_width(gin->size, cwid + 1);
    }
  }

  return;
}

void grid_free_row(grid_s *grid, int row)
{
  _grid_internals *gin;
  int chgt;
  _cell *c;
  _cell *n;
  _cell *e;
  int i;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  chgt = grid_size_get_height(gin->size);

    // Check row
  if (row < 0) row = chgt - 1;
  if (row >= chgt) return;

    // Find row
  for (i = 0, c = gin->origin; (i < row) && c->down; ++i, c = c->down) ;

  if (c == gin->origin) gin->origin = c->down;

  while (c)
  {
      // Unlink cell
    if (c->up) c->up->down = c->down;
    if (c->down) c->down->up = c->up;

    e = c->up;

      // Free cell
    n = c->right;
    _cell_free(c, NULL);
    c = n;
  }

  gin->current = gin->origin;
  vertex_set_y(gin->location, 0);
  vertex_set_x(gin->location, 0);
  grid_size_set_height(gin->size, chgt - 1);
  if (row == (chgt - 1)) gin->end = e;
  if (!gin->end) grid_size_set_width(gin->size, 0);

  return;
}

void grid_free_column(grid_s *grid, int col)
{
  _grid_internals *gin;
  int cwid;
  _cell *c;
  _cell *n;
  _cell *e;
  int i;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  cwid = grid_size_get_width(gin->size);

    // Check col
  if (col < 0) col = cwid - 1;
  if (col >= cwid) return;

    // Find col
  for (i = 0, c = gin->origin; (i < col) && c->right; ++i, c = c->right) ;

  if (c == gin->origin) gin->origin = c->right;

  while (c)
  {
      // Unlink cell
    if (c->left) c->left->right = c->right;
    if (c->right) c->right->left = c->left;

    e = c->left;

      // Free cell
    n = c->right;
    _cell_free(c, NULL);
    c = n;
  }

  gin->current = gin->origin;
  vertex_set_y(gin->location, 0);
  vertex_set_x(gin->location, 0);
  grid_size_set_width(gin->size, cwid - 1);
  if (col == (cwid - 1)) gin->end = e;
  if (!gin->end) grid_size_set_height(gin->size, 0);

  return;
}

void grid_destroy_row(grid_s *grid, int row)
{
  _grid_internals *gin;
  int chgt;
  _cell *c;
  _cell *n;
  _cell *e;
  grid_payload_free fpl;
  int i;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  chgt = grid_size_get_height(gin->size);

    // Check row
  if (row < 0) row = chgt - 1;
  if (row >= chgt) return;

  fpl = _grid_get_pl_free(grid);

    // Find row
  for (i = 0, c = gin->origin; (i < row) && c->down; ++i, c = c->down) ;

  if (c == gin->origin) gin->origin = c->down;

  while (c)
  {
      // Unlink cell
    if (c->up) c->up->down = c->down;
    if (c->down) c->down->up = c->up;

    e = c->up;

      // Free cell
    n = c->right;
    _cell_free(c, fpl);
    c = n;
  }

  gin->current = gin->origin;
  vertex_set_y(gin->location, 0);
  vertex_set_x(gin->location, 0);
  grid_size_set_height(gin->size, chgt - 1);
  if (row == (chgt - 1)) gin->end = e;
  if (!gin->end) grid_size_set_width(gin->size, 0);

  return;
}

void grid_destroy_column(grid_s *grid, int col)
{
  _grid_internals *gin;
  int cwid;
  _cell *c;
  _cell *n;
  _cell *e;
  grid_payload_free fpl;
  int i;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  cwid = grid_size_get_width(gin->size);

    // Check col
  if (col < 0) col = cwid - 1;
  if (col >= cwid) return;

  fpl = _grid_get_pl_free(grid);

    // Find col
  for (i = 0, c = gin->origin; (i < col) && c->right; ++i, c = c->right) ;

  if (c == gin->origin) gin->origin = c->right;

  while (c)
  {
      // Unlink cell
    if (c->left) c->left->right = c->right;
    if (c->right) c->right->left = c->left;

    e = c->left;

      // Free cell
    n = c->right;
    _cell_free(c, fpl);
    c = n;
  }

  gin->current = gin->origin;
  vertex_set_y(gin->location, 0);
  vertex_set_x(gin->location, 0);
  grid_size_set_width(gin->size, cwid - 1);
  if (col == (cwid - 1)) gin->end = e;
  if (!gin->end) grid_size_set_height(gin->size, 0);

  return;
}

void grid_clear_cell(grid_s *grid)
{
  _grid_internals *gin;
  grid_payload_free fpl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  fpl = _grid_get_pl_free(grid);

  if (fpl) fpl(_grid_get_payload(gin->current));

  gin->current->payload = NULL;

  return;
}

void *grid_get_cell(grid_s *grid)
{
  _cell *cell;

  assert(grid);

  cell = _grid_get_cell(grid);
  if (!cell) return NULL;

  return _grid_get_payload(cell);
}

void grid_set_cell(grid_s *grid, void *payload)
{
  _grid_internals *gin;
  
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  grid_clear_cell(grid);

  gin->current->payload = payload;
}

void *grid_find_by_reference(grid_s *grid, void *reference)
{
  _grid_internals *gin;
  _cell *c;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_find_by_reference(grid, reference);
  if (c)
  {
    gin->current = c;
    return c;
  }

  return NULL;
}

void *grid_find_by_value(grid_s *grid,
                         void *value,
                         grid_payload_compare func)
{
  _grid_internals *gin;
  _cell *c;

  assert(grid);
  assert(value);
  assert(func);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_find_by_value(grid, value, func);
  if (c)
  {
    gin->current = c;
    return c;
  }

  return NULL;
}

void *grid_origin(grid_s *grid)
{
  _grid_internals *gin;
  void *pl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  pl = _grid_get_payload(gin->origin);

  gin->current = gin->origin;
  vertex_set_y(gin->location, 0);
  vertex_set_x(gin->location, 0);

  return pl;
}

void *grid_current(grid_s *grid)
{
  _grid_internals *gin;
  void *pl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  pl = _grid_get_payload(gin->current);

  return pl;
}

void *grid_end(grid_s *grid)
{
  _grid_internals *gin;
  void *pl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  pl = _grid_get_payload(gin->end);

  gin->current = gin->end;
  vertex_set_y(gin->location, grid_size_get_height(gin->size) - 1);
  vertex_set_x(gin->location, grid_size_get_width(gin->size) - 1);

  return pl;
}

void *grid_left(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->left) return NULL;

  pl = _grid_get_payload(c->left);

  gin->current = c->left;
  vertex_set_x(gin->location, vertex_get_x(gin->location) - 1);

  return pl;
}

void *grid_right(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->right) return NULL;

  pl = _grid_get_payload(c->right);

  gin->current = c->right;
  vertex_set_x(gin->location, vertex_get_x(gin->location) + 1);

  return pl;
}

void *grid_up(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->up) return NULL;

  pl = _grid_get_payload(c->up);

  gin->current = c->up;
  vertex_set_y(gin->location, vertex_get_y(gin->location) - 1);

  return pl;
}

void *grid_down(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->down) return NULL;

  pl = _grid_get_payload(c->down);

  gin->current = c->down;
  vertex_set_y(gin->location, vertex_get_y(gin->location) + 1);

  return pl;
}

void *grid_goto(grid_s *grid, int row, int col)
{
  _grid_internals *gin;
  grid_size_s *size;
  _cell *c;
  int i;
  int dir;

  assert(grid);

  size = grid_get_size(grid);
  if (!size) return NULL;

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  if (row < 0) row = 0;
  if (row > grid_size_get_height(size) - 1) row = grid_size_get_height(size) - 1;
  if (col < 0) col = 0;
  if (col > grid_size_get_width(size) - 1) col = grid_size_get_width(size) - 1;

  c = gin->current;

  for (i = vertex_get_y(gin->location), dir = (row > i) ? 1 : -1;
       (i != row) && c;
       i += dir, c = ((dir < 0) ? c->up : c->down)) ;
  if (!c) return NULL;

  for (i = vertex_get_x(gin->location), dir = (col > i) ? 1 : -1;
       (i != col) && c;
       i += dir, c = ((dir < 0) ? c->left : c->right)) ;
  if (!c) return NULL;

  gin->current = c;
  vertex_set_y(gin->location, row);
  vertex_set_x(gin->location, col);

  return _grid_get_payload(c);
}

// STATIC functions

static _cell *_cell_new(void *pl)
{
  _cell *c;

  c = (_cell*)malloc(sizeof(_cell));
  memset(c, 0, sizeof(_cell));

  c->payload = pl;

  return c;
}

static void _cell_free(_cell *c, grid_payload_free fpl)
{
  assert(c);
  if (fpl) fpl(c->payload);
  free(c);
}

static grid_payload_free _grid_get_pl_free(grid_s *grid)
{
  _grid_internals *gin;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  return gin->grid_pl_free;
}

static _grid_internals *_grid_get_internals(grid_s *grid)
{
  assert(grid);
  return (_grid_internals*)grid->_internals;
}

static void *_grid_get_payload(_cell *c)
{
  assert(c);
  return c->payload;
}

static _cell *_grid_find_by_reference(grid_s *grid, void *pl)
{
  _cell *c;
  _cell *c2;
  _grid_internals *gin;
  int x, y;

  assert(grid);
  assert(pl);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  for (c = gin->origin, y = 0; c; c = grid_down(grid), ++y)
  {
    for (c2 = c, x = 0; c2; c2 = grid_right(grid), ++x)
      if (_grid_get_payload(c2) == pl)
      {
        vertex_set_y(gin->location, y);
        vertex_set_x(gin->location, x);
        return c2;
      }
  }

  return NULL;
}

static _cell *_grid_find_by_value(grid_s *grid,
                                  void *pl,
                                  grid_payload_compare cf)
{
  _cell *c;
  _cell *c2;
  _grid_internals *gin;
  int x, y;

  assert(grid);
  assert(pl);
  assert(cf);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  for (c = gin->origin, y = 0; c; c = grid_down(grid), ++y)
  {
    for (c2 = c, x = 0; c2; c2 = grid_right(grid), ++x)
      if (cf(_grid_get_payload(c2), pl) == 0)
      {
        vertex_set_y(gin->location, y);
        vertex_set_x(gin->location, x);
        return c2;
      }
  }

  return NULL;
}

static _cell *_grid_get_cell(grid_s *grid)
{
  _grid_internals *gin;

  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  return gin->current;
}

