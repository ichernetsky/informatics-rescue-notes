iris <- read.csv(file = "iris-flower-dataset.csv", head = TRUE, sep = ",")

postscript(file = "../../images/ml/iris-flower-dataset.ps", height = 6, width = 6,
           paper = "special", horizontal = FALSE)
pairs(iris[1:4], pch = 21, bg = c("red", "green3", "blue")[unclass(iris$species)])
dev.off()
