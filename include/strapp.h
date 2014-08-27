/*!
    @file strapp.h

    @brief Header file for strapp string function

    @timestamp Wed, 27 Aug 2014 07:43:34 +0000

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

    @file strapp.h

    Header file for strapp string function

    strapp() is a function designed to enhance and extend the venerable clib
    str* functions.   strapp() stands for "string append", and will append a
    null terminated string to another null terminated string, managing all
    memory allocation/re-allocation as needed.   strapp() can also create
    a new string allocation.  strapp() is intended to serve as both a useful
    addition to the str*() function libraries, and to provide a possibly
    safer mechanism for handling strings in a generic manner.

  */

#ifndef STRAPP_H
#define STRAPP_H

  // strapp function prototype

char *strapp(char *s, char *a);

#endif // STRAPP_H
