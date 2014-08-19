/*!
    @file vertices.h

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

    @file vertices.h

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef VERTICES_H
#define VERTICES_H

#include "list.h"
#include "vertex.h"

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct
{
    /*! brief ELEMENT_BRIEF */
  list_s *vertices;
} vertices_s;

vertices_s *vertices_create(void);
void vertices_destroy(vertices_s *vs);
vertices_s *vertices_copy(vertices_s *vs);
void vertices_add_vertex(vertices_s *vs, vertex_s *v);

#endif // VERTICES_H
