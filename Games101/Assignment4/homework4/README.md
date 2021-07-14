# Games101 作业4

## 得分点

完成作业中所有七项

1. 正常编译执行

2. De Casteljau 算法

   核心实现在`recursive_bezier`方法中，递归实现计算给定t时刻曲线上点的位置。

3. Bézier 曲线的反走样

   在`bezier`方法中，获取到t时刻点的位置后，对周围的四个点根据距离进行计算像素颜色。

## 运行结果

### De Casteljau 算法

![bezier_curve](images\bezier_curve.png)

### Bézier 曲线的反走样

![aa](images\bezier_curve_aa.png)

