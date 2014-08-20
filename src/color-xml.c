/*!
    @file color-xml.c

    @brief SOURCE_BRIEF

    @timestamp Mon, 06 Jan 2014 15:17:36 +0000

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

    SOURCE_BRIEF

    SOURCE_DETAILS

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

#define MAX_SN 40

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

xmlDocPtr color_to_xml_doc(color_s *c)
{
  xmlDocPtr doc = NULL;
  xmlNodePtr root = NULL;

    // Sanity check parameters.
  assert(c);

  doc = xmlNewDoc(BAD_CAST "1.0");

  root = color_to_xml_node(c);

  xmlDocSetRootElement(doc, root);

    // Return RETVAL
  return doc;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

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

    // Return RETVAL
  return node;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

xmlNodePtr color_root_node(xmlDocPtr doc)
{
    // Sanity check parameters.
  assert(doc);
    // Return RETVAL
  return xmlDocGetRootElement(doc);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

color_s *color_from_xml_doc(xmlDocPtr doc)
{
  xmlNodePtr root;

    // Sanity check parameters.
  assert(doc);

  root = color_root_node(doc);

    // Return RETVAL
  return color_from_xml_node(root);
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

color_s *color_from_xml_node(xmlNodePtr node)
{
  color_s *c;
  char *s;

    // Sanity check parameters.
  assert(node);

  c = color_create();
  if (!c) return NULL;

  if (strcmp((char*)node->name, "color")) return c;

  s = (char *)xmlGetProp(node, BAD_CAST "tag");
  if (s)
    c->tag = strdup(s);
  
  s = (char *)xmlGetProp(node, BAD_CAST "r");
  if (s)
    c->r = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "g");
  if (s)
    c->g = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "b");
  if (s)
    c->b = strtod(s, NULL);
  
  s = (char *)xmlGetProp(node, BAD_CAST "a");
  if (s)
    c->a = strtod(s, NULL);
  
    // Return RETVAL
  return c;
}

  /*!

     @brief FUNCTION_BRIEF

     FUNCTION_DETAILS

     @param PARMNAME    PARM_DESCRIPTION

     @retval "RETTYPE" success
     @retval RETVAL    failure

  */

color_s *color_sieve(FILE *infile, FILE *outfile)
{
  doc_list_s *dl;
  color_s *color;
  int i;
  xmlDocPtr doc;
  xmlNodePtr root;
  int have_one = 0;

  if (!infile) return NULL;

  dl = doc_list_create(infile,
                    "<?xml[^<]*?>",
                    "<color[^<]*/>");
  if (!dl) return NULL;

  color = color_create();
  if (!color) return NULL;

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

    // Return RETVAL
  return color;
}

