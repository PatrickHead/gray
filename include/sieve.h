/*!
    @file sieve.h

    @brief HEADER_BRIEF

    @timestamp Tue, 19 Aug 2014 05:12:38 +0000

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

    @file sieve.h

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef SIEVE_H
#define SIEVE_H

  /*!
    brief TYPEDEF_BRIEF
  */

typedef enum
{
  sieve_process_mode_type_passthru,
  sieve_process_mode_type_edit
} sieve_process_mode_t;

void sieve_set_process_mode(sieve_process_mode_t m);
sieve_process_mode_t sieve_get_process_mode(void);

sieve_process_mode_t str2sieve_process_mode_type(char *s);
char *sieve_process_mode_type2str(sieve_process_mode_t m);

#endif // SIEVE_H
