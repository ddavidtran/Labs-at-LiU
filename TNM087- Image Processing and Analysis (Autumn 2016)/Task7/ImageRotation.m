function RImage = ImageRotation(OImage, center, deangle )
%FRotate Rotate an image around a given point
%   Everybody has to use this template
%
%% Who has done it
%
% Author: Same LiU-ID/name as in the Lisam submission
% Co-author: You can work in groups of max 2, this is the LiU-ID/name of
% the other member of the group
%
%% Syntax of the function
%
% Input arguments:  OImage original image
%                   center 2-vector with center point of the rotation 
%                       see the pdf for an definition of center
%                   degangle rotation angle in degrees, rotation is 
%                       clockwise
%
% Output arguments: RImage is the rotated image
%
% You MUST NEVER change the first line
%
%% Basic version control (in case you need more than one attempt)
%
% Version: 1
% Date: today
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

%% Information about the image (size, type etc)
%       You can assume that it has uint8 pixels 
%       What should you do if this is not the case?
%
OImage = imread('Checkerboard.jpg');
[sr,sc,nc] = size(OImage);

%% Generate coordinate vectors for the shifted coordinate system
% (this means converting the index vector for a pixel to the 
%   coordinate vector of the same pixel)
%
ir =[1:sr]; %index vector for pixels along first Matlab dimension
ic =[1:sc]; %same in the second direction

cir = ir-center(2); %shifted ir vector so that center(1) is the origin
cic = ic-center(1);%Same for the second axis

%% Use cir and cic in meshgrid to generate a coordinate grid
%
[C,R] = meshgrid(cir,cic); % cir, cic

%% The polar mesh coordinates are computed with cart2pol
%
[Theta,Rho] = cart2pol(C,R); %

%% Convert the degress, modify the angles and 
%   transform back to Euclidean coordinates 
%   you may skip the next two lines and modify the input to pol2cart
%   if you want

rads = Rho; % degs...
TNew = Theta + ((deangle*pi)/180);% use Theta and degs

[nC,nR] = pol2cart(TNew, rads); %

%% Compute the index vector from the coordinate vector inverting the 
% previous conversion from ir to cir and ic to cic

newir = nR + center(2);
newic = nC + center(1);

%% Now use nearest neighbor interpolation (round) and rotate

RImage = uint8(zeros(sr,sc,nc));
for k = 1:sr
    for l = 1:sc
        if(newir(k,l) < 1 || newir(k,l) > sr)
            newir(k,l) = 0;
        elseif(newic(k,l) < 1 || newic(k,l) > sc)
            newic(k,l) = 0;
        else
        RImage(k,l) = OImage(uint32(round(newir(k,l))), uint32(round(newic(k,l))));
    end
    end
end