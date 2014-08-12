#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "xml-extensions.h"
#include "doc-list.h"
#include "sieve.h"

#include "grid-xml.h"

#define MAX_SN 40

//grid_s *grid_sieve(FILE *infile, FILE *outfile);

xmlDocPtr grid_to_xml_doc(grid_s *g, grid_cell_to_xml_node func)
{
  xmlDocPtr doc = NULL;
  xmlNodePtr root = NULL;

  assert(g);
  assert(func);

  doc = xmlNewDoc(BAD_CAST "1.0");

  root = grid_to_xml_node(g, func);

  xmlDocSetRootElement(doc, root);

  return doc;
}

xmlNodePtr grid_to_xml_node(grid_s *g, grid_cell_to_xml_node func)
{
  xmlNodePtr node;
  xmlNodePtr size_node;
  xmlNodePtr rows_node;
  xmlNodePtr row_node;
  xmlNodePtr cols_node;
  xmlNodePtr cell_node;
  char sn[MAX_SN];
  int y, x;

  assert(g);
  assert(func);

  node = xmlNewNode(NULL, (const xmlChar*)"grid");

  size_node = xmlNewChild(node, NULL, (const xmlChar*)"size", NULL);
  sprintf(sn, "%d", grid_size_get_width(grid_get_size(g)));
  xmlNewProp(size_node, BAD_CAST "width", BAD_CAST sn);
  sprintf(sn, "%d", grid_size_get_height(grid_get_size(g)));
  xmlNewProp(size_node, BAD_CAST "height", BAD_CAST sn);

  rows_node = xmlNewChild(node, NULL, BAD_CAST "rows", NULL);

  for (y = 0; y < grid_size_get_height(grid_get_size(g)); y++)
  {
    row_node = xmlNewChild(rows_node, NULL, BAD_CAST "row", NULL);

    cols_node = xmlNewChild(row_node, NULL, BAD_CAST "columns", NULL);

    for (x = 0; x < grid_size_get_width(grid_get_size(g)); x++)
    {
      cell_node = xmlNewChild(cols_node, NULL, (const xmlChar*)"cell", NULL);
      grid_goto(g, y, x);
      xmlAddChild(cell_node, func(grid_get_cell(g)));
    }
  }

  return node;
}

xmlNodePtr grid_root_node(xmlDocPtr doc)
{
  assert(doc);
  return xmlDocGetRootElement(doc);
}

grid_s *grid_from_xml_doc(xmlDocPtr doc, grid_cell_from_xml_node func)
{
  xmlNodePtr root;

  assert(doc);

  root = grid_root_node(doc);

  return grid_from_xml_node(root, func);
}

grid_s *grid_from_xml_node(xmlNodePtr node, grid_cell_from_xml_node func)
{
  xmlNodePtr size_node;
  xmlNodePtr rows_node;
  xmlNodePtr row_node;
  xmlNodePtr cols_node;
  xmlNodePtr cell_node;
  grid_s *g;
  grid_size_s *gs;
  char *s;
  int row = 1, col = 1;

  assert(node);
  assert(func);

  if (strcmp((char *)node->name, "grid")) return NULL;

  if (!node->children || !node->children->next) return NULL;

  size_node = node->children->next;
  if (strcmp((char *)size_node->name, "size")) return NULL;

  g = grid_create();

  s = (char *)xmlGetProp(size_node, BAD_CAST "width");
  if (s) col = atoi(s);
  s = (char *)xmlGetProp(size_node, BAD_CAST "height");
  if (s) row = atoi(s);

  gs = grid_size_create();
  grid_size_set(gs, row, col);

  grid_set_size(g, gs);

  if (!size_node->next || !size_node->next->next) return g;

  rows_node = size_node->next->next;
  if (strcmp((char *)rows_node->name, "rows")) return g;

  row = 0;

  for (row_node = rows_node->children->next; row_node; row_node = row_node->next)
  {
    if (strcmp((char *)row_node->name, "row")) continue;

    cols_node = row_node->children->next;
    if (strcmp((char *)cols_node->name, "columns")) return g;

    col = 0;

    for (cell_node = cols_node->children->next;
         cell_node;
         cell_node = cell_node->next)
    {
      if (strcmp((char *)cell_node->name, "cell")) continue;

      grid_goto(g, row, col);
      grid_set_cell(g, func(cell_node->children));
      ++col;
    }

    ++row;
  }

  return g;
}

