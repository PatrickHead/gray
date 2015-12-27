/*!
    @file grid-api.h

    @brief Header file for grid (matrix/spreadsheet) data API

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

    @file grid-api.h

    Header file for grid API

    A simplified API for managing a grid with a unified function call using
    a set of standard grid API commands.  This API also extends the standard
    grid functions with abilities such as repeating grid operations multiple
    times with one call, etc.

    The grid API also provides a generic interface for user supplied functions
    to edit, display, delete, copy and paste user data in each cell of the grid.

  */

#ifndef GRID_API_H
#define GRID_API_H

#include "grid.h"

  /*!
    @brief enum defining grid API commands
  */

typedef enum
{
  grid_api_command_nop = 0,
  grid_api_command_up,
  grid_api_command_down,
  grid_api_command_left,
  grid_api_command_right,
  grid_api_command_home,
  grid_api_command_end,
  grid_api_command_goto,
  grid_api_command_edit,
  grid_api_command_show,
  grid_api_command_delete,
  grid_api_command_copy,
  grid_api_command_paste,
  grid_api_command_new_row,
  grid_api_command_del_row,
  grid_api_command_new_column,
  grid_api_command_del_column,
  grid_api_command_get_size,
  grid_api_command_set_size,
  grid_api_command_get_data,
  grid_api_command_set_data
} grid_api_command_t;

  /*!
    @brief Structure to hold grid API status information
  */

typedef struct grid_api_status
{
    /*! @brief Status code */
  int code;
    /*! @brief Current number of rows in grid */
  int rows;
    /*! @brief Current number of columns in grid */
  int columns;
    /*! @brief Current "cursor" location (x,y address) in grid */
  vertex_s *location;
    /*! @brief Pointer to possible returned status data */
  void *data;
} grid_api_status_s;

  /*!
    @brief Union of data types used by grid_api_do()
  */

typedef union
{
    /*! @brief Command repeat count */
  int repeat;
    /*! @brief x,y address */
  vertex_s *location;
    /*! @brief Cell data */
  void *data;
} grid_api_data_u;

  /*!
    @brief Grid data structure
  */

typedef struct grid_api
{
    /*! @brief Pointer to internal API information (encapsulates interface) */
  void *_internals;
} grid_api_s;

  /*!
    @brief Function templates for user defined data and data free functions
  */

typedef void *(*grid_api_data_func)(void *data);
typedef void (*grid_api_free_func)(void *data);

  // grid API functions prototypes

    // grid API management functions

grid_api_s *grid_api_create(void);
void grid_api_destroy(grid_api_s *ga);
void grid_api_set_free(grid_api_s *ga, grid_api_free_func func);
void grid_api_set_edit(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_show(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_delete(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_copy(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_paste(grid_api_s *ga, grid_api_data_func func);

    // grid API command function

grid_api_status_s *grid_api_do(grid_api_s *ga,
                               grid_api_command_t command,
                               grid_api_data_u data);

#endif // GRID_API_H
