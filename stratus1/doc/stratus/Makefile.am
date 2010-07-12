
SUBDIRS = images

doc_en_latex_stratusdir = $(datadir)/doc/en/latex/stratus
doc_en_html_stratusdir  = $(datadir)/doc/en/html/stratus
doc_en_pdf_stratusdir   = $(datadir)/doc/en/pdf/stratus


doc_en_stratus_EXTRA = stratus.tex              \
                       man_new.tex              \
                       man_alim_connectors.tex  \
                       man_alim_rail.tex        \
                       man_arithmetic.tex       \
                       man_boolean.tex          \
                       man_buff.tex             \
                       man_comp.tex             \
                       man_const.tex            \
                       man_def_ab.tex           \
                       man_example.tex          \
                       man_fill_cell.tex        \
                       man_generate.tex         \
                       man_get_ref.tex          \
                       man_inst.tex             \
                       man_library.tex          \
                       man_mux.tex              \
                       man_net.tex              \
                       man_pads.tex             \
                       man_place_bottom.tex     \
                       man_place_centric.tex    \
                       man_place_contact.tex    \
                       man_place_glu.tex        \
                       man_place_left.tex       \
                       man_place_ref.tex        \
                       man_place_right.tex      \
                       man_place_segment.tex    \
                       man_place_pin.tex        \
                       man_place.tex            \
                       man_place_top.tex        \
                       man_copy_up_segment.tex  \
                       man_power_ring.tex       \
                       man_reg.tex              \
                       man_resize_ab.tex        \
                       man_route_ck.tex         \
                       man_set_ref_ins.tex      \
                       man_shift.tex            \
                       man_stratus.tex          \
                       see_also.tex             \
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
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_stratusdir) ;                      \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_stratusdir)/images ;               \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_html_stratusdir) ;                       \
	   $(mkinstalldirs) $(DESTDIR)$(doc_en_pdf_stratusdir) ;                        \
	   cp -r *.tex            $(DESTDIR)$(doc_en_latex_stratusdir) ;                \
	   cp -r *.css            $(DESTDIR)$(doc_en_latex_stratusdir) ;                \
	   cp -r .latex2html-init $(DESTDIR)$(doc_en_latex_stratusdir) ;                \
	   cp -r images/*.png     $(DESTDIR)$(doc_en_latex_stratusdir)/images ;         \
	   rm -f *.aux *.lof *.log *.out *.toc *.dvi ;                                  \
	   latex2html -no_math -html_version 4.0,math -style ./ASIM.css stratus.tex ;   \
	   cp -pr stratus/* $(DESTDIR)$(doc_en_html_stratusdir) ;                       \
	   cp -p ASIM.css $(DESTDIR)$(doc_en_html_stratusdir)/ASIM.css ;                \
	   pdflatex stratus.tex ;                                                       \
	   pdflatex stratus.tex ;                                                       \
	   cp stratus.pdf $(DESTDIR)$(doc_en_pdf_stratusdir) ;                          \
       cp $(srcdir)/html.entry $(DESTDIR)$(doc_en_html_stratusdir);                 \
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

