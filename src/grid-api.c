/*!
    @file grid-api.c

    @brief Source file for grid (matrix/spreadsheet) data API

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

    @file grid-api.c

    Source file for grid API

    A simplified API for managing a grid with a unified function call using
    a set of standard grid API commands.  This API also extends the standard
    grid functions with abilities such as repeating grid operations multiple
    times with one call, etc.

    The grid API also provides a generic interface for user supplied functions
    to edit, display, delete, copy and paste user data in each cell of the grid.

  */

  // Required system headers

#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "grid-api.h"

  /*!
    @brief Grid internals data structure
  */

typedef struct _grid_api_internals
{
    /*! @brief Pointer to external API grid structure */
  grid_s *grid;
    /*! @brief Pointer to user supplied cell payload data edit function */
  grid_api_data_func edit_func;
    /*! @brief Pointer to user supplied cell payload data display function */
  grid_api_data_func show_func;
    /*! @brief Pointer to user supplied cell payload data delete function */
  grid_api_data_func delete_func;
    /*! @brief Pointer to user supplied cell payload data copy function */
  grid_api_data_func copy_func;
    /*! @brief Pointer to user supplied cell payload data paste function */
  grid_api_data_func paste_func;
} _grid_api_internals;

  /*
    Internal function prototypes
  */

static _grid_api_internals *_grid_api_get_internals(grid_api_s *ga);
static grid_api_data_func _grid_api_get_edit_func(grid_api_s *ga);
static grid_api_data_func _grid_api_get_show_func(grid_api_s *ga);
static grid_api_data_func _grid_api_get_delete_func(grid_api_s *ga);
static grid_api_data_func _grid_api_get_copy_func(grid_api_s *ga);
static grid_api_data_func _grid_api_get_paste_func(grid_api_s *ga);

  /*!

     @brief Create a new grid API with grid

     Allocates and initializes all data structures associated with a grid
     API and its included grid.

     @retval "grid_api_s *" success
     @retval NULL    failure

  */

grid_api_s *grid_api_create(void)
{
  grid_api_s *s;

  s = (grid_api_s *)malloc(sizeof(grid_api_s));
  if (!s) return NULL;
  memset(s, 0, sizeof(grid_api_s));

  s->_internals = malloc(sizeof(_grid_api_internals));
  if (!s->_internals)
  {
    free(s);
    return NULL;
  }
  memset(s->_internals, 0, sizeof(_grid_api_internals));

  ((_grid_api_internals *)(s->_internals))->grid = grid_create();
  if (!((_grid_api_internals *)(s->_internals))->grid)
  {
    free(s->_internals);
    free(s);
    return NULL;
  }

    // Return "grid_api_s *"
  return s;
}

  /*!

     @brief Destroy a grid API

     De-allocate all allocated memory associated with a grid API structure.

     @param ga    pointer to existing grid API

     @retval NONE

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

     @brief Set grid payload data free function

     Set payload data memory de-allocation function for grid, which is user
     defined.

     @param ga    pointer to exising grid API
     @param f    pointer to user defined function

     @retval NONE

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

     @brief Set grid payload data edit function

     Set payload data editing function for grid, which is user
     defined.

     @param ga    pointer to exising grid API
     @param f    pointer to user defined function

     @retval NONE

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

     @brief Set grid payload data show function

     Set payload data display/report function for grid, which is user
     defined.

     @param ga    pointer to exising grid API
     @param f    pointer to user defined function

     @retval NONE

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

     @brief Set grid payload data delete function

     Set payload data delete function for grid, which is user
     defined.

     @param ga    pointer to exising grid API
     @param f    pointer to user defined function

     @retval NONE

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

     @brief Set grid payload data copy function

     Set payload data copy function for grid, which is user defined.

     @param ga    pointer to exising grid API
     @param f    pointer to user defined function

     @retval NONE

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

     @brief Set grid payload data paste function

     Set payload data paste function for grid, which is user defined.

     @param ga    pointer to exising grid API
     @param f    pointer to user defined function

     @retval NONE

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

     @brief Main entry point for Grid API

     Command action function for Grid API.  Provides mechanism for all
     actions in a grid via the API.  The user supplies a grid command,
     and the data needed to execute the command.

     The following table describes each API command and data:

       Command                         Data
       ---------------------------     -------------------------------

       grid_api_command_nop            Nothing required
       grid_api_command_up             Repeat count
       grid_api_command_down           Repeat count
       grid_api_command_left           Repeat count
       grid_api_command_right          Repeat count
       grid_api_command_home           Nothing required
       grid_api_command_end            Nothing required
       grid_api_command_goto           Location vertex
       grid_api_command_edit           Nothing required
       grid_api_command_show           Nothing required
       grid_api_command_delete         Nothing required
       grid_api_command_copy           Nothing required
       grid_api_command_paste          Nothing required
       grid_api_command_new_row        Repeat count
       grid_api_command_del_row        Repeat count
       grid_api_command_new_column     Repeat count
       grid_api_command_del_column     Repeat count
       grid_api_command_get_size       Nothing required
       grid_api_command_set_size       Location vertex
       grid_api_command_get_data       Nothing required
       grid_api_command_set_data       User data

     @param ga    pointer to exising grid API
     @param cmd    API command to execute
     @param data    data needed to execute command

     @retval "grid_api_status *" success

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

    // Return "grid_api_status_s *"
  return &stat;
}

  /*!

     @brief INTERNAL: Get internals from Grid API structure

     Return the internals data structure from a Grid API structure.

     @param ga    pointer to existing grid API

     @retval "_grid_api_internals *" success
     @retval NULL    failure

  */

