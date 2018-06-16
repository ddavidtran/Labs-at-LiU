#########################
# FUNCTIONS DEFINITIONS #
#########################
#gaussianKernel(u,h) -> computes Gaussian Kernel
#station_distance(PoI, h) -> computes distance from station to point of interest
#date_distance(DoI, h) -> computes distance between the day a temperature measurement was made and the day of interest.
#calc_time_diff(ToI) -> computes time difference in format HH:MM:SS.
#time_distance(calculated_time, h) -> distance between the hour of the day a temperature measurement was made and the hour of interest.


#############
# READ DATA #
#############
rm(list=ls())
set.seed(1234567890)
library(geosphere)
stations <- read.csv("stations.csv", header=TRUE, sep=",", dec=".", fileEncoding = "latin1")
temps <- read.csv("temps50k.csv")
st <- merge(stations,temps,by="station_number")

##################
# Data variables #
##################
h_distance <- 100000 # These three values are up to the students
h_date <- 25 #days
h_time <- 2.5 #hours
a <- 58.4274 # The point to predict (up to the students)
b <- 14.826
latlong = c(a,b)
date <- "2013-11-04" # The date to predict (up to the students)
times <- c("04:00:00", "06:00:00","08:00:00","10:00:00","12:00:00","14:00:00", "16:00:00","18:00:00","20:00:00","22:00:00", "00:00:00")
temp_sum <- vector(length=length(times))
temp_mult <- vector(length=length(times))
############################################################################################################################

#########################
# FUNCTIONS DEFINITIONS #
#########################
gaussianKernel = function(u,h){
  return(exp(-abs(u/h)^2))
}

station_distance <- function(PoI,h){
  d = numeric(dim(st)[1])
  for(i in 1:length(d)){
    p2 = c(st[i,"latitude"], st[i,"longitude"])
    d[i] = gaussianKernel(distHaversine(p2,PoI),h)
  }
  return(d)
}

date_distance <- function(DoI,h){
  dist_date = numeric(dim(st)[1])
  for(i in 1:length(st$date))
    dist_date[i] = as.numeric(difftime(strptime(DoI, "%Y-%m-%d"),strptime(st$date[i], "%Y-%m-%d"))) %% 182.5
  d = gaussianKernel(dist_date, h)
  return(d)
}

calc_time_diff <- function(ToI){
  return(abs(as.numeric(difftime(strptime(ToI, "%H:%M:%S"), strptime(st$time,"%H:%M:%S"))))/3600)
}

time_distance <- function(calculated_time, h){
  time_diff = calc_time_diff(calculated_time)
  for(i in 1:length(time_diff)){
    if(time_diff[i] > 12)
      time_diff[i] = 24 - time_diff[i]
  }
  d = gaussianKernel(time_diff,h)
  return(d)
}
############################################################################################################################


########
# MAIN #
########

#Filter out measurements that are posterior to the date of interest.
st <- st[!(difftime(st$date,date))>0,]

#Plot Kernels.
#Distance plot.
kernel_dist = gaussianKernel(seq(0,h_distance*2,5000), h_distance)
plot(seq(0,100*2, 5), kernel_dist, ylim = c(0,1), xlab = "Distance", ylab ="Kernel", main = "Distance Kernel")

#Time plot
kernel_time = gaussianKernel(seq(0,h_time*2,0.1), h_time)
plot(seq(0,h_time*2,0.1), kernel_time, ylim = c(0,1), ylab = "Weight", xlab = "Hour", main = "Time Kernel")

#Date plot
kernel_date = gaussianKernel(seq(0,60,1), h_date)
plot(seq(0,60,1), kernel_date, ylab = "Weight", xlab = "Days", main = "Date Kernel")

station_dist = station_distance(c(a,b), h_distance)
date_dist = date_distance(date,h_date)
for(i in 1:length(times)){
  time_dist = time_distance(times[i], h_time)
  kernel_sum = station_dist + time_dist + date_dist
  kernel_prod = station_dist * time_dist * date_dist
  temp_sum[i] = (kernel_sum %*% st$air_temperature)/sum(kernel_sum)
  temp_mult[i] = (kernel_prod %*% st$air_temperature)/sum(kernel_prod)
}
mean_temp_sum = mean(temp_sum)
mean_temp_mult = mean(temp_mult)
plot(seq(from = 4, to = 24, by = 2), temp_sum, type="o", ylim = c(3,10), xlab = "Time of the day", ylab = "Temperature", main = "Sum of Kernels") #Sum of kernels.
plot(seq(from = 4, to = 24, by = 2), temp_mult, type="o", ylim = c(3,10), xlab = "Time of the day", ylab = "Temperature", main = "Product of Kernels") #Product of kernels.
