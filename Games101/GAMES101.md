> title: 20200903GAMES101
>
> date: 2020-09-03 12:33:20



视频

https://www.bilibili.com/video/BV1X7411F744?p=4

课件

https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html

作业

http://games-cn.org/forums/topic/allhw/

论坛

http://games-cn.org/forums/forum/graphics-intro/

其他



# P1 Overview

看效果是不是亮，技术高低（全局光照技术）

VR、AR

特效、动画、模拟

GUI

字体放大，点阵和矢量

The Quick Brown Fox Jumps Over THer Lazy Dog

测试字体用

裸眼3D



学习：

数学

物理光阴影

3D图形

动画/模拟

不会涉及硬件编程



为啥要学：计算机图形学AWESOME



- 光栅化、OPGL、shader

  Rasterization 实时应用，达到30FPS；反之离线

- 曲线曲面

- 光线追踪

  trade off 取舍，效果

- 动画与模拟



计算机视觉

- 从图到物
- 猜测、分析

计算机图形学

- 从物到图

![1599108555007](GAMES101.assets\1599108555007.png)



推荐读物

虎书：第三版足够，第四版可，但不要第二版

作业：一周一个，代码框架已经有 C++；虚拟机

大作业：展示酷炫

IDE：Window-VS/VSCode，QT Creator（不用编辑器）【VSCode最推荐，跨平台】

geek-genus天才、frek怪胎（学天才部分，不用怪胎）

不要把框架发布到网上



# P2 Review of Linear Algebra

基础

- 数学：线性代数、微积分、统计
- 物理：光学、力学（波动光学）
- 信号处理（走样、反走样）、数值分析（积分）、模拟和仿真（有限元、无限元）
- 美学

# 线性代数

## 内容

矩阵和向量、旋转

## 向量

#### 基础概念

数学：向量

物理：矢量

AB：方向、长度

单位向量

```
|| a ||
a hat = a / || a ||
```

#### 基本操作

向量求和

- 平行四边形法则、三角形法则，数学上坐标（直角坐标系、笛卡尔坐标系）相加
- 从(0, 0) 开始，用单位向量表示向量，然后向量的系数用来表示向量(4, 3)

>  图形学中默认列向量（约定俗成）

向量转置

计算长度

**点乘**：a · b = a * b * cos

- 用来求夹角
- 交换律、加法结合律、系数结合律

= x1x2 + y1y2

投影到另外一个向量上

b-> |_ : b pep = ka = || b || cos

投影可以用来分解向量

用途：

- 两个向量的向近程度
- 分解向量
- 两个向量的方向是否相同，点乘大于0相同



**叉乘**：有方向

a * b = a * b * sin（大小）

结果与ab都垂直

右手定则

- 三指
- 点赞手

不满足交换律

右手坐标系 x * y = z

---

视频P2 36：00 开始

a  * b = （yaza-ybza, ）

**判断左右**

**判断内外**

三角形内外，进行着色

边界问题自己说了算



定义坐标系

三维直角坐标系

## 矩阵 Linear Algebra

m * n

行 * 列

矩阵的乘积（可以做乘法的判断）

（M * N）（N * P） = （M * P）

结合律（用处多）、分配律

不符合交换律

**MIT列向量组合计算的方法**-弹幕

矩阵和向量相乘

（M * N）（N * 1）

M = N

变换矩阵

转置 AB T = BT AT

对角阵

逆矩阵

点乘：行 * 列

叉乘：a * b = A* b



**sponza著名场景**

齐次坐标



作业0：配环境

# 变换 Transformation

摄像机运动

动画、缩放

三维到二维的投影

## 二维

缩放

乘以

$\left|s_x, 0| \\|0,  s_y\right|$

对称

切变（拉伸）

旋转（默认原点为中心，逆时针）

$\left|cos x, -sin x| \\|sin x ,  cos x\right|$

新向量 = 变换矩阵 * 老向量

## 齐次坐标

为了平移

$\begin{vmatrix}
x^,\\y^,\end{vmatrix} = \begin{vmatrix}
a & b \\
c & d
\end{vmatrix}
\begin{vmatrix}x\\y\end{vmatrix}+
\begin{vmatrix}t_x\\t_y\end{vmatrix}$

上面的写法需要带t向量为尾巴，目标是为了方便，让平移不会成为特例

tradeoff，没有免费的午餐

$\begin{vmatrix}
x^,\\y^,\\w^,\end{vmatrix} = \begin{vmatrix}
1 & 0 & t_x \\
0 & 1 & t_y \\
0 & 0 & 1 \\
\end{vmatrix}
\begin{vmatrix}x\\y\\1\end{vmatrix}$

向量具有平移不变性

**疑问**

用 $(x, y, 1)^T$表示点

$(x, y, 0)^T$表示向量

P3 38**疑问**

1和0的区别：

- 保证平移不变性
- 保证点和矩阵属性

![1599224006568](GAMES101.assets\1599224006568.png)

**点+点：两个点的终点**



仿射变换：用齐次坐标的形式

![1599224348289](GAMES101.assets\1599224348289.png)



代价：

- 向量增加了一个数字
- 矩阵扩充了一个维度

仿射变换最后一行是001，可以不存储，几乎没有多余的消耗

注意：投影变换不是001

逆变换 $M^{-1}$

## 复合变换

先旋转后平移（顺序重要、即不满足交换律）

$$ = 平移 * (旋转 * 原向量)\\
=  (平移 * 旋转) * 原向量 $$

结合律，可以将逐个应用的变换合并成一个变换

