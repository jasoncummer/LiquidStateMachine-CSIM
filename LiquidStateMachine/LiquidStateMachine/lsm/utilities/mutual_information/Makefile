default: mibayes.mexglx

mibayes.mexglx: mibayes.c mi_and_bayes.c
	mex -O mibayes.c mi_and_bayes.c

clean:
	rm -rf mibayes.mexglx
