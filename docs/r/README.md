# R

* [The R Project for Statistical Computing](https://www.r-project.org/)
* [Introduction to R](http://ramnathv.github.io/pycon2014-r/)

---

## Tips

* plot
  ```R
  dev.new()
  # png(file = "linearregression.png")
  plot(y,x,...)
  # dev.off()
  ```

* png file
  ```R
  dev.new()
  png(file = "linearregression.png")
  plot(y,x,...)
  dev.off()
  ```

## ubuntu 16.04 升级 R 3.6

```bash
sudo apt-add-repository -y "deb https://cloud.r-project.org/bin/linux/ubuntu xenial-cran35/"
# 或者就近的镜像站点
sudo apt-add-repository -y "deb https://mirrors.tuna.tsinghua.edu.cn/CRAN/bin/linux/ubuntu xenial-cran35/"

# 必须导入 key
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys E084DAB9

sudo apt-get update
sudo apt-get install r-base r-base-dev

# 添加二进制编译好的R包仓库，有些R包依赖复杂，编译时间长
# R 3.5.x 系列 二进制 R 包
sudo add-apt-repository ppa:marutter/c2d4u3.5
sudo apt-get update
```

## install on Ubuntu 18.04

* install RStudio prerequisites
  ```sh
  sudo apt update
  sudo apt install r-base
  ```

* [Download RStudio](https://www.rstudio.com/products/rstudio/download/#download)
  ```sh
  sudo dpkg -i xxx.deb
  ```

## Proxy

[Configuring R to Use an HTTP or HTTPS Proxy](https://support.rstudio.com/hc/en-us/articles/200488488-Configuring-R-to-Use-an-HTTP-or-HTTPS-Proxy)

```bash
/usr/lib/R/etc/Renviron.site

http_proxy=http://127.0.0.1:58591
https_proxy=https://127.0.0.1:58591
```
