> Games101-3

# 环境-VS2019运行101作业3

参考

http://games-cn.org/forums/topic/fenxiangyigezaiwindowsshangyongcmakevscodezuozuoyedehuanjing/

注意：

涉及修改环境变量的地方，重启VS后再运行

## opencv

找不到OpenCV的报错

```
CMake Error at ...\homework3\CMakeLists.txt:4 (find_package):
  By not providing "FindOpenCV.cmake" in CMAKE_MODULE_PATH this project has
  asked CMake to find a package configuration file provided by "OpenCV", but
  CMake did not find one.

  Could not find a package configuration file provided by "OpenCV" with any
  of the following names:

    OpenCVConfig.cmake
    opencv-config.cmake

  Add the installation prefix of "OpenCV" to CMAKE_PREFIX_PATH or set
  "OpenCV_DIR" to a directory containing one of the above files.  If "OpenCV"
  provides a separate development package or SDK, be sure it has been
  installed.

```



下载opencv、解压

要**让cmake能找到opencv**，则需要能访问到build目录的 `OpenCVConfig.cmake`，有多种方法

1. 环境变量中添加OPENCV_DIR

   右键此电脑-属性-高级系统设置-环境变量

   用户变量和系统变量都行，看自己，添加新的变量OPENCV_DIR，值为opencv的build目录

2. CMakeLists.txt中设置OPENCV_DIR

   ```
   set(OPENCV_DIR "C:/.../opencv/build")
   find_package(OpenCV REQUIRED)
   ```

   路径注意写左斜杠

3. CMakeLists.txt中设置CMAKE_PREFIX_PATH

   ```
   list(APPEND CMAKE_PREFIX_PATH "C:/.../opencv/build")
   find_package(OpenCV REQUIRED)
   ```



加载opencv_world3414d.dll失败，dll的位置在`C:\...\opencv\build\x64\vc15\bin`为了运行时能加载到dll，可以采用下列方法之一

1. 将`C:\...\opencv\build\x64\vc15\bin`添加到path（推荐）
2. 将opencv_world3414d.dll拷贝到`C:\Windows\SysWOW64`下（网上有人这么做，对系统环境和文件有干扰，我心理洁癖不愿意这么搞）
3. 将opencv_world3414d.dll拷贝到VS编译出的exe的位置（临时用VS + opencv的话可以选择）



![image-20210630204059318](NOTES.assets\image-20210630204059318.png)



## Eigen3

下载eigen3的一个版本，解压缩，然后改名字为eigen3

并将eigen3所在目录添加到include目录

```cmake
# include_directories(/usr/local/include ./include)
include_directories("C:/.../mydir" ./include)
```

（改成eigen3是因为框架代码中include头文件是带eigen3目录结构的）



比如我下载的是eigen3.3.9，解压缩后文件夹名是eigen-3.3.9，把eigen-3.3.9修改成eigen3

https://gitlab.com/libeigen/eigen/-/releases/3.3.9



## 运行

VS2019打开CMakeLists所在目录



编译报错get_projection_matrix缺少返回，添加个返回



报异常：

- 加载图片失败

加载文件失败的原因

- 上级目录需要多几层



launch.vs.json

带参数arg

F5 运行（注意选择启动项为launch.vs.json中配置对应的name名，如果启动的是exe则不带参数）



配launch.vs.json：

```json
{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "",
      "name": "normal",
      "args": [
        "output-normal.png",
        "normal"
      ]
    },
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "",
      "name": "texture",
      "args": [
        "output-texture.png",
        "texture"
      ]
    },
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "",
      "name": "phong",
      "args": [
        "output-phong.png",
        "phong"
      ]
    },
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "",
      "name": "bump",
      "args": [
        "output-bump.png",
        "bump"
      ]
    },
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "",
      "name": "displacement",
      "args": [
        "output-displacement.png",
        "displacement"
      ]
    }
  ]
}
```

运行成功



# 作业

## Math

如果代码中需要使用到宏M_PI，VS环境中是没有的，需要使用下面的方式引入宏定义

```c++
#define _USE_MATH_DEFINES
#include <math.h>
```

然后看代码发现，global.hpp中定义了MY_PI...

## 过程

### Blinn phong

1. 拷贝作业2代码（按照zbuffer中记录值为正做）

   发现牛是背向的，而且法线显示为红红绿绿，不是作业pdf中蓝蓝紫紫的

   原因：zbuffer计算错误，对比时，近的反而深度大

   处理：draw方法中

   ```c++
   //Viewport transformation
   for (auto & vert : v)
   {
       vert.x() = 0.5*width*(vert.x()+1.0);
       vert.y() = 0.5*height*(vert.y()+1.0);
       vert.z() = -vert.z() * f1 + f2; // vert.z()前加-号
   }
   ```

   

