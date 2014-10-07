/*!
    @file reference.h

    @brief Header file for persistent reference id management

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

    @file reference.h

    @brief Header file for persistent reference id management

    A simple, per user, persistent object ID management facility

  */

#ifndef REFERENCE_H
#define REFERENCE_H

  // Reference functions prototypes

int reference_new(char *filename);

#endif //  REFERENCE_H
