/*!
    @file vertices.c

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

    @file vertices.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

  // Required system headers

#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "vertices.h"

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

vertices_s *vertices_create(void)
{
  vertices_s *vs;

  vs = (vertices_s *)malloc(sizeof(vertices_s));
  memset(vs, 0, sizeof(vertices_s));

  vs->vertices = list_create();
  list_set_free(vs->vertices, vertex_destroy_void);

    // Return RETVAL
  return vs;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertices_destroy(vertices_s *vs)
{
    // Sanity check parameters.
  assert(vs);

  if (vs->vertices) list_destroy(vs->vertices);

  free(vs);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

    // Return RETVAL
  return nvs;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

