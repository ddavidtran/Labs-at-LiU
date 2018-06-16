function [ k ] = getOptimalK(X, D, maxK, L)

% X - Data
% maxK - Maximum amount of neighbours to consider
% L - labels

nFolds = 4; % Number of Bins you want to devide your data into
numSamples = Inf; % Number of samples per label per bin, set to inf for max number (total number is numLabels*numSamplesPerBin)
selectAtRandom = true; % true = select features at random, false = select the first features
[ Xt, Dt, Lt ] = selectTrainingSamples(X, D, L, numSamples, nFolds, selectAtRandom );

bestK = 0;
maxAcc = 0;

for n = 1:nFolds
    Train = horzcat(Xt{1:end ~= n});
    trainLabels = horzcat(Lt{1:end ~= n});
    
    for k = 1:maxK
        labels = kNN(Xt{n}, k, Train, trainLabels);
        cM = calcConfusionMatrix(labels, Lt{n});
        acc = calcAccuracy(cM);
        if(acc > maxAcc)
            maxAcc = acc;
            bestK = k;
        end
    end
end

k = bestK;
%Either selectTrainingAndTestSamples.m or SelectTrainingSamples.m.
