/*!
    @file grid-api.c

    @brief SOURCE_BRIEF

    @timestamp Wed, 20 Aug 2014 03:18:03 +0000

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

    @file grid-api.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

  // Required system headers

#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "grid-api.h"

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct _grid_api_internals
{
  grid_s *grid;
  grid_api_data_func edit_func;
  grid_api_data_func show_func;
  grid_api_data_func delete_func;
  grid_api_data_func copy_func;
  grid_api_data_func paste_func;
} _grid_api_internals;

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

_grid_api_internals *_grid_api_get_internals(grid_api_s *ga);
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_edit_func(grid_api_s *ga);
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_show_func(grid_api_s *ga);
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_delete_func(grid_api_s *ga);
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_copy_func(grid_api_s *ga);
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_paste_func(grid_api_s *ga);

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

_grid_api_internals *_grid_api_get_internals(grid_api_s *ga)
{
    // Sanity check parameters.
  assert(ga);
    // Return RETVAL
  return (ga->_internals);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_edit_func(grid_api_s *ga)
{
  _grid_api_internals *gain;
    // Sanity check parameters.
  assert(ga);
  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;
    // Return RETVAL
  return gain->edit_func;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_show_func(grid_api_s *ga)
{
  _grid_api_internals *gain;
    // Sanity check parameters.
  assert(ga);
  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;
    // Return RETVAL
  return gain->show_func;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_delete_func(grid_api_s *ga)
{
  _grid_api_internals *gain;
    // Sanity check parameters.
  assert(ga);
  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;
    // Return RETVAL
  return gain->delete_func;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_copy_func(grid_api_s *ga)
{
  _grid_api_internals *gain;
    // Sanity check parameters.
  assert(ga);
  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;
    // Return RETVAL
  return gain->copy_func;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_data_func _grid_api_get_paste_func(grid_api_s *ga)
{
  _grid_api_internals *gain;
    // Sanity check parameters.
  assert(ga);
  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;
    // Return RETVAL
  return gain->paste_func;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_s *grid_api_create(void)
{
  grid_api_s *s;

  s = (grid_api_s *)malloc(sizeof(grid_api_s));
  memset(s, 0, sizeof(grid_api_s));
  s->_internals = malloc(sizeof(_grid_api_internals));
  memset(s->_internals, 0, sizeof(_grid_api_internals));

  ((_grid_api_internals*)(s->_internals))->grid = grid_create();

    // Return RETVAL
  return s;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_api_destroy(grid_api_s *ga)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);

  gain = _grid_api_get_internals(ga);
  if (gain && gain->grid) grid_destroy(gain->grid);
  if (gain) free(gain);
  free(ga);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_api_set_free(grid_api_s *ga, grid_api_free_func f)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);
  assert(f);

  gain = _grid_api_get_internals(ga);
  grid_set_free(gain->grid, f);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_api_set_edit(grid_api_s *ga, grid_api_data_func f)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);
  assert(f);

  gain = _grid_api_get_internals(ga);
  gain->edit_func = f;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_api_set_show(grid_api_s *ga, grid_api_data_func f)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);
  assert(f);

  gain = _grid_api_get_internals(ga);
  gain->show_func = f;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_api_set_delete(grid_api_s *ga, grid_api_data_func f)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);
  assert(f);

  gain = _grid_api_get_internals(ga);
  gain->delete_func = f;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_api_set_copy(grid_api_s *ga, grid_api_data_func f)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);
  assert(f);

  gain = _grid_api_get_internals(ga);
  gain->copy_func = f;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_api_set_paste(grid_api_s *ga, grid_api_data_func f)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);
  assert(f);

  gain = _grid_api_get_internals(ga);
  gain->paste_func = f;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_api_status_s *grid_api_do(grid_api_s *ga,
                               grid_api_command_t cmd,
                               grid_api_data_u data)
{
  static void *_savedata = NULL;
  static grid_api_status_s stat;
  _grid_api_internals *grid_api;
  int i;
  void *fr;
  grid_api_data_func func;
  vertex_s *v;
  grid_size_s *size;

    // Sanity check parameters.
  assert(ga);

  memset(&stat, 0, sizeof(grid_api_status_s));

  grid_api = _grid_api_get_internals(ga);
  if (!grid_api)
  {
    stat.code = -1;
      // Return RETVAL
  return &stat;
  }

  switch (cmd)
  {
    case grid_api_command_nop:
      stat.code = 0;
      break;
    case grid_api_command_up:
      stat.code = 0;
      for (i = 0; i < data.repeat; i++)
        grid_up(grid_api->grid);
      break;
    case grid_api_command_down:
      stat.code = 0;
      for (i = 0; i < data.repeat; i++)
        grid_down(grid_api->grid);
      break;
    case grid_api_command_left:
      stat.code = 0;
      for (i = 0; i < data.repeat; i++)
        grid_left(grid_api->grid);
      break;
    case grid_api_command_right:
      stat.code = 0;
      for (i = 0; i < data.repeat; i++)
        grid_right(grid_api->grid);
      break;
    case grid_api_command_home:
      stat.code = 0;
      grid_origin(grid_api->grid);
      break;
    case grid_api_command_end:
      stat.code = 0;
      grid_end(grid_api->grid);
      break;
    case grid_api_command_goto:
      stat.code = 0;
      grid_goto(grid_api->grid,
                vertex_get_y(data.location) - 1,
                vertex_get_x(data.location) - 1);
      break;
    case grid_api_command_edit:
      stat.code = 0;
      fr = NULL;
      func = _grid_api_get_edit_func(ga);
      fr = grid_get_cell(grid_api->grid);
      if (func) fr = func(fr);
      if (!fr)
        stat.code = -1;
      else
        grid_set_cell(grid_api->grid, fr);
      break;
    case grid_api_command_show:
      stat.code = 0;
      fr = NULL;
      func = _grid_api_get_show_func(ga);
      if (func) fr = func(grid_get_cell(grid_api->grid));
      if (!fr) stat.code = -1;
      break;
    case grid_api_command_delete:
      stat.code = 0;
      fr = NULL;
      func = _grid_api_get_delete_func(ga);
      if (func) fr = func(grid_get_cell(grid_api->grid));
      if (!fr) stat.code = -1;
      if (fr) grid_clear_cell(grid_api->grid);
      break;
    case grid_api_command_copy:
      stat.code = 0;
      fr = NULL;
      func = _grid_api_get_copy_func(ga);
      if (func) fr = func(grid_get_cell(grid_api->grid));
      if (fr)
        _savedata = fr;
      else
        stat.code = -1;
      break;
    case grid_api_command_paste:
      stat.code = 0;
      fr = NULL;
      func = _grid_api_get_paste_func(ga);
      if (func) fr = func(_savedata);
      if (fr)
        grid_set_cell(grid_api->grid, fr);
      else
        stat.code = -1;
      break;
    case grid_api_command_new_row:
      stat.code = 0;
      v = grid_get_location(grid_api->grid);
      if (v && data.repeat)
      {
        if (data.repeat < 0)
        {
          data.repeat = abs(data.repeat);
          --v->y;
        }
        for (i = 0; i < data.repeat; i++)
          grid_create_row(grid_api->grid, v->y);
      }
      else
        stat.code = -1;
      break;
    case grid_api_command_del_row:
      stat.code = 0;
      v = grid_get_location(grid_api->grid);
      if (v && data.repeat)
      {
        --v->y;
        if (data.repeat < 0)
        {
          data.repeat = abs(data.repeat);
          v->y -= data.repeat;
          if (v->y < 0)
          {
            data.repeat += v->y;
            v->y = 0;
          }
        }
        for (i = 0; i < data.repeat; i++)
          grid_destroy_row(grid_api->grid, v->y);
      }
      else
        stat.code = -1;
      break;
    case grid_api_command_new_column:
      stat.code = 0;
      v = grid_get_location(grid_api->grid);
      if (v && data.repeat)
      {
        if (data.repeat < 0)
        {
          data.repeat = abs(data.repeat);
          --v->x;
        }
        for (i = 0; i < data.repeat; i++)
          grid_create_column(grid_api->grid, v->x);
      }
      else
        stat.code = -1;
      break;
    case grid_api_command_del_column:
      stat.code = 0;
      v = grid_get_location(grid_api->grid);
      if (v && data.repeat)
      {
        if (data.repeat < 0)
        {
          data.repeat = abs(data.repeat);
          --v->x;
          if (v->x < 0)
          {
            stat.code = -1;
            break;
          }
        }
        for (i = 0; i < data.repeat; i++)
          grid_destroy_column(grid_api->grid, v->x);
      }
      else
        stat.code = -1;
      break;
    case grid_api_command_set_size:
      stat.code = 0;
      size = grid_size_create();
      grid_size_set(size,
                    vertex_get_y(data.location),
                    vertex_get_x(data.location));
      grid_set_size(grid_api->grid, size);
      break;
    case grid_api_command_get_size:
      stat.code = 0;
      break;
    case grid_api_command_get_data:
      stat.code = 0;
      stat.data = grid_get_cell(grid_api->grid);
      if (!stat.data) stat.code = -1;
      break;
    case grid_api_command_set_data:
      stat.code = 0;
      if (data.data)
        grid_set_cell(grid_api->grid, data.data);
      else
        stat.code = -1;
      break;
  }

    // Get statistical information that accompanies all commands
  size = grid_get_size(grid_api->grid);
  if (!size)
    stat.code = -1;
  else
  {
    stat.rows = grid_size_get_height(size);
    stat.columns = grid_size_get_width(size);
  }

  stat.location = grid_get_location(grid_api->grid);

    // Return RETVAL
  return &stat;
}

