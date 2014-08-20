/*!
    @file grid-size.c

    @brief SOURCE_BRIEF

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

    @file grid-size.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "grid-size.h"

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_size_s *grid_size_create(void)
{
  grid_size_s *gs;

  gs = (grid_size_s *)malloc(sizeof(grid_size_s));
  memset(gs, 0, sizeof(grid_size_s));

    // Return RETVAL
  return gs;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_size_destroy_void(void *v)
{
    // Sanity check parameters.
  assert(v);
  grid_size_destroy((grid_size_s *)v);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_size_destroy(grid_size_s *gs)
{
    // Sanity check parameters.
  assert(gs);
  free(gs);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

    // Return RETVAL
  return ngs;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_size_set(grid_size_s *gs, int width, int height)
{
    // Sanity check parameters.
  assert(gs);

  grid_size_set_width(gs, width);
  grid_size_set_height(gs, height);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_size_get(grid_size_s *gs, int *width, int *height)
{
    // Sanity check parameters.
  assert(gs);

  *width = grid_size_get_width(gs);
  *height = grid_size_get_height(gs);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_size_set_width(grid_size_s *gs, int width)
{
    // Sanity check parameters.
  assert(gs);

  if (width < 0) width = 0;

  gs->width = width;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

int grid_size_get_width(grid_size_s *gs)
{
    // Sanity check parameters.
  assert(gs);
    // Return RETVAL
  return gs->width;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void grid_size_set_height(grid_size_s *gs, int height)
{
    // Sanity check parameters.
  assert(gs);

  if (height < 0) height = 0;

  gs->height = height;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

int grid_size_get_height(grid_size_s *gs)
{
    // Sanity check parameters.
  assert(gs);
    // Return RETVAL
  return gs->height;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

grid_size_s *str2grid_size(char *s)
{
  grid_size_s *gs;

    // Sanity check parameters.
  assert(s);

  gs = grid_size_create();
  if (!gs) return NULL;

  sscanf(s, "[%d,%d]", &gs->width, &gs->height);

    // Return RETVAL
  return gs;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

char *grid_size2str(grid_size_s *gs)
{
  static char s[45];

    // Sanity check parameters.
  assert(gs);

  sprintf(s, "[%d,%d]\n",
    grid_size_get_width(gs),
    grid_size_get_height(gs));

    // Return RETVAL
  return s;
}

