library(ggplot2)
library(reshape2)

rm(list=ls())
x = c(1,2,4,8,16,32,64)
y = c(10.11, 10.37, 10.56, 11.23, 12.72, 12.77, 12.79)
pressure = c(0.006, 0.018,0.06, 0.217, 0.822, 3.213, 4.489)
lab = c(1,2,3,4,5,6,7,8,9,10,11,12,13)
x_name <- "Cores"
y_name <- "Elapsed_Time"
fill <- "pressure"
df <- data.frame(x,y,pressure)
names(df) <- c(x_name,y_name, fill)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time, fill = pressure)) + 
  geom_point(shape = 21, stroke = 1.5, colour = "black",fill = "firebrick", aes(size = pressure)) + 
  scale_x_continuous(labels = as.character(x), breaks = x) +
  expand_limits(x = 0, y = 0) +
  scale_y_continuous(limits=c(0,14), expand = c(0,0))+
  geom_text(aes(label=pressure), vjust=-1.5, size=3) +
  ggtitle("MPI Lab4 Particle Simulation. Box size 10000x10000. 500 particles/core. \nSimulation time: 10000") 

x = c(1,2,4,8,16,32,64)
y = c(9.29, 9.37, 9.19, 8.68, 8.49, ,)
pressure = c(0.20, 0.596,1.85, 6.16, 18.77, , )
lab = c(1,2,3,4,5,6,7,8,9,10,11,12,13)
x_name <- "Cores"
y_name <- "Elapsed_Time"
fill <- "pressure"
df <- data.frame(x,y,pressure)
names(df) <- c(x_name,y_name, fill)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time, fill = pressure)) + 
  geom_point(shape = 21, stroke = 1.5, colour = "black",fill = "firebrick", aes(size = pressure)) + 
  scale_x_continuous(labels = as.character(x), breaks = x) +
  expand_limits(x = 0, y = 0) +
  scale_y_continuous(limits=c(0,14), expand = c(0,0))+
  geom_text(aes(label=pressure), vjust=-1.5, size=3) +
  ggtitle("MPI Lab4 Particle Simulation. Box size 1000x1000. 500 particles/core")




