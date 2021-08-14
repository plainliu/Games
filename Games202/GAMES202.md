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

# P8 Real-time Global Illumination(screen space)

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

# P9 Real-time Global Illumination(screen space cont.)

## SSDO (Screen Space Directional Occlusion)

和path tracing一样

AO中假设间接光照来自非常远的地方，DO假设来自很近的地方

问题

- 小范围的
- 从camera的角度考虑visibility，不全面
- Screen Space，丢失信息

## SSR (Screen Space Reflection)

28min

Raytracing

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

```c
mip = 0;
while (level > -1)
    step through current cell;
    if (above Z plane) ++level;
    if (below Z plane) --level;
```



问题：

- sreen space 缺失背面信息
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

- PBR：基于物理的渲染（材质、光线、摄像机……）
- PBR材质

实时渲染中的PB不是真正的PB

- 材质少，质量差，做了简化，不是基于物理的

PBR材质

- surfaces 表面

  - 微表面模型
  - Disney 渲染器HYPERION（非PBR，基于艺术的，不是基于物理的）

- volumes 体积

  光线作用一次和多次分离的方法（云、头发等）
  
  single、multiple scattering
  
- 实时渲染中相对离线hacks

- 保证速度的前提下，把离线的效果带到实时

## 微表面的BRDF

![image-20210810203821542](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210810203821542.png)

F菲涅尔项：有多少比率的能量被反射掉

- 观察角度，入射方向和平面法线垂直，反射最多（参照河边看倒影）

- 估计

  ![image-20210810204608233](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210810204608233.png)

G：Shadowing masking

D微表面法线的分布

- 分布相近，glossy
- 分布变化剧烈，diffuse（表面沟壑深）



NDF(Normal Distribution Function)

- 法线分布（与正态无关）
- 模型：Beckmann、GGX、Yan…
- 二维的，分布在**圆上**，中心大，往外面衰减【min25】
- 半球上分布的信息（法线向上），二维表示：从上往下“拍扁”

Beckmann：类似高斯【疑】

- alpha： lob的大小，也就是粗糙程度
- theta：半程向量和n的夹角（各向同性）
- 坡度空间：tan保证微表面的方向不会向下
- 归一化的性质

GGX（TR）（作业）：也类似高斯

- 相比Beckmann，多“长尾巴”（光晕）
- 衰减到一定程度后，衰减速度变慢，更自然的高光和diffuse效果

GGX拓展（Generalized TR）GTR

- 更长的尾巴
- gama = 2，约等于GGX，越大尾巴越小



Shadowing-Masking Term（Geometry term）

- 解决微表面的自遮挡问题
- grazing angle的子遮挡问题最严重
- 概念：shadowing-light，masking-eye
- 目的是让grazing angle变暗，自遮挡严重，越暗。
- 问题：边缘上，球的外圈菲涅尔项全反射，太亮
- Smith：分开考虑shadowing和masking
- m：半程向量



考虑以上三个项之后，仍有问题：

- 越粗糙，越暗（能量丢失）
- 空的背景，测试亮暗（白炉测试）
- 原因：越粗糙，微表面沟壑越深，多次bounce弹射的能量越多，所以只通过一次弹射模拟，会导致能量丢失



Kulla-Conty近似

- 通过经验性的方式补全丢失的能量

- 有多少能量丢失？

- 看多少能量能离开微表面【1h2min】，对BRDF、cos和lighting积分。所有的入射光 radiance 1，f是brdf项。

  球面展开成对theta、ph的积分。对应的立体角是sin theta dtheta dph，令mu = sin theta，原来要积分的cos sin d theta d ph，sin d sin d ph，再替换成mu

  ![image-20210810213322712](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210810213322712.png)

- 被遮挡的损失的能量 1-Eu， 不同的观察方向，积分出来的不一样，和u有关系

- 补上能量 1-Eu，不上一个BRDF项，使得积分出来的能量是1-Eu，考虑对称性，并且简单起见*c*(1 − *E*(*μ**i*))(1 − *E*(*μ**o*))，c为

  ![image-20210810214933924](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210810214933924.png)

- E_avg，预计算，打表。依赖的参数是观测方向u和roughness

- 有颜色的情况，说明本身有颜色的吸收造成的能量损失

  先考虑没有颜色的情况，再考虑吸收的问题

  平均Frensel：不管入射角多大，平均反射多少

  ![image-20210810215929168](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210810215929168.png)

  每次bounce上应该补充的能量，级数相加

  ![image-20210810220120379](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210810220120379.png)



**不要在微表面的BRDF上加diffuse！**

# P11 Real-Time Physically-Based Materials(surface models cont.)

在micro brdf定义好后，如何shading，方法LTC

## LTC：线性变换的余弦

要做的事情

- 主要针对GGX
- shading不考虑shadow
- 微表面模型在多光源情况下的shading结果

做法

