setwd('C:\\Users\\Administrator\\Desktop\\R语言培训')
library(dplyr)##filter, select
library(psych)##describle
library(DescTools)##PostHocTest
library(readxl)##read excel
library(car)##leveneTest()
library(ROCR)##roc 
library(pROC)##roc 
library(survival)##cox
library(survminer)##
library(VGAM)
library(nnet)
library(caret)##confusionmatrix 
library(ggplot2)
library(corrplot)
library(pheatmap)
library(circlize)##circos 
library(rgdal)
library(glmnet)
library(mgcv)
library(RColorBrewer)
library(sqldf)

a <- require(car)
a <- library(car)
##代码提示
# install.packages(c('dplyr', 'psych', 'readxl', 'pROC', 'pheatmap'),
#                  repos = 'https://mirrors.tuna.tsinghua.edu.cn/CRAN/')
# library(GGally)
# library(survminer)##cox 
# install.packages('survival')
# install.packages('survminer')
a <- c(11, 23, 24, 34, 23, 12, 45)
mean(a)
sd(a)
plot(a)

##安装R包rlist
install.packages(c('rlist'))
##加载R包
library(rlist)
??rlist::list.rbind
##workspace 
ls()
# save.image('C:/Users/Administrator/Desktop/R语言培训/test1.RData')
# save.image('C:\\Users\\Administrator\\Desktop\\R语言培训\\test1.RData')

rm(list = ls())
load('test1.RData')
##?mean ??mean
##??

x <- lapply(1:3,function(i) {c(a=i,b=i^2)})
df <- lapply(1:3,function(i) {data.frame(a=i,b=i^2,c=letters[i])})
list.rbind(x)
list.rbind(df)

##setwd('')
##csv文件读取
##第一行是标题
df1 <- read.csv('iris_csv.csv', header = T)
head(df1)
df1
##df1
##colClasses, character, numeric
##skip
head(df1)
##excel, xlsx文件读取
library(readxl)
##col_types---read.csv, colClasses 
df2 <- read_excel('iris_excel.xlsx')
df2 <- as.data.frame(df2)
head(df2)
tmp <- read_excel('iris_excel.xlsx', sheet = 2)
write.csv(tmp, 'tmp.csv', row.names = F)
openxlsx::write.xlsx(tmp, 'tmp.xlsx', row.names = F)
##赋值
a <- 1:3
a
b = c(1, 4, 5)
b
##4这种常见的原子向量, 
int_vec <- c(2L, 33L, 78L)##long 长整型
int_vec
log_vec <- c(TRUE, FALSE, T, F)##T, F
log_vec
num_vec <- c(1, 2.3, 4.45, 6.89)
num_vec
char_vec <- c('test', 'R language', 'learning')
char_vec
##查看向量的类型
int_vec <- c(33L, 34L, 12L)
typeof(int_vec)
class(int_vec)
char_vec <- c('aa', 'bbb', 'cd')
typeof(char_vec)
##强制类型转换---逻辑型， 布尔型
v1 <- c(T, F, 23, 34)
v1
v2 <- c(12.4, 34L, 45)
v2
typeof(v2)
v3 <- c(12.55, 'test', 444)
v3
typeof(v3)
##缺失值NA， not available
vec1 <- c(NA, NA, NA)##
class(vec1)
vec2 <- c(12L, 23L, NA, 33L)
class(vec2)
vec3 <- c(23.4, 12, NA, 44)
class(vec3)
##
vec4 <- c(NA_real_, NA_real_)
typeof(vec4)
class(vec4)
vec5 <- c(NA_character_, NA_character_)
typeof(vec5)
##向量的访问
x <- c(1, 33, 4, 5, 6, 8)
x[1:3]
x[c(2, 4, 5)]
x[x > 5]
x[x %% 2 == 0]
##数组
a <- array(1:24, dim = c(2, 3, 4), 
           dimnames = list(c('a1', 'b1'), 
                           c('a2', 'b2', 'c2'), 
                           c('a3', 'b3', 'c3', 'd4')))
a 
##矩阵,数组的一种，2维
b <- matrix(1:12, nrow = 3, byrow = F)
b
##数据框， data.frame
v1 <- 1:3
v2 <- c('a', 'b', 'c')
v3 <- c('good', 'improved', 'poor')
df <- data.frame(v1, v2, v3)
df
dim(df)##dimension
nrow(df)
ncol(df)
str(df)##structure##查看数据结构
##列表, list
x <- list(1:4, 'dd', c(T, F), c(2.55, 4))
x
##因子-----------, 分类变量
##无序因子
a <- c('a', 'b', 'c', 'c', 'b')
fac <- factor(a)
fac
##有序因子
fac2 <- factor(a, levels = c('a', 'b', 'c'), 
               ordered = T)
fac2
##类型判断和类型转换-----------------
v1 <- c(1L, 2L, 5L)
v2 <- c(2.3, 2, 5.67)
is.numeric(v1)
is.numeric(v2)
v3 <- c(1, 2, 5)
v4 <- list(3:5, 'dd', c(2, 4.4, 9.88))
is.vector(v3)
is.vector(v4)
is.atomic(v3)
##判断一个数据是不是列表
!is.atomic(v4) & is.list(v4)
v <- factor(c(2, 3, 4))
as.numeric(v)
##因子类型转为数值类型
as.numeric(as.character(v))
##子集选取================
##向量子集选取
x <- c(1, 2, 4, 55, 6.8)
x[c(3, 1)]
x <- c(33, 12, 53, 4.01)
x[-c(1, 2)]
y <- c(1, 2, 33, 44, 55)
##取子集，用逻辑值
y[c(T, F, T, F, F)]
y[y > 33]
y[y %% 2 == 0]
##矩阵数组、向量选取
a <- matrix(1:9, ncol = 3)
colnames(a) <- c('aa', 'bb', 'cc')
a
a[1:2, 1:2]
a[c(1, 3), ]##取所有列
a[1:2, c('aa', 'cc')]
##数据框子集选取
df <- data.frame(age = c(23, 34, 31), 
                 weight = c(67, 77, 72), 
                 height = c(170, 175, 168))
##$
df$age
df[, c('age')]##
df[, 'age']
class(df[, 'age'])
df[, 'age', drop = F]
class(df[, 'age', drop = F])
df['age']
class(df['age'])
##
df[['age']]
class(df[['age']])
df[df$age > 30, ]
df[, c('weight', 'height')]

df <- data.frame(age = abs(rnorm(100, 30, 2)), 
                 gender = sample(c(1, 2), 100, 
                          prob = c(0.6, 0.4), replace = T),
                 weight = runif(100, 50, 100), 
                 glu = abs(rnorm(100, 5.6, 2)), 
                 height = runif(100, 150, 180),
                 hpt = sample(c(0, 1), 100, 
                              prob = c(0.8, 0.2), replace = T))
df$gender <- factor(df$gender)
df$hpt <- factor(df$hpt)
df$bmi <- df$weight / (df$height / 100) ^ 2
df$bmi_level <- cut(df$bmi, breaks = c(min(df$bmi), 18, 24, 
                                             max(df$bmi)+ 0.01), 
                    include.lowest = T, right = F)
##删除一个变量
df$bmi <- NULL
# df[, c('age', 'bmi', 'gender')] <- NULL
fit <- glm(hpt ~ ., data = df, 
           family = binomial(link = 'logit'))
