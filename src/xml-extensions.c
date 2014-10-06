/*!
    @file xml-extensions.c

    @brief Source file for libxml2 extensions

    @timestamp Mon, 06 Oct 2014 13:42:48 +0000

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

    @file xml-extensions.c

    Source file for libxml2 extensions

    Contains functions to extend the function library supplied in libxml2
    package.

    Currently contains:
      xmlDocFormatDumpEnc() - save an entire XML document with formatting
                              and encoding information.

  */

  // Required system headers

#include <stdio.h>
#include <assert.h>
#include <libxml/tree.h>

  /*!

     @brief Save XML document with formatting and encoding

     Save an XML document using formatting and encoding.

     NOTE:  See official libxml2 API for details on encoding and format

     @param f    open file to save to
     @param doc    pointer to XML document to save
     @param txt_encoding    text encoding for saved document
     @param format    format of saved document

     @retval NONE

  */

void xmlDocFormatDumpEnc(FILE *f,
                         xmlDocPtr doc,
                         const char * txt_encoding,
                         int format)
{
  xmlChar * buf;
  int blen;

    // Sanity check parameters.
  assert(f);
  assert(doc);

  xmlDocDumpFormatMemoryEnc(doc, &buf, &blen, txt_encoding, format);

  fputs((char *)buf, f);

  xmlFree(buf);
}

