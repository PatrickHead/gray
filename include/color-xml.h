/*!
    @file color-xml.h

    @brief Header file for color XML data

    @timestamp Wed, 20 Aug 2014 03:54:45 +0000

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

    @file color-xml.h

    Header file for managing color data to/from XML format.

    XML Utility functions for converting color data to/from XML format.

    Also includes a stream sieve function for building filter pipe lines on
    STDIO that can capture or edit existing color data in XML format.

    NOTE:  The user is responsible for including at least <libxml/tree.h> in
           any source that includes this header.
  */

#ifndef COLOR_XML_H
#define COLOR_XML_H

  // Base type include file(s)

#include "color.h"

  // Color XML function prototypes

    // Functions to convert to XML

xmlDocPtr color_to_xml_doc(color_s *c);
xmlNodePtr color_to_xml_node(color_s *c);

    // Utility functions

xmlNodePtr color_root_node(xmlDocPtr doc);

    // Functions to convert from XML

color_s *color_from_xml_doc(xmlDocPtr doc);
color_s *color_from_xml_node(xmlNodePtr node);

    // Filter functions

color_s *color_sieve(FILE *infile, FILE *outfile);

#endif // COLOR_XML_H