以上是合并变换

先线性变换再平移



分解变换

将复杂操作分解，**先平移**到原点再做旋转，做完再移动回去

## 三维变换

类比二维的操作

齐次坐标：向量3个数扩展成4个数

0表示向量1表示点

4*4矩阵表示

齐次矩阵



! 第一次从二维的理解到三维如此顺畅



问题：

- 为什么叫仿射变换
- 为什么叫齐次

# P4 Transformation Cont.

## 变换

2D旋转，转-theta，旋转矩阵转置

![1599225949264](GAMES101.assets\1599225949264.png)

旋转 theta 与旋转 -theta ，转置

同时定义上来看，是逆矩阵

所以是**正交矩阵**

### 3D变换

齐次坐标表示

加第四维，表示点和向量

矩阵4*4

变换矩阵：先线性变换，后平移

绕轴旋转

![1599279585714](GAMES101.assets\1599279585714.png)



$$ R_{xyz}(alpha, belta, gama) = R_x(alpha)R_y(belta)R_z(gama)$$

欧拉角

根据过原点的轴旋转

罗德里格斯旋转公式Rodrigues' Rotation Formula（P4 min16）

![1599279929454](GAMES101.assets\1599279929454.png)

根据任意轴旋转，I单位矩阵

3D变换拆分：

1. 先移动到原点
2. 绕原点旋转做变换
3. 再移动回去

**问题：N矩阵与叉乘的关系？**

**四元数：为了旋转与旋转的差值**

### Viewing变换（观测变换）

配置环境

弹幕推荐-MIT线性代数入门、矩阵分析、高代、泛函

![1599226142175](GAMES101.assets\1599226142175.png)

国内多分为视图和投影

#### View / Camera 变换

(min22)

拍照

- 人物安排好位置（模型变换 Model transformation）
- 放好摄像机（视图变换 View transformation）
- 茄子！（投影变换 Projection transformation）

MVP变换



相机的确定因素

- 位置 Position $e$
- 朝向 Look-at/ gaze direction $g$
- 上方向 Up direction $t$



相机与物体的相对位置不变化

- 移动相机和移动物体的操作是一样的，不影响最终“成像”

所以约定将摄像机移动到原点、向-z方向看，其他物体和相机一样变换。

为了方便操作和计算



先平移后旋转
$$
M_{view} = R_{view}T_{view}
$$
平移矩阵
$$
T_{view} = 
\begin{vmatrix}
1 & 0 & 0 & -x_e\\
0 & 1 & 0 & -y_e\\
0 & 0 & 1 & -z_e\\
0 & 0 & 0 & 1\\
\end{vmatrix}
$$
旋转矩阵

旋转$g$到-z方向，$t$到y，$(g * t)$到x

直接旋转不好写

用目标xyz旋转到相机方向，及求逆是比较方便的
$$
R_{view}^{-1} =
\begin{vmatrix}
x_{g \times t}
...
\end{vmatrix}
$$
![1599282516391](GAMES101.assets\1599282516391.png)

求逆

旋转矩阵是正交矩阵

逆 = 转置



ModelView变换

- 模型变换，视图变换，其实作用的对象都是模型（让相机位于原点|变换物体到相机坐标系，一回事）



**【F3D】**

引擎的摄像机只设置眼睛位置和看向的位置

相当于只有g，那up和t是怎么来的【up是设置的，但与g不垂直，up与g计算出right，再由g和right计算出up】

#### Projection 变换

(min39)

Orthographic projection 正交投影

Perspective projection 透视投影（近大远小）

![1599283030622](GAMES101.assets\1599283030622.png)

透视投影：四棱锥

正交投影：相当于相机无限远

##### 正交投影

去掉z方向便成了图

长方体
$$
[l, r]\times[b, t]\times[f, n]\\
(-z : far < near)
$$
变成标准立方体（canonical cude）

1. 长方体平移中心到原点
2. 缩放到[-1, 1]

GL中用左手坐标系（看向z正方向 far > near）

![1599283573874](GAMES101.assets\1599283573874.png)

会拉伸，但最后会有视口变换，再一次拉伸

##### 透视投影

欧式几何，平行永不相交；透视中会相交

齐次坐标 $(x, y, z, 1)$

左右同乘以k（k！= 0），仍然表示的是同一个点，乘以z（后边用） 
$$
(x, y, z, 1)\\
(kx, ky, kz, k(k != 0))\\
(xz, yz, z^2, z(z != 0))
$$
Frustum （视锥）

**理解透视投影**

- 把远平面挤压成和近平面一样大
- 变成正交投影

规定

- 近平面不会变
- 远平面中心不变，z不变

$$
M_{ortho}M_{persp -> ortho}
$$



![1599284610761](GAMES101.assets\1599284610761.png)

P4：min65
$$
y^` = n / z * y\\
x^` = n / z * x\\
\begin{vmatrix}
x\\ y\\ z \\1
\end{vmatrix}
=> 
\begin{vmatrix}
n / z * y\\
n / z * x\\
unknown\\
1
\end{vmatrix}
==
\begin{vmatrix}
n * y\\
n * x\\
unknown\\
z
\end{vmatrix}
$$
计算转换矩阵
$$
M_{persp -> ortho} =
\begin{vmatrix}
1 & 1\\
n * x\\
unknown\\
z
\end{vmatrix}
$$
课堂问题

**对于近平面和原平面中间的任何一个点，经过挤压变化后是否有变化，变大了/变小了**

# P5 Rasterization 1（Triangles）

