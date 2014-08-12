#ifndef COLOR_XML_H
#define COLOR_XML_H

#include "color.h"

xmlDocPtr color_to_xml_doc(color_s *c);
xmlNodePtr color_to_xml_node(color_s *c);
xmlNodePtr color_root_node(xmlDocPtr doc);
color_s *color_from_xml_doc(xmlDocPtr doc);
color_s *color_from_xml_node(xmlNodePtr node);
color_s *color_sieve(FILE *infile, FILE *outfile);

#endif // COLOR_XML_H