tmp <- summary(fit)
ors <- tmp$coefficients[, c(1, 4)]
ors <- data.frame(ors)
ors$vname <- row.names(ors)
rownames(ors) <- NULL
colnames(ors) <- c('beta', 'p', 'vname')
ors <- select(ors, vname, beta, p)
ors$or <- exp(ors$beta)
ors$beta <- NULL
or_ci <- confint(fit)
or_ci <- as.data.frame(or_ci)
or_ci$vname <- row.names(or_ci)
row.names(or_ci) <- NULL
names(or_ci) <- c('lower', 'upper','vname')
or_ci$lower <- exp(or_ci$lower)
or_ci$upper <- exp(or_ci$upper)
rst <- merge(ors, or_ci, by = 'vname')
for (v in names(rst)[-1]){
  rst[, v] <- round(rst[, v], 2)
}
openxlsx::write.xlsx(rst, 'logis_rst.xlsx', row.names = F)
##列表子集选取
a <- list(1:5, 'test', c(12, 44, 67))
names(a) <- c('v1', 'v2', 'v3')
a[[1]]
a[1]
a$v1
a[['v1']]
a['v1']
# a <- 1:3  <-, =
age <- 1:30
bmi <- 1:40

##英文， 数字, _
##名字, .
# family_name 
# familyName
# a = 1:3
##条件判断======================
a <- 12
# ==
if (a %% 2 == 0){
  print('111')
}else{
  print('222')
}
##ifelse， 向量操作
gender <- c('男', '女', '男', '女')
gender <- ifelse(gender == '男', 1, 2)
gender
##while-------------
vec <- c(1, 2, 3, 5, 55, 88)
i <- 1
while(i < 7){
  vec[i] <- vec[i] * 2 + 1
  i <- i + 1
}
vec 
##循环
s <- 0
for (i in 1:100){
  s <- s + i
}
s
##break, next
s <- 0
for (i in c(2, 3, 4, 66)){
  s <- s + i
  if (i == 5){
    next
  }
  print(i)
}
##============
v <- c(1, 44, 56, 89)
for (i in seq_along(v)){
  v[i] <- v[i] * 3
}
v
v <- numeric(0)
length(v)
##length
a <- 1:10 
length(a)
##v 
v <- numeric(0)
##不知道长度，推荐用seq_along
for(i in seq_along(v)){
  v[i] <- v[i] * 3
}
v
v2 <- numeric(0)
for (i in 1:length(v2)){
  v2[i] <- v2[i] * 3
}
v2
##函数=========================
#函数示例
mean(c(1, 2, 3, 5))
##自定义函数
f <- function(x, y){
  x <- 2 * x
  # return(x + 2)
  x + y * 2
}
f(x = 1, y = 4)
##1:10
##rlist::list.rbind
f2 <- function(a = 2, b){
  return (a + b ^ 2)
}
f2(b = 3)## <===> f2(a = 2, b = 3)

##运算符=========================
v <- c(1, 2, 4, 6)
v[2]
df <- data.frame(v1 = 1:4, v2 = 2:5)
df$v1
3 ^ 3
4 / 12
##
v >= 2
!(v >= 2)
##运算符的特别说明
2.1 - 0.2 == 1.9
near(2.1 - 0.2, 1.9)
##缺失值的特别说明, not available
NA > 34
11 == NA
NA + 33
NA * 2
NA / 3
NA == NA
NA ^ 0
a <- c(1, 2, NA, 34)
is.na(a)
##常用函数=====================
##常用数学、统计函数---------------
##四舍五入的特别说明
round(2.5, 0)
round(3.5, 0)
round(-3.55, 1)
round(-3.65, 1)
##自定义四舍五入函数
round_f <- function(data, n = 2){
  data_sign <- sign(data)
  data <- abs(data * 10 ^ n)
  data <- trunc(data + 0.5)
  return (data_sign * data / 10 ^ n)
}
round_f(2.5, 0)
round_f(3.5, 0)
round_f(-3.55, 1)
round_f(-3.65, 1)
sqrt(23)
abs(-12)
trunc(12.45)
log(3)
log10(100)
exp(2)
##常用统计函数
x <- c(12, -3, 34, NA, 67, 34, 88)
mean(x)
##na.rm, na remove 
mean(x, na.rm = T)
sd(x, na.rm = T)
quantile(x, 0.25, na.rm = T)
quantile(x, 0.25, na.rm = T)
quantile(1:100, 0.25)
var(x, na.rm = T)
##其他实用函数
x <- 1:10
length(x)
##sequence 
seq(from = 1, to = 20, by = 2)
##rep, repeat
rep(1:3, times = 2)
rep(1:3, each = 2)
df <- data.frame(x = 1:10, y = 3:12)
head(df, 3)
tail(df, 4)
dim(df)##dimension
str(df)##structure 
summary(df)
##paste-----------
a <- 1:3
b <- 2:4
paste(a, b, sep = '&')
paste(a, collapse = '&')
##rbind, cbind--------------------
##rbind---row bind 
##cbind---column bind
df1 <- data.frame(age = c(22, 44, 45), 
                  bmi = c(23, 24.6, 27.9))
df2 <- data.frame(age = c(55, 67, 45), 
                  bmi = c(26.3, 25.8, 21.6))
df3 <- data.frame(weight = c(55, 66, 77), 
                  height = c(170, 173, 180))
df_1 <- rbind(df1, df2)
df_1
df_2 <- cbind(df2, df3)
df_2
##merge---------------------
authors <- read.csv('authors.csv')
books <- read.csv('books.csv')
head(authors)
head(books)
tmp <- merge(authors, books, by.x = 'surname', by.y = 'name')
tmp
x <- data.frame(k1 = c(NA, NA, 3, 4, 5), 
                k2 = c(1, NA, NA, 4, 5), 
                data = 1:5)
y <- data.frame(k1 = c(NA, 2, NA, 4, 5), 
                k2 = c(NA, NA, 3, 4, 5), 
                data = 1:5)
merge(x, y, by = c("k1", "k2")) ##NA也匹配了
merge(x, y, by = "k1") #
merge(x, y, by = "k2", incomparables = NA, 
      suffixes = c('_x', '_y')) # NA不进行匹配

tmp1 <- data.frame(x = c(1, 2, 3), y = 2:4)
tmp2 <- data.frame(x = 2:6, y = 3:7)
merge(tmp1, tmp2, by = 'x', all.x = T)##左关联 left join
merge(tmp1, tmp2, by = 'x', all.y = T)##右关联 right join
merge(tmp1, tmp2, by = 'x', all = T)##全关联 full join 
##dplyr::filter()
flg <- read_excel('flights.xlsx')
filter(flg, month == 1 & day <= 1)##且& ， 或|
select(flg, year, month, day, arr_time)
##apply函数-------------------
x <- matrix(1:12, nrow = 3)
x
apply(x, 1, mean)
x
apply(x, 2, max)
##具名函数
f1 <- function(x){
  sum(x)
}
##匿名函数, function(i) sum(i)
apply(x, 1, function(i) sum(i))
##lapply()---------------------
x <- as.list(2:6)
##这里使用匿名函数
a <- lapply(x, function(i)  i ^ 2)
##也可以不使用匿名函数
f1 <- function(i){
  i ^ 2
}
lapply(x, f1)
##cut函数-----------------
##连续变量转为分类变量或者等级变量。
age <- c(12, 28, 15, 34, 55, 46, 23, 78, 56, 83)
cut(age, breaks = c(min(age), 30, 45, 60, 80, max(age) + 0.001), 
    include.lowest = T, 
    right = F)
