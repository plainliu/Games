> Games202-2

# 准备

下载作业

VS2019直接打开prt目录，能够用CMakeLists编译

第一次编译，报错

```
C:\liujuanjuan\github-plainliu\Programming\Graphics\Games202\Assignment2\prt\src\prt.cpp(1): warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
C:\liujuanjuan\github-plainliu\Programming\Graphics\Games202\Assignment2\prt\ext\spherical-harmonics\sh\spherical_harmonics.h(1): warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
C:\liujuanjuan\github-plainliu\Programming\Graphics\Games202\Assignment2\prt\src\prt.cpp(45): warning C4189: “index”: 局部变量已初始化但不引用
```

该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失

不带bom头的utf-8文件，在vs中编译，默认utf-8。

结果：

- 导致中文注释吞换行符

添加 win32 target_link_libraries前，line112加一行

```cmake
target_compile_options(nori PUBLIC /utf-8) # MSVC unicode support
```

添加后编译成功



运行时需要场景地址作为参数传入

用launch.vs.json配【没找到哪里生成，手动添加的，加到.vs目录】

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
        "C:\\liujuanjuan\\github-plainliu\\Programming\\Graphics\\Games202\\Assignment2\\prt\\scenes\\prt.xml"
      ]
    }
  ]
}
```

运行成功



目前只有x64-Debug，Debug比Release慢。

CMakeSettings.json 添加x64-Release配置，从debug复制过来

- json中configurationType修改成Release
- 或者编辑模式`CMAKE_BUILD_TYPE`Debug修改成Release



增加configurationType修改后的配置：

```json
{
  "configurations": [
    {
      "name": "x64-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "${projectDir}\\out\\build\\${name}",
      "installRoot": "${projectDir}\\out\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "",
      "ctestCommandArgs": ""
    },
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "Release",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "${projectDir}\\out\\build\\${name}",
      "installRoot": "${projectDir}\\out\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "",
      "ctestCommandArgs": ""
    }
  ]
}
```

