/*!
    @file color.h

    @brief Header file for color data

    @timestamp Tue, 07 Oct 2014 04:43:28 +0000

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

    @file color.h

    Header file for color data structure management.

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

#ifndef COLOR_H
#define COLOR_H

  /*!
    @brief Color data structure.
  */

typedef struct
{
    /*! @brief Unique identifier for color */
  char *tag;
    /*! @brief Red value (0 - 255) */
  double r;
    /*! @brief Green value (0 - 255) */
  double g;
    /*! @brief Blue value (0 - 255) */
  double b;
    /*! @brief Alpha Channel (transparency) (0.0 - 1.0) */
  double a;
} color_s;

  // Macros to easily set a color structure to standard Web colors with
  // extensions for the X.org color palette.  These are the color names
  // that correspond to the HTML 5 standards.

  //Special colors for blue tools
#define color_none(c) color_set(c, "none", 0.0, 0.0, 0.0, 0.0);
#define color_is_none(c) ((!c->r)&&(!c->g)&&(!c->b)&&(!c->a)) ? 1 : 0;

  //Pink colors
#define color_pink(c) color_set_rgba(c, 255.0, 192.0, 203.0, 1.0);
#define color_lightpink(c) color_set_rgba(c, 255.0, 182.0, 193.0, 1.0);
#define color_hotpink(c) color_set_rgba(c, 255.0, 105.0, 180.0, 1.0);
#define color_deeppink(c) color_set_rgba(c, 255.0, 20.0, 147.0, 1.0);
#define color_palevioletred(c) color_set_rgba(c, 219.0, 112.0, 147.0, 1.0);
#define color_mediumvioletred(c) color_set_rgba(c, 199.0, 21.0, 133.0, 1.0);
  //Red colors
#define color_lightsalmon(c) color_set_rgba(c, 255.0, 160.0, 122.0, 1.0);
#define color_salmon(c) color_set_rgba(c, 250.0, 128.0, 114.0, 1.0);
#define color_darksalmon(c) color_set_rgba(c, 233.0, 150.0, 122.0, 1.0);
#define color_lightcoral(c) color_set_rgba(c, 240.0, 128.0, 128.0, 1.0);
#define color_indianred(c) color_set_rgba(c, 205.0, 92.0, 92.0, 1.0);
#define color_crimson(c) color_set_rgba(c, 220.0, 20.0, 60.0, 1.0);
#define color_firebrick(c) color_set_rgba(c, 178.0, 34.0, 34.0, 1.0);
#define color_darkred(c) color_set_rgba(c, 139.0, 0.0, 0.0, 1.0);
#define color_red(c) color_set_rgba(c, 255.0, 0.0, 0.0, 1.0);
  //Orange colors
#define color_orangered(c) color_set_rgba(c, 255.0, 69.0, 0.0, 1.0);
#define color_tomato(c) color_set_rgba(c, 255.0, 99.0, 71.0, 1.0);
#define color_coral(c) color_set_rgba(c, 255.0, 127.0, 80.0, 1.0);
#define color_darkorange(c) color_set_rgba(c, 255.0, 140.0, 0.0, 1.0);
#define color_orange(c) color_set_rgba(c, 255.0, 165.0, 0.0, 1.0);
  //Yellow colors
#define color_yellow(c) color_set_rgba(c, 255.0, 255.0, 0.0, 1.0);
#define color_lightyellow(c) color_set_rgba(c, 255.0, 255.0, 224.0, 1.0);
#define color_lemonchiffon(c) color_set_rgba(c, 255.0, 250.0, 205.0, 1.0);
#define color_lightgoldenrodyellow(c) color_set_rgba(c, 250.0, 250.0, 210.0, 1.0);
#define color_papayawhip(c) color_set_rgba(c, 255.0, 239.0, 213.0, 1.0);
#define color_moccasin(c) color_set_rgba(c, 255.0, 228.0, 181.0, 1.0);
#define color_peachpuff(c) color_set_rgba(c, 255.0, 218.0, 185.0, 1.0);
#define color_palegoldenrod(c) color_set_rgba(c, 238.0, 232.0, 170.0, 1.0);
#define color_khaki(c) color_set_rgba(c, 240.0, 230.0, 140.0, 1.0);
#define color_darkkhaki(c) color_set_rgba(c, 189.0, 183.0, 107.0, 1.0);
#define color_gold(c) color_set_rgba(c, 255.0, 215.0, 0.0, 1.0);
  //Brown colors
