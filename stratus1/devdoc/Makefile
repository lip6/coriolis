all : stratus.pdf stratus/index.html ASIM.css stratus/ASIM.css

stratus.pdf: *.tex
	@pdflatex stratus.tex
	@pdflatex stratus.tex
#	xdvi stratus.dvi

viewpdf: stratus.pdf
	@acroread stratus.pdf &


stratus/index.html: *.tex
	@latex2html -no_math -html_version 4.0,math -style ./ASIM.css stratus.tex

stratus/ASIM.css: ASIM.css
	@cp -p ASIM.css stratus/ASIM.css

    
orthographe: stratus.tex
	aspell -t --lang=fr -c $< 

clean:
	@rm -rf  *.dvi \
             *.ps  \
             *.lof \
             *.log \
             *.out \
             *.toc \
             *.aux \
             *.bak \
	         stratus.pdf \
             stratus/*.html \
             stratus/*.css \
             stratus/*.pl \
             stratus/WARNINGS
	@rm -rf stratus/*
	@rmdir stratus