##常用统计方法介绍
tmp <- mtcars[, c('mpg', 'hp', 'wt')]
summary(tmp)
library(psych)
describe(tmp)
##相关性分析
cor(mtcars$mpg, mtcars$wt)
rst <- cor.test(mtcars$mpg, mtcars$wt)
rst$statistic
rst$p.value
##卡方检验
M <- as.table(rbind(c(762, 327, 468), 
                    c(484, 239, 477)))
dimnames(M) <- list(gender = c("F", "M"),
                    party = c("Democrat", "Independent", "Republican"))
tmp <- chisq.test(M)
##t,检验
head(iris)
x1 <- filter(iris, Species == 'setosa')$Sepal.Length
x2 <- filter(iris, Species == 'versicolor')$Sepal.Length
tmp <- t.test(x1, x2, paired = F, var.equal = T)
##wilcox.test
tmp <- wilcox.test(x1, x2)
##正态性检验
shapiro.test(x1)
qqnorm(x1)
##方差齐性检验
library(car)##leveneTest()
library(graphics)##InsectSprays
library(gplots)
plot(count ~ spray, data = InsectSprays)
bartlett.test(InsectSprays$count, InsectSprays$spray)
bartlett.test(count ~ spray, data = InsectSprays)
leveneTest(count ~ spray, data = InsectSprays)
##方差分析
plot(Sepal.Length ~ Species, data = iris)
fit <- aov(Sepal.Length ~ Species, data = iris)
plotmeans(Sepal.Length ~ Species, data = iris)
summary(fit)
tmp <- summary(fit)[[1]]
##方差分析结果有统计学意义后两两比较
library(DescTools)
tmp <- PostHocTest(fit, method = 'lsd')
rst <- tmp$Species
write.csv(rst, 'rst1.csv', row.names = T)
##双因素方差分析
##ToothGrowth, 随机分配60只豚鼠, 
##分别采用两种喂食方法（橙汁或维生素C），各喂食方法中
##抗坏血酸含量有三种水平（0.5mg, 1mg, 2mg),
##每种处理方式随机分配10只。牙齿长度为因变量。
df <- ToothGrowth
df$dose <- factor(df$dose)
fit <- aov(len ~ supp * dose, data = df)
plotmeans(len ~ interaction(supp, dose, sep = '&'), 
          col = c('red', 'blue'), data = df, 
          connect = list(c(1, 3, 5), c(2, 4, 6)))
summary(fit)
interaction.plot(df$dose, df$supp, df$len, 
                 type = 'b', 
                 col = c('red', 'blue'), 
                 pch = c(16, 18))
##重复测量资料的方差分析 
# 来自基础数据包中的CO2数据，
# 因变量是CO2吸收量(uptake), 
# 自变量是植物类型Type和7种水平的CO2浓度(conc)。
df <- CO2 
##factor 
df$conc <- factor(df$conc)
df <- filter(df, Treatment == 'chilled')
fit <- aov(uptake ~ conc * Type + Error(Plant / (conc)), 
           data = df)
summary(fit)
interaction.plot(df$conc, df$Type, df$uptake, 
                 type = 'b', 
                 col = c('red', 'blue'), 
                 pch = c(21, 16), 
                 main = 'Interaction plot')
boxplot(uptake ~ Type * conc, data = df, 
        col = c('pink', 'steelblue'))
##线性回归
fit <- lm(weight ~ height, data = women)
summary(fit)
##多项式回归
fit2 <- lm(weight ~ height + I(height ^ 2), data = women)
summary(fit2)
##多元线性回归
fit3 <- lm(mpg ~ hp + wt, data = mtcars)
summary(fit3)
coefficients(fit3)
confint(fit3)
# 带交互项的多元线性回归
fit4 <- lm(mpg ~ hp + wt + hp:wt, data = mtcars)
summary(fit4)
##R基础作图 ===========================
x <- 1:100
set.seed(10)#保证结果可重复
y <- rnorm(100, 10, 2)
y
par(mfcol = c(1, 1))
plot(1:10, 2:11, 
     xlab = 'xlab', 
     ylab = 'ylab', 
     main = 'title')
##pch
par(mfcol = c(3, 3))
for (i in 1:9){
  plot(x, y, pch = 2 * i, 
       main = paste('pch = ', i * 2, sep = ''), 
       cex.main = 2)
}
par(mfcol = c(1, 1))
par(mfrow = c(1, 1))
plot(1:10, 1:10, pch = '*')
##ann
par(mfcol = c(2, 1))
for (ann_i in c(T, F)){
  plot(x, y, ann = ann_i, 
       main = paste('ann = ', ann_i, sep = ''), 
       xlab = 'x', ylab = 'y')
}
##bty , box type, n---none , nothing 
par(mfcol = c(3, 3))
for (bty_i in c('o', 'u', '7', 'l', 'c', 'n', ']')){
  plot(x, y, bty = bty_i, 
       main = paste('bty = ', bty_i, sep = ''), 
       cex.main = 2)
}
##cex
par(mfcol = c(2, 2))
for (cex_i in c(0.75, 1, 1.5, 2)){
  plot(x, y, pch = 16, cex = cex_i, 
       main = paste('cex = ', cex_i, sep = ''), 
       cex.main = 2)
}
##cex.axis
par(mfcol = c(2, 2))
for (i in c(0.8, 1, 1.5, 2)){
  plot(x, y, pch = 21, cex.axis = i, 
       main = paste('cex.axis = ', i, sep = ''))
}
##cex.lab, cex.main, cex.sub
par(mfcol = c(2, 2))
for (i in c(0.8, 1, 1.5, 2)){
  plot(x, y, pch = 21, 
       xlab = 'xlab', ylab = 'ylab', 
       sub = 'sub', main = 'main', 
       cex.lab = i, cex.main = i, 
       cex.sub = i)
}
##col ---color 
##col, col.axis, col.lab, col.main, col.sub
par(mfcol = c(2, 2))
for (i in c('red', 'green', 'blue', 'black')){
  plot(x, y, pch = 16, 
       xlab = 'xlab', ylab = 'ylab', 
       col = i, 
       sub = 'sub', main = 'main', 
       col.lab = i, col.main = i, 
       col.sub = i)
}
##font, font.axis, font.lab, font.main, font.axis
par(mfcol = c(2, 3))
for (i in c(1, 2, 3, 4, 5)){
  plot(x, y, pch = 16, 
       xlab = 'xlab', ylab = 'ylab', 
       font = i, 
       sub = 'sub', main = 'main', 
       font.lab = i, font.main = i, 
       font.sub = i)
}
##las
par(mfcol = c(2, 2))
for (i in c(0, 1, 2, 3)){
  plot(x, y, pch = 16, 
       xlab = 'xlab', ylab = 'ylab', 
       las = i, 
       main = paste('las = ', i, sep = ''), 
       cex.main = 2)
}
##lty, line type 
par(mfcol = c(3, 2))
for(i in 1:6){
  plot(x, y, type = 'l', 
       lty = i, 
       main = paste('lty = ', i, sep = ''), 
       cex.main = 2)
}
##lwd, line width
par(mfcol = c(2, 2))
for(i in c(0.5, 1, 2, 3)){
  plot(x, y, type = 'l', 
       lwd = i, 
       main = paste('lwd = ', i, sep = ''), 
       cex.main = 2)
}
opar <- par(no.readonly = T)
##tck, tcl 
plot(x, y, tck = 0.5)
par(mfrow = c(2, 3), oma = c(0, 0, 0, 0), mgp = c(2, 1, 0))
for (i in c(-0.5, 0.5, 1.5)){
  plot(x, y, tcl = i, 
       main = paste('tcl = ', i, sep = ''), 
       cex.main = 2)
}
for (i in c(-0.05, 0.05, 0.1)){
  plot(x, y, tck = i, 
       main = paste('tck = ', i, sep = ''), 
       cex.main = 2)
}
##mpg, oma, omi 
par(mfcol = c(1, 1))
par(mgp = c(4, 1, 0), 
    oma = c(0, 0, 0, 0),##外边距， 基本上不需要改
    mar = c(5, 4, 4, 2))##内边距
