EXTRA_DIST = doxygen.cfg DoxygenLayout.xml

docdir = $(datadir)/doc/@PACKAGE@-@VERSION@

latex/refman.pdf: latex/refman.tex

latex/refman.tex: $(srcdir)/doxygen.cfg \
    $(srcdir)/DoxygenLayout.xml \
		$(top_srcdir)/include/*.h \
		$(top_srcdir)/src/*.c
	@if [ ! -e include ];then ln -s $(top_srcdir)/include include; fi
	@if [ ! -e src ];then ln -s $(top_srcdir)/src src; fi
	@if [ ! -e doc ];then ln -s $(top_srcdir)/doc doc; fi
	@if which doxygen; then doxygen $(srcdir)/doxygen.cfg > /dev/null 2>&1; (cd latex; make > /dev/null 2>&1); fi
	@if [ -L include ];then rm -f include; fi
	@if [ -L src ];then rm -f src; fi
	@if [ -L doc ];then rm -f doc; fi

all: latex/refman.tex all-am
	@if [ -L include ];then rm -f include; fi
	@if [ -L src ];then rm -f src; fi
	@if [ -L doc ];then rm -f doc; fi

clean-local:
	-rm -rf latex html man xml

install-data-local: install-pdf-local install-html-local

install-pdf-local:
	@mkdir -p $(docdir)
	@cp latex/refman.pdf $(docdir)/libgray-reference.pdf

install-html-local:
	@mkdir -p $(docdir)
	@cp -r html $(docdir)

uninstall-local:
	@rm -rf $(docdir)/libgray-reference.pdf $(docdir)/html
