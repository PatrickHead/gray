/*!
    @file vertex-xml.h

    @brief HEADER_BRIEF

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

    @file vertex-xml.h

    HEADER_BRIEF

    HEADER_DETAILS

  */

#ifndef VERTEX_XML_H
#define VERTEX_XML_H

#include "vertex.h"

xmlDocPtr vertex_to_xml_doc(vertex_s *v);
xmlNodePtr vertex_to_xml_node(vertex_s *v);
xmlNodePtr vertex_root_node(xmlDocPtr doc);
vertex_s *vertex_from_xml_doc(xmlDocPtr doc);
vertex_s *vertex_from_xml_node(xmlNodePtr node);
vertex_s *vertex_sieve(FILE *infile, FILE *outfile);

#endif // VERTEX_XML_H
