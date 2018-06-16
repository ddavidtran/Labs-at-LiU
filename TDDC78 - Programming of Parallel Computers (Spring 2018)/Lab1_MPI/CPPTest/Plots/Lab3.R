library(ggplot2)
x = c(1,2,4,8,16)
y = c(1.39, 0.70, 0.38, 0.21, 0.12)
x_name <- "Cores"
y_name <- "Elapsed_Time"

df <- data.frame(x,y)
names(df) <- c(x_name,y_name)
print(df)
ggplot(data = df, aes(x = Cores, y = Elapsed_Time)) + 
  geom_line() +
  scale_x_continuous(labels = as.character(x), breaks = x) +
  ggtitle("OpenMP Lab3 Heat Equation")

