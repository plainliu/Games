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

- box效果可以，但是到pink-room

  单帧降噪，噪声仍十分严重，且

  - filter后缩小看有白块
  - Wavelet filter后缩小看有**黑块**，放大看是黑色的点阵

  【10倍m_sigmaColor】后，噪声明显减小

  - filter后放大看有个别白点

  - Wavelet filter后，效果比正常filter的边缘地方糊，沙发中间的阴影比较明显

  - 加上投影后，

    - 普通filter，有个别黑点
    - Wavelet出现了黑色点阵，图片缩小显示黑块【推测是颜色区间相近，outlier？灾难扩散？调节m_colorBoxK】

    ![image-20210819222712525](C:\liujuanjuan\github-plainliu\Games\Games202\Assignment5\NOTES.assets\image-20210819222712525.png)

pick roon与box比较

- 颜色集中在粉色附近，所以需要提高颜色sigma，更细精度地区分颜色，只有在颜色相近程度更高的情况下才有贡献。
- 几何变复杂，不止正方正球

### 作业问题

问题一

按照作业要求，同时使用objID和Clamping的情况下，效果反而没有只用objID的好

分析：

- 将上一帧的信息Clamping到当前帧（当前帧噪声严重，上一帧视作已经降噪的），与降噪的愿意相背，把降噪了的值拉到了当前帧噪声严重
- 课程中处理的是拖影问题，用降噪替换拖影
- 分析的结论：如果用了objID，则不需要clamp，temporal success，信任上一帧，对于failure的情况，使用clamp
- 如果作业中用clamp，那么增大boxK，降低clamp的影响，从1增加到20，播放视频还是能看到移动的噪声……

问题二

wavelet加速后，不带clamping做对比，效果没有原filter稳定，仍然有噪声

问题三

值是-nan(ind)

![image-20210821085134625](C:\liujuanjuan\github-plainliu\Games\Games202\Assignment5\NOTES.assets\image-20210821085134625.png)

temporal计算后是nan，从计算结果来看，应该在filter阶段就出现nan了，用下面的方式在filter中验证

```c++
CHECK(sumColor.x == sumColor.x);
isinf();
isnan();
```

加打印发现nan是在level=2的时候出现的，此时

- curFilteredColor(x, y) = [inf]
- weight = 0
- w_color = inf

原因：

- 层数为1时（从0开始计数），dotnormal 计算的结果是小于0的，在这种情况下w_normal = 123左右
- 导致exp算出来是接近0的小数
- 颜色本身也小，sumColor值小
- sumColor / sum(exp) 为inf
- 层2使用层1的结果，sum(inf * #DEN) / #DEN 得到了nan

处理：

判断weight非法值的情况下，不参与计算