plot(x, y, bg = 'black')
##xaxs, yaxs
x <- 0:10
y <- 0:10
par(mfrow = c(2, 1))
plot(x, y, xaxs = 'i', yaxs = 'i', 
     type = 'l', bty = 'l', 
     main = paste('xaxs = "i", yaxs = "i"'), 
     cex.main = 2)
plot(x, y, xaxs = 'r', yaxs = 'r', 
     type = 'l', bty = 'l', 
     main = paste('xaxs = "r", yaxs = "r"'), 
     cex.main = 2)
##points, lines, text, abline
par(mfrow = c(1, 1))
##plot高级作图函数
##points低级作图函数
##xlim, ylim，x轴y轴的范围
##xlab, ylab, main 
##polygon, 置信区间
par(mgp = c(2, 0.8, 0))
plot(1, NA, xlim = c(1, 10), ylim = c(1, 10), 
     xlab = 'aaa', 
     ylab = 'bbb', 
     main = 'title', 
     cex.lab = 2,
     cex.main = 2, 
     font.lab = 2, xaxt = 'n', 
     yaxt = 'n', 
     bty = 'l')
polygon(c(1:10, rev(1:10)), c(rep(5, 10), rep(10, 10)), 
        col = 'pink', border = 'pink')
axis(side = 1, tck = -0.01, at = 1:10, cex.axis = 1.6)
axis(side = 2, tck = -0.01, at = 1:10, cex.axis = 1.5)
lines(1:6, seq(6, 8, length.out = 6), type = 'l', 
      lty = 1, lwd = 3)
lines(6:10, seq(8, 10, length.out = 5), type = 'l', 
      lty = 2, lwd = 3)
abline(v = 6, col = 'gray40', lwd = 2, 
       lty = 2)
text(3, 8, 'y = 0.35 * x + 5', cex = 1.6)
points(1:10, seq(5, 10, length.out = 10), 
       pch = 16, cex = 1.6, 
       col = 'red')






points(1:10, 1:10, pch = 16, cex = 2, 
       col = 'red')
lines(c(1, 10), c(2, 8), lwd = 3, 
      lty = 2, col = 'red')##(1, 2)--->(10, 8)
text(3, 5, 'text', cex = 2, col = 'blue')
abline(h = 3, v = 2)
##legend, axis 
y1 <- rnorm(100, 10, 2)
y2 <- rnorm(100, 5, 1)
##type = 'l'-->line 
##type = 'p'-->point 
##type = 'b'-->both
plot(1:100, y1, type = 'l', 
     lty = 2, col = 'blue', 
     ylim = c(min(y1, y2), max(y1, y2)))
lines(1:100, y2, col = 'red')
legend('topleft', legend = c('aa', 'bb'), lty = c(1, 2), 
       col = c('red', 'blue'), bty = 'n', cex = 2)
##side 1-->x轴， side = 2-->y轴
axis(side = 1, at = seq(0, 100, 10))
##logistic 
df <- read_excel('logistic.xlsx')
head(df)
df$y <- factor(df$y)
df$x1_ <- factor(df$x1_)
df$x2_ <- factor(df$x2_)
df$x3_5 <- df$x3 / 5##每改变五个单位的OR
##.-->剩下的所有变量
##logistic回归
index <- sample(1:2, nrow(df), replace = T, prob = c(0.7, 0.3))
set.seed(100)
train_df <- df[index == 1, ]##60%
test_df <- df[index == 2, ]##40% 
##glm, gam,共同的参数 family
##family:gaussian, binomial, poission 
logis_fit <- glm(y ~ x1_ + x2_ + x3_5, data = train_df, 
                 family = binomial(link = 'logit'))
summary(logis_fit)
##response, 预测概率, 分类预测模型，预测效果评价, 
pred <- predict(logis_fit, newdata = test_df, type = 'response')
##有概率转为分类
pred_level <- ifelse(pred >= 0.5, 1, 0)
confusionMatrix(factor(test_df$y), factor(pred_level), positive = '1')
exp(logis_fit$coefficients)
# log(y) ~ x1 + x2
##OR值
exp(confint(logis_fit))
##95%CI 
confint(logis_fit)
##OR 95%CI 
exp(confint(logis_fit))
##roc , 多指标的
pred_val <- predict(logis_fit, newdata = df, 
                    type = 'response')
pred <- prediction(pred_val, df$y)
perf <- performance(pred, 'tpr', 'fpr')
auc <- unlist(slot(performance(pred, 'auc'), 'y.values'))
auc
plot(perf)
##========================
library(RColorBrewer)
display.brewer.all()
my_col <- brewer.pal(3, 'Spectral')
plot(1:10, 2:11, col = my_col[1], type = 'l')
##单个指标roc曲线
cut_off <- roc(response = df$y, df$x3, plot = T, print.auc = T)
plot(1 - cut_off$specificities, cut_off$sensitivities, 
     type = 'l', ylab = 'Sensitivity', 
     xlab = '1 - Specificity', col = 'blue', 
     lwd = 2, main = 'ROC Curve')
##逐步回归,logistic, 向后, enter 
logis_step1 <- step(logis_fit, direction = 'backward')
##向前逐步
logis_step2 <- step(logis_fit, direction = 'forward')
##双向
logis_step3 <- step(logis_fit, direction = 'both')
##多分类logistic=============================
tmp <- iris
tmp$Species <- relevel(as.factor(iris$Species), 
                       ref = "setosa") #选择参考分类
mult_fit <- nnet::multinom(Species ~ ., data = tmp)
summary(mult_fit)
exp(coef(mult_fit))
# 系数显著性检验
z <- summary(mult_fit)$coefficients / summary(mult_fit)$standard.errors
p <- (1 - pnorm(abs(z), 0, 1)) * 2
p
# 相对危险度（相对危险风险比，或者也叫odds），与OR等价
exp(coef(mult_fit))
##基础作图，作图示例========================
tmp_obs <- read_excel('tmp_obs.xlsx')
tmp_pred <- read_excel('tmp_pred.xlsx')
tmp_obs <- tmp_obs[, c('year', 'val')]
tmp_pred <- tmp_pred[, c('year', 'val_pred', 'lower_pred', 'upper_pred')]
tmp_obs <- sqldf('select * from tmp_obs order by year')
tmp_pred <- sqldf('select * from tmp_pred group by year')
min_y <- min(c(tmp_obs$val, tmp_pred$val_pred, 
               tmp_pred$upper_pred, tmp_pred$lower_pred), na.rm = T)
max_y <- max(c(tmp_obs$val, tmp_pred$val_pred, 
               tmp_pred$upper_pred, tmp_pred$lower_pred), na.rm = T)

cairo_pdf(filename = 'global2.pdf', width = 15, height = 7.5)
par(mar = c(4.5, 5, 3.5, 1))
plot(0, NA, 
     xlim = c(1990, 2030), ylim = c(min_y, max_y), 
     xlab = 'Period', bty = 'l', 
     cex.axis = 1.8, cex.lab = 2, 
     pch = 1, col = 'white', 
     main = 'Global', cex.main = 2.2, 
     ylab = 'ASR')
