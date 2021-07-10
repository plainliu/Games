# 1 Intro

## Intro

黑化的PPT

YLQ：UCSB

团结一致抗击疫情，GAMES101

助教



实时 30FPS（Interactive小于30 可接受）

高质量（真实感）

渲染 3D场景计算成Image



话题

1. 阴影
2. 全局光照
3. 物理着色
4. 光追



聊天

- 物理模拟
- 科学与技术（科学不难、技术难）



Cherry Picking

神经网络 Neural Rendering



会用OpenGL Shader，但不会教怎么用OpenGL



UE 学习曲线陡



（What、Why、How）



怎么学

- 科学和技术是不等同的



不论做啥，多写代码，把自己培养成好的工程师



技术上：工业界领先学术界

版权限制，封闭的系统



Practice make perfect



Why

- AWESOME



要求

- 兴趣
- 基础、微积分
- GLSL
- WebGL



不需要好显卡

网站 Games202



Real-Time Rendering （很入门，不用买，也不相关）

作业 GLSL + JS + WebGL



DDL: 11:59 AoE

提交作业的网站 (http://smartchair.org/GAMES202)



不要求IDE

不要上传网络作业



参考答案（参考图）

证书

## 01 Lecture

Motivation

- 照片级别真实感
- 离线渲染可以做到，但实时难

Evolution of real-time rendering

- 20年前
- 20-10：shader
- Sunning Graphics；VR应用；动画、电影
- 可编程的渲染管线

技术和算法成就

- 预计算
- Interactive RT

# P2 Recap of CG Basics

作业0

网站信息



基本知识

## GPU渲染管线

3D顶点和连接关系

（顶点处理）

屏幕上的点，屏幕上的三角形

（光栅化）【Z-Buffer】

Fragments

（着色）【着色模型、Blin-Phong】

OutPut

纹理映射、插值

## OpenGL

一系列API

CPU端写的，调用GPU执行

跨平台

DX只能在Win上运行，Vulkan比较新

碎片化的

C风格的代码

以前不能Debug

用OpenGL是因为快（GPU运行）



> easal（画架）

OpenGL

VBO 存储模型的信息

framebuffer（相当于画架）垂直同步、双重缓冲

shading 顶点着色、片段着色



OpenGL 状态机模型（给定怎么运行）

## GLSL

类C

上古时代在GPU上写汇编

CG from NVIDIA、HLSL



写Shader

编译

attach

link

Use



attribute顶点变量

uniform 全局变量

varying 变量



调试shader

- Nsight调试 （Nsight Graphics）
- RenderDoc
- 这两个不一定能
- 个人debug方法
  - Print：展示成颜色，观察图片像素

## The Rendering Equation

BRDF

环境光

全局光照

- 直接光照
- 间接光（一次间接One-bounce）

## Calculus

微积分

穿插讲

## 作业 0

环境搭建注意事项

VSCode插件打开的本地服务器是以当前工作目录为根启动的，所以workspace需要先打开到作业0，再启动服务。

JS ES6支持 class关键字。

异步串行化：await

浏览器插件可视化输出进行调试**Spector.js**



阅读代码

- 加载mtl文件，得到mtl信息
- 加载obj：将mtl信息设置后，加载obj
- 便利obj，每个子mesh geo，创建**Mesh对象**和**材质对象（创建shader）**
- MeshRender进行调用编译shader【编译时材质只给了flatten_uniforms都是key，是怎么把value给Shader的？】，并加入到渲染队列中

shader

- VS

  位置gl_Position

- FS（PS）

  颜色gl_FragColor

# P3 Real-time Shadows 1

PCSS

The RTR book（第四版）

## Recap: shadow mapping

两趟的算法

1. light看向的场景+深度（light Pass）
2. 从相机出发渲染一次

Shadow Map的算法

1. 记录Light的深度，成Texture
2. 对比摄像机看的点的深度和对应Tex的深度

ShadowMap精度会导致自遮挡问题

- 从上往下照，问题最小
- 斜着照，几乎平行的，问题最严重
- 解决方案
  - bias（但会导致阴影缺失的问题）
  - Second-depth（最小和次小取中间值深度，必须有厚度，另外RTR不相信复杂度，也关注系数）

ShadowMap有分辨率，会导致阴影的锯齿

## The math behind shadow mapping

Calculus

Schwarz不等式

Minkowski不等式

RTR中不关注不等，关注近似相等

> f(x)g(x)dx = 

拆开积分为积分乘积

在实际的积分小的时候，或g足够光滑的时候，比较准

【疑】！！！没听懂 45-53min

L(p, w)

## Percentage closser soft shadows(PCSS)

软阴影

PCF（Percentage Closer Filtering）

- 最初做抗锯齿用
- 对查Buffer的结果进行操作，周围的ShadowMap像素结合计算

从解决PCF抗锯齿，filtering越大，软阴影，做PCSS

实际阴影软的程度和遮挡物的距离有关

（面光源的Map是通过模拟点光源来做）

做第一趟，通过Blocker的距离计算fileter size

# P4 Real-time Shadows 2

## A Deeper Look at PCF

min 21-28 数学公式

Filter / convolution

PCSS三步走

1. Blocker Search

费时的步骤及解决

## Variance Soft Shadow Mapping(VSSM)

解决第三步的速度：

ShadowMap记录depth的同时记录平方，目的是计算方差

得到正态分布方程

切比雪夫不等式，计算概率，比erf更简单

>  C++ erf的方法

解决第一步速度：

遮挡物的平均深度，是遮挡物的平均深度Zocc，非遮挡Zuocc

N1 / N * Zuocc + N2 / N * Zocc = Zavg

经过切比雪夫，得到比例，然后**Zuocc假定为阴影采样点的深度**

## MIPMAP for Range Query

Mipmap：方形的插值

范围查询不准

## SAT for Range Query

Summed Area Table

一维：

On计算一遍，预计算

二维：

每行做一遍，每列做一遍

## Moment shadow maping

VSSM会导致 Light Leaking和非平面接收物

解决问题一

用高阶的矩（四阶矩）

类似某种展开（泰勒）

CDF PDF

## 作业 1

硬阴影

- 显示深度图
  1. WebGLRender 注释掉Camera Pass
  2. DirectLight的fbo = null
  3. 修改shadow fragment shader，显示zzz1
- 阴影采样计算

PCF



PCSS



多光源

# P5 Real-time Environment Mapping

Games101之后重开



## Distance filed soft shadows

很快、需要大存储

空间中一个点到其他物体的最小的距离

**距离场** SDF 类似等值线

移动边界，线性插值，SDF的插值，边界上SDF为0

相关理论：最优传输（Optimal Transport）



应用1：

Ray Marching（sphere tracing）

SDF距离，“安全距离”

SDF是三维的值，费存储

运动的物体可以用SDF，形变的物体需要重新算

应用2：

生成软阴影，不准，但是符合观察

SDF告诉了安全角度，安全角度越大，说明能看到更多光线，visible越大

$arcsin(SDF(p)/(p-o))$

$min{k*SDF(p) / (p-o), 1.0}$

近似以减少复杂运算，k用来控制软硬程度



优点：

- 快
- 高质量

缺点：

- 预计算
- 高存储
- 动态的物体需要重新算
- artifact？



sdf在字体中的应用

SDF生成的物体不好贴纹理？

https://github.com/protectwise/troika/tree/master/packages/troika-three-text

## 环境光照

45min

Spherical map

Cube map



Shading，不考虑Shadow

IBL（Image-Based Lighting）



正半球（方向）



（P5 50min）

**蒙特卡洛方法**（采样费，慢）



**经典近似**（glossy、smooth）

Rendering equation

1. 不考虑visibility
2. lighting项和BRDF项拆分
3. 近似，lighting项和BRDF拆成两部分积分相乘



（P5 55min）

Split Sum（没有采样，UE PBR应用）

步骤1：

Rendering equation拆项，把light拆出来，和BRDF就没有关系了

相当于把环境光给模糊了，类似mipmap，做不同filter的

查多个方向，变成查filter后的**镜面反射方向**

步骤2：

处理公式后半部分（BRDF），预计算的思想

菲尼尔项拆分，依赖三个维度：基础反射率、角度、粗糙度

不能预计算，使之预计算

从BRDF中拆出基础反射率R0，然后就只剩下角度和粗糙度有关的，即二维，预计算成一张表，通过角度和粗糙度查询对应积分的结果， 实时中再和R0做运算

![image-20210710142440618](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210710142440618.png)

![image-20210710142548242](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210710142548242.png)

# P6 Real-time Environment Mapping

5min

PRT

Precomputed Radiance Transfer



问题：环境光照和全局光照的概念



LTC



给定环境光，计算Shadow、Shading

SH：球面谐波函数



## 环境光阴影

环境光、阴影

看待环境光的方法

- 多光源，计算多SM
- 采样，不容易拆分

解决方案

- 看主要光源（最亮的光源，比如太阳）

相关研究

- Imperfect shadow maps
- Light cuts（反射物体当成小光源）
- RTRT
- PRT



## PRT知识储备

1. 频域

   傅里叶级数展开

   从低频到高频的变化

   不是时间上的频率，而是图像上的频率

   进行滤波、卷积

2. 基函数

   傅里叶基函数

   多项式基函数



## 实时环境光照

33min

先做Shading

SH类似傅里叶展开，从低频到高频

第n阶的SH有2n+1个积函数，编号 -n到n

二维的傅里叶 VS SH

勒让德多项式

表示每一个基函数的系数

求SH基函数的系数，数学上投影（点乘）

Render中取前几阶，去掉高阶的项

SH基函数正交（用笛卡尔坐标系类比理解）



Prefiltering + 镜面反射查询 = 无采样+多查询

Diffuse BRDF像一个低通滤波器

通过Diffuse BRDF的阶来判断用几阶的信息描述光照

选择前3阶

SH适合描述低频的信息



Next：

- 引入阴影
- 不止diffuse，基函数思路解决glossy的问题

PRT

- 阴影
- 全局光照

## PRT

### diffuse

考虑visibility

![image-20210710155410786](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210710155410786.png)

lighting用SH基函数表示

预计算light transport，也是球面函数，在只有光照变化的情况下，light transport可以预计算保存下来
$$
/rho * /sum l_i*T_i
$$
visibility预计算了，说明场景中的物体不能动

可以旋转环境光，因为SH的特性（旋转环境光，等价于旋转SH基函数，得到新的线性组合）

（打表查询新的线性组合【疑】）

# P7 Real-time Global Illumination

## PRT

PRT，拆

- 光照（环境），预先计算
- 其他项 Light Transport （顶点），预先计算

应用时

- 光照项和LT再结合
- 这里从n方到O(n)，利用到sh的正交性，点乘



计算的时候是计算顶点，再插值

### glossy

对于glossy的情况，BRDF不一样，是四维的。不止是i的函数还是o的函数，所以对于不同的o，得到的LT是不一样的。

light Transport的维度增加，和观测方向o有关

再将Light Transport投影到SH上

从向量变成了矩阵，LT转成向量

代价：

- 存储增加
- render计算复杂程度增加

### 复杂度

- 正常情况下，diffuse 3阶
- glossy4、5等，科研10阶也不够

LE ： 光路表达（light - eye）

LSDE： caustics 焦散

PRT思路，LE中间的，即light transport

PRT不适合做高频的情况

找方式解决PRT的缺点

最难的是发现问题，而不是解决问题

### wavelet

基函数

- wavelet

- ……

wavelet小波

Haar 小波

一层一层保留高频，可以比sh描述更高频的信息，类似四叉数。压缩质量好。

jpeg使用类似小波的压缩方式，离散余弦变换。



小波的问题：

- 不支持快速旋转

## 全局光照

59min

RSM (Reflective Shadow Maps)

LPV (Light Propagation Volumes)



RTR，全局光照：

- 直接光照

  split sum

  认为好做的

- 间接光照

  一次bounce

目的：

- 间接光照一次
- 快速

间接光照

L（Primerey Light）-Q（次级光源，被直接光照照到的物体）-P（物体）-眼睛

### RSM (Reflective Shadow Maps)

过程

1. 怎么知道有哪些点可以照亮P，也就是有多少Q（从shadowmap）
2. 每个Q如何贡献到P上



RSM

1. 过程1：shadowmap，每个像素认为是个小片
2. 假设次级光源表面都是diffuse（简化从各个不同的P观察Q，光不一样的问题）
3. 辐射度量学

推到过程……【疑】



照不到的简化Q

找距离近的Q，反向到shadowmap上找周围采样，512*512减少到400个Q



RSM中记录的信息

- Depth
- world coordinate
- normal
- flux
- ……



RSM对于手电筒来说，效果很好



好处：

- 好写（shadow map）

问题：

- shadow map的问题，直接光源个数的局限，一个光源一个map
- 没有Q到P的visibility，内部全局不真实
- 很多假设：diffuse Q、shadowmap 周围距离假设
- Sampling rate，quality tradeoff

图像空间的方法

# P8 Real-time Global Illumination

## LPV (Light Propagation Volumes)

CryEngine3-Crisis?孤岛危机

Anten

ShadingPoint各个方向的radiance查询

假设：传播过程中光线沿着直线不变

场景分成3D网格，格子就是用来传输光线的



过程

1. 接收直接光源的点（次级光源）

   RSM，也是多光源多RSM

   得到虚拟光源

2. 次级光源注入3D网格

   工业界，3D纹理

   每个格子叠加，SH记录

   SH：前两阶，4个

3. 网格中传播

   每个格子扩散到周围6个格子

   迭代4-5次就会比较稳定

4. 拿传播的结果渲染全局光

问题

- 格子的精度问题，出现漏光问题（小的几何比格子小）
- 格子精度高，存储大，计算慢

传播过程中不计算visibility

不计算斜上角的格子

比像素数量小一个数量级

传播过程可能不收敛

工业界 cass cad？



对于glossy的物体没有问题

每个格子类似存储LightTransport

## VXGI (Voxel Global Illumination)

体素

两趟的算法

与RSM的区别

- RSM是次级点光源或者片；VXGI是格子
- 从camera出发，每个shading point做一遍空tracing

过程

1. 记录每个voxel中的光源范围和物体法线范围
2. 考虑渲染，从camera出发，每个point出发，圆锥往出去计算

对于diffuse，多个小圆锥

效果可以，对于glossy的物体适用

VXGI 2012年第一篇paper

## SSAO (Screen Space Ambient Occlusion)

RSM是图像空间

LPV、VXGI3D空间

SSAO是屏幕空间方法，屏幕空间的环境光遮蔽

立体感

SSAO

- 对于环境光的近似
- 屏幕空间

假设

- 不知道间接光照是什么
- 不似Blin-Phong模型每个方向的环境光一样，SSAO考虑Shading Point的不同环境光visibility
- diffuse物体

从render等式理解SSAO【疑】

加权平均的visibility



从屏幕空间

- 随机撒点
- ShadowMap深度判断物体内外
- 物体内部的点数超过一半才考虑AO问题

SSAO是没法线的情况下做，false occusion？

带法线，hbao？



有限的物体范围

# P9 Real-time Global Illumination

## SSDO (Screen Space Directional Occlusion)

和path tracing一样

AO中假设间接光照来自非常远的地方，DO假设来自很近的地方

问题

- 小范围的
- 从camera的角度考虑visibility，不全面
- Screen Space，丢失信息

## SSR (Screen Space Reflection)

Retracing

屏幕空间上做光线追踪

task

- 求交：任何光线和场景（屏幕空间的壳）
- shading：给shading point的贡献

反射本质上就是全局光照

可以做specular，也可以做glossy的，也可以做地面不平的



实现

- 逐步（步长）向前看是否相交

动态求步长

- 实现原理，mipmap，小图保存大图四个像素的最小值
- 加速查找
- 从0层没有交点，找level1（迈大步子）走一步没有交点，找level2……如果找到了交点，再降低细化

问题：

- sreen space 缺失信息
- 超出屏幕后反射被截断（解决：做衰减）

Shading

- 反射物是diffuse

可以自然地做出很多效果

- Sharp and blurry reflections
- Contact hardening
- Specular elongation
- Per-picel roughness and normal

提高部分……

# P10 Real-Time Physically-Based Materials(surface models)

PBR和PBR材质

实时渲染中的PB不是真正的PB

PBR材质

- surfaces 表面

  - 微表面模型
  - Disney（非PBR）

- volumes 体积

  光线作用一次和多次分离的方法（云、头发等）

保证速度的前提下，把离线的效果带到实时

## 微表面的BRDF

菲涅尔项：有多少比率的能量被反射掉

MDF：法线分布

- 分布相近：glossy
- 复杂：diffuse



NDF(Normal Distribution Function)

- 法线分布（与正态无关）
- 模型：Beckmann、GGX、Yan…

二维

半球上分布的信息，二维表示：从上往下“拍扁”

因变量：各个方向

Beckmann：类似高斯【疑】

（各向同性）

坡度空间：保证微表面的方向不会向下

GGX（TR）（作业）：

- 相比Beckmann，多“长尾巴”
- 衰减速度慢，更自然的高光和diffuse效果

Generalized TR



Shadowing-Masking Term

grazing angle微表面的自遮挡问题

让grazing angle变暗



问题：（白炉测试）

越粗糙，越暗（能量丢失）

Kulla-Conty近似

没有颜色 1-Eu

有颜色 平均Frensel



不要在微表面的BRDF上加diffuse！

# P10 Real-Time Physically-Based Materials(surface models cont.)

LTC

线性变换的余弦

## Disney principled BRDF

【疑】这里几乎没听懂



## 作业2

C++部分预计算

- CMake + C++17

JS部分渲染



# C++ Windows 编译

cmake：官网下载msi安装程序，按照步骤安装，并添加path到系统目录

编译器

