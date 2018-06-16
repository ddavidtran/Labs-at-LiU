function sfunction = AutoFocus(FStack)
%TNM087Template General Template for TNM087 Lab Tasks
%   Everybody has to use this template
%
%% Who has done it
%
% Author: Davtr766 David Tran
% Co-author: Tobma571 Tobias Matts
%
%% Syntax of the function
%
% Input arguments: FStack is the INPUT VARIABLE with the 192 images of size
% 32x32 (not the filename) 
% 
% Output arguments: sfunction is the output variable that contains the sharpness function  
% sfunction(l) is the sharpness value = normed frequency content of image l
% in FStack
%
% You MUST NEVER change the first line
%
%% Basic version control (in case you need more than one attempt)
%
% Version: 1
% Date: 2017-01-23
%
% Gives a history of your submission to Lisam.
% Version and date for this function have to be updated before each
% submission to Lisam (in case you need more than one attempt)
%
%% General rules
%
% 1) Don't change the structure of the template by removing %% lines
%
% 2) Document what you are doing using comments
%
% 3) Before submitting make the code readable by using automatic indentation
%       ctrl-a / ctrl-i
%
% 4) In case a task requires that you have to submit more than one function
%       save every function in a single file and collect all of them in a
%       zip-archive and upload that to Lisam. NO RAR, NO GZ, ONLY ZIP!
%       All non-zip archives will be rejected automatically
%
% 5) Often you must do something else between the given commands in the
%       template
%
%

%% Size of images and number of images
%
load FStack.mat;
[sx,sy,noimages] = size(FStack);

%% Generate a grid, convert the Euclidean to polar coordinates
%
[X,Y] = meshgrid(1:sx,1:sy);
[TH,R] = cart2pol(X,Y);

%% Number of COMPLETE rings in the Fourier domain 
% ignore the points in the corners

norings = 8; %Change this if you want

RQ = round(R/max(R(:))*norings); %this is the quantized version of R where 
% the pixel value is the index of the ring 
% (origin = 0, and the point (0,r) has index norings  
%...

maxindex = max(RQ(:));

%% Number of grid points in each ring

ptsperring = zeros(norings,1);
for ringno = 1:norings+1
    ptsperring(ringno) = sum(RQ(:) == ringno-1);
end

%% Sum of fft magnitude per image - per ring

absfftsums = zeros(noimages,maxindex);
dist = 1/norings;
for imno = 1:noimages
    padimage = padarray(FStack(:,:,imno),[1 1]); % Zero-pad to increase frequency resolution.
    ftplan = fft2(padimage); % 2D fft
    cftplan = fftshift(ftplan(2:end-1, 2:end-1)); % move origin to the center
    
    for ringno = 1:norings;
        ringmask = (RQ >= dist*(ringno-1) & RQ <= dist*ringno);  %this is a logical array defining the ring
        Akm = abs(ringmask*cftplan);
        absfftsums(imno,ringno) = sum(Akm(:))/ptsperring(ringno); % average over Fourier magnitude in ring
    end
end
    
%% Compute weighted average over the ring sums
%

meanfreqcontent = zeros(noimages,1);

w = 1:maxindex; % here you may use the ring index, radius or something you define

for imno = 1:noimages
    meanimg = FStack(:,:,imno);
    meanfreqcontent(imno) = sum(absfftsums(imno,:) .* w) /mean(meanimg(:));% combine w and absfftsums
end

%% Requested result
% default solution but you can invent something else if you want
sfunction = meanfreqcontent;

end

