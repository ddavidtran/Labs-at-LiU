function [ Y, L ] = runSingleLayer(X, W)
%EVALUATESINGLELAYER Summary of this function goes here
%   Inputs:
%               X  - Features to be classified (matrix)
%               W  - Weights of the neurons (matrix)
%
%   Output:
%               Y = Output for each feature, (matrix)
%               L = The resulting label of each feature, (vector) 

Y = W*X;

% Calculate classified labels
[~, L] = max(Y,[],1);
L = L(:);
end

