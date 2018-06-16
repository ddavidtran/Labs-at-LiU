rm(list=ls())
source("https://bioconductor.org/biocLite.R")
biocLite("EBImage")
library("EBImage")
library(corrgram)
library(colorspace)

img <- readImage("01.jpg")
display(img, method = "browser")
hist(img)
range(img)
mean_pixel_value <- apply(img, 3, mean)


red = as.vector(img[,,1])
green = as.vector(img[,,2])
blue = as.vector(img[,,3])

test <- rgb2hsv(df)