作业说明：

- 基本要求
- 提升要求



观测矩阵之后，到[-1, 1]经典立方体，接下来做：光栅化，显示

## 变换

### View / Camera 变换

透视投影如何定义一个视锥

![1599312098691](GAMES101.assets\1599312098691.png)

长宽比 Aspect ratio = width / height

可视角度 fovY（filed of View）
$$
tan(fovY / 2) = t / |n|
$$
MVP之后，所有物体到-1到1的三次方

## 光栅化

画到屏幕上

屏幕（典型的光栅成像设备）：

- 像素的数组

raster光栅化

像素（Pixel、picture element）

屏幕空间定义，以屏幕左下角为原点，右x上y，表示每个点的坐标

从立方体到屏幕

先不管z，xy进行缩放和平移
$$
M_{viewport} =
\begin{vmatrix}
width/2 & 0 & 0 & width/2 \\
0 & height/2 & 0 & height/2 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{vmatrix}
$$
视口变换

变换结束后，需要把变换后的图像打散成像素

### Different raster displays

光栅呈像显示设备

- 示波器

- 阴极射线管

- CRT

  ![1599313710140](GAMES101.assets\1599313710140.png)

  快速扫描，呈现一副画面

  **隔行扫描**（高速运动的有交叉影）

- 现代显示，显示的区域为内存中的一块区域

- 平板显示设备，LCD，视网膜屏幕、LOLED

  LCD液晶显示器，**两个光栅**，大学物理光的波动

  LED发光二极管

- 电子墨水屏，控制墨水翻转（刷新率低）

### Rastering a triangle

三角形优势

- 是基础的多边形
- 多边形都可以拆成三角形
- 内部是一个平面
- 内外部定义清晰
- 只要定义三个顶点的三个属性，可以做成一个渐变的效果



三角形到像素

简单做法：

- 采样（Sampling）、把一个函数离散化的过程

利用**像素中心**对屏幕进行采样，屏幕上**每个像素的中心是否在三角形内**

- 计算过程中利用**叉乘**，看点在线的**左侧/右侧**

边界上的点（要么自己规定，要么不管）

- 不做处理
- 或特殊处理

如上，画一个三角形，遍历所有区域，没有必要

三角形的**包围盒**

min(x), max(x), min(y), max(y)

AABB 轴向包围盒

光栅化加速方法，还有（每行找最左和最后）

![1599315372404](GAMES101.assets\1599315372404.png)

在三角形细长斜45°的情况下，快

### 真实的屏幕像素

右边Bear Pattern，绿色多，人眼对绿色敏感

![1599315460970](GAMES101.assets\1599315460970.png)

打印机-减色系统

光栅化后，锯齿（Jaggies， Alias）

信号走样问题（Aliasing）

抗锯齿、反走样

# P6 Rasterization 2（Antialiasing and Z-Buffering）

## AntiAliasing

Aliasing走样

涉及信号处理的知识

### Sampleing theory

光栅化：采样

照片：也是采样

采样不仅可以发生在同一幅画面，也可以发生在不同的时间，比如视频和动画，离散的30或24次



Samping Artifacts 采样的问题/瑕疵

- 锯齿
- 摩尔纹
- 车轮反转效应

走样：本质是信号变化的速度太快了，采样的速度跟不上变化的速度

反走样的方案：在采样之前做一次**模糊(滤波 Pre-Filter)**，再采样

如果先采样后模糊（Blurred Aliasing），不能达到反走样的结果

#### 频率、频域

$$
cos 2\pi fx
$$

P6 min16

傅里叶级数展开

傅里叶变换

![1599394302673](GAMES101.assets\1599394302673.png)

最下面的一条采样的速度跟不上变化的速度

根据频率分析走样

滤波(Filtering = Getting rid of...)

P6 min30

傅里叶变换可以将函数从时域变换到频域

图片的频率为啥中间会有两条线

- 图片不是周期性重复的
- 假设图片周围重复图片本身，而示例图左右两侧挨着

图片

- 高通滤波-只剩下高频

  去掉图像的高频信息，图像上剩下的是边界（发生突变的位置）

- 低通滤波后，去掉边界，边界变模糊

- 去掉高通、去掉低通：实验

**《数字图像处理》**

以上是经典的图像操作，现在更多的图像操作是通过机器学习进行

（是否可以弄出来铅笔效果、动画质感的效果？）

#### Filtering

Filtering（滤波） = Convolution（卷积） = Averaging（平均）

滤波**窗口**移动，对信号的采样进行**卷积**操作（加权**平均**）

操作一

每个像素周围卷积/平均操作

实域上的卷积

操作二

将图片先转换成频域，频域上进行滤波

**时域上的卷积 = 频域的乘积**

min45【疑】

采样实际上是在重复频域上的内容

冲激函数

采样稀疏，频谱上密集

如果提高分辨率，锯齿的现象约不严重

【疑问】**频谱上的搬移**？

#### 走样

**走样**：频谱上的搬移密集，丢失信息

解决方案：

1. 提高采样率

   受制于物理限制

2. 反走样

反走样重定义：去掉高频信号后，再采样



如何把一个三角形变模糊：对图像的每个像素卷积操作（用一定大小的滤波器）

像素内部的值平均（求三角形在这个像素中的覆盖范围）

### Antialiasing in practice

对于单个像素中三角形的覆盖范围不好计算，msaa在单个像素中用更多的采样来进行反走样

#### MSAA（MultiSampleAntiAliasing）

