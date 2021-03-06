iris <- read.csv(file = "iris-flower-dataset.csv", head = TRUE, sep = ",")

postscript(file = "../../images/ml/iris-flower-dataset.ps", height = 6, width = 6,
           paper = "special", horizontal = FALSE)
pairs(iris[1:4], pch = 21, bg = c("red", "green3", "blue")[unclass(iris$species)])
dev.off()

iris <- read.csv(file = "iris-flower-kmeans.csv", head = TRUE, sep = ",")
postscript(file = "../../images/ml/iris-flower-kmeans.ps", height = 6, width = 6,
           paper = "special", horizontal = FALSE)
pairs(iris[1:4], pch = 21, bg = c("green3", "red", "blue")[unclass(iris$species)])
dev.off()
