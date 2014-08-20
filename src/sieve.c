/*!
    @file sieve.c

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

    @file sieve.c

    SOURCE_BRIEF

    SOURCE_DETAILS

  */

  // Required system headers

#include <string.h>
#include <assert.h>

  // Project related headers

#include "sieve.h"

static sieve_process_mode_t _g_process_mode = sieve_process_mode_type_passthru;

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

void sieve_set_process_mode(sieve_process_mode_t m)
{
  _g_process_mode = m;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

sieve_process_mode_t sieve_get_process_mode(void)
{
    // Return RETVAL
  return _g_process_mode;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

