/*!
    @file color-xml.c

    @brief Source file for color XML data management routines

    @timestamp Tue, 07 Oct 2014 05:03:53 +0000

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

    @file color-xml.c

    Source file for managing color data to/from XML format.

    XML Utility functions for converting color data to/from XML format.

    Also includes a stream sieve function for building filter pipe lines on
    STDIO that can capture or edit existing color data in XML format.

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
#include "color-xml.h"

  // Common constants

#define MAX_SN 40

  /*!

     @brief Convert color data to XML document

     Converts data in color structure to XML document format.

     @param c    pointer to color data structure

     @retval "xmlDocPtr" success
     @retval NULL    failure

  */

xmlDocPtr color_to_xml_doc(color_s *c)
{
  xmlDocPtr doc = NULL;
  xmlNodePtr root = NULL;

    // Sanity check parameters.
  assert(c);

  doc = xmlNewDoc(BAD_CAST "1.0");
  if (!doc) return NULL;

  root = color_to_xml_node(c);
  if (!doc)
  {
    xmlFreeDoc(doc);
    return NULL;
  }

  xmlDocSetRootElement(doc, root);

    // Return pointer to xmlDoc
  return doc;
}

  /*!

     @brief Convert color data to XML node

     Converts data in color structure to XML node format.

     @param c    pointer to color data structure

     @retval "xmlNodePtr" success
     @retval NULL    failure

  */

xmlNodePtr color_to_xml_node(color_s *c)
{
  xmlNodePtr node;
  char sn[MAX_SN];

    // Sanity check parameters.
  assert(c);

  node = xmlNewNode(NULL, BAD_CAST "color");
  if (!node) return NULL;

  if (c->tag)
    xmlNewProp(node, BAD_CAST "tag", BAD_CAST c->tag);

  snprintf(sn, MAX_SN, "%f", c->r);
  xmlNewProp(node, BAD_CAST "r", BAD_CAST sn);

  snprintf(sn, MAX_SN, "%f", c->g);
  xmlNewProp(node, BAD_CAST "g", BAD_CAST sn);

  snprintf(sn, MAX_SN, "%f", c->b);
  xmlNewProp(node, BAD_CAST "b", BAD_CAST sn);

  snprintf(sn, MAX_SN, "%f", c->a);
  xmlNewProp(node, BAD_CAST "a", BAD_CAST sn);

    // Return pointer to xmlNode
  return node;
}

  /*!

     @brief Returns root node of XML document containing color XML data

     Finds and returns the XML root node of a document containin color data.

     @param doc    pointer to xmlDoc

     @retval "xmlNodePtr" success
     @retval NULL    failure

  */

xmlNodePtr color_root_node(xmlDocPtr doc)
{
    // Sanity check parameters.
  assert(doc);
    // Return pointer to xmlNode
  return xmlDocGetRootElement(doc);
}

  /*!

     @brief Convert XML color document to color data structure

     Converts an XML document containing color data to data structure

     @param doc    pointer to xmlDoc

     @retval "color_s *" success
     @retval NULL    failure

  */

color_s *color_from_xml_doc(xmlDocPtr doc)
{
  xmlNodePtr root;

    // Sanity check parameters.
  assert(doc);

  root = color_root_node(doc);
  if (!root) return NULL;

    // Return pointer to color structure
  return color_from_xml_node(root);
}

  /*!

     @brief Convert XML color node to color data structure

     Converts an XML node containing color data to data structure

     @param doc    pointer to xmlNode

     @retval "color_s *" success
     @retval NULL    failure

  */

color_s *color_from_xml_node(xmlNodePtr node)
{
  color_s *c;
  char *s;

    // Sanity check parameters.
  assert(node);

  if (strcmp((char*)node->name, "color")) return NULL;

  c = color_create();
  if (!c) return NULL;

  s = (char *)xmlGetProp(node, BAD_CAST "tag");
  if (s) c->tag = strdup(s);
  
  s = (char *)xmlGetProp(node, BAD_CAST "r");
  if (s) c->r = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "g");
  if (s) c->g = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "b");
  if (s) c->b = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "a");
  if (s) c->a = strtod(s, NULL);
  
    // Return pointer to color structure
  return c;
}

  /*!

     @brief STDIO sieve for handling XML color data in data stream

     Sieve which captures the first XML document in input stream that contains
     color data when sieve process mode is set to "edit".  All other XML
     documents in stream are simply emitted back to stream in the order that
     they appear.   This includes any subsequent documents containing color
     data, and also the first document containing color data if the sieve
     process mode is set to "passthru".

     @param infile    "FILE *" to input stream
     @param outfile   "FILE *" to output stream

     @retval "color_s *" success
     @retval NULL    failure or no matching document

  */

color_s *color_sieve(FILE *infile, FILE *outfile)
{
  doc_list_s *dl;
  color_s *color;
  int i;
  xmlDocPtr doc;
  xmlNodePtr root;
  int have_one = 0;

    // No processing is done, if input stream is not provided, or not open

  if (!infile) return NULL;

    // Create document list from input stream.  Each document is defined as
    // any xml document, and a matching document is any document containing
    // a "color" element

  dl = doc_list_create(infile,
                    "<?xml[^<]*?>",
                    "<color[^<]*/>");
  if (!dl) return NULL;

    // Create base color structure

  color = color_create();
  if (!color) return NULL;

    // Set default color to "black"

  color_black(color);

    // Process any matching XML documents

  for (i = 0; i < dl->nkeep; i++)
  {
    doc = xmlReadMemory(dl->keep[i], strlen(dl->keep[i]), NULL, "UTF-8", 0);
    if (doc)
    {
      root = xmlDocGetRootElement(doc);
      if (root)
      {
        if (!strcmp((char *)root->name, "color"))
        {
            // Capture first matching document, if in edit mode
          if (!have_one &&
              (sieve_get_process_mode() == sieve_process_mode_type_edit))
          {
            have_one = 1;
            color = color_from_xml_doc(doc);
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

    // Return pointer to color structure, if any
  return color;
}