polygon(c(1990:2030, 2030:1990), 
        c(tmp_pred$lower_pred, rev(tmp_pred$upper_pred)), 
        col = 'pink', border = 'pink')
points(tmp_obs$year, tmp_obs$val, 
       pch = 1, cex = 1.6, 
       lwd = 4, col = 'black')
lines(tmp_pred$year, tmp_pred$val_pred, 
      lwd = 2, col = 'black')
points(tmp_obs$year, tmp_obs$val, 
       pch = 16, col = 'white')
abline(v = 2020, lty = 2, lwd = 2, col = 'gray60')
dev.off()
##森林图===========================
tmp <- read_excel('森林图.xlsx')
par(mar = c(5, 5, 1, 1))
library(forestplot)
tmp_add <- data.frame(tmp[1, ])
tmp_add$vname <- '变量名'
tmp_add$or <- NA
tmp_add$lower <- NA
tmp_add$upper <- NA
tmp_add$or_ci <- 'OR(95%CI)'
tmp <- rbind(tmp_add, tmp)
forestplot(labeltext = tmp[, c('vname', 'or_ci')], 
           mean = tmp$or, 
           lower = tmp$lower, 
           upper = tmp$upper, 
           graph.pos = 1, ##
           zero = NA, 
           lwd.xaxis = 2, 
           lwd.ci = 2, 
           # lwd.zero = 2, 
           # lty.zero = 2, 
           boxsize = 0.2, 
           xlab = 'xlab2', 
           cex.axis = 2, 
           grid = structure(c(1),
                            gp = gpar(col = "gray45",
                                      lty = 2, lwd = 2)),
           txt_gp = fpTxtGp(ticks = gpar(cex = 1.5), 
                            xlab = gpar(cex = 2.5), 
                            cex = c(2)),
           col = fpColors(lines = 'blue', box = 'black'), 
           l病数Basety.ci = 7, 
           title = 'title')

##结果保存
df <- data.frame(y = rnorm(500, 10, 2), x1 = rnorm(500, 4, 2), 
                 x2 = rnorm(500, 3, 1), x3 = rnorm(500, 1.6, 1), 
                 x4 = rnorm(500, 2, 1), x5 = rnorm(500, 3.2, 2))
# df <- read.csv('xxxx.csv')
fit <- lm(y ~ ., data = df)
rst <- summary(fit)$coefficients
rst <- rst[, c(1, 3, 4)]
rst <- as.data.frame(rst)
rst[] <- lapply(rst, round_f, 2)
names(rst) <- c('beta', 't_val', 'p_val')
rst$var <- row.names(rst)
row.names(rst) <- NULL
rst <- select(rst, var, everything())
openxlsx::write.xlsx(rst, 'linear_model.xlsx', row.names = F)
##-----------------------------
##基础作图系统, ggplot2, lattice
##==============================
##poisson 
##治疗条件（Trt)
##年龄Age
##前八周的基础发
##响应变量为sumY，八周内癫痫发病数。
df <- read_excel('poisson.xlsx')
##levels 进行参考分类的设置，第一个
df$Trt <- factor(df$Trt, levels = c('placebo', 'progabide'))
##logis, poisson,--广义线性模型
fit <- glm(sumY ~ Base + Age + Trt + Y1, data = df, 
           family = poisson())
summary(fit)
coefficients(fit)
exp(coefficients(fit))
##cox ============================
df <- read_excel('cox.xlsx')
##censoring status 1=censored, 2=dead
cox_fit <- coxph(Surv(time, status) ~ ph.ecog + age + sex, data = df)
summary(cox_fit)
##cox--hr, logistic--or 
##ph假定
tmp <- cox.zph(cox_fit)
ggcoxzph(tmp)
##生存曲线绘制
ggsurvplot(survfit(cox_fit), data = df, palette = "#2E9FDF", 
          ggtheme = theme_minimal(), legend = "none")
##生存曲线, 分组的生存曲线。
cox_fit2 <- survfit(Surv(time, status) ~ sex, data = df)
summary(cox_fit2)
##坐标轴修改，优化
ggsurvplot(cox_fit2,
           pval = TRUE,
           conf.int = TRUE,
           # risk.table = TRUE,
           risk.table.col = "strata", 
           linetype = "strata",
           surv.median.line = "hv",
           ggtheme = theme_minimal(), 
           # fun = 'event',
           palette = c("blue", "red"), 
           tables.theme = 
             ggplot2::theme(axis.line = element_line(size = 2)))
##gam==============================
# library(mgcv)
# set.seed(2) ## simulate some data... 
# dat <- gamSim(1,n=1000,dist="normal",scale=2)
# openxlsx::write.xlsx(dat, 'gam.xlsx', row.names = F)
df <- read_excel('gam.xlsx')
# train_df <- df[1:600, ]
##60% 40%
##随机生成两个训练和测试数据集的方法==============
index <- sample(1:2, nrow(df), replace = T, prob = c(0.7, 0.3))
set.seed(100)
train_df <- df[index == 1, ]##60%
test_df <- df[index == 2, ]##40% 
# test_df <- df[601:1000, ]
gam_fit <- mgcv::gam(y ~ s(x0) + s(x1)+ s(x2) + s(x3), data = train_df)
summary(gam_fit)
gam.check(gam_fit)
plot(gam_fit, pages = 1, residuals = TRUE)  
plot(gam_fit, pages = 1, seWithMean = TRUE) 
##计算gam预测的置信区间， bootstrap 
nboot <- 500
preds <- matrix(NA_real_, ncol = nboot, nrow = nrow(test_df))
set.seed(20)
for (i in 1:nboot){
  tmp_df <- train_df[sample(1:nrow(train_df), nrow(train_df), replace = T), ]
  #replace = T有放回抽样
  tmp_fit <- mgcv::gam(y ~ s(x0) + s(x1)+ s(x2) + s(x3), 
                       data = tmp_df)
  preds[, i] <- predict(tmp_fit, newdata = test_df)
  if (i %% 100 == 0){
    cat(i, '\n')
  }
}
test_df$val <- apply(preds, 1, mean, na.rm = T)
##lower ci 
test_df$lower <- apply(preds, 1, quantile, 0.025, na.rm = T)
##upper ci 
test_df$upper <- apply(preds, 1, quantile, 0.975, na.rm = T)
head(test_df)
plot(test_df$y, test_df$val, type = 'p', pch = 16)
##mape , mean(abs((预测值 - 实际值) / 实际值) * 100% )
mape <- mean(abs((test_df$val - test_df$y) / test_df$y)) * 100 
mape
##rmse 
rmse <- sqrt(mean((test_df$y - test_df$val) ^ 2))

##lasso =========================
df <- read_excel('lasso.xlsx')
##fit model
df <- as.data.frame(df)
fit <- glmnet(as.matrix(df[, -1]), df$y)
print(fit)
coef(fit, s = 0.1)##s --> specifes the value(s) of lambda at which extraction is 
# Df    %Dev   Lambda
# [1,]  0 0.00000 1.631000
# [2,]  2 0.05528 1.486000
# [3,]  2 0.14590 1.354000
# [4,]  2 0.22110 1.234000
# [5,]  2 0.28360 1.124000
# [6,]  2 0.33540 1.024000
plot(fit, xvar = 'lambda', label = TRUE)
#predict
nx <- matrix(rnorm(200), 10, 20)
predict(fit, newx = nx, s = c(0.1, 0.05))
##交叉验证，找到合适的lambda, rmse mae  mse
###========================
df <- read_excel('lasso.xlsx')
index <- sample(1:2, nrow(df), replace = T, prob = c(0.8, 0.2))

