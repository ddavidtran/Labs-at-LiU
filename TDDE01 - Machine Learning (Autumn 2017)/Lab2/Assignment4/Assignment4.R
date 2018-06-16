rm(list=ls())
library(pls)
set.seed(12345)
spectra_data = read.csv2("NIRSpectra.csv")
data1 = spectra_data
data1$Viscosity = c()
pca_data = prcomp(data1)
lambda = pca_data$sdev^2
#Eigenvalues
print(lambda)
#Proportion of variation
sprintf("%2.3f", lambda/sum(lambda)*100)
screeplot(pca_data) 
plot(pca_data$x[,1], pca_data$x[,2])
#There are unusual diesel fuels according to the plot that deviates from the PC axis with similar viscocity.

#Exercise 2
U = pca_data$rotation
plot(U[,1], main = "Traceplot, PC1") 
plot(U[,2], main = "Traceplot, PC2", ylim = c(-0.1,0.5))
# PC2 is explained by mainly a few original features.

#Exercise 3
library(fastICA)
ica = fastICA(data1,2,alg.typ = "parallel", fun = "logcosh", alpha = 1, method = "R", row.norm = FALSE, maxit = 200,tol = 0.0001, verbose = TRUE)
Wp = ica$K %*% ica$W #K is prewhitening matrix, W is estimated matrix.
plot(Wp[,1], main = "Traceplot 1")
plot(Wp[,2], main = "Traceplot 2")
#Wp measures 

plot(ica$S[,1], ica$S[,2], main = "Score plot ICA")


#Exercise 4
visc_pcr <- pcr(Viscosity~., data = spectra_data, validation = "CV")
validationplot(visc_pcr, val.type = "MSEP") 



