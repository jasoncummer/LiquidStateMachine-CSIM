% MIBAYES Calculate mutual information and train/test Bayes classifier
% 
%   Syntax
% 
%     [MI,HX,HY,HXY,bayes,tables] = mibayes(X,Y,evalSize,handle);
%     tables = mibayes(handle);
%
%   Arguments
%
%            X ... array of x-samples; one column X(:,k) is one sample
%            Y ... array of y-samples; one column Y(:,k) is one sample
%     evalSize ... optional vector of sizes at which to estimate the
%                  mutual  information;
%       handle ... optional handle (double scalar) to distributions/tables;
%                  for details see below.
%
%   Return Values
%
%           MI ... vector of estimated mutual information; same length
%                  as evalSize or one if evalSize is omitted
%           HX ... vector of entropy estimates of the x-samples.  same
%                  length as evalSize or one if evalSize is omitted
%           HY ... vector of entropy estimates of the y-samples.  same
%                  length as evalSize or one if evalSize is omitted
%          HXY ... vector of joint entropy estimates of the y-samples.
%                  same length as evalSize or one if evalSize is omitted
%       tables ... resulting margin and joint count tables; details see below;
%        bayes ... struct array reporting the performance of the empirical 
%                  Bayes classifier trained on the data; details see below
%
%   Basic Description
% 
%     MI = mibayes(X,Y) calculates the direct estimate of the mutual
%     information (MI) between X and Y. X and Y are assumed to be lists of
%     observations/samples of stationary discrete random variables. The k-th
%     pair of samples is given by the two column vectors X(:,k) and Y(:,k).
% 
%   Algorithm
% 
%     The mutual information is directly calculated from the estimated joint
%     probability density function which is just the normalized joint count
%     table. During the counting process we find the minimum set of unique
%     vectors XU and YU: for each sample X(:,k) and Y(:,k) there are
%     index i and j such that X(:,k) == XU(:,i) and Y(:,k) == YU(:,j). We
%     call i (j) the class of sample X(:,k) (Y(:,k)).
% 
%   Detailed Description
% 
%     MI = mibayes(X,Y,evalSize) calculates MI for several data sizes.
%     MI(i) contains  the mutual information calculated from the subsets 
%     X(:,1:evalSize(i)) and Y(:,1:evalSize(i)) where i=1:length(evalSize). 
%     It is required that evalSize is a vector of increasing integers.
%     The samples evalSize(end):nTotalSamples are used as a test set for the
%     Bayes classifier (see below).
%
%     MI = mibayes(X,Y,evalSize,handle) is as described as above but
%     the internal counting tables (see below and Algorithm) which are
%     generated are not delete when MIBAYES exits. If on a subsequent
%     call to MIBAYES the same handle is used the existing count
%     tables are used as initial tables and the new data is added to
%     this previous counts. This allows for incremental estimation of
%     the mutual information as more and more data becomes available.
%
%     tables = mibayes(handle) just returns the current tables with the 
%     given handle.
%
%     [MI,HX,HY,HXY,bayes,tables] = mibayes(X,Y,evalSize,handle)
%     returns in addition to the mutual information (MI) also the following:
% 
%       - HX: entropy of X
%       - HY: entropy of Y
%       - HXY: joint entropy of the pair of samples (X,Y)
% 
%       - bayes: performance of the empirical Bayes classifier which tries to 
%                predict X(:,k) given Y(:,k). Performance is reported as
% 
%           bayes.TrainError ... percentage of miss-classified/miss-predicted 
%                                training samples
%           bayes.LooError   ... percentage of miss-classified/miss-predicted 
%                                samples using a leave-one-out procedure
%           bayes.TestError  ... percentage of miss-classified/miss-predicted 
%                                test samples evalSize(end):nTotalSamples
%
%           bayes.TrainCM  ... the train "confusion matrix" where TrainCM(i,j) 
%                              is the number of cases where a sample of class i 
%                              was classified as class j. 
%           bayes.LooCM    ... the leave-one-out "confusion matrix"
%                              was classified as class j.
%           bayes.TrainCM  ... the test "confusion matrix"
% 
%       TrainError and LooError are of the same length as evalSize
%       (or 1 if evalSize is omitted) and are evaluated for the 
%       corresponding subsets (see above). Note that for the "confusion matrices"
%       and TestError the subset 1:evalSize(end) is used.
%
%       - tables: A struct containing the count tables.
% 
%            tables.UX:  Unique set of vectors for X
%            tables.NX:  Counts of vektors in UX
%            tables.UY:  Unique set of vectors for Y
%            tables.NY:  Counts of vektors in UY
%            tables.NXY: The joint count table. NXY(i,j) is the number of occurrences
%                   of a pair <UX(:,i),UY(:,j)>.
% 
%         Note that for the "state" information the subset 1:evalSize(end) is used.
%
%
%   See also
% 
%     mibayes.c, mi_and_bayes.c, unique
% 
%   Installation
%
%     Since MIBAYES is a MEX file one has to compile it. Use the command
%
%        mex mibayes.c mi_and_bayes.c
%
%     at the matlab prompt to do this.
%
%   Author
% 
%     Thomas Natschlaeger, Apr. 2003, tnatschl@igi.tu-graz.ac.at
% 
 
%   $Author: tnatschl $, $Date: 2003/05/26 12:42:24 $, $Revision: 1.1 $
%   $Cross-References$

