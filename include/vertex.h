/*!
    @file vertex.h

    @brief Header file for vertex data

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

    @file vertex.h

    Header file for vertex data structure management

    As defined herein, a vertex is a structure containing data for a point in
    3 dimensional space.  A vertex also contains an options tag, which is
    intended to be used either as a distinct id for the vertex, or as a
    structure element name reference in a containing parent structure.

  */

#ifndef VERTEX_H
#define VERTEX_H

  /*!
    @brief Vertex data structure
  */

typedef struct
{
    /*! @brief Unique identifier for vertex */
  char *tag;
    /*! @brief X ordinate of vertex */
  double x;
    /*! @brief Y ordinate of vertex */
  double y;
    /*! @brief Z ordinate of vertex */
  double z;
} vertex_s;

  // Vertex function prototypes

    // Structure management functions

vertex_s *vertex_create(void);
void vertex_destroy_void(void *v);
void vertex_destroy(vertex_s *v);
vertex_s *vertex_copy(vertex_s *v);

    // Comprehensive getter/setter

void vertex_set(vertex_s *v, char *tag, double x, double y, double z);
void vertex_get(vertex_s *v, char **tag, double *x, double *y, double *z);

    // Individual element getters/setters

void vertex_set_tag(vertex_s *v, char *tag);
char *vertex_get_tag(vertex_s *v);
void vertex_set_x(vertex_s *v, double x);
double vertex_get_x(vertex_s *v);
void vertex_set_y(vertex_s *v, double y);
double vertex_get_y(vertex_s *v);
void vertex_set_z(vertex_s *v, double z);
double vertex_get_z(vertex_s *v);

   // Conversion functions

vertex_s *str2vertex(char *s);
char *vertex2str(vertex_s *v);

#endif // VERTEX
