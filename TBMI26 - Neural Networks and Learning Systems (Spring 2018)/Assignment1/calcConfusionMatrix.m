function [ cM ] = calcConfusionMatrix( Lclass, Ltrue )
classes = unique(Ltrue);
numClasses = length(classes);
cM = zeros(numClasses);

cM = confusionmat(Lclass, Ltrue);

% Add your own code here

end

