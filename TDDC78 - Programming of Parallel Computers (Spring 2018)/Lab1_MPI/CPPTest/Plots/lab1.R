library(ggplot2)
library(lubridate)
library(reshape2)
rm(list=ls())

#Image 1 Blurc, radius 30.
x = c(1,2,4,8,16,32)
y = seconds(c("11.271579", "6.082132", "3.560819", "1.480032", "1.084914", "1.212892"))
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() + 
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("MPI: Image 1 using Blur filter with radius 30")

#Image 4 Blurc, radius 30.
x = c(1,2,4,8,16)
y = c(194, 96, 44.386361, 22.441998, 12.435198, 6.964373)
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() + 
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("MPI: Image 4 using Blur filter with radius 30")

###########################

#Image 1 thresc
x = c(1,2,4,8,16,32)
y = c(0.221572,0.288805,0.227981,0.222884,0.230112,0.982290)
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() +
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("MPI: Image 1 using threshold filter")

#Image 4 thresc
x = c(1,2,4,8,16,32)
y = c(0.311255,0.290991,0.282195,0.285985,0.292647, 0.959748)
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() +
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("MPI: Image 4 using threshold filter")
