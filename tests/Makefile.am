AM_CFLAGS = -O3 -I .. -I$(top_srcdir)/include -L$(top_srcdir)/src
AM_LDFLAGS =

EXTRA_DIST = grid-xml-test.sh test.xml

noinst_PROGRAMS = list-test grid-test grid-api-test grid-xml-test

list_test_SOURCES = list-test.c
list_test_LDADD = -lgray ${XML_LIBS}

grid_test_SOURCES = grid-test.c
grid_test_LDADD = -lgray ${XML_LIBS}

grid_api_test_SOURCES = grid-api-test.c
grid_api_test_LDADD = -lgray ${XML_LIBS}

grid_xml_test_SOURCES = grid-xml-test.c
grid_xml_test_CFLAGS = ${AM_CFLAGS} ${XML_CFLAGS}
grid_xml_test_LDADD = -lgray ${XML_LIBS}

.PHONY: timestamps
timestamps:
	@$(top_srcdir)/tools/auto-timestamp $(top_srcdir)

all: timestamps all-am

clean-local:
	@(rm -f test.cmp)
