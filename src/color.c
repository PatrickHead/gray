/*!
    @file color.c

    @brief Source file for color data management routines

    @timestamp Mon, 06 Oct 2014 13:42:48 +0000

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

    Source file for color data structure management routines

    A simple color data storage and management system.  The color can have
    a unique name, or tag.  This name can be used to identify the color itself
    or to identify the purpose of the color, such as the name of a structure
    or XML element.

    Each color is stored in the standard RGBA (Red-Green-Blue-Alpha Channel)
    format.  All color values are stored as double precision floats.  The RGB
    values can be in the range of 0.0 through 255.0.  The Alpha Channel value
    an be in the range of 0.0 through 1.0.  When Alpha is 0.0, the color is
    considered to be 'none'.

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

  // Project related headers

#include "color.h"

  /*!

     @brief Create a color structure

     Allocates memory for and inititalizes a color structure.

     @param PARMNAME    PARM_DESCRIPTION

     @retval "color_s *" success
     @retval NULL    failure

  */

color_s *color_create(void)
{
  color_s *c;

  c = (color_s *)malloc(sizeof(color_s));
  if (!c) return NULL;

  memset(c, 0, sizeof(color_s));

  color_set_tag(c, "AUTO");

    // Return pointer to new color_s structure
  return c;
}

  /*!

     @brief Destroy existing color structure

     Frees ALL memory associated with a color structure (deep memory deallocate)

     @param c    "color_s *" - pointer to existing color structure

     @retval NONE

  */

void color_destroy(color_s *c)
{
    // Sanity check parameters.
  assert(c);

  if (c->tag) free(c->tag);
  free(c);
}

  /*!

     @brief Copy an existing color structure

     Performs a deep copy of an existing color structure.

     @param c    "color_s *" - pointer to existing color structure

     @retval "color_s *" success
     @retval NULL    failure

  */

color_s *color_copy(color_s *c)
{
  color_s *nc;

    // Sanity check parameters.
  assert(c);

  nc = color_create();
  if (!nc) return NULL;

  memcpy(nc, c, sizeof(color_s));

  if (c->tag) nc->tag = strdup(c->tag);

    // Return RETVAL
  return nc;
}

  /*!

     @brief Set all elements of a color structure

     A comprehensive "setter" for all elements of a color structure.

     RGB color portions of a color are doubles in the range of (0.0 thru 255.0).
     The A (Alpha channel) portion of a color is in the range of (0.0 thru 1.0).

     @param c    pointer to existing color structure
     @param tag  pointer to string containing a tag (name) for color
     @param r    red portion of RGBA color
     @param g    green portion of RGBA color
     @param b    blue portion of RGBA color
     @param a    alpha portion of RGBA color

     @retval NONE

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

     @brief Get all elements of color structure

     A comprehensive "getter" for all elements of a color structure.

     @param c    pointer to an existing color structure
     @param tag  pointer to char * (contains tag string on return)
     @param r    pointer to double (contains red component of color on return)
     @param g    pointer to double (contains green component of color on return)
     @param b    pointer to double (contains blue component of color on return)
     @param a    pointer to double (contains alpha component of color on return)

     @retval NONE

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

     @brief Set color components of color structure

     Sets all color components of a color structure.

     RGB color portions of a color are doubles in the range of (0.0 thru 255.0).
     The A (Alpha channel) portion of a color is in the range of (0.0 thru 1.0).

     @param c    pointer to existing color structure
     @param r    red component of color
     @param g    green component of color
     @param b    blue component of color
     @param a    alpha component of color

     @retval NONE    failure

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

     @brief Get color components of color structure

     Gets all color components of a color structure.

     @param c    pointer to existing color structure
     @param r    pointer to double (contains red component of color on return)
     @param g    pointer to double (contains green component of color on return)
     @param b    pointer to double (contains blue component of color on return)
     @param a    pointer to double (contains alpha component of color on return)

     @retval NONE    failure

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

     @brief Set tag (name) of color structure

     Set tag (name) of color structure.

     If the color structure already contains a tag, that string will be
     de-allocated first.  The passed tag will be duplicated, therefore the
     passed tag can be a literal or dynamic string.

     @param c    pointer to existing color structure
     @param tag  pointer to string containing new tag value

     @retval NONE

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

     @brief Get tag (name) of color structure

     Returns the tag of a color structure.

     @param c    pointer to existing color structure

     @retval "char *" success

  */