#define color_cornsilk(c) color_set_rgba(c, 255.0, 248.0, 220.0, 1.0);
#define color_blanchedalmond(c) color_set_rgba(c, 255.0, 235.0, 205.0, 1.0);
#define color_bisque(c) color_set_rgba(c, 255.0, 228.0, 196.0, 1.0);
#define color_navajowhite(c) color_set_rgba(c, 255.0, 222.0, 173.0, 1.0);
#define color_wheat(c) color_set_rgba(c, 245.0, 222.0, 179.0, 1.0);
#define color_burlywood(c) color_set_rgba(c, 222.0, 184.0, 135.0, 1.0);
#define color_tan(c) color_set_rgba(c, 210.0, 180.0, 140.0, 1.0);
#define color_rosybrown(c) color_set_rgba(c, 188.0, 143.0, 143.0, 1.0);
#define color_sandybrown(c) color_set_rgba(c, 244.0, 164.0, 96.0, 1.0);
#define color_goldenrod(c) color_set_rgba(c, 218.0, 165.0, 32.0, 1.0);
#define color_darkgoldenrod(c) color_set_rgba(c, 184.0, 134.0, 11.0, 1.0);
#define color_peru(c) color_set_rgba(c, 205.0, 133.0, 63.0, 1.0);
#define color_chocolate(c) color_set_rgba(c, 210.0, 105.0, 30.0, 1.0);
#define color_saddlebrown(c) color_set_rgba(c, 139.0, 69.0, 19.0, 1.0);
#define color_sienna(c) color_set_rgba(c, 160.0, 82.0, 45.0, 1.0);
#define color_brown(c) color_set_rgba(c, 165.0, 42.0, 42.0, 1.0);
#define color_maroon(c) color_set_rgba(c, 128.0, 0.0, 0.0, 1.0);
  //Green colors
#define color_darkolivegreen(c) color_set_rgba(c, 85.0, 107.0, 47.0, 1.0);
#define color_olive(c) color_set_rgba(c, 128.0, 128.0, 0.0, 1.0);
#define color_olivedrab(c) color_set_rgba(c, 107.0, 142.0, 35.0, 1.0);
#define color_yellowgreen(c) color_set_rgba(c, 154.0, 205.0, 50.0, 1.0);
#define color_limegreen(c) color_set_rgba(c, 50.0, 205.0, 50.0, 1.0);
#define color_lime(c) color_set_rgba(c, 0.0, 255.0, 0.0, 1.0);
#define color_lawngreen(c) color_set_rgba(c, 124.0, 252.0, 0.0, 1.0);
#define color_chartreuse(c) color_set_rgba(c, 127.0, 255.0, 0.0, 1.0);
#define color_greenyellow(c) color_set_rgba(c, 173.0, 255.0, 47.0, 1.0);
#define color_springgreen(c) color_set_rgba(c, 0.0, 255.0, 127.0, 1.0);
#define color_mediumspringgreen(c) color_set_rgba(c, 0.0, 250.0, 154.0, 1.0);
#define color_lightgreen(c) color_set_rgba(c, 144.0, 238.0, 144.0, 1.0);
#define color_palegreen(c) color_set_rgba(c, 152.0, 251.0, 152.0, 1.0);
#define color_darkseagreen(c) color_set_rgba(c, 143.0, 188.0, 143.0, 1.0);
#define color_mediumseagreen(c) color_set_rgba(c, 60.0, 179.0, 113.0, 1.0);
#define color_seagreen(c) color_set_rgba(c, 46.0, 139.0, 87.0, 1.0);
#define color_forestgreen(c) color_set_rgba(c, 34.0, 139.0, 34.0, 1.0);
#define color_green(c) color_set_rgba(c, 0.0, 128.0, 0.0, 1.0);
#define color_darkgreen(c) color_set_rgba(c, 0.0, 100.0, 0.0, 1.0);
  //Cyan colors
