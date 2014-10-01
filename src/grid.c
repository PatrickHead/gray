/*!
    @file grid.c

    @brief Source file for grid (matrix/spreadsheet) data

    @timestamp Tue, 30 Sep 2014 15:25:52 +0000

    @author Patrick Head  mailto:patrickhead@gmail.com

    @copyright Copyright (C) 2014  Patrick Head

    @license
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.@n
    @n
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.@n
    @n
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

  /*!

    @file grid.c

    Source file for grid structured data management

    A grid (matrix/spreadsheet) is a 2 dimensional orthoganol structured array
    of data cells.  Each cell contains a pointer to its neighboring cell to the
    left, right, above and below.  These pointers allow for easy navigation to
    any neighboring cell.   The data in each cell is generic, defined and
    managed by the calling program, and can contain any desired data, including
    another grid.

    Functions are provided for adding new rows and columns to the grid, as well
    as removing rows and columns.

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "grid.h"

  /*!
    @brief INTERNAL: cell data structure
  */

typedef struct _cell
{
    /*! @brief Pointer to cell above this one, if any */
  struct _cell *up;
    /*! @brief Pointer to cell below this one, if any */
  struct _cell *down;
    /*! @brief Pointer to cell to the left of this one, if any */
  struct _cell *left;
    /*! @brief Pointer to cell to the right of this one, this one, if any */
  struct _cell *right;
    /*! @brief Pointer to user defined data contained in cell */
  void *payload;
} _cell;

  /*!
    @brief INTERNAL: grid details structure
  */

typedef struct
{
    /*! @brief: pointer to first cell in grid (upper-left) */
  _cell *origin;
    /*! @brief: pointer to current cell in grid */
  _cell *current;
    /*! @brief: pointer to last cell in grid (lower-right) */
  _cell *end;
    /*! @brief current size of grid */
  grid_size_s *size;
    /*! @brief x,y coordinates of current cell */
  vertex_s *location;
    /*! @brief user supplied payload de-allocation function pointer */
  grid_payload_free grid_pl_free;
} _grid_internals;

  // INTERNAL: utility function prototypes for module
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

  /*!

     @brief Create a new grid

     Creates a new grid, allocationg all necessary components, and setting
     reasonable defaults.

     @retval "grid_s *" success
     @retval NULL    failure

  */

grid_s *grid_create(void)
{
  grid_s *g;
  _grid_internals *gi;

  g = malloc(sizeof(grid_s));
  if (!g) return NULL;

  memset(g, 0, sizeof(grid_s));

  gi = (void*)malloc(sizeof(_grid_internals));
  if (!gi)
  {
    grid_destroy(g);
    return NULL;
  }
  memset(gi, 0, sizeof(_grid_internals));
  g->_internals = gi;

  gi->location = vertex_create();
  if (!gi->location)
  {
    grid_destroy(g);
    return NULL;
  }
  vertex_set_y(gi->location, 0);
  vertex_set_x(gi->location, 0);

  gi->size = grid_size_create();
  if (!gi->size)
  {
    grid_destroy(g);
    return NULL;
  }
  grid_size_set(gi->size, 1, 1);

  grid_set_free(g, free);

    // Return "grid_s *"
  return g;
}

  /*!

     @brief Destroy grid, leaving user data

     De-allocates a grid, and all associated "wrapping" structures, but leaves
     the user managed cell payload data intact.

     @param grid    pointer to existing grid

     @retval NONE

  */

void grid_free(grid_s *grid)
{
  _grid_internals *gi;

    // Sanity check parameters.
  assert(grid);

  gi = _grid_get_internals(grid);
  if (!gi) return;

  if (gi->size)
    while (grid_size_get_height(gi->size))
      grid_free_row(grid, 0);

  if (gi->size) grid_size_destroy(gi->size);
  if (gi->location) vertex_destroy(gi->location);

  free(gi);
  free(grid);
}

  /*!

     @brief Destroy grid

     De-allocates a grid, and all associated data, including user supplied cell
     payload data.

     @param grid    pointer to existing grid

     @retval NONE

  */