char *color_get_tag(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return "char *" containing tag of color structure
  return c->tag;
}

  /*!

     @brief Set red component of color structure.

     Sets the red component of a color structure.  (Range is 0.0 - 255.0)

     @param c    pointer to existing color structure
     @param r    red component of color structure

     @retval NONE

  */

void color_set_r(color_s *c, double r)
{
    // Sanity check parameters.
  assert(c);
  c->r = r;
}

  /*!

     @brief Returns red component from color structure

     Returns the red component value from a color structure.

     @param c    pointer to existing color structure

     @retval "double" success

  */

double color_get_r(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return red component of color structure
  return c->r;
}

  /*!

     @brief Set green component of color structure.

     Sets the green component of a color structure.  (Range is 0.0 - 255.0)

     @param c    pointer to existing color structure
     @param g    green component of color structure

     @retval NONE

  */

void color_set_g(color_s *c, double g)
{
    // Sanity check parameters.
  assert(c);
  c->g = g;
}

  /*!

     @brief Returns green component from color structure

     Returns the green component value from a color structure.

     @param c    pointer to existing color structure

     @retval "double" success

  */

double color_get_g(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->g;
}

  /*!

     @brief Set blue component of color structure.

     Sets the blue component of a color structure.  (Range is 0.0 - 255.0)

     @param c    pointer to existing color structure
     @param b    blue component of color structure

     @retval NONE

  */

void color_set_b(color_s *c, double b)
{
    // Sanity check parameters.
  assert(c);
  c->b = b;
}

  /*!

     @brief Returns blue component from color structure

     Returns the blue component value from a color structure.

     @param c    pointer to existing color structure

     @retval "double" success

  */

double color_get_b(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->b;
}

  /*!

     @brief Set alpha component of color structure.

     Sets the alpha component of a color structure.  (Range is 0.0 - 1.0)

     @param c    pointer to existing color structure
     @param a    alpha component of color structure

     @retval NONE

  */

void color_set_a(color_s *c, double a)
{
    // Sanity check parameters.
  assert(c);
  c->a = a;
}

  /*!

     @brief Returns alpha component from color structure

     Returns the alpha component value from a color structure.

     @param c    pointer to existing color structure

     @retval "double" success

  */

double color_get_a(color_s *c)
{
    // Sanity check parameters.
  assert(c);
    // Return RETVAL
  return c->a;
}

  /*!

     @brief Convert a string into a color structure.

     Converts a specifically formatted string into a color structure.

     The string is in the following format:

        [R,G,B,A]

     where:

       R is red component of color
       G is green component of color
       B is blue component of color
       A is alpha component of color

     NOTE:  the tag element of the color is set to "AUTO"

     @param s    pointer to formatted color defining string

     @retval "color_s *" success
     @retval NULL

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

    // Return pointer to new color structure
  return c;
}

  /*!

     @brief Converts a color structure to a formatted string

     Creates a specifically formatted string that represents the data contained
     in a color structure.

     NOTE:  See str2color for definition of string
     NOTE:  returned string is statically allocated, and this function is NOT
            re-entrant friendly.

     @param c    pointer to existing color structure

     @retval "char *"

  */

char *color2str(color_s *c)
{
  static char s[80];

    // Sanity check parameters.
  assert(c);

  memset(s, 0, 80);
  snprintf(s, 80, "[%f,%f,%f,%f]\n", c->r, c->g, c->b, c->a);

    // Return pointer to static string containing formatted string
  return s;
}

