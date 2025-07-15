# force-directed graph

# 载入软件包
library(networkD3)

# 创建数据
src    <- c("A", "A", "A", "A", "B", "B", "C", "C", "D")
target <- c("B", "C", "D", "J", "E", "F", "G", "H", "I")
networkData <- data.frame(src, target, zoom = TRUE)

# 画图
simpleNetwork(networkData, nodeColour="#FF69B4", fontSize=12)
