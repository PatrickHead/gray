/*!
    @file grid-size.c

    @brief Source for grid size data management routines

    @timestamp Tue, 07 Oct 2014 07:32:14 +0000

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

    @file grid-size.c

    Source file for grid size data, a support structure for grid data.

    "grid size" is a simple structure describing the width and height of a grid

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "grid-size.h"

  /*!

     @brief Create "grid size" structure

     Allocates and returns a "grid size" structure.
     Defaults:
       width is 0
       height is 0

     @retval "grid_size_s *" success
     @retval NULL    failure

  */

grid_size_s *grid_size_create(void)
{
  grid_size_s *gs;

  gs = (grid_size_s *)malloc(sizeof(grid_size_s));
  if (gs) memset(gs, 0, sizeof(grid_size_s));

    // Return RETVAL
  return gs;
}

  /*!

     @brief Destroy a "grid size" structure (using void reference)

     Un-allocates all memory associated with a "grid size" structure, using
     a "void *" reference.

     NOTE:  This "void *" version is possibly used by generic structured data
            handlers, such as lists, grids, etc.

     @param v    (void *) - address of grid_size_s structure

     @retval NONE

  */

void grid_size_destroy_void(void *v)
{
    // Sanity check parameters.
  assert(v);

    // Call type specific destroy function
  grid_size_destroy((grid_size_s *)v);
}

  /*!

     @brief Destroy a "grid size" structure

     Un-allocates all memory associated with a "grid size" structure.

     @param gs    (grid_size_s *) - address of grid_size_s structure

     @retval NONE

  */

void grid_size_destroy(grid_size_s *gs)
{
    // Sanity check parameters.
  assert(gs);

  free(gs);
}

  /*!

     @brief Copy an existing "grid size" structure

     Makes a deep copy of a "grid size" data structure.

     @param gs    pointer to existing "grid size" structure

     @retval "grid_size_s *" success
     @retval NULL    failure

  */

grid_size_s *grid_size_copy(grid_size_s *gs)
{
  grid_size_s *ngs;

    // Sanity check parameters.
  assert(gs);

  ngs = grid_size_create();
  if (!ngs) return NULL;

  grid_size_set_width(ngs, grid_size_get_width(gs));
  grid_size_set_height(ngs, grid_size_get_height(gs));

    // Return "grid_size_s *"
  return ngs;
}

  /*!

     @brief Set all elements of a "grid size" structure

     A comprehensive "setter" for all elements of a "grid size" structure.

     @param gs      pointer to existing "grid size" structure
     @param width   width of grid
     @param height  height of grid

     @retval NONE

  */

void grid_size_set(grid_size_s *gs, int width, int height)
{
    // Sanity check parameters.
  assert(gs);

  grid_size_set_width(gs, width);
  grid_size_set_height(gs, height);
}

  /*!

     @brief Get all elements of "grid size" structure

     A comprehensive "getter" for all elements of a "grid size" structure.

     @param gs      pointer to an existing "grid size" structure
     @param width   pointer to int
     @param height  pointer to int

     @retval NONE

  */

void grid_size_get(grid_size_s *gs, int *width, int *height)
{
    // Sanity check parameters.
  assert(gs);

  *width = grid_size_get_width(gs);
  *height = grid_size_get_height(gs);
}

  /*!

     @brief Set width of "grid size" structure.

     Sets the width of a "grid size" structure.

     @param gs     pointer to existing "grid size" structure
     @param width  width of grid

     @retval NONE

  */

void grid_size_set_width(grid_size_s *gs, int width)
{
    // Sanity check parameters.
  assert(gs);

  if (width < 0) width = 0;

  gs->width = width;
}

  /*!

     @brief Returns width from "grid size" structure

     Returns the width value from a "grid size" structure.

     @param gs   pointer to existing "grid size" structure

     @retval "int" success

  */

int grid_size_get_width(grid_size_s *gs)
{
    // Sanity check parameters.
  assert(gs);
    // Return width of grid
  return gs->width;
}

  /*!

     @brief Set height of "grid size" structure.

     Sets the height of a "grid size" structure.

     @param gs      pointer to existing "grid size" structure
     @param height  width of grid

     @retval NONE

  */

void grid_size_set_height(grid_size_s *gs, int height)
{
    // Sanity check parameters.
  assert(gs);

  if (height < 0) height = 0;

  gs->height = height;
}

  /*!

     @brief Returns height from "grid size" structure

     Returns the height value from a "grid size" structure.

     @param gs   pointer to existing "grid size" structure

     @retval "int" success

  */

int grid_size_get_height(grid_size_s *gs)
{
    // Sanity check parameters.
  assert(gs);
    // Return RETVAL
  return gs->height;
}

  /*!

     @brief Convert a string into a "grid size" structure.

     Converts a specifically formatted string into a "grid size" structure.

     The string is in the following format:

        [W,H]

     where:

       W is width of grid
       H is height of grid

     @param s    pointer to formatted "grid size" defining string

     @retval "grid_size_s *" success
     @retval NULL

  */

grid_size_s *str2grid_size(char *s)
{
  grid_size_s *gs;

    // Sanity check parameters.
  assert(s);

  gs = grid_size_create();
  if (!gs) return NULL;

  sscanf(s, "[%d,%d]", &gs->width, &gs->height);

    // Return "grid_size_s *"
  return gs;
}

  /*!

     @brief Converts a "grid size" structure to a formatted string

     Creates a specifically formatted string that represents the data contained
     in a "grid size" structure.

     NOTE:  See str2grid_size for definition of string
     NOTE:  returned string is statically allocated, and this function is NOT
            re-entrant friendly.

     @param gs    pointer to existing "grid size" structure

     @retval "char *"

  */

char *grid_size2str(grid_size_s *gs)
{
  static char s[45];

    // Sanity check parameters.
  assert(gs);

  sprintf(s, "[%d,%d]\n",
    grid_size_get_width(gs),
    grid_size_get_height(gs));

    // Return "char *"
  return s;
}

