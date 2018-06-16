function [ Profile1, Profile2 ] = Vignette( Im1, Im2, norings )
%Vignette: Compare vignetting properties of two images
%   If Im1 and Im2 show the same object imaged under different conditions
%   then Profile1 and Profile2 describe the mean gray value as a function
%   of the radius
%
%% Who has done it
%
% Author: David Tran davtr766
% Co-author: Tobias Matts tobma571
%
%% Syntax of the function
%
% Input arguments:  Im1, Im2 are input images, you can assume that they are
%                       gray value images (of type uint8, uint16 or double)
%                   norings is optional and describes the number of
%                       concentric rings to use. The default is to use 50 rings
% Output arguments: Profile1 and Profile2 are vectors with the mean gray
%                       values in each ring. The final results are normed
%                       so that the maximum values in Profile1 and Profile2
%                       are equal to one
%
% The images to use are CWhite1.jpg (Canon) and HWhite1.jpg (Holga9
%
% You MUST NEVER change the first line
%
%% Basic version control (in case you need more than one attempt)
%
% Version: 2
% Date: 2016-12-15
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
%% Your code starts here
%
%% Generate two square images cIm1 and cIm2 that have the same size
% Use the center of the images and if at least one of them is an RGB image 
% either convert to gray value or exit with an error message
[sr1, sc1, nc] = size(Im1);
[sr2, sc2, nc2] = size(Im2);
        
i = min([sr1 sc1 sr2 sc2]); % Check smallest dimension.
    
cIm1 = Im1((sr1-i)/2 +1: (sr1-i)/2 + i, (sc1-i)/2 + 1: (sc1-i)/2 + i); % Crop Im1 
cIm2 = Im2((sr2-i)/2 +1: (sr2-i)/2 + i, (sc2-i)/2 + 1: (sc2-i)/2 + i); % Crop Im2

Profile1 = zeros(norings,1);
Profile2 = Profile1;
%% Now you have two gray value images of the size (square) size and you 
%   generate grid with the help of an axis vector ax that defines your
%   rings
%
% or read the documentation of linspace
ax = linspace(-1,1,i);
ax2 = linspace(0,1,norings);
dist = 1/norings;
[C R] = meshgrid(ax, ax); %Euclidean mesh
[~,Rho] = cart2pol(C,R); %Polar coordinates comment on the ~ used
%% Do the actual calculations
for ringno = 1:norings
    RMask = (Rho > dist*(ringno-1) & Rho < dist*ringno); % Generate a mask describing the ring number ringno
    nopixels = numel(RMask); % Compute the number of pixes in RMask
    pixelsum = sum(cIm1(RMask));;% Compute the sum over all pixel values in RMask in Im1
    Profile1(ringno) = mean(cIm1(RMask)); % ../.. Mean gray value of pixels i RMask
    Profile2(ringno) = mean(cIm2(RMask)); % ../.. Mean gray value of pixels i RMask
end
%% Finally the normalization to max value one 
%
Profile1 = Profile1/(max(Profile1)); %Normalize values to max 1.
Profile2 = Profile2/(max(Profile2)); %Normalize values to max 1.
%% Extra question: How can you find out if Im1 is better than Im2?
%
plot1 = plot(Profile1);
hold on;
plot2 = plot(Profile2);
legend([plot1 plot2], 'Profile1', 'Profile2');
end
