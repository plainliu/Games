> Games202-3

# 1 直接光照

从题目知道需要做的是两个函数

EvalDiffuse

EvalDirectionalLight

通过rendering的公式

积分 light * diffuse * BRDF * visibility

- diffuse：作业框架中，直接返回的GetGBufferDiffuse值
- BRDF： EvalDiffuse
- light： 放到EvalDirectionalLight中，平行光
- visibility: GetGBufferuShadow，根据作业描述放到EvalDirectionalLight中



这时，光能找到的部分亮度是一致的，再考虑BRDF

因为是diffuse物体，不考虑出射光的方向，认为各个方向一致。EvalDiffuse

但是根据入射光的方向处理，直射的亮度高，非直射亮度低。



过程

1. 只加直接光照，不算BRDF，背面的边缘锯齿有亮度（只根据shadow map计算的可见性，shadow map有精度限制，不是完美的物体边缘），光能照亮的部分都是一个亮度（需要BRDF计算）

   做法：EvalDirectionalLight() light * visibility，相当于考虑直接光照和阴影本身

2. 加BRDF后，边缘锯齿亮度没有了，因为BRDF中的光线角度照不到背面边缘的shader point

3. 地面的背面是亮的，有正方体阴影的位置是暗的？虽然diffuse物体各个方向BRDF值一样，但是需要考虑出射角方向，如果是背面，是不能照亮的。增加出射角度判断后，解决。

4. BRDF中的rho是写死的，具体怎么计算rho呢？



关于BRDF：Games101

https://www.bilibili.com/video/BV1X7411F744?p=15 29min

# 2 SSR

反射，即shading point是否会反射场景中的信息（次级光源）

从摄像机位置看，是否有次级光源照到shading point，shading point再反射到摄像机位置

出射方向已知，材质已知，次级光源用屏幕空间的信息近似



求交：

- 照到摄像机方向的反射信息记为Lo
- 反射出Lo的光线Li
- 看Li是否与屏幕空间对应的场景的一层壳相交
- 相交则反射交点信息



求交过程

- 从当前位置开始，判断光线当前的位置Lp和场景壳是否相交

  主要是从屏幕空间出发，Lp和映射到屏幕同一uv的场景位置Sp，看Lp和Sp的深度判断他们是否相交

  - 求Lp对应的uv
  - uv得到场景对应位置的坐标Sp
  - 得到Lp和Sp的深度
  - 对比深度

- 尝试向前走一步，继续上述过程



验证：

- 场景中的shading point作为`RayMarch`求交的光线起点
- 根据法线和出射方向，计算光线方向（纯镜面）
- 渲染hitpos的颜色



已知法线直接求出射方向计算复杂，需要计算cos等

转换到以法线为一个轴的坐标系中计算，方便很多，反射直接加负号即可，再转回去。

卡在了镜面反射计算……



**蒙特卡洛方法**



目前RayMarching的结果不对，平面上靠近eye的位置有个圆弧

![image-20210716234508025](C:\liujuanjuan\github-plainliu\Games\Games202\Assignment3\NOTES.assets\image-20210716234508025.png)

找了一整天，计算wi的时候wo没有归一化，导致计算的wi不正确

- 假定y方向为法线方向，直接根据xoz平面对称，写出wi_up，测试发现这时地面上的反射是正常的，所以问题在wi上
- 一句一句检查，没有归一化……

镜面反射效果

- march步长0.05时效果可以，减小步长，效果反而不好【为啥】



raymarching中

- uv来控制0-1之间，超出范围不trace
- 对于屏幕空间的光线追踪，如果场景的壳存在大的gap，而tracing的时候穿过gap，这样按道理不应该match到东西，但实际搞出来会match到gap上边界上的值，需要做处理。处理方式【？】



现在做出来，看洞穴的效果，要比pdf的暗【TODO】



关于屏幕空间的问题：

- 缺少背面信息，ray tracing的时候从背面trace到了远处的点，如图，导致阴影中采样到了不正确的颜色

  按照图中方式trace，trace起始点A到画红圈的位置O后发现当前trace节点D和屏幕空间的点S对比，是小的，所以返回了O位置的颜色

- 解决方案

  - 简单估计，如果depth的差距大于一定值，则认为不相交。比如利用O位置有个“错开”的gap，这种状态下大多数情况是当前trace节点D和屏幕空间的点S相比，深度差异大。

    ```glsl
    if (curdep - sendep >= marchstep * 2.0)
        return false;
    ```

    

  - 实际来说，最终采样的屏幕空间黄色点S“**要能被起始点A看见**”，计算trace方向与O点的法线之间的夹角。

    ```glsl
    vec3 hitsenpos = GetGBufferPosWorld(uv);
    vec3 n = normalize(GetGBufferNormalWorld(uv));
    if (dot(n, hitsenpos - ori) >= 0.0)
        return false;
    ```

    

![image-20210808185030904](C:\liujuanjuan\github-plainliu\Games\Games202\Assignment3\NOTES.assets\image-20210808185030904.png)