train_df <- df[index == 1, ]
test_df <- df[index == 2, ]
cvfit <- cv.glmnet(as.matrix(train_df[, -1]), train_df$y, 
                   nfolds = 10, 
                   type.measure = 'mse')
## 1000, 10份， 100个
##9份训练， 1份验证， test_df
##type.measure: mse, mae
par(mar = c(5, 5, 3, 3))
plot(cvfit)
cvfit$lambda.min
##筛选的变量
coef(cvfit, s = cvfit$lambda.min)
preds <- predict(cvfit, newx = as.matrix(test_df[, -1]), 
                 s = cvfit$lambda.min)
plot(test_df$y, preds, type = 'p', pch = 16, col = 'red')
lines(c(-6, 6), c(-6, 6))
##连续变量预测误差评价
(mape <- mean(abs((preds - test_df$y) / test_df$y)) * 100)
##=======================
plot(fit, xvar = 'lambda', label = T)
plot(fit, xvar = 'norm', label = T)
plot(fit, xvar = 'dev', label = T)
##散点图===============================
##Q-Q plot----------------
df <- data.frame(x = rnorm(250 , mean = 10 , sd = 1))
##shapiro.test()
par(mar = c(5, 5, 5, 1))
ggplot(df, aes(sample = x))+
  geom_qq(col = 'red') + 
  geom_qq_line(col = "blue", size = 1) + 
  xlab('xlab') + ylab('ylab') + 
  theme(axis.line = element_line(), 
        axis.title = element_text(size = 20), 
        axis.text = element_text(size = 20), 
        panel.grid = element_blank(), 
        panel.background = element_blank()) + 
  ggtitle('mytitle')
##散点图=================
df <- read.csv("散点图.csv",stringsAsFactors = FALSE) 
ggplot(data = df, aes(x = x, y = y)) + 
  geom_point(fill = 'red', 
             col = rgb(191 / 255, 145 / 255, 105 / 255), size = 5, 
             shape = 21) + ##shape, pch 
  # geom_smooth(method = "lm",se=TRUE, formula=y ~ splines::bs(x, 5), colour="red")+
  # geom_smooth(method = 'gam',formula = y ~s(x))+
  # geom_smooth(method = 'loess', span = 0.4,
              # se = TRUE, colour = "blue",
              # fill="pink", alpha=0.2)+
  scale_y_continuous(breaks = seq(0, 125, 25))+
  theme(
    text = element_text(size = 12,color = "black"),
    plot.title = element_text(size = 12, color="black"),
    legend.position= "none", 
    panel.grid = element_blank(), 
    panel.background = element_blank(), 
    axis.line = element_line())
##气泡图=======================
mtcars
##颜色名字, rgb(red / 255, green / 255, blue, 255)

ggplot(data = mtcars, aes(x = wt, y = mpg))+
  geom_point(aes(size = disp, fill = disp), shape = 21,
             colour = "black", alpha = 0.8) +
  scale_fill_gradient2(low = "#377EB8", high = "#E41A1C",
                       midpoint = mean(mtcars$disp))+
  scale_size_area(max_size = 12)+
  guides(size = guide_legend((title = "Disp")),
         fill = guide_legend((title = "Disp")))+
  xlab('Weight') + 
  theme(
    legend.text= element_text(size= 12, color="black"),
    axis.title = element_text(size=16, color="black", face = 'bold'),
    axis.text = element_text(size=12, color="black"),
    legend.position = "left", 
    legend.title = element_text(size = 16, face = 'bold'))
##相关系数热力图=====================
library(reshape2)
library(RColorBrewer)
brewer.pal(9, 'Set1')
mat <- round(cor(mtcars), 1)
mydata <- melt(mat)  
colnames(mydata) <- c("Var1", "Var2", "value")
ggplot(mydata, aes(x = Var1, y = Var2, fill = value,label=value)) +  
  geom_tile(colour= "black") +
  geom_text(size =3, colour = "white")+
  coord_equal()+
  scale_fill_gradientn(colours=c(brewer.pal(7,"Set1")[2], "white", 
                                 brewer.pal(7,"Set1")[1]), na.value=NA)+
  theme(panel.background = element_rect(fill="white", colour = NA),
        panel.grid.major = element_line(colour = "grey60", size=.25, linetype = "dotted" ),
        panel.grid.minor = element_line(colour = "grey60", size=.25,linetype = "dotted" ),
        text = element_text(size = 15),
        plot.title = element_text(size = 15), 
        axis.title = element_text(size = 22), 
        axis.text = element_text(size = 13, face = 'bold')
  )
##--------------------
x <- rnorm(100, 5, 3)
y <- rnorm(100, 10, 2)
plot(mtcars$wt, mtcars$mpg, 
     pch = 16, col = 'pink', 
     cex = 2, cex.lab = 2, 
     font.lab = 2, bty = 'l')

ggplot(data = mtcars, aes(x = wt, y = mpg)) + 
  geom_point(fill = 'steelblue', colour = 'black', 
             size = 3, shape = 21) 
##添加趋势线
ggplot(data = mtcars, aes(x = wt, y = mpg)) + 
  geom_point() + 
  stat_smooth(method = lm, level = 0.95)
##柱状图
#条图
barplot(GNP ~ Year, data = longley, axes = T, col = 'red', 
        border = 'red', xlab = 'Year', cex.lab = 2, 
        cex.axis = 1.4)
axis(side = 1)
##堆叠的复试条图
barplot(cbind(Employed, Unemployed) ~ Year, data = longley, beside = T,
        col = c('blue', 'red'), border = c('blue', 'red'))
##复式条图
barplot(VADeaths, beside = T,
        col = c("lightblue", "mistyrose", "lightcyan",
                "lavender", "cornsilk"),
        legend = rownames(VADeaths), ylim = c(0, 100))
title(main = "Death Rates in Virginia", font.main = 3)
##直方图
##birthwt
library(MASS)
ggplot(birthwt, aes(x = bwt)) + 
  geom_histogram(position = 'identity', alpha = 0.4, 
                 binwidth = 100, fill = 'steelblue', col = 'red') + 
  theme(axis.title = element_text(size = 20, face = 'italic'), 
        axis.line = element_line())
##箱式图 
ggplot(birthwt, aes(x = factor(race), y = bwt)) + 
  geom_boxplot(width = 0.34) + 
  geom_jitter(aes(color = factor(race)), 
              width = 0.1) + 
  theme(panel.background = element_blank(), 
        panel.grid = element_blank(), 
        axis.line = element_line(), 
        axis.title = element_text(size = 18, face = 'bold'), 
        axis.text = element_text(size = 14, face = 'bold'), 
        legend.text = element_text(size = 16), 
        legend.title = element_text(size = 18)) + 
  guides(col = guide_legend((title = "Race"))) + 
  xlab('Race') + ylab('Birth weight(g)')
##饼图
library(RColorBrewer)
df <- data.frame(v = c(20, 30, 10, 40), 
                 group = letters[1:4])
df$pct <- paste(round_f(df$v / sum(df$v) * 100, 2), '%', sep = '')
df$pct <- paste(df$group, df$pct, sep = ' ')
pie(df$v, init.angle = 0, clockwise = T, 
    lab = df$pct, 
    cex.lab = 2, 
    border = brewer.pal(4, 'YlOrRd'), 
    col = brewer.pal(4, 'YlOrRd'), 
    cex = 2)
