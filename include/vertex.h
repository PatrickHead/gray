/*!
    @file vertex.h

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

    @file vertex.h

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef VERTEX_H
#define VERTEX_H

  /*!
    brief TYPEDEF_BRIEF
  */

typedef struct
{
    /*! brief ELEMENT_BRIEF */
  char *tag;
    /*! brief ELEMENT_BRIEF */
  double x;
    /*! brief ELEMENT_BRIEF */
  double y;
    /*! brief ELEMENT_BRIEF */
  double z;
} vertex_s;

vertex_s *vertex_create(void);
void vertex_destroy_void(void *v);
void vertex_destroy(vertex_s *v);
vertex_s *vertex_copy(vertex_s *v);

void vertex_set(vertex_s *v, char *tag, double x, double y, double z);
void vertex_get(vertex_s *v, char **tag, double *x, double *y, double *z);

void vertex_set_tag(vertex_s *v, char *tag);
char *vertex_get_tag(vertex_s *v);
void vertex_set_x(vertex_s *v, double x);
double vertex_get_x(vertex_s *v);
void vertex_set_y(vertex_s *v, double y);
double vertex_get_y(vertex_s *v);
void vertex_set_z(vertex_s *v, double z);
double vertex_get_z(vertex_s *v);

vertex_s *str2vertex(char *s);
char *vertex2str(vertex_s *v);

#endif // VERTEX