void grid_destroy(grid_s *grid)
{
  _grid_internals *gi;

    // Sanity check parameters.
  assert(grid);

  gi = _grid_get_internals(grid);
  if (!gi) return;

  if (gi->size)
    while (grid_size_get_height(gi->size))
      grid_destroy_row(grid, 0);

  if (gi->size) grid_size_destroy(gi->size);
  if (gi->location) vertex_destroy(gi->location);

  free(gi);
  free(grid);
}

  /*!

     @brief Set user defined payload de-allocation function

     Stores the user supplied cell payload de-allocation function

     @param grid    pointer to existing grid
     @param func    pointer to function that can de-allocate cell payload data

     @retval NONE    failure

  */

void grid_set_free(grid_s *grid, grid_payload_free func)
{
  _grid_internals *gin;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (gin) gin->grid_pl_free = func;
}

  /*!

     @brief Set size of grid

     Explicitly set size of grid.  If size is smaller than current size, then
     remove necessary cells.  If size is greater that current, then add new
     empty cells.

     @param grid    pointer to existing grid
     @param gs    pointer to "grid size" structure containing desired grid size

     @retval NONE    failure

  */

void grid_set_size(grid_s *grid, grid_size_s *gs)
{
  grid_size_s *size;
  int rows, cols;
  int width, height;
  int i;

    // Sanity check parameters.
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
}

  /*!

     @brief Set size of grid, leaving user data

     Explicitly set size of grid.  If size is smaller than current size, then
     remove necessary cells, leaving user supplied data intact.  If size is
     greater that current, then add new empty cells.

     @param grid    pointer to existing grid
     @param gs    pointer to "grid size" structure containing desired grid size

     @retval NONE    failure

  */

void grid_set_size_free_only(grid_s *grid, grid_size_s *gs)
{
  grid_size_s *size;
  int rows, cols;
  int width, height;
  int i;

    // Sanity check parameters.
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
}

  /*!

     @brief Get "grid size" structure from grid

     Get "grid size" structure from grid

     @param grid    pointer to exising grid

     @retval "grid_size_s *" success
     @retval NULL    failure

  */

grid_size_s *grid_get_size(grid_s *grid)
{
  _grid_internals *gi;

    // Sanity check parameters.
  assert(grid);

  gi = _grid_get_internals(grid);
  if (!gi) return NULL;

    // Return "grid_size_s *"
  return gi->size;
}

  /*!

     @brief Get location structure from grid

     Get location structure from grid

     @param grid    pointer to exising grid

     @retval "vertex_s *" success
     @retval NULL    failure

  */

vertex_s *grid_get_location(grid_s *grid)
{
  _grid_internals *gin;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

    // Return "vertex *"
  return gin->location;
}

  /*!

     @brief Create a new row in grid

     Creates a new row, with no cell payload data.  The row will contain the 
     current numer of columns as is set by the grid width.  The user supplied row
     parameter determines where the new row will be created.   If the row
     parameter is negative, the new row will be created after the current last
     row.  For a non-negative row parameter, the new row will be inserted before
     the row parameter value.  One exception, if the row parameter contains
     a value greater than the last row, then it will be set to the last row.

     @param grid    pointer to existing grid
     @param row     row AFTER which to add new row

     @retval NONE    failure

  */

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

    // Sanity check parameters.
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
}

  /*!

     @brief Create a new column in grid

     Creates a new column of cells in grid.  The new column contains a
     number of cells equal to the current height of the grid.  If a the
     requested column insertion number is greater than the current width
     of the grid is passed to this function, then the function returns
     with no effect.  If the column insertion number is the current width
     of the grid plus one, then the column is appended to the end of the
     grid.  If the column number is negative, the new column is also appended
     to the end of the grid.  Otherwise, the new column is inserted before
     the column insert number.

     @param grid    pointer to existing grid
     @param col    column insertion number

     @retval NONE

  */

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

    // Sanity check parameters.
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
}

  /*!

     @brief De-allocates a row in a grid, leaving data intact

     De-allocates all memory associated with all cells in a row, while
     leaving the memory associated with the cell payload data intact.

     @param grid    pointer to existing grid
     @param row    row number to free

     @retval NONE

  */

void grid_free_row(grid_s *grid, int row)
{
  _grid_internals *gin;
  int chgt;
  _cell *c;
  _cell *n;
  _cell *e;
  int i;

    // Sanity check parameters.
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
}

  /*!

     @brief De-allocates a row in a grid

     De-allocates all memory associated with all cells in a row, including
     the memory associated with the cell payload.

     @param grid    pointer to existing grid
     @param row    row number to free

     @retval NONE

  */

