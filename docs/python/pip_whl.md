# pip & wheel

* [pip](https://pip.pypa.io/en/stable/): The PyPA recommended tool for installing Python packages

* [wheel](https://wheel.readthedocs.io/en/latest/) is the reference implementation of the Python wheel packaging standard
    - [Python Wheels](https://pythonwheels.com/)

---

## Install pip

```sh
python get-pip.py
```

## 配置 国内源

```ini title="~/.pip/pip.conf"
[global]
trusted-host =  pypi.douban.com
index-url = http://pypi.douban.com/simple
```

## Check

```sh
python -m pip list
python -m pip show sphinx
python -m pip search "query"
```

## Install & Uninstall

=== "general"
    ```sh
    python -m pip install -i https://pypi.tuna.tsinghua.edu.cn/simple <package-name>

    python -m pip uninstall SomePackage
    ```

=== "wheel"
    ```sh
    python -m pip install SomePackage-1.0-py2.py3-none-any.whl
    ```

=== "requirements.txt"
    ```sh
    pip install pipreqs

    pip install -r requirements.txt

    # export dependencies
    pip freeze requirements.txt
    ```
