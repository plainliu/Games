# GAMES101 homework1

完成了基础部分和提高部分。



基础部分

1. 构建模型矩阵main.cpp `get_model_matrix`方法
2. 构建透视投影矩阵main.cpp `get_projection_matrix`方法
3. 看到变换后的三角形
4. 当按 **A** 键与 **D** 键时，三角形能正确旋转

提高项 main.cpp中：

- 新增方法`Eigen::Matrix4f get_rotation()`

- `main()`中传入的旋转矩阵替换成了`get_rotation()`

- 新增按键xyz表示绕各个坐标轴旋转，s表示绕三角形第二个顶点（初始位置时，上面的点）对应的向量方向旋转

  旋转方式仍为ad键

