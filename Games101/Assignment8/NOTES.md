> Games101-8

# 作业8

## 环境

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

## 基础部分

问题1

加上显式**/**半隐式欧拉法后，蓝色绳子掉下来的节点每次启动都不一致？

只有pinned_nodes中标记的质点是固定的，其他是随机的？

应该初始化全部不固定

在pinned_nodes中指定固定点，构造Mass的参数需要改改。

问题2

explicit method 计算前几帧看着没问题，之后质点乱飞【待解决】

