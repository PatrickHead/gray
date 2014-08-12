#ifndef VERTICES_XML_H
#define VERTICES_XML_H

#include "vertices.h"

#include "vertex-xml.h"

xmlDocPtr vertices_to_xml_doc(vertices_s *ls);
xmlNodePtr vertices_to_xml_node(vertices_s *ls);
xmlNodePtr vertices_root_node(xmlDocPtr doc);
vertices_s *vertices_from_xml_doc(xmlDocPtr doc);
vertices_s *vertices_from_xml_node(xmlNodePtr node);
vertices_s *vertices_sieve(FILE *infile, FILE *outfile);

#endif // VERTICES_XML_H
