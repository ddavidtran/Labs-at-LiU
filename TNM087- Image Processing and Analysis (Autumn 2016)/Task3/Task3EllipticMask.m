function MImage = EllipticMask(FImage)
%EllipsMask Generate a mask for one eye and the complete face
%   Change pixel color for one eye and extract the face
%
%% Who has done it
%
% Author: Same LiU-ID/name as in the Lisam submission
% Co-author: You can work in groups of max 2, this is the LiU-ID/name of
% the other member of the group
%
%% Syntax of the function
%
% Input arguments:  Fimage: Image containing a face 
%
% Output argument:  Mimage: Image with elliptical mask and a red eye
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

%% Your code starts here
%

%% create the output image (RGB!) which is a copy of the original image
% Use einstein.jpg as your FImage

[sr,sc] = size(FImage);
MImage = FImage;

%% Generate the coordinates of the grid points
 [C R] = meshgrid((1:sc),(1:sr));
% Remember the matlab convention regarding rows, columns, x and y coordinates. Feel free to use 
% [Y,X] = meshgrid((1:sx),(1:sy)) or whatever notation instead if you feel more comfortable with that notation



%% Pick three points that define the elliptical mask of the face
% Read more about ellipses at https://en.wikipedia.org/wiki/Ellipse
% Your solution must at least be able to solve the problem for the case 
% where the axes of the ellipse are parallel to the coordinate axes
%
imshow(MImage);
fpts = ginput(3);

%First two points, specify major axis.
centerX = (fpts(1,2)+fpts(2,2))/2;
centerY = (fpts(1,1)+fpts(2,1))/2;

%Third point specifies the radius between third point and major axis.
radiusY = fpts(3,1) - fpts(1,1);
radiusX = fpts(3,2) - fpts(1,2);

ellipse = ((C - centerY).^2 ./ radiusY^2) + ((R - centerX).^2 ./ radiusX^2) <=1;

%% Generate the elliptical mask and 
% set all points in MImage outside the mask to black 

fmask = ellipse; % this is the mask use C and R to generate it
MImage(~fmask) = 0;% here you modify the image using fmask

%% Pick two points defining one eye, generate the eyemask, paint it red
epts = ginput(2);
%First point is midpoint.
midPointX = epts(1,2);
midPointY = epts(1,1);
secondPointX = epts(2,2);
secondPointY = epts(2,1);
radius = sqrt((secondPointX-midPointX).^2 - (secondPointY-midPointY).^2);
Circle = (R - midPointX).^2 + (C - midPointY).^2 <= radius.^2;
% circular eye mask again, use C and R
emask = Circle;
 % replace eye points with red pixels
emask_R = MImage;
emask_G = MImage;
emask_B = MImage;
emask_R(emask) = 255;
emask_B(emask) = 0;
emask_G(emask) = 0;

% Concatenating the channels and storing in MImage 
MImage = cat(3,emask_R,emask_G,emask_B);

%% Display result if you want
%
imshow(MImage);

end

