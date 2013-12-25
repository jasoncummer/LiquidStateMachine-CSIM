function install

addpath('..');

condmex( 'mibayes', 'mutual_information', 'mex -O mibayes.c mi_and_bayes.c' );


fprintf('Utilities *succesfully* installed.\n\n');
