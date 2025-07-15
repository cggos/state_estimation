# Anaconda

* [Anaconda](https://www.anaconda.com/): The Most Popular Python Data Science Platform

* conda: https://docs.conda.io

---

## conda 配置

```sh title="~/.condarc"
ssl_verify: true
channels:
  - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
  - defaults
auto_activate_base: false
default_python:
show_channel_urls: true

proxy_servers:
    http: socks5://127.0.0.1:1080
    https: socks5://127.0.0.1:1080
```

## conda 命令

* version
  ```sh
  conda --version
  ```

* config
  ```sh
  conda config --set auto_activate_base true # false
  ```

* env
  ```sh
  conda info --envs
  # or
  conda env list

  conda activate <env-name>
  ```

* 配置 国内源
  ```sh
  # 查看
  conda config --show-sources

  # 添加清华的源
  conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free

  # 移除源
  conda config --remove channels 'https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/'

  # 是否显示channel的来源
  conda config --set show_channel_urls yes # or no
  ```

* packages
  ```sh
  conda search numpy

  # 如果使用conda安装包的时候还是很慢，那么可以考虑使用pip来安装
  conda install [-n env-name -c channel-name] scipy
  conda install  -c  <mirror-url>  <package-name>
  conda install  --channel  <mirror-url>  <package-name>

  conda update conda
  conda update python
  ```

## with Jupyter

* add conda environment to jupyter lab
  ```sh
  conda install [-n env-name] ipykernel

  ipython kernel install --user --name=<any_name_for_kernel>
  # or
  python -m ipykernel install --name <env-name>
  ```