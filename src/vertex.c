/*!
    @file vertex.c

    @brief Source file for vertex data

    @timestamp Tue, 07 Oct 2014 05:03:53 +0000

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

    Source file for vertex data structure management

    As defined herein, a vertex is a structure containing data for a point in
    3 dimensional space.  A vertex also contains an options tag, which is
    intended to be used either as a distinct id for the vertex, or as a
    structure element name reference in a containing parent structure.

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "vertex.h"

  /*!

     @brief Create a vertex structure

     Allocate memory for and initialize a vertex structure.

     @retval "vertex_s *" success
     @retval NULL    failure

  */

vertex_s *vertex_create(void)
{
  vertex_s *v;

  v = (vertex_s *)malloc(sizeof(vertex_s));
  if (v) return NULL;
  memset(v, 0, sizeof(vertex_s));

    // Default tag
  vertex_set_tag(v, "AUTO");

    // Return "vertex_s *"
  return v;
}

  /*!

     @brief Destroy a vertex structure (using void reference)

     Un-allocate all memory associated with a vertex structure, using
     a "void *" reference.

     NOTE:  This "void *" version is possibly used by generic structured data
            handlers, such as lists, grids, etc.

     @param v    (void *) - address of grid_size_s structure

     @retval NONE

  */

void vertex_destroy_void(void *v)
{
    // Sanity check parameters
  assert(v);

    // Call type specific destroy function
  vertex_destroy((vertex_s *)v);
}

  /*!

     @brief Destroy a vertex structure

     Un-allocate all memory associated with a vertex structure.

     @param v    address of vertex structure

     @retval NONE

  */

void vertex_destroy(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);

  if (v->tag) free(v->tag);
  free(v);
}

  /*!

     @brief Copy an existing vertex structure

     Make a deep copy of a vertex data structure.

     @param v    pointer to existing vertex structure

     @retval "vertex_s *" success
     @retval NULL    failure

  */

vertex_s *vertex_copy(vertex_s *v)
{
  vertex_s *nv;

    // Sanity check parameters.
  assert(v);

  nv = vertex_create();
  memcpy(nv, v, sizeof(vertex_s));

  if (v->tag) nv->tag = strdup(v->tag);

    // Return "vertex_s *"
  return nv;
}

  /*!

     @brief Set all elements of a vertex structure

     A comprehensive "setter" for all elements of a vertex structure.

     @param v      pointer to existing vertex structure
     @param x   X ordinate
     @param y   Y ordinate
     @param z   Z ordinate

     @retval NONE

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

     @brief Get all elements of vertex structure

     A comprehensive "getter" for all elements of a vertex structure.

     @param v      pointer to an existing vertex structure
     @param tag   pointer to string
     @param x   pointer to double
     @param y   pointer to double
     @param z   pointer to double

     @retval NONE

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

     @brief Set tag of vertex

     Set the tag name of a vertex structure.

     @param v     pointer to existing vertex structure
     @param tag  tag name for vertex

     @retval NONE

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

     @brief Get tag from vertex

     Returns the tag name from a vertex structure.

     @param v   pointer to existing vertex structure

     @retval "char *" success

  */

char *vertex_get_tag(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return "char *"
  return v->tag;
}

  /*!

     @brief Set x parameter of vertex

     Set the x parameter of a vertex structure.

     @param v     pointer to existing vertex structure
     @param x     X parameter

     @retval NONE

  */

void vertex_set_x(vertex_s *v, double x)
{
    // Sanity check parameters.
  assert(v);
  v->x = x;
}

  /*!

     @brief Get x parameter from vertex

     Returns the x parameter from a vertex structure.

     @param v   pointer to existing vertex structure

     @retval "double" success

  */

double vertex_get_x(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return RETVAL
  return v->x;
}

  /*!

     @brief Set y parameter of vertex

     Set the y parameter of a vertex structure.

     @param v     pointer to existing vertex structure
     @param y     Y parameter

     @retval NONE

  */

void vertex_set_y(vertex_s *v, double y)
{
    // Sanity check parameters.
  assert(v);
  v->y = y;
}

  /*!

     @brief Get y parameter from vertex

     Returns the y parameter from a vertex structure.

     @param v   pointer to existing vertex structure

     @retval "double" success

  */

double vertex_get_y(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return RETVAL
  return v->y;
}

  /*!

     @brief Set z parameter of vertex

     Set the z parameter of a vertex structure.

     @param v     pointer to existing vertex structure
     @param z     Z parameter

     @retval NONE

  */

void vertex_set_z(vertex_s *v, double z)
{
    // Sanity check parameters.
  assert(v);
  v->z = z;
}

  /*!

     @brief Get z parameter from vertex

     Returns the z parameter from a vertex structure.

     @param v   pointer to existing vertex structure

     @retval "double" success

  */

double vertex_get_z(vertex_s *v)
{
    // Sanity check parameters.
  assert(v);
    // Return RETVAL
  return v->z;
}

  /*!

     @brief Convert a string into a vertex structure.

     Converts a specifically formatted string into a vertex structure.

     The string is in the following format:

        [X,Y,Z]

     where:

       X is X parameter
       Y is Y parameter
       Z is Z parameter

     NOTE:  "tag" will always be set to "AUTO"

     @param s    pointer to formatted vertex defining string

     @retval "vertex_s *" success
     @retval NULL

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

    // Return "vertex_s *"
  return v;
}

  /*!

     @brief Converts a vertex structure to a formatted string

     Creates a specifically formatted string that represents the data contained
     in a vertex structure.

     NOTE:  See str2vertex for definition of string
     NOTE:  returned string is statically allocated, and this function is NOT
            re-entrant friendly.

     @param v    pointer to existing vertex structure

     @retval "char *"

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

