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

