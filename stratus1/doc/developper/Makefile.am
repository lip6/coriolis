
doc_en_latex_stratusdevdir = $(datadir)/doc/en/latex/stratusdev
doc_en_html_stratusdevdir  = $(datadir)/doc/en/html/stratusdev
doc_en_pdf_stratusdevdir   = $(datadir)/doc/en/pdf/stratusdev


doc_en_stratus_EXTRA = stratus.tex              \
                       man_inst.tex             \
                       man_net.tex              \
                       man_model.tex            \
                       man_stratus.tex          \
                       html.entry               \
                       ASIM.css                 \
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
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_stratusdevdir) ;                      \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_stratusdevdir)/images ;               \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_html_stratusdevdir) ;                       \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_pdf_stratusdevdir) ;                        \
	   cp -r *.tex            $(DESTDIR)$(doc_en_latex_stratusdevdir) ;                \
	   cp -r *.css            $(DESTDIR)$(doc_en_latex_stratusdevdir) ;                \
	   cp -r .latex2html-init $(DESTDIR)$(doc_en_latex_stratusdevdir) ;                \
	   cp -r images/*.png     $(DESTDIR)$(doc_en_latex_stratusdevdir)/images ;         \
	   rm -f *.aux *.lof *.log *.out *.toc *.dvi ;                                  \
	   latex2html -no_math -html_version 4.0,math -style ./ASIM.css stratus.tex ;   \
	   cp -pr stratus/* $(DESTDIR)$(doc_en_html_stratusdevdir) ;                       \
	   cp -p ASIM.css $(DESTDIR)$(doc_en_html_stratusdevdir)/ASIM.css ;                \
	   pdflatex stratus.tex ;                                                       \
	   pdflatex stratus.tex ;                                                       \
	   cp stratus.pdf $(DESTDIR)$(doc_en_pdf_stratusdevdir) ;                          \
       cp $(srcdir)/html.entry $(DESTDIR)$(doc_en_html_stratusdevdir);                 \
       if [ -x $(datadir)/doc/en/html/generateIndex.sh ]; then                      \
	     $(datadir)/doc/en/html/generateIndex.sh                                    \
	                      --prefix=${prefix} --destdir=$(DESTDIR) ;                 \
	   fi ;                                                                         \
	 else                                                                           \
	   echo "[WARNING] latex2html not found, skipping doc generation.";             \
	 fi


EXTRA_DIST = $(doc_en_stratus_EXTRA)

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

