#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

#include "grid-xml.h"

void* xml2data(xmlNodePtr node);
xmlNodePtr data2xml(void* payload);

int main(int argc, char **argv)
{
  char fname[80];
  grid_s *g;
  xmlDocPtr doc;

  if (argc < 2)
    return -1;

  sprintf(fname, "%.70s.xml", argv[1]);

  doc = xmlReadFile(fname, "UTF-8", 0);

  g = grid_from_xml_doc(doc, xml2data);

  if (g)
    doc = grid_to_xml_doc(g, data2xml);

  sprintf(fname, "%.70s.cmp", argv[1]);
  xmlSaveFormatFileEnc(fname, doc, "UTF-8", 1);

  return 0;
}

void* xml2data(xmlNodePtr node)
{
  return (void*)xmlNodeGetContent(node);
}

xmlNodePtr data2xml(void* payload)
{
  xmlNodePtr text;

  text = xmlNewText((const xmlChar*)payload);

  return text;
}