一个像素被划分成多个点采样，像素的颜色由这几个点采样结果的平均

MSAA主要是进行模糊，同时隐含了采样

缺点/代价：

- 增加了计算量

工业上，使用优化的方法

- 使用更加有效的图案来采样每个像素

其他的抗锯齿方案：

- FXAA（Fast Approximate AA）先生成图，再将边界换成没锯齿的
- TAA（Temporal AA） 时间线上采用不一样的采样，利用上一帧的样本

超分辨率

- 问题：拉伸图片，出现锯齿
- 方案：DLSS深度学习，补充损失的信息

# P7 Shading 1  (Illumination, Shading and Graphics Pipeline)

## Visibility / occlusion

### Z-buffer

光照

深度测试

图形管线



问题：

- 三角形的可见性、遮挡

方案：

- z-buffer



画家算法

- 先光栅化远处的
- 后光栅化近处的，覆盖以前的

如果三个三角形有互相遮挡关系，那不能对他们的深度进行排序

z-buffer算法

- 每个像素记录最小深度
- 同步生成一个**深度图（Depth Buffer）**

最初记录是无限远的，每个像素内记录最浅深度，在遍历过程中更新为最小的深度信息

O（n）复杂度，没有进行排序

对于深度相同的情况（z-fighting）

z-buffer处理不了透明物体的处理

【疑】

问题：水是怎么表示的？

学习甜甜圈的blender制作，做完用软渲染搞出来

## Shading

引入明暗和颜色不同

课程中**对不同的物体应用不同的颜色**，不同的材质与环境的作用不相同

### Blinn-Phong 反射模型

观察

- 高光（Specular highlights）

- 漫反射（Diffuse reflection）

- 环境光（Ambient lighting）

定义（shading point）：

- 法线方向
- 观测方向
- 光照方向

- 表面参数/属性：颜色，shininess（反射光的能力）

shading != shadow

着色具有局部性，不考虑其他物体的存在，不会生成阴影

#### 漫反射

漫反射项和v没有关系

1. 多少能量被吸收 $cos(\theta)$
2. 多少能力到达 $I/r^2$
3. 为什么有颜色 $k_d$

$$
k_d * (I / r^2)* max(0, cos(\theta))
$$



不同角度着色颜色不一样

类比地球上的春夏秋冬，相同的单位面积接收到的能量不一样（光照方向和法线方向的夹角余弦作为描述）



点光源

- 每一个时刻集中在某一个球壳上
- 能量守恒，时间推移，球壳越大，球壳上每个点上的能量约少

到达shading point的能量$I/r^2$

对于同一个点，不同的观察角度是一致的



为什么有颜色

kd，反射能量的能力，如果是1，表示不吸收，最亮，0表示黑色，最暗；延申到三维，rgb，即吸收颜色的能力。

# P8 Shading 2 (Shading, Pipeline and Texture Mapping)

## Shading

### Blinn-Phong 反射模型

#### 高光 Specular

反射的方向接近镜面反射

观察方向越接近镜面反射方向，高光越明显

Blinn-Phong模型，高光程度，实际是半程向量与法线方向的夹角是否接近
$$
L_s = k_s * (I / r^2)* max(0, cos(\alpha))^p
$$


![1599455902405](GAMES101.assets\1599455902405.png)

Blinn-Phong是phong模型的改进，是否接近用n和v

p是为了对cos进行幂运算，集中高光

一般100-200

#### 环境光

简化假设：假设每一个点从四面八方射进去，一个值
$$
L_a = k_a I_a
$$
进阶：全局光照

#### 着色点颜色

blinn-phong模型的结果

![1599456386710](GAMES101.assets\1599456386710.png)

为啥入射光的距离有影响，而观测方向没有影响

- 需要redience inredience？知识

### 着色频率

Flat shading

- 一个三角形一个法线

- 顶点少的时候，平面一次着色，效果不好

Gouraud shading

- 求顶点法线

- 顶点着色，中间插值

Phong shading

- 求顶点法线

- 每一个像素上插值发现方向
- 每个像素着色



着色效果与顶点频率也有关系，频率高时，flat shading效果也好



顶点的法线：

共用顶点的三角形法线求平均，由各个三角形的面积为权重平均（记得归一化）

三角形内每个像素的法线方向，插值得到，通过重心坐标求。

### 渲染管线/实时渲染管线

空间中的点

投影到屏幕上

三角形

光栅化

着色

图

![1599457551652](GAMES101.assets\1599457551652.png)

以上过程在硬件中写好的，显卡

主要过程

1. 投影、变换
2. 光栅化（采样、深度测试）
3. 着色



着色在**顶点着色、像素着色**都可能有

shader是在硬件上执行的语言，控制像素和顶点着色

写通用的代码，每个顶点或像素执行一次

纹理映射，三角形内显示纹理



顶点着色器：Vertex Shader 顶点着色

像素/片段着色器：Fragment Shader 像素着色 Pixel Shader

GLSL：opengl的着色语言



目前为止，图形管线的部分大概做什么就有了

可以接着去用图形API（opengl）去写，或者写shader实现有意思的效果



推荐 **shadertoy** 在线写shader

Inigo Quilez
Procedurally modeled, 800 line shader.
http://shadertoy.com/view/ld3Gz2

Inigo Quilez, https://youtu.be/XuSnLbB1j6E



GPU

图形管线的硬件实现

还有其他类型的着色器

Geometry shader

Comput shader

GPU高度并行的计算

### 纹理映射 Texture Mapping

