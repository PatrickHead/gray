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
