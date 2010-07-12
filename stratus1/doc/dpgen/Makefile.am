
doc_en_latex_stratusdir = $(datadir)/doc/en/latex/dpgen
doc_en_html_stratusdir  = $(datadir)/doc/en/html/dpgen
doc_en_pdf_stratusdir   = $(datadir)/doc/en/pdf/dpgen


doc_en_stratus_EXTRA = dpgen.tex                \
                       man_dpgenadsb2f.tex      \
                       man_dpgenand2.tex        \
                       man_dpgenand3.tex        \
                       man_dpgenand4.tex        \
                       man_dpgenbuff.tex        \
                       man_dpgenbuse.tex        \
                       man_dpgenconst.tex       \
                       man_dpgendff.tex         \
                       man_dpgendfft.tex        \
                       man_dpgenfifo.tex        \
                       man_dpgeninv.tex         \
                       man_dpgenmux2.tex        \
                       man_dpgennand2mask.tex   \
                       man_dpgennand2.tex       \
                       man_dpgennand3.tex       \
                       man_dpgennand4.tex       \
                       man_dpgennbuse.tex       \
                       man_dpgennmux2.tex       \
                       man_dpgennor2mask.tex    \
                       man_dpgennor2.tex        \
                       man_dpgennor3.tex        \
                       man_dpgennor4.tex        \
                       man_dpgennul.tex         \
                       man_dpgenor2.tex         \
                       man_dpgenor3.tex         \
                       man_dpgenor4.tex         \
                       man_dpgenram.tex         \
                       man_dpgenrf1d.tex        \
                       man_dpgenrf1.tex         \
                       man_dpgenrom2.tex        \
                       man_dpgenrom4.tex        \
                       man_dpgensff.tex         \
                       man_dpgensfft.tex        \
                       man_dpgenshift.tex       \
                       man_dpgenshrot.tex       \
                       man_dpgenxnor2mask.tex   \
                       man_dpgenxnor2.tex       \
                       man_dpgenxor2.tex        \
                       html.entry               \
                       ASIM.css                 \
                       .latex2html-init

install-data-hook:
	@if [ `which latex2html 2>/dev/null` ]; then                                \
	   for file in `ls $(srcdir)/*.tex`; do	                                    \
		 filename=`basename $$file`;                                            \
 	     if [ ! -L $$filename -a ! -f $$filename ]; then                        \
 	       ln -sf $(srcdir)/$$filename . ;                                      \
 		 fi ;                                                                   \
	   done ;                                                                   \
	   if [ ! -L ASIM.css -a ! -f ASIM.css ]; then                              \
	     ln -sf $(srcdir)/*.css            . ;                                  \
	     ln -sf $(srcdir)/.latex2html-init . ;                                  \
	   fi ;                                                                     \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_stratusdir) ;                  \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_html_stratusdir) ;                   \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_pdf_stratusdir) ;                    \
	   cp -r *.tex            $(DESTDIR)$(doc_en_latex_stratusdir) ;            \
	   cp -r *.css            $(DESTDIR)$(doc_en_latex_stratusdir) ;            \
	   cp -r .latex2html-init $(DESTDIR)$(doc_en_latex_stratusdir) ;            \
       rm -f *.aux *.lof *.log *.out *.toc *.dvi ;                              \
	   latex2html -no_math -html_version 4.0,math -style ./ASIM.css dpgen.tex ; \
	   cp -pr dpgen/* $(DESTDIR)$(doc_en_html_stratusdir) ;                     \
	   cp -p ASIM.css $(DESTDIR)$(doc_en_html_stratusdir)/ASIM.css ;            \
	   pdflatex dpgen.tex ;                                                     \
	   pdflatex dpgen.tex ;                                                     \
	   cp dpgen.pdf $(DESTDIR)$(doc_en_pdf_stratusdir) ;                        \
       cp $(srcdir)/html.entry $(DESTDIR)$(doc_en_html_stratusdir);             \
       if [ -x $(datadir)/doc/en/html/generateIndex.sh ]; then                  \
	     $(datadir)/doc/en/html/generateIndex.sh                                \
	                     --prefix=${prefix} --destdir=$(DESTDIR) ;              \
	   fi ;                                                                     \
	 else                                                                       \
	   echo "[WARNING] latex2html not found, skipping doc generation.";         \
	 fi


EXTRA_DIST = $(doc_en_stratus_EXTRA)

CLEANFILES = *.dvi \
             *.ps  \
             *.lof \
             *.log \
             *.out \
             *.toc \
             *.aux \
             *.bak \
             stratus/*.html \
             stratus/*.css \
             stratus/*.pl \
             stratus/WARNINGS
