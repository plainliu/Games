# Games202 作业3

## 完成部分

1. 直接光照

   代码改动：

   `src\shaders\ssrShader\ssrFragment.glsl`

   补充`EvalDiffuse`和`EvalDirectionalLight`函数实现，并在main中计算fragColor时使用这两个信息。

2. 实现 Screen Space Ray Tracing

   代码改动：

   `src\shaders\ssrShader\ssrFragment.glsl`

   补充`RayMarch`实现。

3. 实现间接光照

   代码改动：

   `src\shaders\ssrShader\ssrFragment.glsl`

   `main()`中shader point上半球采样光线，利用`RayMarch`与场景求交，计算间接光照。

## 效果

### Cube1

直接光照

![](images\cube1_direct.png)

间接光照

![](images\cube1_indirect.png)

### Cube2

直接光照

![](images\cube2_direct.png)

间接光照

![](images\cube2_indirect.png)

### 山洞

直接光照

![](images\cave_direct.png)

间接光照

![](images\cave_indirect.png)


