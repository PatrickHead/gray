/*!
    @file grid-size.h

    @brief HEADER_BRIEF

    @timestamp Tue, 19 Aug 2014 05:12:38 +0000

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

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef GRID_SIZE_H
#define GRID_SIZE_H

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct
{
    /*! brief ELEMENT_BRIEF */
  int width;
    /*! brief ELEMENT_BRIEF */
  int height;
} grid_size_s;

grid_size_s *grid_size_create(void);
void grid_size_destroy_void(void *v);
void grid_size_destroy(grid_size_s *gs);
grid_size_s *grid_size_copy(grid_size_s *gs);

void grid_size_set(grid_size_s *gs, int width, int height);
void grid_size_get(grid_size_s *gs, int *width, int *height);

int grid_size_get_width(grid_size_s *gs);
void grid_size_set_width(grid_size_s *gs, int width);

int grid_size_get_height(grid_size_s *gs);
void grid_size_set_height(grid_size_s *gs, int height);

char *grid_size2str(grid_size_s *gs);
grid_size_s *str2grid_size(char *s);

#endif // GRID_SIZE_H
