/*!
    @file grid-size.h

    @brief Header file for grid size data

    @timestamp Mon, 06 Oct 2014 14:34:40 +0000

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

    @file grid-size.h

    Header file for grid size data, a support structure for grid data.

    "grid size" is a simple structure describing the width and height of a grid

  */

#ifndef GRID_SIZE_H
#define GRID_SIZE_H

  /*!
    @brief TYPEDEF_BRIEF
  */

typedef struct
{
    /*! @brief width of grid */
  int width;
    /*! @brief height of grid */
  int height;
} grid_size_s;

  // grid_size function prototypes

    // Structure management functions 

grid_size_s *grid_size_create(void);
void grid_size_destroy_void(void *v);
void grid_size_destroy(grid_size_s *gs);
grid_size_s *grid_size_copy(grid_size_s *gs);

    // Comprehensive getter/setter

void grid_size_set(grid_size_s *gs, int width, int height);
void grid_size_get(grid_size_s *gs, int *width, int *height);

    // Individual element getters/setters

int grid_size_get_width(grid_size_s *gs);
void grid_size_set_width(grid_size_s *gs, int width);

int grid_size_get_height(grid_size_s *gs);
void grid_size_set_height(grid_size_s *gs, int height);

    // Conversion functions

char *grid_size2str(grid_size_s *gs);
grid_size_s *str2grid_size(char *s);

#endif // GRID_SIZE_H
