rm(list=ls())
library(tree)
library(boot)
set.seed(12345)
state_data = read.csv2("State.csv")

#Exercise 1.
state_data <- state_data[order(state_data$MET),] #Reorder data with respect to increase of "MET"
plot(state_data$MET, state_data$EX) #Plot EX versus MET

#Exercise 2.
#Appropriate model: Linear Regression Curve.
nr_obs = dim(state_data)[1]
tree_model <- tree(EX~MET, data = state_data, control = tree.control(nr_obs, minsize = 8)) #Fit model.
cv_model = cv.tree(tree_model) #Perform cross-validation.
plot(cv_model, type = "b")

#Selected tree from cross-validation.
bestNrLeafs = cv_model$size[cv_model$dev == min(cv_model$dev)]
selected_tree = prune.tree(tree_model, best = bestNrLeafs)
summary(selected_tree)
plot(selected_tree)
text(selected_tree, cex=.75)


#Plot the orignal data and the fitted data.
predicted_data = predict(selected_tree, state_data)
plot(state_data$MET, predicted_data, type = "b")

#Plot the histogram residuals.
resid_tree = resid(selected_tree) #state_data$EX - predicted_data
hist(resid_tree)
#Residual: Difference between data and fitted data.
#Comment: Would have been better if histogram resemble a normal distribution which would have been good. Our histogram
#is not good.

#Exercise 3.
library(boot)
#Get bootstrap samples.
f = function(state_data, index){
  data1 = state_data[index,] #Extract bootstrap samples.
  res_tree = tree(EX~MET, data = data1, control = tree.control(nr_obs, minsize = 8))
  pruned_tree = prune.tree(res_tree, bestNrLeafs)
  pred = predict(pruned_tree,newdata = state_data)
  return(pred)
}
res = boot(state_data, f, R = 1000)
confidence_band = envelope(res, level = 0.95)
plot(state_data$MET, state_data$EX, main = "Bootstrap confidence band", xlab = "MET", ylab = "EX", ylim = range(150,500))
points(state_data$MET, predicted_data, type = "l")
points(state_data$MET, confidence_band$point[2,], type = "l", col = "blue")
points(state_data$MET, confidence_band$point[1,], type = "l", col = "blue")
#The width of the confidence band increases at around MET ~ 20-30. This means our level of uncertainty increases
#when MET = 20-30. 

#Exercise 4
mle = selected_tree

rng = function(state_data, mle){
  data1 = data.frame(EX = state_data$EX, MET = state_data$MET)
  n = length(state_data$EX)
  data1$EX = rnorm(n, predict(mle, newdata = data1), sd(resid(mle)))
  return(data1)
}

f2 = function(data1){
  res_tree = tree(EX~MET, data = data1, control = tree.control(nr_obs, minsize = 8))
  pruned_tree = prune.tree(res_tree, best = bestNrLeafs)
  pred = predict(res_tree,newdata = data1)
  return(pred)
}

res = boot(state_data, statistic = f2, R= 1000, mle = mle, ran.gen = rng, sim = "parametric")
confidence_band_par = envelope(res, level = 0.95)
points(state_data$MET, confidence_band_par$point[2,], type = "l", col = "red")
points(state_data$MET, confidence_band_par$point[1,], type = "l", col = "red")
points(state_data$MET, confidence_band_par$overall[2,], type = "l", col = "green") #Pred.band
points(state_data$MET, confidence_band_par$overall[1,], type = "l", col = "green") #Pred.band
legend("topright", lty = c(1,1), col = c("Blue", "Black","Red", "Green"), legend = c("Non-parametric", "Fitted Line", "Parametric", "Prediction-band"), cex = 0.50)
#Comment: The confidence band for the parametric bootstrap is less bumpy compared to the non-parametric bootstrap. 
#The confidence bands are depended on the fitted model and the standard deviation and not the data values. 


#Exercise 5
#Non-parametric bootstrap would be more appropriate in this case since parametric bootstrap assumes that we know the
#normal distribution is correct, which again, is not correct.. 