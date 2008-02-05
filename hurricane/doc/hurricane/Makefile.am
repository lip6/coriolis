
SUBDIRS = images


doc_en_latex_hurricanedir = $(datadir)/doc/en/latex/hurricane.new
doc_en_html_hurricanedir  = $(datadir)/doc/en/html/hurricane.new
doc_en_rtf_hurricanedir   = $(datadir)/doc/en/rtf/hurricane.new
doc_en_man_hurricanedir   = $(datadir)/man


doc_en_hurricane_CLEAN =
doc_en_hurricane_EXTRA = doxyfile                     \
                         asimbook.cls                 \
                         header.tex                   \
                         ASIM.css                     \
                         header.html                  \
                         footer.html                  \
                         customSummary.html           \
                         customHierarchy.html         \
                         BasicLayer.dox               \
                         Box.dox                      \
                         Cell.dox                     \
                         Collection.dox               \
                         Command.dox                  \
                         Commons.dox                  \
                         Component.dox                \
                         CompositeLayer.dox           \
                         Contact.dox                  \
                         DataBase.dox                 \
                         DBo.dox                      \
                         Editor.dox                   \
                         Entity.dox                   \
                         Error.dox                    \
                         Exception.dox                \
                         ExpandRules.dox              \
                         Filter.dox                   \
                         Generalities.dox             \
                         GenericCollection.dox        \
                         GenericFilter.dox            \
                         GenericLocator.dox           \
                         Go.dox                       \
                         Hook.dox                     \
                         Horizontal.dox               \
                         Hurricane.dox                \
                         Instance.dox                 \
                         Interruption.dox             \
                         Interval.dox                 \
                         Layer.dox                    \
                         Library.dox                  \
                         ListCollection.dox           \
                         Locator.dox                  \
                         MainView.dox                 \
                         MapCollection.dox            \
                         MapView.dox                  \
                         Name.dox                     \
                         Net.dox                      \
                         NotFilter.dox                \
                         Occurrence.dox               \
                         Pad.dox                      \
                         Path.dox                     \
                         Pin.dox                      \
                         Plug.dox                     \
                         Point.dox                    \
                         Primitive.dox                \
                         PrivateProperty.dox          \
                         Property.dox                 \
                         QuadTree.dox                 \
                         Quark.dox                    \
                         Relation.dox                 \
                         Rubber.dox                   \
                         Segment.dox                  \
                         SelectCommand.dox            \
                         SelectNetCommand.dox         \
                         Selector.dox                 \
                         SetCollection.dox            \
                         SharedProperty.dox           \
                         Slice.dox                    \
                         SlotAdapter.dox              \
                         StandardPrivateProperty.dox  \
                         StandardRelation.dox         \
                         StandardSharedProperty.dox   \
                         SubSetCollection.dox         \
                         SubTypeCollection.dox        \
                         Symbol.dox                   \
                         Tabulation.dox               \
                         Technology.dox               \
                         Transformation.dox           \
                         Unit.dox                     \
                         UpdateSession.dox            \
                         VectorCollection.dox         \
                         Vertical.dox                 \
                         View.dox                     \
                         Warning.dox                  \
                         ZoomCommand.dox

install-data-hook: $(DESTDIR)$(doc_en_html_hurricanedir)/index.html

$(DESTDIR)$(doc_en_html_hurricanedir)/index.html: $(doc_en_hurricane_EXTRA)
	 @if [ `which doxygen 2>/dev/null` ]; then                                             \
	    cd $(top_srcdir)/documentation/hurricane/doc.new/images;                           \
	    for figure in DummyFig*.fig; do                                                    \
	      zoom="0.7";                                                                      \
	      case $$figure in                                                                 \
	        DummyFig-1.fig) zoom="0.7";;                                                   \
	        DummyFig-2.fig) zoom="0.9";;                                                   \
	      esac;                                                                            \
	      fig2dev -L png -m $$zoom $$figure `basename $$figure .fig`.png;                  \
	      fig2dev -L pdf -p dummy  $$figure `basename $$figure .fig`.pdf;                  \
	    done;                                                                              \
            cd ..;                                                                             \
	    cd $(top_srcdir)/documentation/hurricane/doc.new; mkdir -p en/hurricane; doxygen;  \
	    cp asimbook.cls en/hurricane/latex;                                                \
	    cp custom*.html en/hurricane/html;                                                 \
	    cd en/hurricane;                                                                   \
	    $(mkinstalldirs) $(DESTDIR)$(doc_en_latex_hurricanedir);                           \
	    $(mkinstalldirs) $(DESTDIR)$(doc_en_html_hurricanedir);                            \
	    $(mkinstalldirs) $(DESTDIR)$(doc_en_rtf_hurricanedir);                             \
	    $(mkinstalldirs) $(DESTDIR)$(doc_en_man_hurricanedir)/man3;                        \
	    $(INSTALL) -m 664 latex/*               $(DESTDIR)$(doc_en_latex_hurricanedir);    \
	    $(INSTALL) -m 664 html/*.{html,css,png,tag} $(DESTDIR)$(doc_en_html_hurricanedir); \
	    $(INSTALL) -m 664 ../../html.entry      $(DESTDIR)$(doc_en_html_hurricanedir);     \
	    $(INSTALL) -m 664 rtf/*.rtf             $(DESTDIR)$(doc_en_rtf_hurricanedir);      \
	    $(INSTALL) -m 664 man/man3/*.3          $(DESTDIR)$(doc_en_man_hurricanedir)/man3; \
        if [ -x $(datadir)/doc/en/html/generateIndex.sh ]; then                            \
          $(datadir)/doc/en/html/generateIndex.sh --prefix=${prefix} --destdir=$(DESTDIR); \
        fi                                                                                 \
	  else                                                                                 \
	    echo "warning: doxygen was not found, skipping doc generation.";                   \
	  fi



EXTRA_DIST = $(doc_en_hurricane_EXTRA)  
CLEANFILES = $(doc_en_hurricane_CLEAN)  

