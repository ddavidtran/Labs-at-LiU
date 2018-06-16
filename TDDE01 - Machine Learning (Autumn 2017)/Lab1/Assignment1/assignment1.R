#Help. Reset variables with this command: rm(list=ls())
data = read.csv2("spambase.csv")

#Divide the data into two set, training set and test set.
n=dim(data)[1]
set.seed(12345)
id = sample(1:n, floor(n*0.5))
train = data[id,]
test = data[-id,]


knearest=function(data,k,newdata) {
  n1=dim(data)[1] #Dimension of data in row.
  n2=dim(newdata)[1] #Dimension newdata in row.
  p1=dim(data)[2] #Dimension of data in column.
  p2=dim(newdata)[2] #Dimension of newdata in column.
  Prob=numeric(n2) #Probability vector of size n2.
  X=as.matrix(data[,-p1]) #Removes last column which is the solution. (Spam) 
  Xn=as.matrix(newdata[,-p1]) #Removes last column which is the solution. (Spam) 
  X=X/matrix(sqrt(rowSums(X^2)), nrow=n1, ncol=p1-1)
  
#MISSING: implement steps ii)-iv)
  Y=as.matrix(newdata[,-p2])
  Y=Y/matrix(sqrt(rowSums(Y^2)), nrow=n2, ncol=p2-1)
  
  C = X %*% t(Y)
  D = 1 - C 
  Dsort = apply(D,2,sort)
  
  #MISSING: derive probability value 'Prob[i]' by using the
  #target values of the nearest neighbors
  #MISSING: use the computed distance matrix to find 
  #which observations are the nearest neighbors to case #i
  for (i in 1:n2 ){
    probval = 0
    for(n in 1:k){
      pos = match(Dsort[n,i], D[,i])
      probval = probval + data[pos,p1]
    }
    print(probval/k)
    Prob[i]=probval/k
  }
  return(Prob)
}


ROC=function(p, input){
  m=length(p)
  TPR=numeric(m)
  FPR=numeric(m)
  for(i in 1:m){
    t = table(test[,ncol],as.numeric(input[i,])) 
    TP = t[2,2]
    FP = t[1,2]
    sum_TP_FN = TP + t[2,1] #TP + FN
    sum_FP_TN = FP + t[1,1] #FP + TN
    TPR[i]= TP/sum_TP_FN
    FPR[i]= FP/sum_FP_TN
  }
  return (list(TPR=TPR,FPR=FPR))
}

#Exercise 3
knn5 = knearest(train,5,test)
knn5 = round(knn5)
ncol = dim(test)[2]
confMat5 = table(test[,ncol],knn5,dnn = list("Truth","Prediction")) #Confusion matrix
miss_class_rate = (confMat5[1,2] + confMat5[2,1])/1370

#3b. Train mot train
knn5b = knearest(train,5,train)
knn5b = round(knn5b)
ncol = dim(test)[2]
confMat5b = table(train[,ncol],knn5b,dnn = list("Truth","Prediction")) #Confusion matrix
miss_class_rate1 = (confMat5b[1,2] + confMat5b[2,1])/1370

#Exercise 4
knn1 = knearest(train,1,test)
knn1 = round(knn1)
ncol = dim(test)[2]
confMat1 = table(test[,ncol],knn1, dnn = list("Truth","Prediction")) #Confusion matrix
miss_class_rate2 = (confMat1[1,2] + confMat1[2,1])/1370

#4b Test mot test
knn1b = knearest(train,1,train)
knn1b = round(knn1b)
ncol = dim(test)[2]
confMat1b = table(train[,ncol],knn1b, dnn = list("Truth","Prediction")) #Confusion matrix
miss_class_rate3 = (confMat1b[1,2] + confMat1b[2,1])/1370

#Exercise 5
kknn5 = kknn(formula = formula(train), train, test, k = 5)
kknn5 = kknn5[["fitted.values"]] #Convert to vector.
kknn5_ex5 = round(kknn5)
confMatKKNN = table(test[,ncol],kknn5_ex5,dnn = list("Truth","Prediction")) #Confusion matrix
miss_class_rate4 = (confMatKKNN[1,2] + confMatKKNN[2,1])/1370


#Matrix where every row is probability vector of pi[i].
#Function to convert vector to matrix.
vecToMat = function(vec){
  pi = seq(from = 0.05, to = 0.95, by = 0.05)
  mat = matrix(nrow = length(pi), ncol = length(kknn5), byrow = TRUE)
  for(i in 1:length(pi)){
    prob = numeric(length(vec))
    for(j in 1:length(prob)){
      if(vec[j] > pi[i])
        prob[j] = 1
      else
        prob[j] = 0
    }
    mat[i,] = prob;
  }
  return(mat)
}

mat_kknn5 = vecToMat(kknn5)
knn5 = knearest(train,5,test)
mat_knn5 = vecToMat(knn5)

pi = seq(from = 0.05, to = 0.95, by = 0.05)
ROCkknn <- ROC(pi,mat_kknn5)
ROCknn5 <- ROC(pi, mat_knn5)
plot(ROCkknn$FPR, ROCkknn$TPR,type="l",col="red", xlab = "FPR", ylab = "TPR", xlim = c(0,0.4), ylim = c(0,0.8))
par(new = TRUE)
plot(ROCknn5$FPR, ROCknn5$TPR,type="l",col="green", xlab = "FPR", ylab = "TPR", xlim = c(0,0.4), ylim = c(0,0.8))