#define color_mediumaquamarine(c) color_set_rgba(c, 102.0, 205.0, 170.0, 1.0);
#define color_cyan(c) color_set_rgba(c, 0.0, 255.0, 255.0, 1.0);
#define color_aqua(c) color_set_rgba(c, 0.0, 255.0, 255.0, 1.0);
#define color_lightcyan(c) color_set_rgba(c, 224.0, 255.0, 255.0, 1.0);
#define color_paleturquoise(c) color_set_rgba(c, 175.0, 238.0, 238.0, 1.0);
#define color_aquamarine(c) color_set_rgba(c, 127.0, 255.0, 212.0, 1.0);
#define color_turquoise(c) color_set_rgba(c, 64.0, 224.0, 208.0, 1.0);
#define color_mediumturquoise(c) color_set_rgba(c, 72.0, 209.0, 204.0, 1.0);
#define color_darkturquoise(c) color_set_rgba(c, 0.0, 206.0, 209.0, 1.0);
#define color_lightseagreen(c) color_set_rgba(c, 32.0, 178.0, 170.0, 1.0);
#define color_cadetblue(c) color_set_rgba(c, 95.0, 158.0, 160.0, 1.0);
#define color_darkcyan(c) color_set_rgba(c, 0.0, 139.0, 139.0, 1.0);
#define color_teal(c) color_set_rgba(c, 0.0, 128.0, 128.0, 1.0);
  //Blue colors
#define color_lightsteelblue(c) color_set_rgba(c, 176.0, 196.0, 222.0, 1.0);
#define color_powderblue(c) color_set_rgba(c, 176.0, 224.0, 230.0, 1.0);
#define color_lightblue(c) color_set_rgba(c, 173.0, 216.0, 230.0, 1.0);
#define color_skyblue(c) color_set_rgba(c, 135.0, 206.0, 235.0, 1.0);
#define color_lightskyblue(c) color_set_rgba(c, 135.0, 206.0, 250.0, 1.0);
#define color_deepskyblue(c) color_set_rgba(c, 0.0, 191.0, 255.0, 1.0);
#define color_dodgerblue(c) color_set_rgba(c, 30.0, 144.0, 255.0, 1.0);
#define color_cornflowerblue(c) color_set_rgba(c, 100.0, 149.0, 237.0, 1.0);
#define color_steelblue(c) color_set_rgba(c, 70.0, 130.0, 180.0, 1.0);
#define color_royalblue(c) color_set_rgba(c, 65.0, 105.0, 225.0, 1.0);
#define color_blue(c) color_set_rgba(c, 0.0, 0.0, 255.0, 1.0);
#define color_mediumblue(c) color_set_rgba(c, 0.0, 0.0, 205.0, 1.0);
#define color_darkblue(c) color_set_rgba(c, 0.0, 0.0, 139.0, 1.0);
#define color_navy(c) color_set_rgba(c, 0.0, 0.0, 128.0, 1.0);
#define color_midnightblue(c) color_set_rgba(c, 25.0, 25.0, 112.0, 1.0);
  //Purple colors
#define color_lavender(c) color_set_rgba(c, 230.0, 230.0, 250.0, 1.0);
#define color_thistle(c) color_set_rgba(c, 216.0, 191.0, 216.0, 1.0);
#define color_plum(c) color_set_rgba(c, 221.0, 160.0, 221.0, 1.0);
#define color_violet(c) color_set_rgba(c, 238.0, 130.0, 238.0, 1.0);
#define color_orchid(c) color_set_rgba(c, 218.0, 112.0, 214.0, 1.0);
#define color_magenta(c) color_set_rgba(c, 255.0, 0.0, 255.0, 1.0);
#define color_fuschia(c) color_set_rgba(c, 255.0, 0.0, 255.0, 1.0);
#define color_mediumorchid(c) color_set_rgba(c, 186.0, 85.0, 211.0, 1.0);
#define color_mediumpurple(c) color_set_rgba(c, 147.0, 112.0, 219.0, 1.0);
#define color_blueviolet(c) color_set_rgba(c, 138.0, 43.0, 226.0, 1.0);
#define color_darkviolet(c) color_set_rgba(c, 148.0, 0.0, 211.0, 1.0);
#define color_darkorchid(c) color_set_rgba(c, 153.0, 50.0, 204.0, 1.0);
#define color_darkmagenta(c) color_set_rgba(c, 139.0, 0.0, 139.0, 1.0);
#define color_purple(c) color_set_rgba(c, 128.0, 0.0, 128.0, 1.0);
#define color_indigo(c) color_set_rgba(c, 75.0, 0.0, 130.0, 1.0);
#define color_darkslateblue(c) color_set_rgba(c, 72.0, 61.0, 139.0, 1.0);
#define color_slateblue(c) color_set_rgba(c, 106.0, 90.0, 205.0, 1.0);
#define color_mediumslateblue(c) color_set_rgba(c, 123.0, 104.0, 238.0, 1.0);
  //White colors