- brdf lob是四维的，wi 两维，wo 两维，在固定观察方向后，2维
- 2D brdf lob => cosin，光源跟着brdf变换
- 转换的目的，统一不同的brdf的计算，而且转换后有解析解【13min】
- 具体换元积分【18min】

【疑】

## Disney principled BRDF

微表面模型的缺点

- 解释不了多层的材质（刷了清漆的桌面）
- 不好用：PBR物理量对艺术家不友好

Disney principled BRDF

- 艺术家友好的，不要求物理正确性

- 不是真实的PBR

- 设计原则

  物理量少

  参数少

  参数范围0-1

  所有的组合在一起得到可靠的结果

- 拟合的复杂公式，有开放源代码

- 次表面反射：比diffuse更平的效果

好处

- 方便使用
- 简单模型描述更多的材质
- 不是基于物理的，还是叫PBR，学术界和工业界
- 巨大的参数空间，表示能力强

坏处

- 参数空间大，难以训练，容易出现冗余

游戏引擎里不用Disney

## 非真实感渲染 Non-Photorealistic Rendering (NPR)

实时渲染中

- 快速
- 可靠
- 风格化

真实感渲染

- 像照片

非真实感

- 艺术表现

- 从真实感出发

- 应用：艺术、可视化、结构、教育、娱乐

- 应用：游戏

  阴影、描边

从真实感渲染+艺术效果=>渲染中应该如何做

Outline Rendering（B边界、C折痕、M材质边界、S轮廓-多个边共享）

- view normal 垂直，边界，假设降低90度，描边变宽：描边不一样粗
- 模型外侧画一个大一圈的模型：背面的三角形扩大一圈
- 在图像上做处理：Sobel detector，图像上做filter《数字图像处理》，锐化类似
- 在深度+normal判断边界

Color blocks

- 色块 阈值化，二值化
- 阈值化，多值化，对于某个范围对应一个颜色

Strokes Surface Stylization

- 素描是甚么：明暗程度决定素描密度
- 设计不同密度的纹理，即明暗
- 远处也达到相同的明暗：做mipmap

NPR：

- 重点是从艺术效果到渲染的翻译过程
- NPR的过程做if else
- NPR的好坏主要是NPR之前真实感的渲染的好坏

# P12 Real-Time Ray Tracing 1

毛发：RTE、BSSRDF……离线渲染

离线渲染课

- 复杂的光线传播的方法
- 复杂的材质：散射介质

UE5

- Nanite
- Lumen

## RTRT

硬件：NVIDIA RTX

应用广泛

RTX

- 星球大战
- 保时捷车
- ……

RTX actually do

- 软阴影
- 反射
- 环境光遮蔽

硬件：GPU光线和场景的求交，每秒10g光线

**1 sample per pixel** = 1 SPP，一个光路的样本

RTRT最关键的技术：降噪

降噪目的（1 SPP下）

- 高质量（不糊、没有明显bug）
- 速度（几ms）

现有技术不可能

- Sheared filtering series
- 离线的方法
- 深度学习

RTX的optix 没有做时间上的filtering

工业界的idea

- **Temporal** 利用时间，递归
- 假设前一帧已经滤波
- 用motion vectors找到上一帧中的位置
- 相当于增加了SPP

G-Buffer

- 记录屏幕空间的一组信息（类似深度图的生成）：世界坐标中的xyz、法线……
- for free：轻量级

Temporal-back projection

- 找像素对应的内容，上一帧在哪里
- 当前点的世界坐标
  - G-Buffer中读取（在第一趟 primer ray 用光栅化做的时候顺便得到的）
  - MVPE逆变换
- 上一帧世界坐标在哪里
  - 逆变换
- 从世界坐标系计算上一帧的屏幕坐标
  - 利用上一帧的MVPE计算
- 计算机视觉中-光流：只基于内容，但这里需要准确的的motion vector

两帧结合

1. 当前帧做自己的降噪

2. 时间上做降噪

   两帧之间做线性插值 alpha = 0.1-0.2，当前帧占用比例小

降噪前图片噪点大，看着暗

- 能力守恒的，看着暗是因为原大于1的点被clamp掉
- HDR显示器显示看不会这么暗

Temporal 问题

- 上一帧不能用的情况

  - 切换场景

    快速变化的光源

    场景中切换镜头

  - 屏幕空间的信息

    倒退的场景

    上一帧不在屏幕空间

  - disocclusion

    被遮挡的区域突然出现，屏幕空间没有存储背后的信息（拖尾）

- 处理

  - Clamping

    让上一帧clamp

  - Detection

    检测用不用上一帧，利用object ID

    - 调整alpha

- 其他问题

  shading问题，motion vector = 0，只移动光源

  - 阴影拖尾

  反射平面不动，移动被反射的物体

  - 反射效果滞后

Temporal 加速和TAA是相同的思路

学术界降低Temporal Failure的影响

- EG paper

# P13 Real-Time Ray Tracing 2

当前帧的降噪（空间上的滤波）：边缘领域（计算机视觉、图形学……）、双边滤波

## Filtering

