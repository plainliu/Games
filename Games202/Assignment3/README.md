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

