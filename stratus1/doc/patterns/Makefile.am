
doc_en_latex_patternsdir = $(datadir)/doc/en/latex/patterns
doc_en_html_patternsdir  = $(datadir)/doc/en/html/patterns
doc_en_pdf_patternsdir   = $(datadir)/doc/en/pdf/patterns


doc_en_patterns_EXTRA = patterns.tex     \
                       html.entry        \
                       ASIM.css          \
                       .latex2html-init


install-data-hook:
	@if [ `which latex2html 2>/dev/null` ]; then                                    \
	   for file in `ls $(srcdir)/*.tex`; do	                                        \
		 filename=`basename $$file`;                                                \
 	     if [ ! -L $$filename -a ! -f $$filename ]; then                            \
 	       ln -sf $(srcdir)/$$filename . ;                                          \
 		 fi ;                                                                       \
	   done ;                                                                       \
	   if [ ! -L ASIM.css -a ! -f ASIM.css ]; then                                  \
	     ln -sf $(srcdir)/*.css            . ;                                      \
	     ln -sf $(srcdir)/.latex2html-init . ;                                      \
	     $(mkinstalldirs) images ;                                                  \
	     ln -sf $(srcdir)/images/*.png ./images/ ;                                  \
	   fi ;                                                                         \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_patternsdir) ;                     \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_patternsdir)/images ;              \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_html_patternsdir) ;                      \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_pdf_patternsdir) ;                       \
	   cp -r *.tex            $(DESTDIR)$(doc_en_latex_patternsdir) ;               \
	   cp -r *.css            $(DESTDIR)$(doc_en_latex_patternsdir) ;               \
	   cp -r .latex2html-init $(DESTDIR)$(doc_en_latex_patternsdir) ;               \
	   cp -r images/*.png     $(DESTDIR)$(doc_en_latex_patternsdir)/images ;        \
	   rm -f *.aux *.lof *.log *.out *.toc *.dvi ;                                  \
	   latex2html -no_math -html_version 4.0,math -style ./ASIM.css patterns.tex ;  \
	   cp -pr patterns/* $(DESTDIR)$(doc_en_html_patternsdir) ;                     \
	   cp -p ASIM.css $(DESTDIR)$(doc_en_html_patternsdir)/ASIM.css ;               \
	   pdflatex patterns.tex ;                                                      \
	   pdflatex patterns.tex ;                                                      \
	   cp patterns.pdf $(DESTDIR)$(doc_en_pdf_patternsdir) ;                        \
       cp $(srcdir)/html.entry $(DESTDIR)$(doc_en_html_patternsdir);                \
       if [ -x $(datadir)/doc/en/html/generateIndex.sh ]; then                      \
	     $(datadir)/doc/en/html/generateIndex.sh                                    \
	                      --prefix=${prefix} --destdir=$(DESTDIR) ;                 \
	   fi ;                                                                         \
	 else                                                                           \
	   echo "[WARNING] latex2html not found, skipping doc generation.";             \
	 fi


EXTRA_DIST = $(doc_en_patterns_EXTRA)

CLEANFILES = *.dvi              \
             *.ps               \
             *.lof              \
             *.log              \
             *.out              \
             *.toc              \
             *.aux              \
             *.bak              \
             stratus/*.html     \
             stratus/*.css      \
             stratus/*.pl       \
             stratus/WARNINGS