三角形内部填充了图片，其实对于漫反射项来说
$$
L_d = k_d * (I / r^2) * (n \dot I)
$$
漫反射系数$k_d$不一样

用图片定义任何一个点的不同属性

3维物体表面是2D，图蒙在3D物体表面，纹理映射

3D物理上的任何一个三角形的点对应到2D

展开成平面

保证展开的三角形尽可能小，尽可能保证原衔接方式；研究方向：参数化

纹理上的坐标系：UV

UV的范围一般认为是（0， 1），正方形和矩形都是

纹理可以重复使用（纹理本身设计左右衔接，上下衔接）（tiled）（纹理无缝衔接）

从纹理上的UV对应三角形的UV（用到重心坐标）

不同的材质，不同的着色方法

# P9 Shading 3 (Texture Mapping cont.)

## Shading

### 重心坐标

作业3将发布

Barycentric Coodinate

重心坐标

插值：三角形内部属性根据顶点属性插值，得到平滑的过度

- 纹理坐标
- 颜色
- 法线



任意一个三角形平面的点
$$
(x, y) = \alpha A + \beta B + \gamma C\\
\alpha + \beta + \gamma = 1
$$
任意一个三角形内部的点
$$
满足 三个参数 > 0
$$
根据定义
$$
A: (1, 0, 0)\\
B: (0, 1, 0)\\
C: (0, 0, 1)
$$
三角形内部一个点O的坐标，根据面积计算，点A对面的三角形为a
$$
a = S_{OBC}\\
b = S_{OAC}\\
c = S_{OAB}\\
\alpha = a / (a + b + c)
$$
三角形重心$(1 / 3, 1 / 3, 1 / 3)$

插值的属性利用重心坐标为参数
$$
V = \alpha V_A + \beta V_B + \gamma V_C
$$
计算参数
$$
\alpha = \frac
{-(x - x_B)(y_C - y_B) + (y - y_B)(x_C - x_B)}
{-(x_A - x_B)(y_C - y_B) + (y_A - y_B)(x_C - x_B)}\\
\beta = \frac
{-(x - x_C)(y_A - y_C) + (y - y_C)(x_A - x_C)}
{-(x_A - x_C)(y_A - y_C) + (y_A - y_C)(x_A - x_C)}\\
\gamma = 1 - \alpha - \beta
$$


**注：**在投影变换下不能保证重心坐标不发生变化，投影中会拉伸

插值三维空间属性应该用三维的坐标，为了不发生拉伸，不用投影之后的三角形的坐标

比如深度的插值是需要利用三维空间的坐标做插值

### 纹理应用

利用重心坐标插值得到每个点的UV
$$
texcolor = texture.sample(u, v)
$$
以上处理可能出现的问题

#### 纹理放大

如果贴图很小，三角形大，对应纹理UV不是整数

texel：纹理上的一个像素，纹素

直接对应，多个像素对应同一个UV，即多个pixel对应同一个texel

**Nearest** 四舍五入取UV，即取最近的点



**Bilinear** 双线性插值

![image-20210701230924597](C:\liujuanjuan\github-plainliu\Games\Games101\GAMES101.assets\image-20210701230924597.png)

取周围四个点的颜色综合考虑
$$
u_0 = lerp(s, u_{00}, u_{10})\\
u_1 = lerp(s, u_{01}, u_{11})\\
f(x, y) = lerp(t, u_0, u_1)
$$
先横后竖和先竖后横

**Bicubic** 取周围16个点

#### 纹理太大

近处锯齿、远处摩尔纹

走样问题

解决：

1. 增加采样点，取一定范围的平均值（超采样太费）
2. 提前采样做平均，mipmap



mipmap方案：

避免采样，mipmap，做一张低分辨率的图

mipMap允许我们做范围查询（可以查平均值、最大值、最小值...这里我们做平均值查询），近似的、正方形的

- Fast 快速
- approx 近似的，不准的
- square 只能做正方形的范围查询

分辨率降低，不同的层，一次砍一半，到最后成为一个像素

Image pyramid 图像金字塔

存储量增加：级数求和 4/3，存储了多了1/3

应用时，怎么知道是第几层【min52】：

- 上面的像素和右边的像素，三个点在UV上的点
- 原像素正方形区域对应到纹理上的边长L
- 得到

$$
log_2 L
$$

不同层的接触线，防止接缝现象出现，**层之间做插值**

三线性插值 Trilinear Interpolation

- 两层内进行双线性插值
- 两层间进行线性插值
- 得到连续的插值结果

mipmap局限性 OverBlur

- 远处的位置，过分模糊

解决方案：**各向异性过滤**

mipmap实际上是正方形的压缩，ripmap各向异性过滤允许对矩形的查询查找【min62】

斜的细长不规则贴图（开销是原图的三倍，只费显存，开多少层关系不大，对帧率几乎没有影响）

**EWA过滤**：多次查询

# P10 Geometry 1(Introduction)

## Shading

### 应用纹理

#### 反射-环境光照

纹理：本身是一张图

mipmap

现代GPU，可以理解成一块内存区域，在这块区域上做范围查询；纹理理解成数据，而不仅仅是一张贴图

其他应用，环境光贴图，做出反射的效果

认为环境光是无限远的，不会有任何深度

Spherical Environment Map：存在上下拉伸的问题

弹幕-墨卡托投影

Cube Map 包围盒

问题：

环境贴图是对于一个模型而言的，还是对于一个环境只有一个贴图？

那这个贴图，不同的物体反射的是否会是一样的呢



图形中常用模型

