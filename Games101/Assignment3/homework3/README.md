# Games101 作业3

## 得分点

完成作业中所有七项

1. 正常编译执行

2. 参数插值

   在`rasterizer.cpp`的`rasterize_triangle`里循环中实现插值颜色等。

3. Blinn-phong反射模型

   在`main.cpp`的`phong_fragment_shader`函数中，完成 漫反射项+高光项+环境光 计算。

4. Texture mapping

   `texture_fragment_shader`中kd用图片查找的结果替换

5. Bump mapping、Displacement mapping

6. Bonus 尝试更多模型：

   加载Games202 Marry，texture mapping + Blinn-phong反射模型（使用了单独的`get_model_matrix_marry`返回模型矩阵）。

   运行：

   - 请取消`main`方法中Marry相关的注释。
   - 支持normal fragment、blinn-phong反射模型、texture mapping

7. Bonus 双线性纹理插值：

   Texture类中增加`getColorBilinear`方法

   使用：

   - `main`方法中小牛的贴图使用低分辨率图`spot_texture-256x256.png`
   - `texture_fragment_shader`中将`getColor`方法替换为`getColorBilinear`

## 运行结果

### normal

![normal](images\output-normal.png)

### Blinn-Phong

实现 Blinn-Phong 反射模型

![phong](images\output-phong.png)

### Texture Mapping

![texture](images\output-texture.png)

### Bump Mapping

![Bump](images\output-bump.png)

### Displacement Mapping

![Displacement](images\output-displacement.png)

### 其他模型-Marry

![Marry](images\output-texture-marry.png)

### 双线性纹理插值

使用256x256的图片和`getColor`查找纹理

![texture-256x256](images\output-texture-256x256.png)

使用256x256的图片和双线性纹理插值

![texture-256-bilinear](images\output-texture-256-bilinear.png)

