rm(list=ls())
library(neuralnet)
set.seed(1234567890)
Var <- runif(50, 0, 10)
trva <- data.frame(Var, Sin=sin(Var))
training_data <- trva[1:25,] # Training
validation_data <- trva[26:50,] # Validation

winit <- runif(31, -1,1) # Random initialization of the weights in the interval [-1, 1]
minMSE <- Inf
threshold <- 0
MSE_val <- numeric(10) #MSE for validation
MSE_train <- numeric(10) #MSE for train
  for(i in 1:10) {
    nn <- neuralnet(formula = Sin~Var, data = training_data, hidden = 10, threshold = (i/1000), startweights = winit)
    predict_val <- compute(nn, validation_data$Var) #Compute function with test data to see how we performed.
    predict_train <- compute(nn, training_data$Var)
    MSE_val[i] = (1/2) * sum((predict_val$net.result - validation_data$Sin)^2)
    MSE_train[i] = (1/2) * sum((predict_train$net.result - training_data$Sin)^2)
    if(MSE_val[i] < minMSE){
      minMSE = MSE_val[i]
      threshold = i/1000
    }
}
plot(seq(0.001, 0.01, by = 0.001), MSE_val, type = "b", ylim = c(0,0.014), ylab = "MSE", xlab = "Threshold", col = "red", main = "MSE Plot by Threshold")
points(seq(0.001, 0.01, by = 0.001), MSE_train, type = "b", col = "blue")
legend("top", lty = c(1,1), col = c("Red", "Blue"), legend = c("MSE Validation", "MSE Training"), cex = 0.75)

#Appropriate value for threshold according to the plot is 0.004.
plot(nn <- neuralnet(formula = Sin~Var, data = trva, hidden = 10, threshold = threshold, startweights = winit))

# Plot of the predictions (black dots) and the data (red dots)
plot(prediction(nn)$rep1, col = "blue")
points(trva, col = "red")
legend("top", pch = c(1,1), col = c("Blue", "Red"), legend = c("NN prediction", "Sin(x)"), cex = 0.75)

