> Games202-4

# 环境-VS2019

https://docs.qq.com/doc/DUG1UZGVDamh1YXVq

M_PI报错

- VS中用M_PI，数学库编译需要添加宏 _USE_MATH_DEFINES

resolution报错

- C++ 标准并未支持 VLA，替换为vector

# 作业

离线端：

- 计算Emu不加菲涅尔项（没懂为啥）



PDF：

- GGX法线分布有错误，多写了括号

web端

- 球边缘和部分凹陷的位置亮黄色：直接挪用离线部分的实现不可以，需要在计算GeometrySchlickGGX时，NdotV小于0取0。



重要性采样

- Hammersley 球面上均匀采样
- Hammersley 本身是2D 在0到1的区间中均匀采样

```c++
// 3D 均匀采样
float phi = Xi.y * 2.0 * PI;
float cosTheta = sqrt( 1.0 - Xi.x );
float sinTheta = sqrt( 1.0 - cosTheta * cosTheta );
Vec3f( cos( phi ) * sinTheta, sin( phi ) * sinTheta, cosTheta );


```



