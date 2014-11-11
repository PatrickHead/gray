/*!
    @file vertices-xml.h

    @brief Header file for list of vertices XML data

    @timestamp Tue, 07 Oct 2014 07:32:14 +0000

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

    @file vertices-xml.h

    Header file for managing list of vertices data to/from XML format.

    XML Utility functions for converting list of vertices data to/from
    XML format.

    Also includes a stream sieve function for building filter pipe lines on
    STDIO that can capture or edit existing vertices data in XML format.

    NOTE:  The user is responsible for including at least <libxml/tree.h> in
           any source that includes this header.

  */

#ifndef VERTICES_XML_H
#define VERTICES_XML_H

  // Base type include file(s)

#include "vertices.h"
#include "vertex-xml.h"

  // Vertices XML function prototypes


    // Functions to convert to XML

xmlDocPtr vertices_to_xml_doc(vertices_s *ls);
xmlNodePtr vertices_to_xml_node(vertices_s *ls);

    // Utility functions

xmlNodePtr vertices_root_node(xmlDocPtr doc);

    // Functions to convert from XML

vertices_s *vertices_from_xml_doc(xmlDocPtr doc);
vertices_s *vertices_from_xml_node(xmlNodePtr node);

    // Filter functions

vertices_s *vertices_sieve(FILE *infile, FILE *outfile);

#endif // VERTICES_XML_H
