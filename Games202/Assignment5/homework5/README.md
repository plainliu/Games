# Denoise

## 作业完成部分

1. 单帧降噪
2. 两帧之间投影
3. 两帧之间积累
4. Bonus 1 À-Trous Wavelet 加速单帧降噪

代码改动：

- 单帧降噪

  `Denoiser::Filter` 中 `Joint bilateral filter`部分

  核心实现对应`LevelFilter`函数中第0层的做法。通过`NormalLevelFilter`调用`LevelFilter`。

- Bonus 1

  `Denoiser::Filter` 中 `A Trous Wavelet` 部分（调用部分已注释，默认普通filter）

  每层filter计算指定坐标颜色的实现为`LevelFilter`函数。通过`ATrousWaveletLevelFilter`调用`LevelFilter`。

- 两帧间的投影和积累的实现在PDF原指定函数中。

结果

- results文件夹
- 另clamp后效果噪声比clamp前多，box场景的filter结果在result下放了两个版本

