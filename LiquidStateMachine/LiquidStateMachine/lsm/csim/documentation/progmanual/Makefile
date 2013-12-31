NAVBUTTONS=../resources/contents.gif ../resources/next.gif ../resources/prev.gif \
	   ../resources/contents.png ../resources/next.png ../resources/prev.png

default: pdf html

pdf: progmanual.pdf

html: progmanual.html

progmanual.html: progmanual.tex progmanual.bbl ../commonstuff.skel ../*_stuff.tex pm-*.tex ../dot.latex2html-init
	@(sed -e s#{_STUFF_}#{../html_stuff}# ../commonstuff.skel > commonstuff.tex)
	@latex2html -init_file ../dot.latex2html-init -image_type gif -local_icons -math -html_version 4.0 progmanual > /dev/null
	@(for f in *.html; do \
            sed -e s/"WIDTH=\"65\" HEIGHT=\"24\""// -e s/"WIDTH=\"63\" HEIGHT=\"24\""// -e s/"WIDTH=\"37\" HEIGHT=\"24\""// $$f > /tmp/f.$$$$; \
             mv /tmp/f.$$$$ $$f; \
          done; )
	@cp $(NAVBUTTONS) .

progmanual.pdf: progmanual.tex  progmanual.bbl ../commonstuff.skel ../*_stuff.tex pm-*.tex
	@(sed -e s#{_STUFF_}#{../pdflatex_stuff}# ../commonstuff.skel > commonstuff.tex)
	@(if pdflatex progmanual | grep "Rerun to get cross-references right"; then \
            pdflatex progmanual; \
          fi)

progmanual.bbl: progmanual.tex
	@(sed -e s#{_STUFF_}#{../latex_stuff}# ../commonstuff.skel > commonstuff.tex)
	@latex progmanual > /dev/null
	@bibtex progmanual > /dev/null

progmanual.dvi: progmanual.tex
	@(sed -e s#{_STUFF_}#{../latex_stuff}# ../commonstuff.skel > commonstuff.tex)
	@latex progmanual
	@bibtex progmanual
	@latex progmanual

mrproper: clean
	@rm -f progmanual.pdf progmanual.ps* *.aux *.bbl *.blg *.dvi *.log
	@rm -f *.out *.toc *.html *.pl *.gif *.png *.old WARNINGS commonstuff.tex images.tex
	@rm -rf auto

clean: texclean
	rm -f progmanual.html progmanual.pdf progmanual.ps.gz

distclean: texclean
	@rm -f WARNINGS commonstuff.tex *.pl images.tex

texclean:
	rm -f *.aux *.bbl *.blg *.dvi *.log *.out *.toc

