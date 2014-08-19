/*!
    @file grid-xml.h

    @brief HEADER_BRIEF

    @timestamp Mon, 06 Jan 2014 15:17:36 +0000

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

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef GRID_XML_H
#define GRID_XML_H

#include "grid.h"

  /*!
    brief TYPEDEF_BRIEF
  */

typedef xmlNodePtr (*grid_cell_to_xml_node)(void *payload);
  /*!
    brief TYPEDEF_BRIEF
  */

typedef void *(*grid_cell_from_xml_node)(xmlNodePtr node);

xmlDocPtr grid_to_xml_doc(grid_s *g, grid_cell_to_xml_node func);
xmlNodePtr grid_to_xml_node(grid_s *g, grid_cell_to_xml_node func);

xmlNodePtr grid_root_node(xmlDocPtr doc);

grid_s *grid_from_xml_doc(xmlDocPtr doc, grid_cell_from_xml_node func);
grid_s *grid_from_xml_node(xmlNodePtr node, grid_cell_from_xml_node func);

grid_s *grid_sieve(FILE *infile, FILE *outfile);

#endif // GRID_XML_H


