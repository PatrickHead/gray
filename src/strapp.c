/*!
    @file strapp.c

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

    @file strapp.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

/*!
    @file strapp.c

    @brief Source code for string append function.
    @brief In the spirit of the standard C string functions.

    @timestamp Tue, 12 Aug 2014 06:24:39 +0000

    @author Patrick Head  mailto:patrickhead@gmail.com

    @copyright Copyright (C) 2013  Patrick Head

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

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

char *strapp(char *s, char *a)
{
  if (!a)
      // Return RETVAL
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

    // Return RETVAL
  return s;
}

