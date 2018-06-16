function [ ImSize, ImType, BitPerPixel, MaxMin, RGBpts, figh ] = ...
    BasicImageInfo( filename, nopts)
%BasicImageInfo: Collect basic information about an image
%   Use imfinfo to extract the size of the image and the image type
%   Use ginput to pick a number of positions in a figure
%   Mark these points with white squares
%
%% Who has done it
%
% Author: davtr766/David Tran
% Co-author: tobma571/Tobias Matts
% 
%
%% Syntax of the function
%
% Input arguments:  filename: pathname to the image file
%                   nopts: Number of pixel positions to pick with ginput
% Output arguments: ImSize: size of the image
%                       in a two-element vector of the form: [Width, Height]
%                   ImType: string usually either jpg or tif
%                   BitPerPixel: Number of bits per pixel (8 or 16)
%                   MinMax: Maximum and minimum values of the elements
%                       in the image matrix in a two-element vector: [minvalue, maxvalue]
%                   RGBpts: First use ginput to select the coordinates
%                       of nopts points in the image,
%                       then select the RGB vectors at these positions in
%                       RGBpts which is a matrix of size (nopts,3)
%                   figh: this is a handle to a figure in which you marked
%                       the selected positions with white squares
%
% You MUST NEVER change the first line
%
%% Basic version control (in case you need more than one attempt)
%
% Version: 2
% Date: 2016-11-18
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
%

%% Internal variable describing the size of the marked square
Qsize = 10;

%% Your code starts here
% 

%% Collect image information with imfinfo 
%   (ONE line of code for each output variable)
img = imfinfo(filename); %Extract information about the file.
ImSize = [img.Width img.Height]; %Declare two-element vector
ImType = img.Format;
BitPerPixel = img.BitDepth;
% Use 'ColorType' to find out if it is a color or a grayvalue image 
%% Compute minimum and maximum values
%   (ONE line of code)
OImage = imread(filename); % Reads the image from the input file 
MaxMin = [max(OImage(:)),min(OImage(:))]; % Creates a two-vector containing minimum and maximum value of the image

%% Pick the pixel positions and collect the RGBvectors
% Decide what you do if it is a grayvalue image
%
RGBpts = zeros(nopts,size(OImage,3)); % Creates an matrix. The size of the matrix depends on if it is a grayscale or RGB image.
fh1 = imshow(filename);
PtPos = uint32(ginput(nopts)); % Takes point position of the cursor.
for k = 1:nopts
    RGBpts(k,:) = OImage(PtPos(k,2),PtPos(k,1),:); % Assign RGB values of the point position.
end

%% Generate the white squares and display the result
%
figh = figure;
DImage = OImage;
z = size(DImage,3); % Determine if it is a grayscale or RGB image.
qr = round(Qsize/2); %From your mail.
for k = 1:nopts
    %Generate the white squares
  DImage((PtPos(k,2):(PtPos(k,2)+qr)),(PtPos(k,1):(PtPos(k,1)+qr)),:) = power(2,BitPerPixel/z)-1; % Assign maximum value of RGB in 5x5 area

imshow(DImage);

end