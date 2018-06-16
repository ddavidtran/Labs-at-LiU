library(ggplot2)
library(lubridate)
library(reshape2)
rm(list=ls())

#Image 1 Blurc, radius 30.
x = c(1,2,4,8,16)
y = seconds(c("11.271579", "6.082132", "3.560819", "1.480032", "1.084914"))
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() + 
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("Image 1 using Blur filter with radius 30")

#Image 4 Blurc, radius 30.
x = c(1,2,4,8,16)
y = c("03:14.216576", "01:36.999622", "00:44.386361", "00:22.441998", "00:12.435198")
x_name <- "Cores"
y_name <- "Elapsed_Time"

require(reshape2)
df <- data.frame(x,y)
df$hms <- format(df$y, format = "%M:%S")
df$hms <- as.POSIXct(df$hms, format = "%M:%S")
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = hms)) + 
  geom_line() +
  scale_x_datetime(labels = date_format("%M:%S")) +
ggtitle("Image 4 using Blur filter with radius 30")

###########################

#Image 1 thresc
x = c(1,2,4,8,16)
y = c(5.081747,03.364913,03.252787,61,2)
x_name <- "Cores"
y_name <- "Elapsed_Time"

require(reshape2)
df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() +
  ggtitle("Image 1")

#Image 4 thresc
x = c(1,2,4,8,16)
y = c(5.081747,03.364913,03.252787,61,2)
x_name <- "Cores"
y_name <- "Elapsed_Time"

require(reshape2)
df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() +
  ggtitle("Image 1")
