L2HUTILS=../../../develop/post_l2h/

NAVBUTTONS=$(L2HUTILS)/contents.gif $(L2HUTILS)/next.gif $(L2HUTILS)/prev.gif \
	   $(L2HUTILS)/contents.png $(L2HUTILS)/next.png $(L2HUTILS)/prev.png

POSTL2H = $(L2HUTILS)/post_l2h

TEXFILES= usermanual.tex \
           doxygen.tex \
	   um-tp-html.tex \
           um-tp-paper.tex \
	   about.tex \
           prelim.tex \
           getstarted.tex \
           inout.tex \
	   cmd_ref.tex \
           um_class_ref.tex \
	   additional.tex \
           usermodels.tex \
	   fields.tex

EPSGRAPHICS = first-model-csim.eps first-model.eps  first-model-output.eps
PDFGRAPHICS := $(EPSGRAPHICS:%.eps=%.pdf)

default: all

all: dvi pdf ps html

pdf: usermanual.pdf

dvi: usermanual.dvi

ps: usermanual.ps.gz

html: usermanual.html

$(PDFGRAPHICS): %.pdf : %.eps
	epstopdf $<

usermanual.html: $(TEXFILES) usermanual.bbl latex2html-init usermanual.dvi
	@echo running latex2html ...
	@latex2html -init_file latex2html-init -image_type gif -local_icons -white -math -html_version 4.0 -antialias usermanual > /dev/null
	@echo post processing latex2html output ...
	@(for f in *.html; do \
            $(POSTL2H) $$f > /tmp/f.$$$$; \
            mv /tmp/f.$$$$ $$f; \
          done; )
	@cp $(NAVBUTTONS) .

usermanual.pdf: $(TEXFILES) usermanual.bbl $(PDFGRAPHICS)
	@( echo "generating $@ ..." ; \
	   aux=`basename $< .tex`.aux ; \
	   rm -rf $$aux ; \
	   log=`basename $< .tex`.log ; \
	   pdflatex $< > /dev/null 2>&1 ; \
	   pdflatex $< > /dev/null 2>&1 ; \
	   latex_count=5 ; \
	   while egrep -s 'Rerun (LaTeX|to get cross-references right)' $$log && [ $$latex_count -gt 0 ] ; do \
	     echo "Rerunning pdflatex...." ; \
	     pdflatex $< > /dev/null 2>&1 ; \
	     latex_count=`expr $$latex_count - 1` ;\
	   done \
	)

usermanual.bbl: $(TEXFILES) references.bib
	@latex usermanual > /dev/null
	@bibtex usermanual > /dev/null

usermanual.ps.gz: usermanual.dvi
	@echo "generating $@ ..." ;
	@dvips $< -o  > /dev/null 2>&1 ;
	@gzip -f `basename $< .dvi`.ps

usermanual.dvi: $(TEXFILES) usermanual.bbl $(EPSGRAPHICS)
	@( echo "generating $@ ..." ; \
	   log="usermanual.log" ; \
	   latex usermanual > /dev/null 2>&1 ; \
	   latex_count=5 ; \
	   while egrep -s 'Rerun (LaTeX|to get cross-references right)' $$log && [ $$latex_count -gt 0 ] ; do \
	     echo "Rerunning latex...." ; \
	     latex usermanual > /dev/null 2>&1 ; \
	     latex_count=`expr $$latex_count - 1` ;\
	   done \
	)

um_class_ref.tex: fields-*.tex desc-*.tex
	@echo -n "generating Object Reference (um_class_ref.tex): "
	@(echo "%% Automatically generated object reference" > um_class_ref.tex ;\
	for f in desc-*.tex; do \
	  c=`echo $$f | tr -s '_' | sed -e s#"desc-\(.*\).tex"#"\1"#`; \
	  if [ -f fields-$$c.tex ]; then \
            echo -n $$c" " ; \
	    echo -e "\n\n"  >>  um_class_ref.tex; \
	    cat $$f >> um_class_ref.tex ;\
	    h=`head -1 rw-fields-$$c.tex`; \
	    if [ "$$h" != "" ]; then \
	      echo "\subsubsection*{Read/writable Fields}" >>  um_class_ref.tex ;\
	      echo "\begin{description}" >>  um_class_ref.tex ;\
	      echo "\setlength{\itemsep}{-0.9ex plus 0.1ex minus 0.1ex}" >>  um_class_ref.tex ;\
	      cat rw-fields-$$c.tex  >> um_class_ref.tex ;\
	      echo "\end{description}" >>  um_class_ref.tex ;\
	    fi; \
	    h=`head -1 ro-fields-$$c.tex`; \
	    if [ "$$h" != "" ]; then \
	      echo "\subsubsection*{Readonly Fields}" >>  um_class_ref.tex ;\
	      echo "\begin{description}" >>  um_class_ref.tex ;\
	      echo "\setlength{\itemsep}{-0.9ex plus 0.1ex minus 0.1ex}" >>  um_class_ref.tex ;\
	      cat ro-fields-$$c.tex >>  um_class_ref.tex ;\
	      echo "\end{description}" >>  um_class_ref.tex ;\
	    fi ;\
          fi; \
        done; \
	echo " "; )

fields-*.tex: ../../src/*.tex
	@echo copying field descriptions
	@cp ../../src/*.tex .
	@(for f in fields-*.tex; do \
	    grep READWRITE $$f > rw-$$f ; \
	    grep READONLY  $$f > ro-$$f ; \
	  done; )

../../src/*.tex:
	@echo generating field descriptions
	$(MAKE) -C ../../src field-files

desc-*.tex: class*.tex
	@echo extracting detailed description from doxygen latex output
	@(for c in class*.tex; do \
	    ./strip_detailed_desc $$c ; \
	  done; )

class*.tex: ../reference/latex/class*.tex
	@echo copying doxygen latex output
	@cp ../reference/latex/class*.tex . 

../reference/latex/class*.tex:
	@echo generating doxygen latex output
	$(MAKE) -C ../reference latex

fields.tex: ../reference/latex/fields.tex
	perl -n -e 'print if($$.>1);' < ../reference/latex/fields.tex > fields.tex 

mrproper: clean
	@rm -f usermanual.pdf usermanual.ps* *.aux *.bbl *.blg *.dvi *.log
	@rm -f *.out *.toc *.html *.pl images.ps *.gif *.png *.old WARNINGS commonstuff.tex images.tex
	@rm -rf auto
	@rm -f class*.tex ??-fields-*.tex fields-*.tex desc-*.tex um_class_ref.tex *.brf

clean: texclean
	@rm -f usermanual.html usermanual.pdf progmanual.ps.gz

distclean: texclean
	@rm -f WARNINGS commonstuff.tex *.pl images.tex ??-fields-*.tex fields-*.tex desc-*.tex

texclean:
	@rm -f *.aux *.bbl *.blg *.dvi *.log *.out *.toc