display.brewer.all()
##矩阵散点图
# par(mar = c(3, 1, 3, 1), oma = c(1, 1, 1, 1))
par(mar = c(7, 7, 7, 7), oma = c(7, 7, 7, 7))
corrplot(cor(mtcars), method = 'pie', title = 'cor')
##热力图
par(oma = c(0, 0, 0, 0), mar = c(3, 2, 2, 2), mgp = c(3, 1, 0))
df <- mtcars
df <- scale(df)
cols <- colorRampPalette(brewer.pal(n = 9, name = 'YlOrRd'))(100)
pheatmap(df, col = cols, 
         cluster_rows = F, 
         cluster_cols = F, 
         fontsize = 16, 
         labels_col = str_to_title(names(mtcars)))##首字母大写
##热力图2
par(mar = c(5, 5, 5, 5))
heatmap(as.matrix(mtcars), 
        Rowv = NA,
        Colv = NA, 
        # col = heat.colors(10), 
        col = cols, 
        scale = 'column', 
        main = 'test heatmap', 
        labCol = str_to_title(names(mtcars)), 
        cexRow = 2, cexCol = 2)
##world map ======================
tmp <- read_excel('world.xlsx')
shp <- readOGR('map.shp')
shp@data$id <- rownames(shp@data)
shp_point <- fortify(shp)
df_map <- full_join(shp_point, shp@data, by = 'id')
##dply, left_join, merge
tmp <- left_join(df_map, tmp, by = c('sovereignt' = 'location_name'))
##产生渐变的颜色
colors <- colorRampPalette(brewer.pal(9, 'YlOrRd'))(30)
cairo_pdf(filename = 'map2.pdf', width = 15, height = 9)
p <- ggplot(tmp) + 
  geom_polygon(aes(x = long, y = lat, group = group, fill = val),
               size = 0.01, colour = 'gray30') + 
  scale_fill_gradientn(colours = colors, na.value = 'white') +
  theme(legend.position = 'right', 
        legend.background = element_blank(), 
        panel.background = element_blank(), 
        axis.line = element_line(size = 0.3), 
        axis.text = element_text(size = 12, face = 'bold'), 
        axis.title = element_text(size = 14, face = 'bold'), 
        legend.title = element_text(size = 12, face = 'bold'), 
        legend.text = element_text(size = 12, face = 'bold'), 
        plot.title = element_text(size = 20, face = 'bold', hjust = 0.5)) + 
  xlab('Long') + ylab('Lat') 
plot(p)
dev.off()
##ggplot, geom_area =================
library(sqldf)
tmp <- read_excel('geom_area.xlsx')
tmp_order <- sqldf('select * from tmp group by age_id order by age_id desc')
tmp$age_name <- factor(tmp$age_name, 
                       levels = tmp_order$age_name)
rm(tmp_order); gc()
p <- ggplot(data = tmp, 
       aes(x = year, 
           y = val, 
           fill = age_name)) + 
  geom_area() + 
  scale_fill_manual(values = colorRampPalette(brewer.pal(9, 'Set1'))(length(unique(tmp$age_name))), 
                    name = 'Ages (years old)') + 
  xlab('xlab') + ylab('ylab') + 
  scale_x_continuous(breaks = c(1990, 1995, 2000, 2005, 2010, 2015, 2019), 
                     limits = c(1990, 2019)) + 
  theme(panel.grid = element_blank(), 
        panel.background = element_blank(), 
        axis.line = element_line(size = 0.3),  
        axis.text = element_text(size = 13, face = 'bold'), 
        axis.title = element_text(size = 14, face = 'bold'), 
        legend.title = element_text(size = 14, face = 'bold'), 
        legend.text = element_text(size = 13, face = 'bold'), 
        legend.position = 'right', 
        plot.title = element_text(size = 20, face = 'bold', hjust = 0.5)) + 
  ggtitle('title')
