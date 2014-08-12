#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "xml-extensions.h"
#include "doc-list.h"

#include "sieve.h"
#include "vertices-xml.h"

#define MAX_SN 40

xmlDocPtr vertices_to_xml_doc(vertices_s *vs)
{
  xmlDocPtr doc = NULL;
  xmlNodePtr root = NULL;

  assert(vs);

  doc = xmlNewDoc(BAD_CAST "1.0");

  root = vertices_to_xml_node(vs);

  xmlDocSetRootElement(doc, root);

  return doc;
}

xmlNodePtr vertices_to_xml_node(vertices_s *vs)
{
  xmlNodePtr vsn;
  xmlNodePtr vn;
  vertex_s *v;

  assert(vs);

  vsn = xmlNewNode(NULL, BAD_CAST "vertices");
  if (!vsn) return NULL;

  if (vs->vertices)
  {
    for (v = (vertex_s *)list_head(vs->vertices);
         v;
         v = (vertex_s *)list_next(vs->vertices))
    {
      vn = vertex_to_xml_node(v);
      if (vn)
      {
        xmlAddChild(vsn, xmlCopyNode(vn, 1));
        xmlFreeNode(vn);
      }
    }
  }

  return vsn;
}

xmlNodePtr vertices_root_node(xmlDocPtr doc)
{
  assert(doc);
  return xmlDocGetRootElement(doc);
}

vertices_s *vertices_from_xml_doc(xmlDocPtr doc)
{
  xmlNodePtr root;

  assert(doc);

  root = vertices_root_node(doc);

  return vertices_from_xml_node(root);
}

vertices_s *vertices_from_xml_node(xmlNodePtr node)
{
  vertices_s *vs;
  vertex_s *v;

  assert(node);

  if (strcmp((char*)node->name, "vertices")) return NULL;

  vs = vertices_create();
  if (!vs) return NULL;
  if (!vs->vertices) return NULL;

  for (node = node->children; node; node = node->next)
  {
    if (node->type == XML_ELEMENT_NODE)
    {
      if (!strcmp((char*)node->name, "vertex"))
      {
        v = vertex_from_xml_node(node);
        list_insert(vs->vertices, v, (void *)TAIL);
      }
    }
  }

  return vs;
}

vertices_s *vertices_sieve(FILE *infile, FILE *outfile)
{
  doc_list_s *dl;
  vertices_s *vs = NULL;
  vertex_s *v;
  int i;
  xmlDocPtr doc;
  xmlNodePtr root;
  int have_one = 0;

  if (!infile) return NULL;

  dl = doc_list_create(infile,
                    "<?xml[^<]*?>",
                    "<vertices[^<]*>.*</vertices>"
                    "|<vertex[^<]*/>");
  if (!dl) return NULL;

    // Process any matching XML documents

  for (i = 0; i < dl->nkeep; i++)
  {
    doc = xmlReadMemory(dl->keep[i], strlen(dl->keep[i]), NULL, "UTF-8", 0);
    if (doc)
    {
      root = xmlDocGetRootElement(doc);
      if (root)
      {
        if (!strcmp((char *)root->name, "vertices"))
        {
          if (!have_one &&
              (sieve_get_process_mode() == sieve_process_mode_type_edit))
          {
            have_one = 1;
            vs = vertices_from_xml_doc(doc);
            if (vs) doc_list_consume(dl, dl->keep[i]);
          }
        }
        else if (!strcmp((char *)root->name, "vertex"))
        {
          if (!vs) vs = vertices_create();
          if (vs)
          {
            v = vertex_from_xml_doc(doc);
            vertices_add_vertex(vs, v);
            doc_list_consume(dl, dl->keep[i]);
          }
        }
      }
      xmlFreeDoc(doc);
    }
  }

    // Emit all non-matching XML documents

  if (outfile)
  {
    for (i = 0; i < dl->nlist; i++)
    {
      if (!dl->list[i]) continue;
      doc = xmlReadMemory(dl->list[i], strlen(dl->list[i]), NULL, "UTF-8", 0);
      if (doc)
      {
        xmlDocFormatDumpEnc(outfile, doc, "UTF-8", 1);
        xmlFreeDoc(doc);
      }
    }
  }

  return vs;
}

