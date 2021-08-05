> Games101-5

# 作业6

问题：

比pdf的兔子暗，而且在三角形顶点附近有黑色的块

![image-20210805000933995](C:\liujuanjuan\github-plainliu\Games\Games101\Assignment6\NOTES.assets\image-20210805000933995.png)

查问题：

1. 对于黑色的点，下断点debug，倒发现了光线与包围盒计算的漏洞，导致bvh求交部分包围盒判断相交不正确，继续往下找了，查找时间长

   ```c++
   // before
   //if (dirIsNeg[0] == 0 && tminx < 0 || dirIsNeg[0] > 0 && tmaxx < 0 )
   //    return false;
   
   //if ( dirIsNeg[1] == 0 && tminy < 0 || dirIsNeg[1] > 0 && tmaxy < 0 )
   //    return false;
   
   //if ( dirIsNeg[2] == 0 && tminz < 0 || dirIsNeg[2] > 0 && tmaxz < 0 )
   //    return false;
   
   // after
   float rangemin, rangemax;
   rangemin = dirIsNeg[0] ? tminx : tmaxx, rangemax = dirIsNeg[0] ? tmaxx : tminx;
   rangemin = std::max(rangemin, dirIsNeg[1] ? tminy : tmaxy), rangemax = std::min(rangemax, dirIsNeg[1] ? tmaxy : tminy);
   rangemin = std::max(rangemin, dirIsNeg[2] ? tminz : tmaxz), rangemax = std::min(rangemax, dirIsNeg[2] ? tmaxz : tminz);
   
   if (rangemin < 0 || rangemin > rangemax)
       return false;
   ```

   

2. 看起来最终是否相交大方向没有问题，先看看法线是否正确，因为法线是一个计算颜色的基础属性，且想用来检查相交的三角形是否正确

   ```
   hitColor = (N + 1.0f) * 0.5f;
   ```

   红框位置明显有问题

   ![image-20210805001153615](C:\liujuanjuan\github-plainliu\Games\Games101\Assignment6\NOTES.assets\image-20210805001153615.png)

3. 结合顶点位置的黑色和法线问题，猜测射线求交有个误差区间，用个bias的东西，或者是边界计算错误

4. 分析：耳朵部分是背后的部分，光线可能与前面的三角形相交，也可能和后面的三角形相交，如果前后三角形正好处于同一节点的左右节点，代码逻辑错误，直接返回了第一次检查到的left，没有对比两次的t

   ```c++
   // before
   Intersection left = getIntersection( node->left, ray );
   if ( left.happened ) return left;
   
   Intersection right = getIntersection( node->right, ray );
   if ( right.happened ) return right;
   
   // after
   Intersection left = getIntersection( node->left, ray );
   Intersection right = getIntersection( node->right, ray );
   if ( left.distance && right.happened )
       return left.distance <= right.distance ? left : right;
   else if ( left.happened )
       return left;
   return right;
   ```

   修改后，解决了耳朵穿插的问题，但是三角形顶点附近的黑色仍然存在。

5. hit的三角形正确的情况下，着色结果为黑色？断点黑色像素，看着lightAmt一直是0。只显示lightAmt，三角形顶点不正确，说明三角形顶点附近光算错了。

   算法是从着色点到光源看中间是否有交点

   ![image-20210805204813200](C:\liujuanjuan\github-plainliu\Games\Games101\Assignment6\NOTES.assets\image-20210805204813200.png)

6. 两个光源，只保留一个

   - 第一个：全黑，只有顶点位置有一点白（结合上图，顶点附近黑色，顶点最中间白色）
   - 第二个：顶点黑色，其他看起来颜色没大问题

   从第二个着手，如果isShadow，输出着色点到光源的光线碰撞点的颜色。统一是紫色和玫红色

   ![image-20210805222554323](C:\liujuanjuan\github-plainliu\Games\Games101\Assignment6\NOTES.assets\image-20210805222554323.png)

7. 先只保留第二个，断点错误的着色点（isShadow==true），看上下文信息。着色点视线的交点信息中，交点坐标z是0，麻了，返回交点信息的时候xy返回成了uv，没有插值坐标。

8. 解决

