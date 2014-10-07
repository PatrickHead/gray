/*!
    @file sieve.c

    @brief Source file for sieve type filter mode management

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

    @file sieve.c

    Source file for sieve type filter mode management

    A sieve is a special type of STDIO filter mechanism.  A sieve uses the
    doc-list facility to manage a list of document parts parsed from any STDIO
    stream, and to mark those of interest to the user.  This module is for
    managing the mode of the sieve, either passthru, or edit.  While it is
    entirely up to the user to determine for what the modes will be used, the
    intent is for edit mode to pick the first "kept" document and make changes
    to it, then pass it on to the STDIO output stream, while passthru mode is
    intended to simply pass all incoming document parts on to the output stream,
    but optionally adding additional data (document parts) to the stream.

  */

  // Required system headers

#include <string.h>
#include <assert.h>

  // Project related headers

#include "sieve.h"

  // Module private process mode flag, defaults to "passthru"
static sieve_process_mode_t _g_process_mode = sieve_process_mode_type_passthru;

  /*!

     @brief Set current process mode

     Set the current sieve process mode.

     @param m    desired sieve process mode

     @retval NONE

  */

void sieve_set_process_mode(sieve_process_mode_t m)
{
  _g_process_mode = m;
}

  /*!

     @brief Get current process mode

     Get the current sieve process mode.

     @retval "sieve_process_mode" success

  */

sieve_process_mode_t sieve_get_process_mode(void)
{
    // Return RETVAL
  return _g_process_mode;
}

  /*!

     @brief Convert string to sieve process mode

     Convert a string representation of sieve process mode to a
     sieve_process_mode_t type.

     The following are the valid string representations:

       "passthru"
       "edit"


     NOTE:  If the string representation does not match one of the above,
            sieve_process_mode_type_passthru is returned.

     @param s    string representation of process mode

     @retval "sieve_process_mode_t" success

  */

sieve_process_mode_t str2sieve_process_mode_type(char *s)
{
    // Sanity check parameters.
  assert(s);

  if (!strcmp(s, "passthru")) return sieve_process_mode_type_passthru;
  if (!strcmp(s, "edit")) return sieve_process_mode_type_edit;

    // Return RETVAL
  return sieve_process_mode_type_passthru;
}

  /*!

     @brief Convert sieve process mode to string

     Convert a seive process mode type to a string representation.

     @param m    sieve process mode to convert

     @retval "char *" success

  */

char *sieve_process_mode_type2str(sieve_process_mode_t m)
{
  switch (m)
  {
    case sieve_process_mode_type_passthru: return "passthru";
    case sieve_process_mode_type_edit: return "edit";
  }

    // Return RETVAL
  return "passthru";
}

