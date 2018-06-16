machine_data = read.csv2("machines.csv")

#Exercise 2.
loglkh = function(theta, nr_obs){
  if(nr_obs == dim(machine_data)[1]){
    len_machine = dim(machine_data)[1]
  }
  else
    len_machine = nr_obs
  
  machine_vec = as.numeric(unlist(machine_data)) #Convert machine_data to vector.
  samples = 10
  len = length(theta)
  prob = rep(1,len) #Create probability vector of 1's.
  for(i in theta){
    for(j in 1:len_machine){
      index = i / (1/samples)+1 #Convert to real indices.
      prob[index] = prob[index] + log(i*exp(-i*machine_vec[j])) #Sum of loglikelihood formula
    }
  }
  return (prob)
}

#Exercise 3
theta = seq(from = 0, to = 3, by = 0.1)
nr_obs = dim(machine_data)[1]
loglkh2 = loglkh(theta,nr_obs)
thetamax = theta[which(loglkh2==max(loglkh2))]

#Plot
theta<-theta
plot(theta,loglkh2,type="l",col="green")
lines(theta,loglkh2,col="green", xlab = theta)

par(new=TRUE) #Hold on in MATLAB.

nr_obs6 = 6
loglkh_6 = loglkh(theta, nr_obs6)
thetamax_6 = theta[which(loglkh_6==max(loglkh_6))]

#Plot
plot(x,loglkh_6,type="l",col="red")
lines(x,loglkh_6,col="red", xlab = theta)

#Exercise 4
loglkh_bayesian = function(theta, nr_obs){
  p = loglkh(theta,nr_obs)
  len = length(theta)
  prob_bay = rep(0,len) #Create probability vector of 0's.
  j = 1;
  for(i in theta){
    prior = 10*exp(-10*i)
    prob_bay[j] = prob_bay[j] + p[j] + log(prior)
    j = j+1
  }
  return (prob_bay)
}

par(new = TRUE)
loglkh_bay <- loglkh_bayesian(theta,nr_obs)
plot(x,loglkh_bay,type="l",col="black")
lines(x,loglkh_bay,col="black")
thetamax_bay = theta[which(loglkh_bay==max(loglkh_bay))]

#Exercise 5
observation_exp = rexp(50, thetamax)
hist(observation_exp,main = "Histogram of Exponential Distribution")
hist(machine_data$Length, main = "Histogram of original data")