#define color_white(c) color_set_rgba(c, 255.0, 255.0, 255.0, 1.0);
#define color_snow(c) color_set_rgba(c, 255.0, 250.0, 250.0, 1.0);
#define color_honeydew(c) color_set_rgba(c, 240.0, 255.0, 240.0, 1.0);
#define color_mintcream(c) color_set_rgba(c, 245.0, 255.0, 250.0, 1.0);
#define color_azure(c) color_set_rgba(c, 240.0, 255.0, 255.0, 1.0);
#define color_aliceblue(c) color_set_rgba(c, 240.0, 248.0, 255.0, 1.0);
#define color_ghostwhite(c) color_set_rgba(c, 248.0, 248.0, 255.0, 1.0);
#define color_whitesmoke(c) color_set_rgba(c, 245.0, 245.0, 245.0, 1.0);
#define color_seashell(c) color_set_rgba(c, 255.0, 245.0, 238.0, 1.0);
#define color_beige(c) color_set_rgba(c, 245.0, 245.0, 220.0, 1.0);
#define color_oldlace(c) color_set_rgba(c, 253.0, 245.0, 230.0, 1.0);
#define color_floralwhite(c) color_set_rgba(c, 255.0, 250.0, 240.0, 1.0);
#define color_ivory(c) color_set_rgba(c, 255.0, 255.0, 240.0, 1.0);
#define color_antiquewhite(c) color_set_rgba(c, 250.0, 235.0, 215.0, 1.0);
#define color_linen(c) color_set_rgba(c, 250.0, 240.0, 230.0, 1.0);
#define color_lavenderblush(c) color_set_rgba(c, 255.0, 240.0, 245.0, 1.0);
#define color_mistyrose(c) color_set_rgba(c, 255.0, 228.0, 225.0, 1.0);
  //Gray/Black colors
#define color_gainsboro(c) color_set_rgba(c, 220.0, 220.0, 220.0, 1.0);
#define color_lightgray(c) color_set_rgba(c, 211.0, 211.0, 211.0, 1.0);
#define color_silver(c) color_set_rgba(c, 192.0, 192.0, 192.0, 1.0);
#define color_darkgray(c) color_set_rgba(c, 169.0, 169.0, 169.0, 1.0);
#define color_gray(c) color_set_rgba(c, 128.0, 128.0, 128.0, 1.0);
#define color_dimgray(c) color_set_rgba(c, 105.0, 105.0, 105.0, 1.0);
#define color_lightslategray(c) color_set_rgba(c, 119.0, 136.0, 153.0, 1.0);
#define color_slategray(c) color_set_rgba(c, 112.0, 128.0, 144.0, 1.0);
#define color_darkslategray(c) color_set_rgba(c, 47.0, 79.0, 79.0, 1.0);
#define color_black(c) color_set_rgba(c, 0.0, 0.0, 0.0, 1.0);

  // Color function prototypes

    // Structure management functions

color_s *color_create(void);
void color_destroy(color_s *c);
color_s *color_copy(color_s *c);

    // Comprehensive getter/setter

void color_set(color_s *c, char *tag, double r, double g, double b, double a);
void color_get(color_s *c,
               char **tag,
               double *r,
               double *g,
               double *b,
               double *a);

    // Color value only getter/setter

void color_set_rgba(color_s *c, double r, double g, double b, double a);
void color_get_rgba(color_s *c, double *r, double *g, double *b, double *a);

    // Individual element getters/setters

void color_set_tag(color_s *c, char *tag);
char *color_get_tag(color_s *c);

void color_set_r(color_s *c, double r);
double color_get_r(color_s *c);

void color_set_g(color_s *c, double g);
double color_get_g(color_s *c);

void color_set_b(color_s *c, double b);
double color_get_b(color_s *c);

void color_set_a(color_s *c, double a);
double color_get_a(color_s *c);

    // Conversion functions

color_s *str2color(char *s);
char *color2str(color_s *c);

#endif // COLOR