cairo_pdf(filename = 'geom_area.pdf', width = 15, height = 9)
plot(p)
dev.off()
##geom_bar =========================
tmp <- read_excel('geom_bar.xlsx')
tmp <- select(tmp, location_name, val, rei_name)
tmp_s <- sqldf('select location_name, sum(val) as val_loc from 
                 tmp group by location_name order by val_loc asc')
order_locs <- tmp_s$location_name
rm(tmp_s); gc()
tmp_rei_names <- unique(tmp$rei_name)
my_cols <- brewer.pal(12, 'Paired')[c(6, 12, 1:5, 7:11)]
tmp_my_cols <- colorRampPalette(my_cols)(20)
tmp$location_name <- factor(tmp$location_name, levels = order_locs)
tmp$rei_name <- factor(tmp$rei_name, levels = rev(rei_order_names))
p<- ggplot(data = tmp, aes(x = location_name, y = val, fill = rei_name)) + 
  geom_bar(stat = 'identity', position = 'stack', width = 0.618) + 
  theme(panel.background = element_blank(), 
        panel.grid = element_blank(), 
        axis.line = element_line(), 
        axis.title = element_text(size = 16, face = 'bold'), 
        legend.title = element_text(size = 14, face = 'bold'), 
        axis.text = element_text(size = 12, face = 'bold'), 
        legend.text = element_text(size = 11, face = 'bold')) + 
  xlab('xlab') + ylab('ylab') + 
  coord_flip() + 
  scale_fill_manual(values = rev(tmp_my_cols), 
                    guide = guide_legend(reverse = T), 
                    name = 'Risk factors')
cairo_pdf(filename = 'geom_bar.pdf', width = 15, height = 9)
plot(p)
dev.off()
gc()
##=============================
##track
##sector
##cell
set.seed(999)
bed = generateRandomBed()
head(bed)

bed = generateRandomBed(nr = 200, nc = 4)
nrow(bed)
circos.clear()
circos.par(gap.degree = 1, star.degree = 0)

##人类基因的link==========================
set.seed(123)
bed1 = generateRandomBed(nr = 100)
bed1 = bed1[sample(nrow(bed1), 20), ]
bed2 = generateRandomBed(nr = 100)
bed2 = bed2[sample(nrow(bed2), 20), ]
circos.initializeWithIdeogram()
circos.genomicLink(bed1, bed2, col = rand_color(nrow(bed1), transparency = 0.5), 
                   border = NA)


par(mfrow = c(1, 1))
set.seed(999)
##circos gene point 
circos.par("track.height" = 0.1, start.degree = 90,
           canvas.xlim = c(0, 1), canvas.ylim = c(0, 1), gap.degree = 270)
circos.initializeWithIdeogram(chromosome.index = "chr1", plotType = NULL)

bed = generateRandomBed(nr = 300)
circos.genomicTrack(bed, panel.fun = function(region, value, ...) {
  circos.genomicPoints(region, value, pch = 16, cex = 0.5, ...)
})

circos.genomicTrack(bed, stack = TRUE, 
                    panel.fun = function(region, value, ...) {
                      circos.genomicPoints(region, value, pch = 16, cex = 0.5,...)
                      i = getI(...)
                      circos.lines(CELL_META$cell.xlim, c(i, i), lty = 2, col = "#00000040")
                    })

bed1 = generateRandomBed(nr = 300)
bed2 = generateRandomBed(nr = 300)
bed_list = list(bed1, bed2)
circos.genomicTrack(bed_list, 
                    panel.fun = function(region, value, ...) {
                      i = getI(...)
                      circos.genomicPoints(region, value, pch = 16, cex = 0.5, col = i, ...)
                    })




circos.genomicTrack(bed_list, stack = TRUE, 
                    panel.fun = function(region, value, ...) {
                      i = getI(...)
                      circos.genomicPoints(region, value, pch = 16, cex = 0.5, col = i, ...)
                      circos.lines(CELL_META$cell.xlim, c(i, i), lty = 2, col = "#00000040")
                    })
bed = generateRandomBed(nr = 300, nc = 4)
circos.genomicTrack(bed, 
                    panel.fun = function(region, value, ...) {
                      circos.genomicPoints(region, value, pch = 16, cex = 0.5, col = 1:4, ...)
                    })
bed = generateRandomBed(nr = 300, nc = 4)
circos.genomicTrack(bed, stack = TRUE, 
                    panel.fun = function(region, value, ...) {
                      i = getI(...)
                      circos.genomicPoints(region, value, pch = 16, cex = 0.5, col = i, ...)
                      circos.lines(CELL_META$cell.xlim, c(i, i), lty = 2, col = "#00000040")
                    })
circos.clear()
##circos线条====================

circos.par("track.height" = 0.08, start.degree = 90,
           canvas.xlim = c(0, 1), canvas.ylim = c(0, 1), gap.degree = 270,
           cell.padding = c(0, 0, 0, 0))
circos.initializeWithIdeogram(chromosome.index = "chr1", plotType = NULL)

bed = generateRandomBed(nr = 500)
circos.genomicTrack(bed, 
                    panel.fun = function(region, value, ...) {
                      circos.genomicLines(region, value)
                    })

circos.genomicTrack(bed, 
                    panel.fun = function(region, value, ...) {
                      circos.genomicLines(region, value, area = TRUE)
                    })
circos.genomicTrack(bed, 
                    panel.fun = function(region, value, ...) {
                      circos.genomicLines(region, value, type = "h")
                    })
bed1 = generateRandomBed(nr = 500)
bed2 = generateRandomBed(nr = 500)
bed_list = list(bed1, bed2)
circos.genomicTrack(bed_list, 
                    panel.fun = function(region, value, ...) {
                      i = getI(...)
                      circos.genomicLines(region, value, col = i, ...)
                    })
circos.genomicTrack(bed_list, stack = TRUE, 
                    panel.fun = function(region, value, ...) {
                      i = getI(...)
                      circos.genomicLines(region, value, col = i, ...)
                    })
bed = generateRandomBed(nr = 500, nc = 4)
circos.genomicTrack(bed, stack = TRUE, 
                    panel.fun = function(region, value, ...) {
                      i = getI(...)
                      circos.genomicLines(region, value, col = i, ...)
                    })

bed = generateRandomBed(nr = 200)
circos.genomicTrack(bed, 
                    panel.fun = function(region, value, ...) {
                      circos.genomicLines(region, value, type = "segment", lwd = 2, 
                                          col = rand_color(nrow(region)), ...)
                    })
circos.clear()
##circos heatmap====================
circos.initializeWithIdeogram()
bed = generateRandomBed(nr = 100, nc = 4)
col_fun = colorRamp2(c(-1, 0, 1), c("green", "black", "red"))
circos.genomicHeatmap(bed, col = col_fun, side = "inside", border = "white")
circos.clear()

circos.initializeWithIdeogram()
# bed = generateRandomBed(nr = 50, fun = function(k) sample(letters, k, replace = TRUE))
# bed[1, 4] = "aaaaa"
# circos.genomicLabels(bed, labels.column = 4, side = "inside")
# circos.clear()
##=========
load(system.file(package = "circlize", "extdata", "DMR.RData"))
circos.initializeWithIdeogram(chromosome.index = paste0("chr", 1:22))
bed_list = list(DMR_hyper, DMR_hypo)
circos.genomicRainfall(bed_list, pch = 16, cex = 0.4, col = c("#FF000080", "#0000FF80"))
circos.genomicDensity(DMR_hyper, col = c("#FF000080"), track.height = 0.1)
circos.genomicDensity(DMR_hypo, col = c("#0000FF80"), track.height = 0.1)
##circlize ===============================
##cirlize弦图 ==================
##from to
set.seed(999)
mat = matrix(sample(18, 18), 3, 6) 
rownames(mat) = paste0("S", 1:3)
colnames(mat) = paste0("E", 1:6)
mat
# E1 E2 E3 E4 E5 E6
# S1  4 14 13 17  5  2
# S2  7  1  6  8 12 15
# S3  9 10  3 16 11 18
chordDiagram(mat)
df = data.frame(from = rep(rownames(mat), times = ncol(mat)),
                to = rep(colnames(mat), each = nrow(mat)),
                value = as.vector(mat),
                stringsAsFactors = FALSE)
chordDiagram(df)
# from to value
# 1   S1 E1     4
# 2   S2 E1     7
# 3   S3 E1     9
# 4   S1 E2    14
# 5   S2 E2     1
# 6   S3 E2    10
#基因数据进行link=============================
set.seed(123)
bed1 = generateRandomBed(nr = 100)
bed1 = bed1[sample(nrow(bed1), 20), ]
bed2 = generateRandomBed(nr = 100)
bed2 = bed2[sample(nrow(bed2), 20), ]
circos.initializeWithIdeogram(species = 'hg19')
circos.genomicLink(bed1, bed2, 
                   col = rand_color(nrow(bed1), transparency = 0.5), 
                   border = NA)
circos.clear()
##==========================================
par(mfrow = c(1, 2))
chordDiagram(mat, order = c("S2", "S1", "S3", "E4", "E1", "E5", "E2", "E6", "E3"))
circos.clear()

chordDiagram(mat, order = c("S2", "S1", "E4", "E1", "S3", "E5", "E2", "E6", "E3"))
circos.clear()

circos.par(gap.after = c(rep(5, nrow(mat)-1), 15, rep(5, ncol(mat)-1), 15))
chordDiagram(mat)
circos.clear()

circos.par(gap.after = c(rep(5, length(unique(df[[1]]))-1), 15, 
                         rep(5, length(unique(df[[2]]))-1), 15))
chordDiagram(df)
circos.clear()


circos.par(gap.after = c("S1" = 5, "S2" = 5, "S3" = 15, "E1" = 5, "E2" = 5,
                         "E3" = 5, "E4" = 5, "E5" = 5, "E6" = 15))
chordDiagram(mat)
circos.clear()
chordDiagram(mat, big.gap = 30)
circos.clear()
par(mfrow = c(1, 2))
circos.par(start.degree = 85, clock.wise = FALSE)
chordDiagram(mat)
circos.clear()
circos.par(start.degree = 85)
chordDiagram(mat, order = c(rev(colnames(mat)), rev(rownames(mat))))
circos.clear()
##grid color---------------------
##颜色设置
par(mfrow = c(1, 1))
grid.col = c(S1 = "red", S2 = "green", S3 = "blue",
             E1 = "grey", E2 = "grey", E3 = "grey", 
             E4 = "grey", E5 = "grey", E6 = "grey")
chordDiagram(mat, grid.col = grid.col)

chordDiagram(t(mat), grid.col = grid.col)
##
chordDiagram(mat, grid.col = grid.col, transparency = 0)

col_mat = rand_color(length(mat), transparency = 0.5)
dim(col_mat) = dim(mat)  # to make sure it is a matrix
chordDiagram(mat, grid.col = grid.col, col = col_mat)
##透明度
# plot(1:10, 1:10, col = rgb(1, 0, 0, alpha = 0.3), pch = 16, 
#      cex = 3)

col = rand_color(nrow(df))
chordDiagram(df, grid.col = grid.col, col = col)

col_fun = colorRamp2(range(mat), c("#FFEEEE", "#FF0000"), transparency = 0.5)
chordDiagram(mat, grid.col = grid.col, col = col_fun)
##from to , val 






