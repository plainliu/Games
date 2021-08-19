> Games202-5

# 环境-VS2019

## 准备工作

解压examples.zip到homework5目录（没有嵌套examples文件夹）

ffmpeg工具：http://ffmpeg.org/download.html

- 下载 https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip
- 解压
- bin目录加环境变量

## 运行

1. VS2019打开homework5 CMakeLists.txt所在目录
2. F5（注意修改main中examples地址，VS中增加相对路径，比如修改后`"../../../examples/box/input"`）
3. 执行 image2video.bat（环境中有ffmpeg）

# 作业

## 当前帧的降噪

1. 高斯模糊

   整个图像模糊

2. 高斯+颜色距离+法线距离+深度距离

问题：

```c++
float w_normal = Sqr(SafeAcos(Dot(frameInfo.m_normal(i, j), frameInfo.m_normal(x, y)))) / //计算得到的 w_normal太大导致weight用exp计算得到0
//(SafeAcos(Dot(frameInfo.m_normal(i, j), frameInfo.m_normal(x, y)))) /
(2.0f * Sqr(m_sigmaNormal));
```

SafeAcos = 1.5708

Sqr = 2.4674

w_normal = 124.384

exp(-w_normal) = 0

filter核整个for都是0【？】，加起来是0，导致报错在 /= weight

最后计算颜色的时候，将weight = 0的点视作黑色，可以看到黑色是边缘上的黑框，也就是导致报错的点的filer点都在黑框上，而且确实调试中第一个计算的点就报错了。

从normal_0.exr中看到边框的法线是0。

解决方案

- 对于normal，先计算dot，如果dot = 0，则该点不贡献信息

  以解决当前点的法线为0和当前滤波点的法线为0的两种情况

- 对于sumWeight为0的点，不计算/sumWeight，颜色直接给0

  以解决滤波采样的点和当前点综合计算的sumWeight是0是情况

## 投影上一帧结果

根据MVP倒推上一帧所在位置

- 判断上一帧是否在屏幕内
- 判断上一帧物体id和当前帧是否相同

相同的情况下保存到**m_accColor**中。

## 累积多帧信息

PDF中写的十分清楚，套公式计算即可。

## Bonus À-Trous Wavelet

需要注意的点

1. 先整个图像`5*5`滤波一层，再隔2个后 `5*5`一层。而不是对于单个顶点做n层后，进行下一个像素。

问题：

- box 0-3做4层后，出现明显方格子，条纹；减少到0-2做3层，效果好很多

