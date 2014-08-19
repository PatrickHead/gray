/*!
    @file grid-api.h

    @brief HEADER_BRIEF

    @timestamp Mon, 06 Jan 2014 15:17:36 +0000

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

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef GRID_API_H
#define GRID_API_H

#include "grid.h"

  /*!
    brief TYPEDEF_BRIEF
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
    brief TYPEDEF_BRIEF
  */

typedef struct grid_api_status
{
    /*! brief ELEMENT_BRIEF */
  int code;
    /*! brief ELEMENT_BRIEF */
  int rows;
    /*! brief ELEMENT_BRIEF */
  int columns;
    /*! brief ELEMENT_BRIEF */
  vertex_s *location;
    /*! brief ELEMENT_BRIEF */
  void *data;
} grid_api_status_s;

  /*!
    brief TYPEDEF_BRIEF
  */

typedef union
{
    /*! brief ELEMENT_BRIEF */
  int repeat;
    /*! brief ELEMENT_BRIEF */
  vertex_s *location;
    /*! brief ELEMENT_BRIEF */
  void *data;
} grid_api_data_u;

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct grid_api
{
    /*! brief ELEMENT_BRIEF */
  void *_internals;
} grid_api_s;

  /*!
    brief TYPEDEF_BRIEF
  */

typedef void *(*grid_api_data_func)(void *data);
  /*!
    brief TYPEDEF_BRIEF
  */

typedef void (*grid_api_free_func)(void *data);

grid_api_s *grid_api_create(void);
void grid_api_destroy(grid_api_s *ga);
void grid_api_set_free(grid_api_s *ga, grid_api_free_func func);

void grid_api_set_edit(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_show(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_delete(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_copy(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_paste(grid_api_s *ga, grid_api_data_func func);

grid_api_status_s *grid_api_do(grid_api_s *ga,
                               grid_api_command_t command,
                               grid_api_data_u data);

#endif // GRID_API_H
