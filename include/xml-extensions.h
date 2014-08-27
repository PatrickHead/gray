/*!
    @file xml-extensions.h

    @brief Header file for libxml2 extensions

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

    @file xml-extensions.h

    Header file for libxml2 extensions

    Contains functions to extend the function library supplied in libxml2
    package.

    Currently contains:
      xmlDocFormatDumpEnc() - save an entire XML document with formatting
                              and encoding information.

  */

#ifndef XML_EXTENSIONS_H
#define XML_EXTENSIONS_H

  // XML extension function prototypes

void xmlDocFormatDumpEnc(FILE *f,
                         xmlDocPtr doc,
                         const char * txt_encoding,
                         int format);

#endif // XML_EXTENSIONS_H
