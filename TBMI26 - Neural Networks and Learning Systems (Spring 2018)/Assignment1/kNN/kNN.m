function [ labelsOut ] = kNN(X, k, Xt, Lt)
%KNN Your implementation of the kNN algorithm
%   Inputs:
%               X  - Features to be classified
%               k  - Number of neighbors
%               Xt - Training features
%               LT - Correct labels of each feature vector [1 2 ...]'
%
%   Output:
%               LabelsOut = Vector with the classified labels

labelsOut  = zeros(size(X,2),1);
classes = unique(Lt);
numClasses = length(classes);
nobs = length(X(1,:));

for i = 1:nobs
    Y = sqrt((Xt(1,:) - X(1,i)).^2 + (Xt(2,:) - X(2,i)).^2); 
    Ysort = sort(Y);
    [~,idxValue] = ismember(Ysort(1:k),Y);
    labelsOut(i) = mode(Lt(idxValue));  
end



