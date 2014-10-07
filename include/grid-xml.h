/*!
    @file grid-xml.h

    @brief Header file for grid (matrix/spreadsheet) XML data

    @timestamp Tue, 07 Oct 2014 04:43:44 +0000

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

    @file grid-xml.h

    Header file for managing grid data to/from XML format.

    XML Utility functions for converting grid data to/from XML format.

    Also includes a stream sieve function for building filter pipe lines on
    STDIO that can capture or edit existing color data in XML format.

    NOTE:  The user is responsible for including at least <libxml/tree.h> in
           any source that includes this header.

  */

#ifndef GRID_XML_H
#define GRID_XML_H

  // Base type include file(s)

#include "grid.h"

  /*!
    @brief Prototypes for user defined XML data conversion functions
  */

typedef xmlNodePtr (*grid_cell_to_xml_node)(void *payload);
typedef void *(*grid_cell_from_xml_node)(xmlNodePtr node);

  // grid-xml function prototypes

    // Functions to convert to XML

xmlDocPtr grid_to_xml_doc(grid_s *g, grid_cell_to_xml_node func);
xmlNodePtr grid_to_xml_node(grid_s *g, grid_cell_to_xml_node func);

    // Utility functions

xmlNodePtr grid_root_node(xmlDocPtr doc);

    // Functions to convert from XML

grid_s *grid_from_xml_doc(xmlDocPtr doc, grid_cell_from_xml_node func);
grid_s *grid_from_xml_node(xmlNodePtr node, grid_cell_from_xml_node func);

    // Filter functions

grid_s *grid_sieve(FILE *infile, FILE *outfile);

#endif // GRID_XML_H
