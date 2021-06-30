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





作业2

## Math

main.cpp中报M_PI缺失的问题

```c++
#define _USE_MATH_DEFINES
#include <math.h>
```



## 运行

VS2019打开CMakeLists所在目录



编译报错get_projection_matrix缺少返回，添加个返回



launch.vs.json

带参数arg



报异常：

- 加载图片失败

加载文件失败的原因

- 上级目录需要多几层





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
      "name": "CMakeLists.txt",
      "args": [
        ""
      ]
    }
  ]
}
```

运行成功