void grid_free_column(grid_s *grid, int col)
{
  _grid_internals *gin;
  int cwid;
  _cell *c;
  _cell *n;
  _cell *e;
  int i;

    // Sanity check parameters.
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
}

  /*!

     @brief De-allocates a row in a grid

     De-allocates all memory associated with all cells in a row, including
     the memory associated with the cell payload.

     @param grid    pointer to existing grid
     @param row    row number to free

     @retval NONE

  */

void grid_destroy_row(grid_s *grid, int row)
{
  _grid_internals *gin;
  int chgt;
  _cell *c;
  _cell *n;
  _cell *e;
  grid_payload_free fpl;
  int i;

    // Sanity check parameters.
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
}

  /*!

     @brief De-allocates a column in a grid

     De-allocates all memory associated with all cells in a column, including
     the memory associated with the cell payload.

     @param grid    pointer to existing grid
     @param col    column number to free

     @retval NONE

  */

void grid_destroy_column(grid_s *grid, int col)
{
  _grid_internals *gin;
  int cwid;
  _cell *c;
  _cell *n;
  _cell *e;
  grid_payload_free fpl;
  int i;

    // Sanity check parameters.
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
}

  /*!

     @brief De-allocate the payload data of the current cell

     De-allocates the data payload for the current cell in the grid.

     @param grid    pointer to existing grid

     @retval NONE

  */

void grid_clear_cell(grid_s *grid)
{
  _grid_internals *gin;
  grid_payload_free fpl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  fpl = _grid_get_pl_free(grid);

  if (fpl) fpl(_grid_get_payload(gin->current));

  gin->current->payload = NULL;
}

  /*!

     @brief Get current cell in grid

     Returns a pointer to the data payload in the current cell of a grid

     @param grid    pointer to existing grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_get_cell(grid_s *grid)
{
  _cell *cell;

    // Sanity check parameters.
  assert(grid);

  cell = _grid_get_cell(grid);
  if (!cell) return NULL;

    // Return "void *"
  return _grid_get_payload(cell);
}

  /*!

     @brief Assign payload data to a cell

     Assigns, or re-assigns the payload data for a cell in a grid.

     NOTE:  The current cell payload data, if any, will be de-allocated.

     @param grid    pointer to exising grid
     @param payload    pointer to new payload data

     @retval NONE

  */

