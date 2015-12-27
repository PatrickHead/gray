/*!
    @file grid.h

    @brief Header file for grid (matrix/spreadsheet) data

    @timestamp Fri, 09 Oct 2015 05:37:44 +0000

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

    @file grid.h

    Header file for grid structured data management

    A grid (matrix/spreadsheet) is a 2 dimensional orthoganol structured array
    of data cells.  Each cell contains a pointer to its neighboring cell to the
    left, right, above and below.  These pointers allow for easy navigation to
    any neighboring cell.   The data in each cell is generic, defined and
    managed by the calling program, and can contain any desired data, including
    another grid.

    Functions are provided for adding new rows and columns to the grid, as well
    as removing rows and columns.

  */

#ifndef GRID_H
#define GRID_H

  // Base type include file(s)

#include "grid-size.h"
#include "vertex.h"

  /*!
    @brief Grid data structure
  */

typedef struct
{
    /*! @brief Pointer to internal information (encapsulates interface) */
  void *_internals;
} grid_s;

  /*!
    @brief Function templates for user defined data compare and free functions
  */

typedef void (*grid_payload_free)(void *payload);
typedef int (*grid_payload_compare)(void *pl1, void *pl2);

  // Grid function prototypes

    // Structure managment functions 

grid_s *grid_create(void);
void grid_destroy(grid_s *g);
void grid_free(grid_s *g);
void grid_set_free(grid_s *g, grid_payload_free func);

    // Getters/setters

void grid_set_size(grid_s *g, grid_size_s *gs);
void grid_set_size_free_only(grid_s *g, grid_size_s *gs);
grid_size_s *grid_get_size(grid_s *g);
vertex_s *grid_get_location(grid_s *g);

    // Row/column management functions

void grid_create_row(grid_s *g, int row);
void grid_create_column(grid_s *g, int column);
void grid_free_row(grid_s *g, int row);
void grid_free_column(grid_s *g, int column);
void grid_destroy_row(grid_s *g, int row);
void grid_destroy_column(grid_s *g, int column);

    // Cell management functions

void grid_clear_cell(grid_s *g);
void *grid_get_cell(grid_s *g);
void grid_set_cell(grid_s *g, void *payload);

    // Cell search functions

void *grid_find_by_reference(grid_s *g, void *reference);
void *grid_find_by_value(grid_s *g,
                       void *value,
                       grid_payload_compare func);

    // Cell "cursor" movement functions

void *grid_origin(grid_s *g);
void *grid_current(grid_s *g);
void *grid_end(grid_s *g);
void *grid_left(grid_s *g);
void *grid_right(grid_s *g);
void *grid_up(grid_s *g);
void *grid_down(grid_s *g);
void *grid_goto(grid_s *g, int row, int column);

#endif // GRID_H
