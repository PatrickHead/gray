/*!
    @file vertex-xml.h

    @brief Header file for vertex XML data

    @timestamp Fri, 09 Oct 2015 05:37:44 +0000

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

    @file vertex-xml.h

    Header file for managing vertex data to/from XML format.

    XML Utility functions for converting vertex data to/from XML format.

    Also includes a stream sieve function for building filter pipe lines on
    STDIO that can capture or edit existing vertex data in XML format.

    NOTE:  The user is responsible for including at least <libxml/tree.h> in
           any source that includes this header.

  */

#ifndef VERTEX_XML_H
#define VERTEX_XML_H

  // Base type include file(s)

#include "vertex.h"

  // Vertex XML function prototypes

    // Functions to convert to XML

xmlDocPtr vertex_to_xml_doc(vertex_s *v);
xmlNodePtr vertex_to_xml_node(vertex_s *v);

    // Utility functions

xmlNodePtr vertex_root_node(xmlDocPtr doc);

    // Functions to convert from XML

vertex_s *vertex_from_xml_doc(xmlDocPtr doc);
vertex_s *vertex_from_xml_node(xmlNodePtr node);

    // Filter functions

vertex_s *vertex_sieve(FILE *infile, FILE *outfile);

#endif // VERTEX_XML_H
