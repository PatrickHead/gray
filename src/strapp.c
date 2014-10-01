/*!
    @file strapp.c

    @brief SOURCE_BRIEF

    @timestamp Wed, 20 Aug 2014 03:18:04 +0000

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

    @file strapp.c

    Source code for strapp(), a string append function.

    ( See function documentation for details. )

  */

  // Required system headers

#include <stdlib.h>
#include <string.h>

  /*!

     @brief Appends a string to an existing string.
    
     This function will append a string to the end of an existing string, like
     the venerable POSIX strcat function.  Unlike strcat, this function will
     reallocate the memory associated with the original string, or make an
     initial allocation for the appended string, if the original string is
     passed as a NULL.
    
     @param s    original string (NULL if original string does not exist)
     @param a    string to append to original
    
     @retval "char *" to expanded string
             NULL     failure

  */

char *strapp(char *s, char *a)
{
  if (!a)
    return s;

  if (!s)
  {
    s = malloc(strlen(a) + 1);
    memset(s, 0, strlen(a) + 1);
  }
  else
  {
    s = realloc(s, strlen(s) + strlen(a) + 1);
  }

  strcat(s, a);

    // Return "char *"
  return s;
}

