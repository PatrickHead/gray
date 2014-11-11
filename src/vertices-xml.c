/*!
    @file vertices-xml.c

    @brief Source file for vertices data

    @timestamp Tue, 07 Oct 2014 07:32:14 +0000

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

    @file vertices-xml.c

    Header file for managing list of vertices data to/from XML format.

    XML Utility functions for converting list of vertices data to/from
    XML format.

    Also includes a stream sieve function for building filter pipe lines on
    STDIO that can capture or edit existing vertices data in XML format.

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

  // Project related headers

#include "xml-extensions.h"
#include "doc-list.h"

#include "sieve.h"
#include "vertices-xml.h"

  // Common constants

#define MAX_SN 40

  /*!

     @brief Convert vertices data to XML document

     Converts data in vertices structure to XML document format.

     @param vs    pointer to vertices data structure

     @retval "xmlDocPtr" success
     @retval NULL    failure

  */

xmlDocPtr vertices_to_xml_doc(vertices_s *vs)
{
  xmlDocPtr doc = NULL;
  xmlNodePtr root = NULL;

    // Sanity check parameters.
  assert(vs);

  doc = xmlNewDoc(BAD_CAST "1.0");
  if (!doc) return NULL;

  root = vertices_to_xml_node(vs);
  if (!root)
  {
    xmlFreeDoc(doc);
    return NULL;
  }

  xmlDocSetRootElement(doc, root);

    // Return "xmlDocPtr"
  return doc;
}

  /*!

     @brief Convert vertices data to XML node

     Converts data in vertices structure to XML node format.

     @param v    pointer to vertices data structure

     @retval "xmlNodePtr" success
     @retval NULL    failure

  */

xmlNodePtr vertices_to_xml_node(vertices_s *vs)
{
  xmlNodePtr vsn;
  xmlNodePtr vn;
  vertex_s *v;

    // Sanity check parameters.
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

    // Return "xmlNodePtr"
  return vsn;
}

  /*!

     @brief Returns root node of XML document containing vertices XML data

     Finds and returns the XML root node of a document containin vertices data.

     @param doc    pointer to xmlDoc

     @retval "xmlNodePtr" success
     @retval NULL    failure

  */

xmlNodePtr vertices_root_node(xmlDocPtr doc)
{
    // Sanity check parameters.
  assert(doc);
    // Return RETVAL
  return xmlDocGetRootElement(doc);
}

  /*!

     @brief Convert XML vertices document to vertices data structure

     Converts an XML document containing vertices data to data structure

     @param doc    pointer to xmlDoc

     @retval "vertices_s *" success
     @retval NULL    failure

  */

vertices_s *vertices_from_xml_doc(xmlDocPtr doc)
{
  xmlNodePtr root;

    // Sanity check parameters.
  assert(doc);

  root = vertices_root_node(doc);
  if (!root) return NULL;

    // Return RETVAL
  return vertices_from_xml_node(root);
}

  /*!

     @brief Convert XML vertices node to vertices data structure

     Converts an XML node containing vertices data to data structure

     @param doc    pointer to xmlNode

     @retval "vertices_s *" success
     @retval NULL    failure

  */

vertices_s *vertices_from_xml_node(xmlNodePtr node)
{
  vertices_s *vs;
  vertex_s *v;

    // Sanity check parameters.
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

    // Return "vertices_s *"
  return vs;
}

  /*!

     @brief STDIO sieve for handling XML vertices data in data stream

     Sieve which captures the first XML document in input stream that contains
     vertices data when sieve process mode is set to "edit".  All other XML
     documents in stream are simply emitted back to stream in the order that
     they appear.   This includes any subsequent documents containing vertices
     data, and also the first document containing vertices data if the sieve
     process mode is set to "passthru".

     @param infile    "FILE *" to input stream
     @param outfile   "FILE *" to output stream

     @retval "vertices_s *" success
     @retval NULL    failure or no matching document

  */

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

    // Return "vertices_s *"
  return vs;
}