static _grid_api_internals *_grid_api_get_internals(grid_api_s *ga)
{
    // Sanity check parameters.
  assert(ga);
    // Return "_grid_api_internals *"
  return (ga->_internals);
}

  /*!

     @brief INTERNAL:  Get cell edit function

     Return grid cell payload data edit function from grid internals.

     @param ga    pointer to exising grid API

     @retval "grid_api_data_func" success
     @retval NULL    failure

  */

static grid_api_data_func _grid_api_get_edit_func(grid_api_s *ga)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);

  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;

    // Return "grid_api_data_func"
  return gain->edit_func;
}

  /*!

     @brief INTERNAL:  Get cell show function

     Return grid cell payload data show function from grid internals.

     @param ga    pointer to exising grid API

     @retval "grid_api_data_func" success
     @retval NULL    failure

  */

static grid_api_data_func _grid_api_get_show_func(grid_api_s *ga)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);

  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;

    // Return "grid_api_data_func"
  return gain->show_func;
}

  /*!

     @brief INTERNAL:  Get cell delete function

     Return grid cell payload data delete function from grid internals.

     @param ga    pointer to exising grid API

     @retval "grid_api_data_func" success
     @retval NULL    failure

  */

static grid_api_data_func _grid_api_get_delete_func(grid_api_s *ga)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);

  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;

    // Return "grid_api_data_func"
  return gain->delete_func;
}

  /*!

     @brief INTERNAL:  Get cell copy function

     Return grid cell payload data copy function from grid internals.

     @param ga    pointer to exising grid API

     @retval "grid_api_data_func" success
     @retval NULL    failure

  */

static grid_api_data_func _grid_api_get_copy_func(grid_api_s *ga)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);

  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;

    // Return "grid_api_data_func"
  return gain->copy_func;
}

  /*!

     @brief INTERNAL:  Get cell paste function

     Return grid cell payload data paste function from grid internals.

     @param ga    pointer to exising grid API

     @retval "grid_api_data_func" success
     @retval NULL    failure

  */

static grid_api_data_func _grid_api_get_paste_func(grid_api_s *ga)
{
  _grid_api_internals *gain;

    // Sanity check parameters.
  assert(ga);

  gain = _grid_api_get_internals(ga);
  if (!gain) return NULL;

    // Return "grid_api_data_func"
  return gain->paste_func;
}