void grid_set_cell(grid_s *grid, void *payload)
{
  _grid_internals *gin;
  
    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return;

  grid_clear_cell(grid);

  gin->current->payload = payload;
}

  /*!

     @brief Find a cell by reference

     Locates a cell in a grid based on a reference pointer value.  If the 
     cell is located, the current grid cell is set to the found cell,
     and the cell payload data is returned.

     @param grid    pointer to existing grid
     @param reference    pointer to payload data that may exist in grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_find_by_reference(grid_s *grid, void *reference)
{
  _grid_internals *gin;
  _cell *c;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_find_by_reference(grid, reference);
  if (c)
  {
    gin->current = c;
      // Return "void *"
  return c;
  }

    // Return NULL
  return NULL;
}

  /*!

     @brief Find a cell by value

     Locates a cell in a grid by value.  If the cell is located, the
     current grid cell is set to the found cell, and the payload data is
     returned.

     NOTE:  A user defined cell compare function must be supplied.

     @param grid    pointer to existing grid
     @param value    pointer to value to find
     @param func    pointer to function to compare cell values

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_find_by_value(grid_s *grid,
                         void *value,
                         grid_payload_compare func)
{
  _grid_internals *gin;
  _cell *c;

    // Sanity check parameters.
  assert(grid);
  assert(value);
  assert(func);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_find_by_value(grid, value, func);
  if (c)
  {
    gin->current = c;
      // Return "void *"
  return c;
  }

    // Return NULL
  return NULL;
}

  /*!

     @brief Located first cell in grid

     Locates the first cell in the grid, and returns the payload data of
     the origin cell.  The current grid cell is set to the origin cell.

     @param grid    pointer to existing grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_origin(grid_s *grid)
{
  _grid_internals *gin;
  void *pl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  pl = _grid_get_payload(gin->origin);

  gin->current = gin->origin;
  vertex_set_y(gin->location, 0);
  vertex_set_x(gin->location, 0);

    // Return "void *"
  return pl;
}

  /*!

     @brief Locate current cell in grid

     Returns the payload data of the current cell in a grid.

     @param grid    pointer to existing grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_current(grid_s *grid)
{
  _grid_internals *gin;
  void *pl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  pl = _grid_get_payload(gin->current);

    // Return "void *"
  return pl;
}

  /*!

     @brief Locate the last (lower, right) cell in a grid

     Locates the last cell in a grid.   The last cell is the lower, right
     most cell in the grid.  Returns the payload data of that cell.  The
     current grid cell is set to the last cell.

     @param grid    pointer to existing cell

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_end(grid_s *grid)
{
  _grid_internals *gin;
  void *pl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  pl = _grid_get_payload(gin->end);

  gin->current = gin->end;
  vertex_set_y(gin->location, grid_size_get_height(gin->size) - 1);
  vertex_set_x(gin->location, grid_size_get_width(gin->size) - 1);

    // Return "void *"
  return pl;
}

  /*!

     @brief Move one cell to the left in a grid

     Moves the current cell location in a grid one cell to the left.  Returns
     the payload data of that cell.  If the current cell location is already
     a the left most position, then no action is taken, and nothing is
     returned.

     @param grid    pointer to existing grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_left(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->left) return NULL;

  pl = _grid_get_payload(c->left);

  gin->current = c->left;
  vertex_set_x(gin->location, vertex_get_x(gin->location) - 1);

    // Return "void *"
  return pl;
}

  /*!

     @brief Move one cell to the right in a grid

     Moves the current cell location in a grid one cell to the right.  Returns
     the payload data of that cell.  If the current cell location is already
     a the right most position, then no action is taken, and nothing is
     returned.

     @param grid    pointer to existing grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_right(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->right) return NULL;

  pl = _grid_get_payload(c->right);

  gin->current = c->right;
  vertex_set_x(gin->location, vertex_get_x(gin->location) + 1);

    // Return "void *"
  return pl;
}

  /*!

     @brief Move up one cell in a grid

     Moves the current cell location in a grid up one cell.  Returns
     the payload data of that cell.  If the current cell location is already
     a the top most position, then no action is taken, and nothing is
     returned.

     @param grid    pointer to existing grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_up(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->up) return NULL;

  pl = _grid_get_payload(c->up);

  gin->current = c->up;
  vertex_set_y(gin->location, vertex_get_y(gin->location) - 1);

    // Return "void *"
  return pl;
}

  /*!

     @brief Move down one one cell in a grid

     Moves the current cell location in a grid down one cell.  Returns
     the payload data of that cell.  If the current cell location is already
     a the bottom most position, then no action is taken, and nothing is
     returned.

     @param grid    pointer to existing grid

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_down(grid_s *grid)
{
  _grid_internals *gin;
  _cell *c;
  void *pl;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

  c = _grid_get_cell(grid);
  if (!c) return NULL;

  if (!c->down) return NULL;

  pl = _grid_get_payload(c->down);

  gin->current = c->down;
  vertex_set_y(gin->location, vertex_get_y(gin->location) + 1);

    // Return "void *"
  return pl;
}

  /*!

     @brief Move to specific cell location in a grid

     Moves current cell location in a grid to a specific row and column.  The
     payload data of that cell is returned.  If the new location is outside
     the current grid boundaries, then no action is taken, and nothing is
     returned.

     NOTE:  Cell locations in a grid are based at 1, therefore the first
            (origin) cell location is 1, 1.

     @param grid    pointer to existing grid
     @param row    row number of new cell location
     @param col    column number of new cell location

     @retval "void *" success
     @retval NULL    failure

  */

void *grid_goto(grid_s *grid, int row, int col)
{
  _grid_internals *gin;
  grid_size_s *size;
  _cell *c;
  int i;
  int dir;

    // Sanity check parameters.
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

    // Return "void *"
  return _grid_get_payload(c);
}

// STATIC functions

  /*!

     @brief INTERNAL: Create a new cell

     Allocates a new _cell structure.

     @param pl    pointer to payload data for new cell

     @retval "_cell *" success

  */

