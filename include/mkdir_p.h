/*!
    @file mkdir_p.h

    @brief Header file for "mkdir -p" like function.

    @timestamp Tue, 07 Oct 2014 07:32:14 +0000

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

#ifndef MKDIR_P_H
#define MKDIR_P_H

#include <sys/stat.h>

int mkdir_p(const char *pathname, mode_t mode);

#endif // MKDIR_P_H
