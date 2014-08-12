EXTRA_DIST = .egg printable.egg egg.pdf PROJECT TODOS VERSION

SUBDIRS = reference-manual user-guide man-pages

docdir = $(datadir)/doc/@PACKAGE@-@VERSION@
doc_DATA = .egg printable.egg egg.pdf PROJECT TODOS VERSION

printable.egg: $(srcdir)/.egg
	@echo Creating printable.egg
	@if which pr > /dev/null; then pr -F -W 70 -o 10 -h "EGG Grammar Definition" \
		-D "$$(date -r $(srcdir)/.egg +'rev. %Y%m%d')" \
		$(srcdir)/.egg > printable.egg; fi

egg.pdf: printable.egg
	@echo Creating egg.pdf
	@if which enscript ps2pdf > /dev/null; then enscript -q -t \
		egg.pdf -B -sPAPERSIZE=letter -o - \
		printable.egg | ps2pdf -q - egg.pdf; fi

all: egg.pdf all-am

clean-local:
	-rm -f egg.pdf printable.egg
