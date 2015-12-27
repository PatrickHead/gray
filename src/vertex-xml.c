/*!
    @file vertex-xml.c

    @brief Source file for vertex XML data

    @timestamp Fri, 09 Oct 2015 05:37:44 +0000

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

    @file vertex-xml.c

    Source file for managing vertex data to/from XML format.

    XML Utility functions for converting vertex data to/from XML format.

    Also includes a stream sieve function for building filter pipe lines on
    STDIO that can capture or edit existing vertex data in XML format.

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

#include "vertex-xml.h"

  // Common constants 

#define MAX_SN 40

  /*!

     @brief Convert vertex data to XML document

     Converts data in vertex structure to XML document format.

     @param v    pointer to vertex data structure

     @retval "xmlDocPtr" success
     @retval NULL    failure

  */

xmlDocPtr vertex_to_xml_doc(vertex_s *v)
{
  xmlDocPtr doc = NULL;
  xmlNodePtr root = NULL;

    // Sanity check parameters.
  assert(v);

  doc = xmlNewDoc(BAD_CAST "1.0");
  if (!doc) return NULL;

  root = vertex_to_xml_node(v);
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

     @brief Convert vertex data to XML node

     Converts data in vertex structure to XML node format.

     @param v    pointer to vertex data structure

     @retval "xmlNodePtr" success
     @retval NULL    failure

  */

xmlNodePtr vertex_to_xml_node(vertex_s *v)
{
  xmlNodePtr node;
  char sn[MAX_SN];

    // Sanity check parameters.
  assert(v);

  node = xmlNewNode(NULL, BAD_CAST "vertex");
  if (!node) return NULL;

  if (v->tag)
    xmlNewProp(node, BAD_CAST "tag", BAD_CAST v->tag);

  snprintf(sn, MAX_SN, "%f", v->x);
  xmlNewProp(node, BAD_CAST "x", BAD_CAST sn);

  snprintf(sn, MAX_SN, "%f", v->y);
  xmlNewProp(node, BAD_CAST "y", BAD_CAST sn);

  snprintf(sn, MAX_SN, "%f", v->z);
  xmlNewProp(node, BAD_CAST "z", BAD_CAST sn);

    // Return "xmlNodePtr"
  return node;
}

  /*!

     @brief Returns root node of XML document containing vertex XML data

     Finds and returns the XML root node of a document containin vertex data.

     @param doc    pointer to xmlDoc

     @retval "xmlNodePtr" success
     @retval NULL    failure

  */

xmlNodePtr vertex_root_node(xmlDocPtr doc)
{
    // Sanity check parameters.
  assert(doc);
    // Return "xmlNodePtr"
  return xmlDocGetRootElement(doc);
}

  /*!

     @brief Convert XML vertex document to vertex data structure

     Converts an XML document containing vertex data to data structure

     @param doc    pointer to xmlDoc

     @retval "vertex_s *" success
     @retval NULL    failure

  */

vertex_s *vertex_from_xml_doc(xmlDocPtr doc)
{
  xmlNodePtr root;

    // Sanity check parameters.
  assert(doc);

  root = vertex_root_node(doc);
  if (!root) return NULL;

    // Return "vertex_s *"
  return vertex_from_xml_node(root);
}

  /*!

     @brief Convert XML vertex node to vertex data structure

     Converts an XML node containing vertex data to data structure

     @param doc    pointer to xmlNode

     @retval "vertex_s *" success
     @retval NULL    failure

  */

vertex_s *vertex_from_xml_node(xmlNodePtr node)
{
  vertex_s *v;
  char *s;

    // Sanity check parameters.
  assert(node);

  if (strcmp((char*)node->name, "vertex")) return NULL;

  v = vertex_create();
  if (!v) return NULL;

  s = (char *)xmlGetProp(node, BAD_CAST "tag");
  if (s)
    v->tag = strdup(s);
  
  s = (char *)xmlGetProp(node, BAD_CAST "x");
  if (s)
    v->x = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "y");
  if (s)
    v->y = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "z");
  if (s)
    v->z = strtod(s, NULL);
  
    // Return "vertex_s *"
  return v;
}

  /*!

     @brief STDIO sieve for handling XML vertex data in data stream

     Sieve which captures the first XML document in input stream that contains
     vertex data when sieve process mode is set to "edit".  All other XML
     documents in stream are simply emitted back to stream in the order that
     they appear.   This includes any subsequent documents containing vertex
     data, and also the first document containing vertex data if the sieve
     process mode is set to "passthru".

     @param infile    "FILE *" to input stream
     @param outfile   "FILE *" to output stream

     @retval "vertex_s *" success
     @retval NULL    failure or no matching document

  */

vertex_s *vertex_sieve(FILE *infile, FILE *outfile)
{
  doc_list_s *dl;
  vertex_s *v;
  int i;
  xmlDocPtr doc;
  xmlNodePtr root;
  int have_one = 0;

  if (!infile) return NULL;

  dl = doc_list_create(infile,
                    "<?xml[^<]*?>",
                    "<vertex[^<]*/>");
  if (!dl) return NULL;

  v = vertex_create();
  if (!v) return NULL;

    // Process any matching XML documents

  for (i = 0; i < dl->nkeep; i++)
  {
    doc = xmlReadMemory(dl->keep[i], strlen(dl->keep[i]), NULL, "UTF-8", 0);
    if (doc)
    {
      root = xmlDocGetRootElement(doc);
      if (root)
      {
        if (!strcmp((char *)root->name, "vertex"))
        {
          if (!have_one &&
              (sieve_get_process_mode() == sieve_process_mode_type_edit))
          {
            have_one = 1;
            v = vertex_from_xml_doc(doc);
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

    // Return "vertex_s *"
  return v;
}

