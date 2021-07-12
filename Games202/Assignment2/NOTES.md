> Games202-2

# 准备

下载作业

## 编译

VS2019直接打开prt目录，用CMakeLists编译

第一次编译，报错

```
C:\liujuanjuan\github-plainliu\Programming\Graphics\Games202\Assignment2\prt\src\prt.cpp(1): warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
C:\liujuanjuan\github-plainliu\Programming\Graphics\Games202\Assignment2\prt\ext\spherical-harmonics\sh\spherical_harmonics.h(1): warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
C:\liujuanjuan\github-plainliu\Programming\Graphics\Games202\Assignment2\prt\src\prt.cpp(45): warning C4189: “index”: 局部变量已初始化但不引用
```

该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失

不带bom头的utf-8文件，在vs中编译，默认utf-8。

- 导致中文注释吞换行符

添加 win32 target_link_libraries前，line112加一行

```cmake
target_compile_options(nori PUBLIC /utf-8) # MSVC unicode support
```

添加后编译成功

## 运行

运行时需要场景地址作为参数传入

添加launch.vs.json【右键CMakeLists.txt--默认】

配launch.vs.json：

https://blog.csdn.net/aoxin/article/details/112250054

```json
{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "",
      "name": "CMakeLists.txt",
      "args": [
        "C:\\liujuanjuan\\github-plainliu\\Games\\Games202\\Assignment2\\prt\\scenes\\prt.xml"
      ]
    }
  ]
}
```

运行成功

## 增加Release配置

目前只有x64-Debug，Debug比Release慢。

CMakeSettings.json 添加x64-Release配置，从debug复制过来

- json中configurationType修改成Release
- 或者编辑模式`CMAKE_BUILD_TYPE`Debug修改成Release



增加configurationType修改后的配置：

```json
{
  "configurations": [
    {
      "name": "x64-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "${projectDir}\\out\\build\\${name}",
      "installRoot": "${projectDir}\\out\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "",
      "ctestCommandArgs": ""
    },
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "Release",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "${projectDir}\\out\\build\\${name}",
      "installRoot": "${projectDir}\\out\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "",
      "ctestCommandArgs": ""
    }
  ]
}
```

# 作业

## 预计算球谐系数

问题：

1. 线性空间是什么样的概念，gamma矫正作用机制
2. le是啥，为啥用采样点的值直接作为le的值
3. 将光照投影到球谐函数上得到系数，公式是怎么来的
4. 为啥作业中的w可以用面积

天空盒-环境光-SH表示

- 已知采样点，计算每个通道的球谐函数系数
- 已知采样点（每个像素），采样点方向，采样点的面积



SHcoeff = Z S Lenv(ωi)SH(ωi)dω

SHdcoeff = X i Lenv(ωi)SH(ωo)∆ω



Lenv(ωi)即对应ωi处对应的Le值

【推导？】



2.2.1 Diffuse Unshadowed

伪代码便利已经在框架中实现，`sh::ProjectFunction`

n_samples开放得到$\theta \ph$两个方向采样的数量，按照立体角平均分隔，每个小区间中采样。

`std::mt19937`高性能生成随机数。



问题：

- 为什么是把H投影到SH？

  MDU = max(Nx · ωi , 0)  投影到SH

- `sh::ProjectFunction`中没有和sample数量对应的每个delta值相关的计算，为什么没有sample数量越大，求得的系数越大的问题？（蒙特卡洛采样数）

- LightTransport和通道有没有关系？环境光的SH结果中有RGB，框架代码在light transport部分没有看到相关的



2.2.1 Diffuse Shadowed

增加了visibility项，计算某个顶点的SH系数时，每个方向除了判断cos大于0，看该顶点与当前采样方向中间有没有遮挡



对比pdf效果图，我的结果偏暗

和pi有没有关系？

http://games-cn.org/forums/topic/zuoyeziliao-daimakanwu/

## 实时

室内场景对比，要黑很多

除以pi

除以2.2？



原因：

1. 低级错误：vs计算点乘时gb通道用的r通道计算的，所以rgb全部是一个值，导致看起来灰色到黑色，没有环境光的颜色
2. 需要pow(1/2.2)，计算后和prt中计算的颜色才一样

问题：

1. gamma矫正为啥prt不需要，而实时需要做
2. 群友提到的除以pi的问题，是在哪里搞，目前没有除以pi，prt和实时端颜色是一致的（康奈尔box）
3. vs计算颜色时，L和LT系数点乘计算得颜色，为啥没有sh基函数的事

吸色调试法

另，web上切换环境贴图，light参数没有切换

- 框架中是加载模型的时候就给定的，ui切换模型不会重新加载（暂时忽略）

## Diffuse Inter-reflection(bonus)

shadow是自遮挡

增加反射

Diffuse shadowed + 

isFinite() 函数用于检查其参数是否是无穷大。

如果 number 是 NaN（非数字），或者是正、负无穷大的数，则返回 false

只显示一次bounce的结果，眼睛位置和额头位置固定显示几个亮点，颜色值异常大：

- 定位发现，创建的数组，初始值本身不对：均赋值为0后再运算

反射光总体颜色偏亮，原因是每个光的权重算错了，1 / sample 给复制成了sh的 4pi / sample。

