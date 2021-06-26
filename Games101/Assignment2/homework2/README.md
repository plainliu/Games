# GAMES101 homework2

完成了提高题

## 函数说明

main.cpp

1. 补充函数`Eigen::Matrix4f get_projection_matrix`，实现mvp中的投影矩阵

rasterizer.cpp

1. `insideTriangle(float x, float y, const Vector3f* _v)`
   - 修改了xy的参数类型int为float
   - 实现测试点是否在三角形内的判断
2. `rst::rasterizer::rasterize_triangle(const Triangle& t)`
   - 计算包围盒
   - 对包围盒内每个像素的各个采样点：
     - 判断是否在三角形内
     - 判断深度是否靠近摄像机
     - 更新采样点depth
     - 设置采样点颜色
3. `rst::rasterizer::draw( )`
   - 在函数最后将SSAA采样点颜色求平均，设置到frame_buffer中



## 实验结果

基础光栅化

![光栅化结果](img\rasterize.png)

SSAA（对每个像素进行2*2采样）

![SSAA结果](img\ssaa.png)