2. blinn phong做的时候一直不对，光从牛下巴照过来的，而且计算时，光强度很弱，是乘了100后才能看到下巴的光。

   然后就说看看贴图，是不是光栅化有问题，结果有重心坐标小于0的？插值后得到的uv有小于0的？筛选掉小于0的着色，结果如下

   基本上可以看到被剔除的点在三角形边界上

3. cwiseProduct，着色模型中用到乘法，需要点对点乘的时候用这个方法

![image-20210703150436635](C:\liujuanjuan\github-plainliu\Games\Games101\Assignment3\NOTES.assets\image-20210703150436635.png)

着色判断是否可见（法线和观测方向余弦大于0），变成下面这样了

![image-20210703152829492](C:\liujuanjuan\github-plainliu\Games\Games101\Assignment3\NOTES.assets\image-20210703152829492.png)

1边缘上uv计算错误的原因：传入的坐标加了0.5导致的

```c++

//auto[alpha, beta, gamma] = computeBarycentric2D((float)x + 0.5f, (float)y + 0.5f, t.v);
auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
```

问题：**load obj 拿到的数据中uv有负的，但是在计算后拿贴图纹理的时候kd没有负数了**【需要推一下】

2法线和观测方向加判断，牛背没有了，原因

3论坛上提到的问题，View空间得到的z拿来插值，和实际有误差

http://games-cn.org/forums/topic/zuoye3-guanyushenduzhiwentizijicaidekengheyixiexiangfa/



按照z=10的摄像机进行的变换，变换后，按道理，eye应该到了原点，看向z负方向

那么在shader中，应该摄像机z=0了，但是shader中还是用10来计算，也就是摄像机用的世界坐标系，物体用的投影坐标系



目前为止自己的代码里，两个问题

1. 为啥没有牛背了
2. 为啥phong shader中计算的光照强度那么小

两个问题的原因，shadingpoint的位置插值写错了，应该用view_pos，导致插值的位置不对，计算光的方向和观测方向都离谱【需要推一下】

```c++
//auto interpolated_shadingcoords = Eigen::Vector3f(x, y, alpha * v[0].z() + beta * v[0].z() + gamma * v[0].z());
auto interpolated_shadingcoords = interpolate(alpha, beta, gamma, view_pos[0], view_pos[1], view_pos[2], 1);
```

环境光，对于每个光源加一次，还是总共加一次？（环境光模拟的是间接光照，每个光源算一次应该对的）【存疑】

### 纹理贴图

需要通过uv从贴图上获取颜色，debug模式拉起就会崩溃

- 崩溃时，u是负数
- 下条件断点，访问调用栈，三角形的第一个顶点的u就是负数（重心坐标计算得到的u也是一个负数，访问图片位置崩溃）【先当0处理，需要进一步讨论下是否为资源的问题】

Release 左右转动几次后出现崩溃，做了判断规避后也不行【为啥呢】

```
array = 0x000000721a0fec04 {-nan(ind), -nan(ind)}
```





打印warning：

```
libpng warning: iCCP: known incorrect sRGB profile
```

网上搜了是cv的检查严格，对程序没有影响，暂时不管



### bump mapping

凹凸贴图，扰动法线

TBN切线空间【详细看看】

法线方向为Z轴，再选取两条过该点的切线，作为另外两个轴，其中一条称为Tangent，另一条称为Bit-Tangent，即切线空间的基为TBN

凹凸贴图用norm计算高度变化，是否有点浪费呢，三个通道计算一个值？

注释中的伪代码【推导】

### marry

getindex出界：默认2.5，太大了，出屏幕了，改成2，y向下3

uv -nan(ind)：因为重心坐标计算出了-inf

>可能的情况：
>
>1. 分母为”0”,如果分母为零，自然时不能得到一个确定的数字的。
>2. 对负数开平方。
>3. 有些编译器在对无穷大与无穷小的计算时也会出现此类情况。

对重心坐标判断大于0并没有解决

【矛盾：在三角形内，说明应该是大于0了，但是算出来的重心坐标有非法值，且alpha = 8？】

算出来的重心坐标有 inf

对非法uv的过滤【inf nan与其他正常数比较结果永远为非？】

```cpp
// 不可以
if (u < 0.0f || v < 0.f || u >= 1.f || v >= 1.f)
	continue;

// 可以
if (u >= 0.0f && v >= 0.f && u < 1.f && v < 1.f)
	;
else
	continue;
```

框架中getIndex和setPixel有问题

- 修改到和作业二一致

getColor中uv的运用，和v_img、u_img的临界，导致u的范围是[0, 1)，而v的是(0, 1]

```cpp
Eigen::Vector3f getColor(float u, float v)
{
    auto u_img = u * width;
    auto v_img = (1 - v) * height;
    auto color = image_data.at<cv::Vec3b>(v_img, u_img);
    return Eigen::Vector3f(color[0], color[1], color[2]);
}
```

## 其他发现

VS中居然可以直接打开obj文件，三维显示，并且可以用鼠标操作视角和位置

- 左键拖动
- alt + 左键旋转
- ctrl + 滚轮 缩放