- 犹他茶壶
- 斯坦福兔子、龙
- 康奈尔盒子



Sky Box环境光

- Sphere Map
- Cube Map

#### 凹凸贴图/法线贴图

定义每个位置的**相对高度** -> 法线变化

不改变几何形体的前提下，对法线扰动



Bump Mapping

计算：

1D：微分近似得到切线，垂直得到法线

2D：梯度，du、dv得到两个切线，$(-dp/du, -dp/dv, 1)$归一化

假设局部坐标系，stn，认为发现在stn中001



Displacement mapping

位移贴图：与凹凸贴图一致，真的移动顶点的位置，需要三角形很细

DirectX：动态的曲面细分，根据需要细分三角形

### 其他应用

纹理上面的是二维的，我们也可以做三维的

三维：定义空间中的任何一个点

三维噪声（Perlin noise）



纹理也可以用来记录已经算好的信息（比如阴影环境光遮蔽）



3D的纹理，利用在体积的渲染中

## 几何 Geometry

P10 min38

### 几何表示

隐式几何：只告诉约束关系

- 面上有哪些点，困难
- 判断点在不在面上，简单

表示方式：

- 数学公式

- CSG(Constructive Solid Geometry) 几何Bool运算

- 距离函数 Distance Functions

  函数表示、水平集表示

  等高线

  三维水平集运用，比如描述密度

- 分形（自相似、递归）（broccoli-romanasco...）



显式几何：

- 三角形表示
- 参数映射
- 判断点在点在内还是外，困难

# P11 Geometry 2(Curves and Surfaces)

曲线和曲面

## 几何 Geometry

### 几何显式表示

点云：密度低效果不好

- 扫描的基础数据

多边形面

- 主要三角形、四角形面（最广泛）
- obj (Wavefront Object File)
  - v顶点
  - vn法线
  - vt纹理坐标
  - f三角形 顶点、纹理坐标、法线

### 曲线

应用

- 摄像机运动曲线
- 字体

#### 贝塞尔曲线

贝塞尔曲线

- t找位置
- 递归每条线段上找，直到最后一个

控制点

代数表示

性质

- 起点b0，终点b1
- 仿射变换不变性
- 凸包性质

Piecewise Bezier Curves

- 每四个控制点定义一条贝塞尔曲线
- 衔接光滑：衔接的切线**共线、等距**，连续性

连续性

- 曲线连续C0连续，终点连起点
- C1连续**共线、等距**
- C2连续

#### 样条

Spline曲线 样条

- B样条 伯恩斯坦多项式 基函数样条

### 曲面

贝塞尔曲面

两个方向上分别贝塞尔曲线

### 几何处理

网格细分

网格简化

网格规范化

# P12 Geometry 3

## 几何

网格细分

网格简化

网格规范化

### 细分 Subdivision

引入更多三角形，两部操作

- 增加数量
- 变化顶点位置

Loop Subdivision

1. 增加顶点数量，1变4三角形

2. 顶点

   - 新的顶点（ 3/8 *(A+B) + 1/8 * (C+D) ）

   - 旧的顶点

     ![image-20210801185354335](C:\liujuanjuan\github-plainliu\Games\Games101\GAMES101.assets\image-20210801185354335.png)

Catmull-Clark Subdivision

- 非四边形面
- 奇异点

细分

- 增加顶点：一次细分后，引入奇异点，全变成四边形
- 变化顶点

### 简化

去掉一些三角形，性能

层次结构的几何

Eage Collapse

- 二次误差度量【疑】（到原本的几个面的距离平方和最小）

- 对于每个边的坍缩，排序，操作最小，然后更新相关的数据

  【优先队列】数据结构

- 

### 规范化

正规化、正则化

## 光线追踪之前

min50

阴影

- Shadow Mapping 图像空间的做法，硬阴影

  1. 光源出发记录深度
  2. 实际从相机出发投影回光源，看是否遮挡

- Shadow Map问题

  有分辨率问题

bias处理精度问题

软阴影（光源有一定的大小）

- 阴影
- 半影

# P13 Ray Tracing 1(Whitted-Style Ray Tracing)

光栅化的问题

-  全局效果，软阴影
- Glossy 反射
- 间接光照，弹射不止一次

光线追踪

- 慢，一般离线用
- 质量高

光线

- 直线传播
- 光线交叉不影响
- 光线可逆性

RayCasting

- 像素，眼睛开始创建光线
- 找到最近的交点
- 着色

Whitted最近的点，可能反射的同时进行折射，再加起来

primary ray

光线的定义

- 起点+t方向

光线与不同物体的求交

- 球，解方程，求解个数

推广到一般

f(o + td) = 0, 求t

光线和三角形的交点

