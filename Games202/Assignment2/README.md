# Games202 作业2

## 完成部分

完成了所有基础部分和两个Bonus

1. 格式正确，包含所有必须文件

2. 预计算环境光照

3. 预计算 Diffuse Unshadowed LT

4. 预计算 Diffuse Shadowed LT

5. 预计算数据使用

6.  Bonus 1：Diffuse Inter-reflection

   代码改动：`prt.cpp`中`PRTIntegrator::preprocess`函数中，Line240开始，TODO标记的代码块为添加的Diffuse Inter-reflection功能代码

7. Bonus 2：SH 旋转

   代码改动：实时端，`src\utils\tools.js`中的函数

   - `getRotationPrecomputeL`
   - `computeSquareMatrix_3by3`
   - `computeSquareMatrix_5by5`

   `src\renderers\WebGLRenderer.js`中

   - `render`方法的camera pass里，将转换后的光照sh系数传入到shader中

## 效果

Diffuse Unshadowed LT

![](images\PRT_Unshadowed_Mary.png)

Diffuse Shadowed LT

![](images\PRT_Shadowed_Mary.png)

Bonus 1：Diffuse Inter-reflection

![](images\PRT_InterRef_Mary.png)

Bonus 2：SH 旋转

![](images\PRT_Rotation_Mary.gif)

