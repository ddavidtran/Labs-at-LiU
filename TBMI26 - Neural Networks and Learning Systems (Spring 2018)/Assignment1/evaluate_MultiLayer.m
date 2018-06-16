%% This script will help you test out your single layer neural network code
clc
clear
%% Select which data to use:

% 1 = dot cloud 1
% 2 = dot cloud 2
% 3 = dot cloud 3
% 4 = OCR data

dataSetNr = 4; % Change this to load new data 

[X, D, L] = loadDataSet( dataSetNr );

%% Select a subset of the training features

numBins = 2; % Number of Bins you want to devide your data into
numSamplesPerLabelPerBin = inf; % Number of samples per label per bin, set to inf for max number (total number is numLabels*numSamplesPerBin)
selectAtRandom = true; % true = select features at random, false = select the first features

[ Xt, Dt, Lt ] = selectTrainingSamples(X, D, L, numSamplesPerLabelPerBin, numBins, selectAtRandom );

% Note: Xt, Dt, Lt will be cell arrays, to extract a bin from them use i.e.
% XBin1 = Xt{1};
%% Modify the X Matrices so that a bias is added

% The Training Data
ConstantBias = ones(size(Xt{1}));
Xtraining = vertcat(ConstantBias(1,:), Xt{1});

% The Test Data
Xtest = vertcat(ConstantBias(2,:), Xt{2});


%% Train your single layer network
% Note: You nned to modify trainSingleLayer() in order to train the network

numHidden = 65; % Change this, Number of hidde neurons 
numClasses = size(Dt{1},1);
numFeat = length(Xtraining(:,1));
a = -0.3;
b = 0.3;
numIterations = 20000; % Change this, Numner of iterations (Epochs)
learningRate = 0.0012; % Change this, Your learningrate
worstAcc = inf;
threshhold = 0.96;
numBelowThres = 0;
Tests = 1;

for(i = 1:Tests)

W0 = (b-a).*rand(numHidden, numFeat) + a; % Change this, Initiate your weight matrix W
V0 = (b-a).*rand(numClasses, numHidden+1) + a; % Change this, Initiate your weight matrix V

%
tic
[W,V, trainingError, testError ] = trainMultiLayer(Xtraining,Dt{1},Xtest,Dt{2}, W0,V0,numIterations, learningRate );
trainingTime = toc;

% Plot errors
figure(1101)
clf
[mErr, mErrInd] = min(testError);
plot(trainingError,'k','linewidth',1.5)
hold on
plot(testError,'r','linewidth',1.5)
plot(mErrInd,mErr,'bo','linewidth',1.5)
hold off
title('Training and Test Errors, Multi-Layer')
legend('Training Error','Test Error','Min Test Error')

% Calculate The Confusion Matrix and the Accuracy of the Evaluation Data
% Note: you have to modify the calcConfusionMatrix() function yourselfs.

[ Y, LMultiLayerTraining ] = runMultiLayer(Xtraining, W, V);
tic
[ Y, LMultiLayerTest ] = runMultiLayer(Xtest, W,V);
classificationTime = toc/length(Xtest);
% The confucionMatrix
cM = calcConfusionMatrix( LMultiLayerTest, Lt{2});

% The accuracy
acc = calcAccuracy(cM);
if(acc < worstAcc)
    worstAcc = acc;
end
if(acc < threshhold)
    numBelowThres = numBelowThres + 1;
end

display(['Time spent training: ' num2str(trainingTime) ' sec'])
display(['Time spent classifying 1 feature vector: ' num2str(classificationTime) ' sec'])
display(['Accuracy: ' num2str(acc)])
end

display(['Worst Accuracy: ' num2str(worstAcc)])
display(['Times below: ' num2str(numBelowThres) ' out of ' num2str(Tests)])
%% Plot classifications
% Note: You do not need to change this code.

if dataSetNr < 4
    plotResultMultiLayer(W,V,Xtraining,Lt{1},LMultiLayerTraining,Xtest,Lt{2},LMultiLayerTest)
else
    plotResultsOCR( Xtest, Lt{2}, LMultiLayerTest )
end
