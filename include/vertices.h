/*!
    @file vertices.h

    @brief Header file for list of vertices data

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

    @file vertices.h

    Header file for list of vertices (see vertex.h) data management functions

    This module provides functions for managing a list of vertex data
    structures.  Base structure management functions are provided, as well
    as functions to add to a list, and copy an entire list.

  */

#ifndef VERTICES_H
#define VERTICES_H

#include "list.h"
#include "vertex.h"

  /*!
    @brief Vertices data structure
  */

typedef struct
{
    /*! @brief Pointer to list of vertices */
  list_s *vertices;
} vertices_s;

  // Vertices function prototypes

    // Structure management functions

vertices_s *vertices_create(void);
void vertices_destroy(vertices_s *vs);
vertices_s *vertices_copy(vertices_s *vs);

   // Element management functions

void vertices_add_vertex(vertices_s *vs, vertex_s *v);

#endif // VERTICES_H
