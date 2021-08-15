> Games202-5

# 环境-VS2019

## 准备工作

解压examples.zip到homework5目录（没有嵌套examples文件夹）

ffmpeg工具：http://ffmpeg.org/download.html

- 下载 https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip
- 解压
- bin目录加环境变量

## 运行

1. VS2019打开homework5 CMakeLists.txt所在目录
2. F5（注意修改main中examples地址，VS中增加相对路径，比如修改后`"../../../examples/box/input"`）
3. 执行 image2video.bat（环境中有ffmpeg）

# 作业

## 当前帧的降噪

1. 高斯模糊

   整个图像模糊

2. 高斯+颜色距离

