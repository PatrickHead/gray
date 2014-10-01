/*!
    @file reference.c

    @brief Source file for persistent reference id management

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

    @file reference.c

    @brief Source file for persistent reference id management

    A simple, per user, persistent object ID management facility.


  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>

  // Project related headers

#include "reference.h"

  /*!

     @brief Create/update a reference counter

     Increments and returns a new reference number store in a persistent
     reference counter file.  Will create a new reference counter file if
     the named file does not already exist.  The user supplied filename is
     any valid filename, or relative path.  All reference counter files are
     stored under the current user's home directory.

     @param filename    string containing the reference counter file name

     @retval "int" success

  */

int reference_new(char *filename)
{
  FILE *f;
  char *home;
  char ref_name[2048];
  int ref;

  assert(filename);

  home = getenv("HOME");
  snprintf(ref_name, 2048, "%s/%s", home, filename);

  f = fopen(ref_name, "r");
  if (!f)
  {
    f = fopen(ref_name, "w");
    if (!f)
      return 0;
    fprintf(f, "1\n");
    fclose(f);
    return 1;
  }
    
  fscanf(f, "%d", &ref);
  fclose(f);

  ++ref;

  f = fopen(ref_name, "w");
  if (f)
  {
    fprintf(f, "%d\n", ref);
    fclose(f);
  }
  
    // Return int
  return ref;
}