static _cell *_cell_new(void *pl)
{
  _cell *c;

  c = (_cell*)malloc(sizeof(_cell));
  memset(c, 0, sizeof(_cell));

  c->payload = pl;

    // Return "_cell *"
  return c;
}

  /*!

     @brief INTERNAL:  De-allocate a cell and possibly payload data

     De-allocates an existing cell, and possibly a the payload data
     associated with the cell.  If no payload data destructor function
     is supplied, then the payload data is left intact.

     @param c    pointer to existing cell
     @param fpl    pointer to user defined payload data destructor

     @retval "_cell *" success

  */

static void _cell_free(_cell *c, grid_payload_free fpl)
{
    // Sanity check parameters.
  assert(c);
  if (fpl) fpl(c->payload);
  free(c);
}

  /*!

     @brief INTERNAL: Get payload data destructor function

     Returns the previously assigned payload data destructor.

     @param grid    pointer to existing grid

     @retval "grid_payload_free *" success

  */

static grid_payload_free _grid_get_pl_free(grid_s *grid)
{
  _grid_internals *gin;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

    // Return "grid_payload_free *"
  return gin->grid_pl_free;
}

  /*!

     @brief INTERNAL:  Get grid internals structure

     Returns the internals structure associated with a grid

     @param grid    pointer to existing grid

     @retval "_grid_internals *" success

  */

static _grid_internals *_grid_get_internals(grid_s *grid)
{
    // Sanity check parameters.
  assert(grid);
    // Return "_grid_internals *"
  return (_grid_internals *)grid->_internals;
}

  /*!

     @brief INTERNAL:  Get payload data from a cell

     Returns the payload data associated with a cell.

     @param c    pointer to existing cell

     @retval "void *" success

  */

static void *_grid_get_payload(_cell *c)
{
    // Sanity check parameters.
  assert(c);
    // Return "void *"
  return c->payload;
}

  /*!

     @brief INTERNAL:  Find a cell by reference

     Locates a cell in a grid based on a reference pointer value.  If the 
     cell is located, the current grid cell is set to the found cell,
     and the cell payload data is returned.

     NOTE:  This function does the actual work of finding a cell by
            reference.  The public API function above is a wrapper around
            this function.

     NOTE:  This function differs from the public API version by returning
            the entire internal cell structure, not just the payload data.

     @param grid    pointer to existing grid
     @param pl    pointer to payload data that may exist in grid

     @retval "_cell *" success
     @retval NULL    failure

  */

static _cell *_grid_find_by_reference(grid_s *grid, void *pl)
{
  _cell *c;
  _cell *c2;
  _grid_internals *gin;
  int x, y;

    // Sanity check parameters.
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
          // Return "_cell *"
        return c2;
      }
  }

    // Return NULL
  return NULL;
}

  /*!

     @brief INTERNAL:  Find a cell by value

     Locates a cell in a grid by value.  If the cell is located, the
     current grid cell is set to the found cell, and the payload data is
     returned.

     NOTE:  A user defined cell compare function must be supplied.

     NOTE:  This function does the actual work of finding a cell by
            value.  The public API function above is a wrapper around
            this function.

     NOTE:  This function differs from the public API version by returning
            the entire internal cell structure, not just the payload data.

     @param grid    pointer to existing grid
     @param pl    pointer to payload data that may exist in grid
     @param cf    pointer to function to compare cell values

     @retval "_cell *" success
     @retval NULL    failure

  */

static _cell *_grid_find_by_value(grid_s *grid,
                                  void *pl,
                                  grid_payload_compare cf)
{
  _cell *c;
  _cell *c2;
  _grid_internals *gin;
  int x, y;

    // Sanity check parameters.
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
          // Return "_cell *"
        return c2;
      }
  }

    // Return NULL
  return NULL;
}

  /*!

     @brief INTERNAL:  Get current cell

     Returns the cell structure at the current grid cell location.

     @param grid    pointer to existing grid

     @retval "_cell *" success

  */

static _cell *_grid_get_cell(grid_s *grid)
{
  _grid_internals *gin;

    // Sanity check parameters.
  assert(grid);

  gin = _grid_get_internals(grid);
  if (!gin) return NULL;

    // Return "_cell *"
  return gin->current;
}

