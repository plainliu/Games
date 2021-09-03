> Games101-8

# 作业8

## 基础部分

看群里基本都是虚拟机进行的，快提交作业，就直接上虚拟机做。

虚拟机安装依赖的问题

```sh
# libglu1末尾是1，输入错误
sudo apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev

# Unable to locate package xorg−dev
# http://mirrors.ustc.edu.cn/help/ubuntu.html 换中科大源也没用
# 最后发现是-错了，从pdf复制的编码不对
# sudo apt install xorg−dev
sudo apt install xorg-dev
```


