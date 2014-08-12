#ifndef GRID_XML_H
#define GRID_XML_H

#include "grid.h"

typedef xmlNodePtr (*grid_cell_to_xml_node)(void *payload);
typedef void *(*grid_cell_from_xml_node)(xmlNodePtr node);

xmlDocPtr grid_to_xml_doc(grid_s *g, grid_cell_to_xml_node func);
xmlNodePtr grid_to_xml_node(grid_s *g, grid_cell_to_xml_node func);

xmlNodePtr grid_root_node(xmlDocPtr doc);

grid_s *grid_from_xml_doc(xmlDocPtr doc, grid_cell_from_xml_node func);
grid_s *grid_from_xml_node(xmlNodePtr node, grid_cell_from_xml_node func);

grid_s *grid_sieve(FILE *infile, FILE *outfile);

#endif // GRID_XML_H


