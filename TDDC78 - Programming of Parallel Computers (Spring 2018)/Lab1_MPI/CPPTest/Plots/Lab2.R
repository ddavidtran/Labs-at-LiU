library(ggplot2)
library(lubridate)
library(reshape2)
rm(list=ls())

## PTHREADS ##

#Image 1 Blurc, radius 30.
x = c(1,2,4,8,16)
y = seconds(c(8.49, 4.27, 2.10, 1.08, 0.91))
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() + 
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("Pthreads Image 1 using Blur filter with radius 30")

#Image 4 Blurc, radius 30.
x = c(1,2,4,8,16)
y = c(159.66, 79.82, 39.78, 20.05, 14.30)
x_name <- "Cores"
y_name <- "Elapsed_Time"

require(reshape2)
df <- data.frame(x,y)

names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() + 
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("Pthreads Image 4 using Blur filter with radius 30")

###########################

#Image 1 thresc
x = c(1,2,4,8,16)
y = c(0.0034,0.0017,0.0013,0.0013,0.0018)
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() +
  ggtitle("Pthreads Image 1 using threshold filter")

#Image 4 thresc
x = c(1,2,4,8,16)
y = c(0.034,0.017, 0.011, 0.006, 0.0063)
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() +
  ggtitle("Pthreads Image 4 using threshold filter")
