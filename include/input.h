/*!
    @file input.h

    @brief Header file for tracked input source.

    @timestamp Tue, 07 Oct 2014 04:48:04 +0000

    @author Patrick Head  mailto:patrickhead@gmail.com

    @copyright Copyright (C) 2013  Patrick Head

    @license
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.\n
    \n
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.\n
    \n
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INPUT_H
#define INPUT_H

  /*!

    @file input.h

    This is the header file for a tracked input source module. 

  */

  /*!
    @brief Definition of structure to capture and track the current line and
           character position of the input source.
  */

typedef struct
{
    /*! @brief Line number of location in input source */
  int line_number;
    /*! @brief Charactor position in line location in input source */
  int character_offset;
} input_location;

unsigned char input_initialize(char *source);
void input_cleanup(void);

unsigned char input_byte(void);

unsigned char input_eof(void);

long input_get_position(void);
unsigned char input_set_position(long offset);

unsigned char input_get_location(input_location *loc);

#endif // INPUT_H

