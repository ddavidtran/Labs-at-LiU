  rm(list=ls())
  library(tree)
  data = read.csv2("creditscoring.csv")
  
  # Exercise 1
  #Divide data into training/validation/test as 50/25/25.
  n=dim(data)[1]
  set.seed(12345)
  id = sample(1:n, floor(n*0.5))
  train = data[id,] # 50% training.
  validation_test = data[-id,] # 50% validation/test.
  
  n1 = dim(validation_test)[1]
  new_id = sample(1:n1, floor(n1*0.5))
  validation = validation_test[new_id,] # 25% validation.
  test = validation_test[-new_id,] # 25% test
  
  
  # Exercise 2
  tree_model = tree(good_bad~., data = train)
  fit_dev <- tree(tree_model, split = c("deviance"))
  fit_gini <- tree(tree_model, split = c("gini"))
  
  #Confusion matrix and missclassification rate for Deviance impurity.
  Y_dev_train = predict(fit_dev, newdata = train, type = "class")
  conf_mat1 = table(train$good_bad, Y_dev_train) 
  missclassification_rate1 = (conf_mat1[2,1] + conf_mat1[1,2])/sum(conf_mat1)
  
  Y_dev_test = predict(fit_dev, newdata = test, type = "class")
  conf_mat2 = table(test$good_bad, Y_dev_test) 
  missclassification_rate2 = (conf_mat2[2,1] + conf_mat2[1,2])/sum(conf_mat2)
  
  #Confusion matrix and missclassification rate for Gini index impurity.
  Y_gini_train = predict(fit_gini, newdata = train, type = "class")
  conf_mat3 = table(train$good_bad, Y_gini_train) 
  missclassification_rate3 = (conf_mat3[2,1] + conf_mat3[1,2])/sum(conf_mat3)
  
  Y_gini_test = predict(fit_gini, newdata = test, type = "class")
  conf_mat4 = table(test$good_bad, Y_gini_test) 
  missclassification_rate4 = (conf_mat4[2,1] + conf_mat4[1,2])/sum(conf_mat4)
  
  #Conclusion, Deviance is better because the missclassification rate is much lower for deviance compared to gini index..
  
  
  #Exercise 3
  #Function to select optimal tree by train/validation.
  getOptimalTree = function(fitted_model){
    nr_leafs <- sum(fitted_model$frame$var=="<leaf>") #Get number of leafs in the tree.
    trainScore <- numeric(nr_leafs)
    validationScore <- numeric(nr_leafs)
    
    for(i in 2:nr_leafs){
      pruned_tree = prune.tree(fitted_model, best = i) #Pruned tree.
      pred = predict(pruned_tree, newdata = validation, type = "tree") #Prediction to test
      trainScore[i] = deviance(pruned_tree)
      validationScore[i] = deviance(pred)
    }
    return(list(NumberOfLeafs=2:nr_leafs, trainScore = trainScore[2:nr_leafs], validationScore=validationScore[2:nr_leafs], depth = depth))
  }
  
  opt_tree_train = getOptimalTree(fit_dev)
  
  
  #Plot dependence of deviance for training data.
  plot(opt_tree_train$NumberOfLeafs, opt_tree_train$trainScore, type = "b", col = "red", ylim = c(0,600))
  points(opt_tree_train$NumberOfLeafs, opt_tree_train$validationScore, type = "b", col = "blue")
  legend("bottom", lty = c(1,1), col = c("Red","Blue"), legend = c("TrainScore", "ValidationScore"))
  
  
  #Estimate missclassification rate for the test data.
  finalTree = prune.tree(fit_dev, best = 4)
  Yfit = predict(finalTree, newdata = test, type = "class")
  conf_finalTree <- table(test$good_bad, Yfit)
  miss_rate = (conf_finalTree[2,1] + conf_finalTree[1,2])/sum(conf_finalTree) #0.284
  plot(finalTree)
  text(finalTree,cex = .6)
  
  #Exercise 4
  #Naive bayes for training data
  fit_bayes = naiveBayes(good_bad~., data = train)
  Yfit = predict(fit_bayes, newdata = train)
  conf_bayes_train = table(train$good_bad, Yfit)
  miss_rate_bayes = (conf_bayes_train[2,1] + conf_bayes_train[1,2])/sum(conf_bayes_train) #0.3
  print(miss_rate_bayes)
  
  #Naive bayes for test data
  Yfit = predict(fit_bayes, newdata = test)
  conf_bayes_test = table(test$good_bad, Yfit)
  miss_rate_bayes_test = (conf_bayes_test[2,1] + conf_bayes_test[1,2])/sum(conf_bayes_test) #0.32
  print(miss_rate_bayes_test)
  
  #Very similar results compared  
  
  #Exercise 5
  #Penalty of factor 10 for false negatives
  classify = function(data){
    newdata = replace(data, data[,1]*10 > data[,2], 'bad')
    newdata = replace(newdata, data[,1]*10 <= data[,2], 'good')
    return(newdata[,1])
  }
  predicted_model = predict(fit_bayes, train, type = "raw")
  naive_bayes_model_train = classify(predicted_model)

  conf_bayes_train = table(train$good_bad,naive_bayes_model_train)
  print(conf_bayes_train)
  miss_rate_bayes = (conf_bayes_train[2,1] + conf_bayes_train[1,2])/sum(conf_bayes_train) 
  print(miss_rate_bayes)
  
  predicted_model_test = predict(fit_bayes, newdata = test, type = "raw")
  naive_bayes_model_test = classify(predicted_model_test)
  
  conf_bayes_test = table(test$good_bad,naive_bayes_model_test)
  print(conf_bayes_test)
  miss_rate_bayes = (conf_bayes_test[2,1] + conf_bayes_test[1,2])/sum(conf_bayes_test) 
  print(miss_rate_bayes)