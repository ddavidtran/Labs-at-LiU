teactor_data = read.csv2("tecator.csv")
plot(teactor_data$Moisture, teactor_data$Protein, xlab = "Moisture", ylab = "Protein")

#Exercise 2

#Exercise 3
#Divide the data into training and validation set(50%/50%).
n=dim(teactor_data)[1]
id=sample(1:n, floor(n*0.5))
train=teactor_data[id,]
test=teactor_data[-id,]

#Fit model Mi, i = 1,...,6.
MSE = function(predict, obs){ #Prediction vector, Observation vector.
  return(mean((predict - obs)^2))
}

MSE_training = numeric(6)
MSE_test = numeric(6)
Moisture = teactor_data$Moisture
Protein = teactor_data$Protein
for(i in 1:6){
  model = lm(Moisture~poly(Protein,i), data = train)
  model_predict_train = predict.lm(model)
  model_predict_test = predict.lm(model, data = test)
  MSE_training[i] = MSE(unname(model_predict_train), train$Moisture)
  MSE_test[i] = MSE(unname(model_predict_test), test$Moisture)
}
x = seq(from = 1, to = 6)
plot(x, MSE_training, col = "red", type = "l")
par(new = TRUE)
plot(x, MSE_test, col = "blue", type = "l")

#Ex4
library(MASS)
lm_fat = lm(Fat~., data=teactor_data[, 2:102]) #First column and last columns are not features.
step = stepAIC(lm_fat)

#Ex5
library(glmnet)
covariates=scale(teactor_data[,2:101]) #Features
response=scale(teactor_data[, 102]) #Fat
model0=glmnet(as.matrix(covariates), response, alpha=0,family="gaussian")
plot(model0, xvar="lambda", label=TRUE)

#Ex6
lasso = glmnet(as.matrix(covariates), response, alpha=1,family="gaussian") #alpha = 1, gaussian => LASSO
plot(lasso, xvar="lambda", label=TRUE)

#Ex7
cv_lasso = cv.glmnet(as.matrix(covariates), response, alpha=1,family="gaussian")
cv_lasso$lambda.min #Get minimum lambda
plot(cv_lasso)
coef(cv_lasso, s="lambda.min")