plane: (p - p') ·N = 0

ray: o + td

ray带入plane公式，解一次方程，得到交点

然后判断交点是否在三角形内

Moller Trumbore 算法

- 一次计算出交点是否在平面内

- 方程

  ![image-20210801224056791](C:\liujuanjuan\github-plainliu\Games\Games101\GAMES101.assets\image-20210801224056791.png)

光线和表面的求交加速

- 慢：pixel * 三角形个数
- 加速方式：包围盒（长方体、球体）

长方体

- 三个对面行程的交集
- AABB 轴对齐包围盒
- 光线和aabb的求交（三个对面的t区间，求交集）


# P14 Ray Tracing 2 ( Acceleration & Radiometry)

GTC会议

- DLSS 2.0
- RTXGI

AABB如何加速光线追踪

- Uniform grids
- Spatial partions

## Uniform Spatial Patitions(Grids)



1. 场景包围盒
2. 划分格子
3. 看哪个格子有物体（物体表面）
4. 光线和盒子相交且盒子中有物体，再判断内部

如何看光线与哪些盒子相交（）

- 接下来的

格子的数量，需要平衡

对于场景比较平整的情况，比较好；但对于场景分布不均匀的情况，效果不好

## Spatial Partitions

空间划分

应用：

- Oct-Tree 八叉树，每个子节点分成8块
- **KD-Tree** 永远砍一刀，xyz轮着划分
- BSP-Tree 每次选择一个方向切开

KD-Tree

- 每个轴划分
- 划分位置，可自定义
- 实际物体只存储在叶子节点上

一层一层判断是否相交，类似二分查找

问题：

- 不太好写三角形与AABB的相交（三角形比AABB大的情况）；
- 一个物体与多个盒子相交的情况

解决：BVH，从物体出发，而不是从空间出发

## Object Partitions & Bounding Volume Hierarchy(BVH)

非常广泛的应用

划分物体，分别求AABB，把三角形划分成一棵树

引起问题：包围盒相交

划分

- 每次划分沿着长轴划分
- 取中间的物体划分（不需要排序，求第n，快速选择的启发，On复杂度）

## 辐射度量学

定义光，精确表示光

- 描述光照
- 定义了光的一些量 Radiant ……
- 物理正确的光计算

Radiant flux、intensity、iradiance、radiance

学习方法

- WHY 为什么学
- WHAT 学的是什么
- HOW 怎么解决、运作（最不重要的）

### Radiant Energy and Flux

**单位时间**的能量 功率 flux power （lm = lumen 流明）

Flux 代表了光的亮度(power)

### Radiant Intensity

光源四面八方发射的能量

单位立体角的能量（单位：candela）

立体角：面积 / 半径^2

微分立体角：sin d d phi

**方向上对应的强度**

均匀的光线，微分立体角上的光的强度，Flux / 4 pi

power per solid **angle**

# P15 Ray Tracing 3(Light Transport & Global Illumination)

## 辐射度量学

### Irradiance

物体表面接收的能量(lux)

power per unit **area**

【Intensity是“角”投射出去的光强度，Irradiance是单位面积接受能量】

面必须和光线**垂直**（参考bulinn - phong，兰伯特；地球上冬夏）

光线在传播过程中r^2衰减：能量没变，越远面积越大，Intensity没有衰减，Irradiance在衰减。

### Radiance

光线在传播中如何度量能量

单位立体角、单位面积的能量

per unit solid angle per projected unit area.

![image-20210822101447137](C:\liujuanjuan\github-plainliu\Games\Games101\GAMES101.assets\image-20210822101447137.png)

单位面积可能给各个方向辐射能量，考虑theta

Radiance: Irradiance per solid angle

Radiance: Intensity per projected unit area

Irradiance和Radiance图形学中用的多

![image-20210822103119046](C:\liujuanjuan\github-plainliu\Games\Games101\GAMES101.assets\image-20210822103119046.png)

## BRDF 【29min-46min】

Bidirectional Reflectance Distribution Function

双向反射分布函数

反射（光线吸收再投射出去）

BRDGF：从某个方向进来的能量，到各个方向反射的能量的分布

理解

1. 光反射出去
2. 光吸收再反射

入射的irradiance转换成能量辐射radiance的分布

双向：

- 对于某观测方向，加起来所有方向贡献的光线的radiance

反射方程

渲染方程

- 物体自己产生的光 + 反射的光

解渲染方程 P16

多光源，相加

面光源，积分光源

L = E + KL 【50min】

中间推算过程【疑】

L = E + KE + K^2E + ...

全局光照

光栅化能告诉的 L = E + KE，后面的部分不好做

通过ray tracing做后面的部分好做

## 概率论回顾

随机变量

随机变量的分布

概率

概率和为1

期望

连续情况下概率分布

- 概率：积分出的坐标系中的面积

概率密度函数 pdf

Y = f(X)

引入蒙特卡洛积分

# P16 Ray Tracing 4 (Monte Carlo Path Tracing)

蒙特卡洛路径追踪

## 蒙特卡洛积分 【9min】

计算定积分

用一种数值方法来积分

黎曼积分，均分，每一份中间点算高度

蒙特卡洛，随机取一个点，算长方形面积，多采样，算出值；多次取点，求平均。

## Path Tracing【20min】

Whitted风格的问题

1. glossy的效果有问题
2. 漫反射面间接反射到漫反射的效果没有考虑

康奈尔盒子

Whitted风格有问题，渲染方程没有问题

解渲染方程

用蒙特卡洛积分方法求解

采样方式：最简单可以用平均采样，pdf = 1 / 2pi

直接光，计算渲染方程时，选中的采样方向与光源不相交，则为0，否则返回光的能量。

算间接光：

- hit到物体的情况下，将物体直接光照的结果作为光源计算
- 得到递归的算法

问题：

1. 递归，光线的数量会爆炸 N^bounces，只有N = 1的情况下不会爆炸

   N = 1的情况下做蒙特卡洛积分，叫路径追踪

   N != 1的情况下，现在少用了，叫分布式的ray tracing

2. 递归的问题，没有停止

   计算机计算次数少停下来，能量就会有损失

   解决方法：RR 俄罗斯轮盘赌 Russian Roulette【54min】

   E = P * (Lo / P) + (1 - P) * 0 = Lo

samples per pixel SPP

采样光源【1h1min】

- 从像素出发采样光源，靠运气
- 变换思路，从光源采样
- 把渲染方程写成对光源上的积分
- 避免采样率不够的时候采样不到光源的问题

拆成两部分

- 光源的贡献
  - 对光源采样
- 间接光的贡献
  - 用RR

采样光源的问题

- 判断遮挡问题

对于路径追踪，点光源不好做

path tracing 可以做到真实的照片级别的结果

以前 Ray tracing是Whitted风格的

现在是一系列光线传播方法的集合

其他问题

- 针对特定的采样函数，重要性采样
- 好的随机数，低差异序列
- 结合采样立体角和光源，multiple imp. sampling
- 像素中不同结果的合并，加权
- radiance到颜色，需要做gamma 矫正
- ……

# P17 Materials and Appearances

材质的外观

- 光线和材质的作用结果

weta menucu？渲染器

BRDF决定了材质

材质 = BRDF

【10min】

漫反射

完全不吸收的材质， p /  pi

diffuse

**Glossy**

反射方向的计算

- wi + wo = 2 cos n
- 方位角（参考立体角）

**镜面**

BRDF 集中在反射方向，写正确不易

**折射**

几何光学

caustics

折射率

计算折射方向

全反射现象（水中到空气）

BTDF

菲涅尔项

导体和绝缘体

## Macrofacet Material

【43min】

微表面模型

从远处看：平的粗糙的

近处：高低起伏的

材质和几何

用微表面法线分布描述表面的粗糙程度

grazing angle 容易挡，G项修正这个问题

优点：描述的能力强

PBR使用微表面模型

## BRDFs 区分材质的方式

各项同性材质

- 微表面分布均匀

各项异性材质

- 表面的法线有分布

VRay 渲染器

BRDF的性质

- 非负

- 线性性质（不同的计算结果可以相加）

- 可逆性（交换wi wo，结果一样）

- 能量守恒

- 各项同性：降维

  各向异性

测量BRDF

- 枚举所有出射方向、入射方向，得到BRDF
- 存储、压缩

库 MERL BRDF Database



# ShaderToy

https://www.shadertoy.com/view/stjXDD 国旗

https://www.shadertoy.com/view/NtlSDs

# 群友推荐资料

[Graphics Mesh Resources（图形学模型资源）? - 知乎 (zhihu.com)](https://www.zhihu.com/question/49226908)

https://github.com/ACskyline/Wave-Particles-with-Interactive-Vortices



# PPM浏览器

openseeit

WPS图片



# 作业

## 0 环境

### 搭建配置

下载虚拟机-VirtualBox-6.1.4-136177-Win.exe

- https://download.virtualbox.org/virtualbox/6.1.4/VirtualBox-6.1.4-136177-Win.exe
- 安装
  - 中间断开网络重置
  - 自动安装Oracle 通用串行总线控制器

下载虚拟硬盘-**GAMES101_Ubuntu 18.04.2 (64bit).vdi**

- rar解压到vdi

配置虚拟机

- 打开VirtualBox
- 新建Linux|Ubuntu-64，内存2G
  - 坑：只能选择32位的（检查时候允许虚拟化，BIOS设置B450-F7-OC-CPU特征-SVM允许）
- 选择使用已有的虚拟硬盘文件（vdi）
- 点击创建
- 选择虚拟机，启动
- 密码 Ilovegraphics

安装Guest Additions

- 进入系统
- 菜单-设备-安装增强功能
  - 自己电脑安装正常
  - 公司电脑安装跳出system问题
- 如果失败，`ctrl alt t`终端安装

导入作业/导出作业

- 菜单-设备-拖放-双向

  - 设置后不能拖放

  - 关闭虚拟机的情况下，右键设置-存储-控制器：

    - SATA- 勾选使用主机输入输出缓存

    - 选中叶子的虚拟机，勾选固态驱动器

  - 重新启动

- 直接拖入系统

  - 也可以采用共享文件夹的方式
  - 双向拷贝不好用，属于box的版本bug

- VSCode查看/编辑

作业进行

- 在VSCode编辑

- 命令行编译运行

  ```sh
  // main目录
  mkdir build
  cd build
  cmake ..
  make
  ./Transformation
  ```

  

C++ https://devdocs.io/cpp/

头文件

函数体main入口

C++的三角函数运算使用弧度制（cmath）

常见错误

Eigen库（线性代数运算库）



### 环境相关

虚拟机中安装了git

设置ssh

然后拉取私有仓库，讲代码提交到私有库中，方便同步



ssh设置

```sh
git config --global user.name "Games101"

git config --global user.email "xxxx@qq.com"
ssh-keygen -t rsa -C "xxxx@qq.com"
```

### 代码

```
#include<eigen3>
```



```
Eigen::Vector3f v(1.0f, ...)
重写了+ * <<
Matrix3f i
i << 1.0, 2.0, ...; // 9个数
```

线性代数库

https://eigen.tuxfamily.org/dox/group__TutorialMatrixArithmetic.html

向量相乘

默认是列向量，所以第一个值需要转置

```
v.transpose() * w
```

作业

trans * rotate * P


$$
M_t M_r P = \begin{vmatrix}
1 & 0 & 0\\
0 & 1 & 0\\
0 & 0 & 1
\end{vmatrix}
\begin{vmatrix}
cos\theta & -sin\theta & 0\\
sin\theta & cos\theta & 0\\
0 & 0 & 1
\end{vmatrix}
$$

## 1

std::stof

namespace

列表初始化



## TODO：推导重心坐标公式

