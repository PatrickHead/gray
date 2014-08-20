/*!
    @file color.c

    @brief SOURCE_BRIEF

    @timestamp Wed, 20 Aug 2014 03:18:03 +0000

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

    @file color.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "color.h"

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

color_s *color_create(void)
{
  color_s *c;

  c = (color_s *)malloc(sizeof(color_s));
  memset(c, 0, sizeof(color_s));

  color_set_tag(c, "AUTO");

    // Return RETVAL
  return c;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_destroy(color_s *c)
{
    // Sanity check parameters.
  assert(c);

  if (c->tag) free(c->tag);
  free(c);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

color_s *color_copy(color_s *c)
{
  color_s *nc;

    // Sanity check parameters.
  assert(c);

  nc = color_create();
  memcpy(nc, c, sizeof(color_s));

  if (c->tag) nc->tag = strdup(c->tag);

    // Return RETVAL
  return nc;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_set(color_s *c, char *tag, double r, double g, double b, double a)
{
    // Sanity check parameters.
  assert(c);
  assert(tag);

  color_set_tag(c, tag);
  color_set_rgba(c, r, g, b, a);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_get(color_s *c,
               char **tag,
               double *r,
               double *g,
               double *b,
               double *a)
{
    // Sanity check parameters.
  assert(c);
  assert(tag);
  assert(r);
  assert(g);
  assert(b);
  assert(a);

  *tag = color_get_tag(c);
  color_get_rgba(c, r, g, b, a);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_set_rgba(color_s *c, double r, double g, double b, double a)
{
    // Sanity check parameters.
  assert(c);

  color_set_r(c, r);
  color_set_g(c, g);
  color_set_b(c, b);
  color_set_a(c, a);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_get_rgba(color_s *c, double *r, double *g, double *b, double *a)
{
    // Sanity check parameters.
  assert(c);
  assert(r);
  assert(g);
  assert(b);
  assert(a);

  *r = color_get_r(c);
  *g = color_get_g(c);
  *b = color_get_b(c);
  *a = color_get_a(c);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_set_tag(color_s *c, char *tag)
{
    // Sanity check parameters.
  assert(c);
  assert(tag);

  if (c->tag) free(c->tag);
  c->tag = strdup(tag);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

char *color_get_tag(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->tag;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_set_r(color_s *c, double r)
{
    // Sanity check parameters.
  assert(c);
  c->r = r;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

double color_get_r(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->r;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_set_g(color_s *c, double g)
{
    // Sanity check parameters.
  assert(c);
  c->g = g;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

double color_get_g(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->g;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_set_b(color_s *c, double b)
{
    // Sanity check parameters.
  assert(c);
  c->b = b;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

double color_get_b(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->b;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void color_set_a(color_s *c, double a)
{
    // Sanity check parameters.
  assert(c);
  c->a = a;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

double color_get_a(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->a;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

color_s *str2color(char *s)
{
  color_s *c;

    // Sanity check parameters.
  assert(s);

  c = color_create();
  if (!c) return NULL;

  color_set_tag(c, "AUTO");

  sscanf(s, "[%lf,%lf,%lf,%lf]", &c->r, &c->g, &c->b, &c->a);

    // Return RETVAL
  return c;
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

char *color2str(color_s *c)
{
  static char s[80];

    // Sanity check parameters.
  assert(c);

  memset(s, 0, 80);
  snprintf(s, 80, "[%f,%f,%f,%f]\n", c->r, c->g, c->b, c->a);

    // Return RETVAL
  return s;
}

