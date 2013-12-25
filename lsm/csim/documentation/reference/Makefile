DOXYFILE=$(shell echo `pwd`/Doxyfile)

default: html

build: mrproper html latex

html: html/index.html

latex: latex/refman.tex

html/index.html: ../../src/*.h ../../src/*.cpp Doxyfile-html
	@(sed -e s#"__ROOT__"#"`pwd`"# Doxyfile-html > Doxyfile)
	@(cd ../../src; doxygen $(DOXYFILE); )

latex/refman.tex: ../../src/*.h ../../src/*.cpp Doxyfile-latex
	@(sed -e s#"__ROOT__"#"`pwd`"# Doxyfile-latex > Doxyfile)
	@(cd ../../src; doxygen $(DOXYFILE); )


clean:
	rm -rf html latex Doxyfile

mrproper:
	rm -rf html latex Doxyfile

distclean:
	@rm -f Doxyfile