低通滤波

高斯滤波器

- 取周围所有点的贡献，再除以权重和
- 理论上取周围很大范围的点，实际取3 sigma
- 权重和为0，返回0

Bilateral Filtering

- 高斯滤波对图像每个地方糊掉，整个图模糊
- 为了保留边界（高频信息），使用双边滤波
- 边界：变化剧烈
- 处理：**像素差异足够大**的时候，贡献变小（颜色距离）
- 问题：怎么区分是噪声还是边界svgf

Joint/Cross Bilateral Filtering 联合双边滤波

- 判断标准：位置距离+颜色距离

- 非常适合做蒙特卡洛的双边滤波

- 利用：

  G-Buffer

  不止高斯滤波

- 怎样做

  1. 像素之间的绝对距离
  2. 颜色距离
  3. 深度差异，差异越大，贡献越小
  4. 法线差异，差异越大，贡献越小

  这几个贡献是乘起来的，几个滤波相乘，各自的参数调节

Implementing Large Filters

- 滤波核大了后，计算缓慢

- 解决思路：

  1. Separate Passes

     先水平filter一遍，再竖直filter一遍

     n方--2n

     因为2维高斯在数学上本来就是分开定义的

     高斯滤波可以这么做，联合双边滤波理论上不可以，但实际上对滤波核不特别大的情况，32*32的情况下硬拆，几乎没有art

  2. Progressively Growing Sizes

     逐步增大的filter

     a-trous wavelet

     - 每一次都是5*5的大小
     - 第二趟的5个之间有间隔，i趟间隔是2^i
     - 64^2 = 5^2 * 5，做五层5*5

     为什么【1h5min】要分层向上，为啥更高层上间隔更大

Outlier Removel 

- 问题：原来超级亮的点，被扩散到周围

- 处理：滤波之前就给过滤掉（有能量不守恒的问题，但暂不考虑）

- 检测outlier：7*7

  算均值和方差，超过一定范围

- removal

  做法：clamp到这个范围

Temporal Clamping

- clamp上一帧

# P14 A Glimpse of Industrial Solutions

## RTRT 降噪解决方案

### SVGF（Spatiotemporal Variance-Guided Filtering）

深度学习

深度的梯度

- 沿着这个面法线的方向上，深度差异，而不是屏幕空间的深度差异

  ![image-20210814222312086](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210814222312086.png)

- 法线（不应用法线贴图）

  ![image-20210814222331719](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210814222331719.png)

- 亮度差异

  ![image-20210814222408844](C:\liujuanjuan\github-plainliu\Games\Games202\GAMES202.assets\image-20210814222408844.png)

改进 ASVGF

### RAE

Recurrent denoising AutoEncoder

神经网络 AutoEncoder/U-Net structure

- “漏斗形”

对比

## 工业界解决方案

### TAA（Temporal Anti-Aliasing）

反走样

Temporal 反走样

MSAA和SSAA

- SSAA：扩大分辨率，计算，再缩小

- MSAA：一个像素内几个感知点，对于同一个primitive，所有感知点，只做一次shading。维护深度和颜色的表。

  临近的像素复用

基于图像的反走样

- SMAA（FXAA-MLAA-SMAA）
- “矢量化”

G-Buffer一定不做反走样

### Temporal Super Resolution

增加最后图像的分辨率

DLSS

低分辨率变成高分辨率

- DLSS 1.0：神经网络训练

- DLSS 2.0：应用上一帧的信息，核心思路TAA

  问题：Temporal Failure，不能再用clamping

### 延迟渲染

深度测试不通过的像素不着色

光栅化两次

1. 得到深度缓存
2. 所有场景做一次光栅化，只做通过深度测试通过的像素

early-Z

问题：

- 不能做aa
- 但可以做taa

### Tiled Shading

降低shading中光源的信息

继续优化 Clustered Shading，深度上做切分

避免没有意义的shading

### Level of Detail Solutions

LOD

cascaded

1. shadowMap：距离不一样的地方用不一样大小的shadowMap
2. LPV

最大的问题：

- 层级之间的转换：插值

模型的层级，geometric LoD

- 转换中间平滑过渡：用TAA
- UE5 Nanite（硬件软光栅）

FYI

### Global Illumination Solutions

各种方案不能完美，除了RTRT，但慢

多种方法结合

Lumen的实现

## 总结

什么是有趣的

- 更多思考

工程能力

其他技术

- SDF纹理
- 透明物体
- 特效
- 后期处理
- 随机数种子、蓝噪声
- 盯着的位置分配高算力
- Probe
- ReSTIR, Neural Radiance Caching, etc
- Many-light theory and light cuts
- Participating media, SSSSS
- Hair appearance
- ……

Games203视觉

- 3D重建、算法
- 3D信息的表示
- 3D识别

Games2XX离线渲染

- 采样和光线传播理论
- Appearance Modeling
- State of the Art Research Topics

GAMES

# 资料

https://ciechanow.ski/lights-and-shadows/

