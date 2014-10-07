/*!
    @file sieve.h

    @brief Header file for sieve type filter mode management

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

    @file sieve.h

    Header file for sieve type filter mode management

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

#ifndef SIEVE_H
#define SIEVE_H

  /*!
    @brief Sieve mode type
  */

typedef enum
{
  sieve_process_mode_type_passthru,
  sieve_process_mode_type_edit
} sieve_process_mode_t;

  // Sieve function prototypes

    // Sieve mode getter/setter

void sieve_set_process_mode(sieve_process_mode_t m);
sieve_process_mode_t sieve_get_process_mode(void);

    // Type name conversion functions

sieve_process_mode_t str2sieve_process_mode_type(char *s);
char *sieve_process_mode_type2str(sieve_process_mode_t m);

#endif // SIEVE_H
