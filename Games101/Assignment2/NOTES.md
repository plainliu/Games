# GAMES101 homework2

## 环境

计算深度的代码取消注释后，第一行有报错

```c++
// If so, use the following code to get the interpolated z value.
auto[alpha, beta, gamma] = computeBarycentric2D(sampley, sampley, t.v);
float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
z_interpolated *= w_reciprocal;
```

是因为这里使用了C++17的特性，默认环境不支持，解决方法：

- C/C++ 插件中设置CPP标准为C++17

## 问题

### 报错 Segmentation fault

Segmentation fault (core dumped)

- 数组越界

### 提高部分的实现

课程中SSAA和MSAA没有明确区分，作业实现的时候用的是SSAA

两者伪代码如下：

SSAA

```
ssaa_buffer

for each (x, y) in aabb do
	for sampling point in N*N do
		if (insideTriangle && testZbuffer)
            paint ssaa_buffer(sampling point index)
	end
end

ssaa_buffer to frame_buffer
```



MSAA

```
for each (x, y) in aabb do
	color = 0;
	for sampling point in N*N do
		//color += sampling color
		if (insideTriangle && testZbuffer)
            color += sampling triangle color;
        else//存疑
        	color += sampling bg color;
	end
	color /= N*N;
end
```

## 后续

问题1

首先用SSAA实现提高部分是不符合要求的

- 要求应该是**MSAA**（太晚了，已经到ddl了，暂且不再修改，就这样提交）

问题2

投影矩阵的计算中，正交矩阵是先平移后缩放，所以平移矩阵是乘在右侧的

**问题3**

有群友说，作业框架中计算出来的深度都是正数，且近的三角形反而深度大。

测试：MVP结束后，前后三角形的z大小是正常的，都是正数（为啥是正的），之后对顶点都做了除以w的操作，后续就相反了。

- 需要继续看看

nf给成负的得到的投影矩阵，mvp计算后depth就是负的（hard code的nf是正的，变换z后虽然变成了正的，但是是线性变换，远离原点的负数变换后靠近原点了，所以遇到了depth靠近eye的反而大的问题）
如果nf给成正的，遮挡关系上蓝色三角形在前面了，而且从逻辑上三角形就跑到eye后面去了

然后为了不再为这件事困扰，我改成和课程一致了，初始化depth也改成负无穷了

不过lookat是怎么定的？

