> Games101-5

# 环境-VS2019作业5

```
D8021	无效的数值参数“/Wextra”
-Wshadow
-Wreturn-type
```

GCC编译选项-Wextra

vs2019 cl编译器尝试使用

直接F5，RTCu运行时检查的原因

```
Run-Time Check Failure #3 - The variable 'y' is being used without being initialized.
```

支持浏览ppm的软件

http://openseeit.sourceforge.net/

参考：

https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays

# 作业

## 构造光线

假设：屏幕为xoy平面上，z=-1的长方形，高度为2，即y方向上[-1, 1]

求wh位置的点坐标

现象：

- 光线方向没有归一化的情况下，前面的球有黑边
- 阴影中有个别蓝点，这个地方的坐标是最大的负值浮点数，把判断条件改成1-u-v>=–FLT_EPSILON就没了
  - VS中 `FLT_EPSILON`
  - Linux上 `__FLT_EPSILON__`
  - C++11 `std::numeric_limits<float>::epsilon()`
- 没做b2的大于0判断，地面扩展到屏幕最下面了

## 提交前

虚拟机运行检查

删除无用文件，提交图片

