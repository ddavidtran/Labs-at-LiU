rm(list=ls())
library(ggplot2)
library(googleVis)

data = read.csv2("Lab2_data.csv") # Read data.
x_cord = data$GazePointX.px. #X points
y_cord = max(data$GazePointY.px.)-data$GazePointY.px. #Y point, max(y)-y because we want to flip limit of axis.

# Plot all gaze points.
plot(x_cord,y_cord, pch = 21, bg = "violet", main = "Gaze points in (X,Y)", xlab="x", ylab="y")
xmax = max(x_cord)
ymax = max(y_cord)

# Plot points where GazeEventDuration exceeds 800ms.
plot(x_cord[data$GazeEventDuration.mS.>800],y_cord[data$GazeEventDuration.mS.>800], pch = 21, bg = "firebrick",xlim=c(0,xmax), ylim = c(0,ymax), main= "Gaze Points in with over 800ms", xlab="x", ylab="y")

#Plot Gaze points dependence of time. More red means more frequent.
ggplot(data,aes(x_cord,y_cord,color=data$RecordingTimestamp))+geom_point()+
  scale_color_gradient(low="yellow",high="red")

#googleVis motionchart to plot movement of gaze points over time.
gvis_data <- data #New variable of original data.
gvis_data$ID <- 1
gvis_data$GazePointY.px. <- y_cord
M1 <- gvisMotionChart(gvis_data, idvar="ID", timevar="RecordingTimestamp",xvar="GazePointX.px.", yvar="GazePointY.px.")
plot(M1) #Works only for Firefox.


#DBScan over the data.
library(dbscan)
db_data = as.data.frame(cbind(x = data$GazePointX.px., y =max(data$GazePointY.px.)-data$GazePointY.px.)) #New variable of original data.
db2 <-dbscan(db_data, eps = 15, minPts = 6) #eps = 15 and minPts = 8. Play around!

#Data preprocessing
db_data <- cbind(db_data, cluster = db2$cluster) 
db_data<-db_data[!(db_data$cluster==0),]

#Plot cluster
ggplot(db_data, aes(db_data$x,db_data$y, color=db_data$cluster))+geom_point()+
  scale_color_gradient(low="green",high="red")

