/*!
    @file vertex.c

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

    @file vertex.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "vertex.h"

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

vertex_s *vertex_create(void)
{
  vertex_s *v;

  v = (vertex_s *)malloc(sizeof(vertex_s));
  memset(v, 0, sizeof(vertex_s));

  vertex_set_tag(v, "AUTO");

    // Return RETVAL
  return v;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_destroy_void(void *v)
{
  vertex_destroy((vertex_s *)v);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_destroy(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);

  if (v->tag) free(v->tag);
  free(v);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

vertex_s *vertex_copy(vertex_s *v)
{
  vertex_s *nv;

    // Sanity check parameters.
  assert(v);

  nv = vertex_create();
  memcpy(nv, v, sizeof(vertex_s));

  if (v->tag) nv->tag = strdup(v->tag);

    // Return RETVAL
  return nv;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_set(vertex_s *v, char *tag, double x, double y, double z)
{
    // Sanity check parameters.
  assert(v);
  assert(tag);

  vertex_set_tag(v, tag);
  vertex_set_x(v, x);
  vertex_set_y(v, y);
  vertex_set_z(v, z);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_get(vertex_s *v, char **tag, double *x, double *y, double *z)
{
    // Sanity check parameters.
  assert(v);
  assert(tag);
  assert(x);
  assert(y);
  assert(z);

  *tag = vertex_get_tag(v);
  *x = vertex_get_x(v);
  *y = vertex_get_y(v);
  *z = vertex_get_z(v);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_set_tag(vertex_s *v, char *tag)
{
    // Sanity check parameters.
  assert(v);
  assert(tag);

  if (v->tag) free(v->tag);
  v->tag = strdup(tag);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

char *vertex_get_tag(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return RETVAL
  return v->tag;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_set_x(vertex_s *v, double x)
{
    // Sanity check parameters.
  assert(v);
  v->x = x;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

double vertex_get_x(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return RETVAL
  return v->x;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_set_y(vertex_s *v, double y)
{
    // Sanity check parameters.
  assert(v);
  v->y = y;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

double vertex_get_y(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return RETVAL
  return v->y;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void vertex_set_z(vertex_s *v, double z)
{
    // Sanity check parameters.
  assert(v);
  v->z = z;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

double vertex_get_z(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return RETVAL
  return v->z;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

vertex_s *str2vertex(char *s)
{
  vertex_s *v;

    // Sanity check parameters.
  assert(s);

  v = vertex_create();
  if (!v) return NULL;

  vertex_set_tag(v, "AUTO");

  sscanf(s, "[%lf,%lf,%lf]", &v->x, &v->y, &v->z);

    // Return RETVAL
  return v;
}

  // NOTE: returned string is statically allocated, and this function is NOT
  //       re-entrant friendly.
  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

char *vertex2str(vertex_s *v)
{
  static char s[40];

    // Sanity check parameters.
  assert(v);

  memset(s, 0, 40);
  snprintf(s, 40, "[%f,%f,%f]\n", v->x, v->y, v->z);

    // Return RETVAL
  return s;
}

