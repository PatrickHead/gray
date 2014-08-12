#include <stdio.h>
#include <assert.h>
#include <libxml/tree.h>

void xmlDocFormatDumpEnc(FILE *f,
                         xmlDocPtr doc,
                         const char * txt_encoding,
                         int format)
{
  xmlChar * buf;
  int blen;

  assert(f);
  assert(doc);

  xmlDocDumpFormatMemoryEnc(doc, &buf, &blen, txt_encoding, format);

  fputs((char *)buf, f);

  xmlFree(buf);
}

