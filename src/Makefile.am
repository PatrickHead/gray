EXTRA_DIST = libgray.pc.in

pkgconfiglibdir = $(libdir)/pkgconfig 
pkgconfiglib_DATA = libgray.pc 

AM_CFLAGS = -O3 -I .. -I $(top_srcdir)/include
AM_LDFLAGS =

lib_LTLIBRARIES = libgray.la

LDADD = libgray.la

libgray_la_SOURCES = color.c color-xml.c doc-list.c grid-api.c grid.c grid-size.c grid-xml.c list.c reference.c sieve.c strapp.c vertex.c vertex-xml.c vertices.c vertices-xml.c xml-extensions.c
libgray_la_LDFLAGS = -version-info 1:0:1
libgray_la_CFLAGS = ${AM_CFLAGS} ${XML_CFLAGS}

.PHONY: timestamps
timestamps:
	@$(top_srcdir)/tools/auto-timestamp $(top_srcdir)

all: timestamps all-am
