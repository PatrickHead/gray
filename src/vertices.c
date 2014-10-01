/*!
    @file vertices.c

    @brief Source file for list of vertices data

    @timestamp Wed, 01 Oct 2014 15:24:09 +0000

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

    @file vertices.c

    Source file for list of vertices (see vertex.h) data management functions

    This module provides functions for managing a list of vertex data
    structures.  Base structure management functions are provided, as well
    as functions to add to a list, and copy an entire list.

  */

  // Required system headers

#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "vertices.h"

  /*!

     @brief Create a vertices structure

     Allocate memory for and initialize a vertices structure.

     @retval "vertices_s *" success
     @retval NULL    failure

  */

vertices_s *vertices_create(void)
{
  vertices_s *vs;

  vs = (vertices_s *)malloc(sizeof(vertices_s));
  if (!vs) return NULL;
  memset(vs, 0, sizeof(vertices_s));

  vs->vertices = list_create();
  list_set_free(vs->vertices, vertex_destroy_void);

    // Return "vertices_s *"
  return vs;
}

  /*!

     @brief Destroy a vertices structure

     Un-allocate all memory associated with a vertices structure.

     @param vs    address of vertices structure

     @retval NONE

  */

void vertices_destroy(vertices_s *vs)
{
    // Sanity check parameters.
  assert(vs);

  if (vs->vertices) list_destroy(vs->vertices);

  free(vs);
}

  /*!

     @brief Copy an existing vertices structure

     Make a deep copy of a vertices data structure.

     @param vs    pointer to existing vertices structure

     @retval "vertices_s *" success
     @retval NULL    failure

  */

vertices_s *vertices_copy(vertices_s *vs)
{
  vertices_s *nvs;
  vertex_s *v;
  vertex_s *nv;

    // Sanity check parameters.
  assert(vs);

  nvs = vertices_create();
  memcpy(nvs, vs, sizeof(vertices_s));

  if (vs->vertices)
  {
    nvs->vertices = list_create();
    for (v = (vertex_s *)list_head(vs->vertices);
         v;
         v = (vertex_s *)list_next(vs->vertices))
    {
      nv = vertex_copy(v);
      if (nv) list_insert(nvs->vertices, (void *)nv, (void *)TAIL);
    }
  }

    // Return "vertices_s *"
  return nvs;
}

  /*!

     @brief Add vertex to vertices

     Add a new vertex item to a vertices list.

     @param vs    pointer to existing vertices
     @param v    pointer to vertex to add

     @retval NONE

  */

void vertices_add_vertex(vertices_s *vs, vertex_s *v)
{
  vertex_s *nv;

    // Sanity check parameters.
  assert(vs);
  assert(v);

  nv = vertex_copy(v);
  if (nv) list_insert(vs->vertices, nv, (void *)TAIL);
}

