# 资料

[LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/)

# 入门

## OpenGL

OpenGL的立即渲染模式是什么样的

为什么后来的叫做核心模式

扩展驱动中实现

OpenGL是一个**状态机**，状态即上下文，设置上下文并利用上下文渲染。

- 状态设置函数：改变上下文

- 状态使用函数：利用上下文执行操作

对象

- 创建对象（得到ID）

- 绑定对象至上下文目标位置

- 设置选项（设置保存到对象中）

- 解绑

## 创建窗口

GLFW

- 创建OpenGL上下文

- 定义窗口和处理用户输入

GLAD

- 简化运行时获取函数地址

双缓冲

清空屏幕

```C++
glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数
glClear(GL_COLOR_BUFFER_BIT); // 状态使用函数
```

## 你好，三角形

概念

- 顶点数组对象：Vertex Array Object，VAO
- 顶点缓冲对象：Vertex Buffer Object，VBO
- 索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，IBO

NDC标准化设备坐标（-1.0到1.0）

屏幕空间坐标(Screen-space Coordinates)（与glViewport设置的数据对应）

### 图形渲染管线

![](https://learnopengl-cn.github.io/img/01/04/pipeline.png)

蓝色的是可自定义的部分

顶点输入：顶点数据（顶点属性的数组）

- **顶点缓冲对象**(Vertex Buffer Objects, VBO)：GPU上顶点数据的内存，一批发送大量顶点数据

顶点着色器：

- I：单个顶点属性

- O：转换后的属性，主要是MVP转换3D坐标等，处理顶点属性

- 

图元装配：图元Primitive：GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP……（顶点怎么组合）

- I：所有顶点

- O：装配成指定图元（三角形）

几何着色器：

- I：图元形式的顶点结合

- O：产生新顶点结构，构造新图元【？】

光栅化阶段：

- I：图元

- O：映射为屏幕上的像素

裁切（丢弃超出视图的像素）

片段着色器：（片段：OpenGL中的一个片段是OpenGL渲染一个像素所需的所有数据）

- I：片段

- O：计算像素最终颜色（光照、阴影、光的颜色等）

Alpha测试和混合阶段

- 检测深度，是否丢弃及透明混合

### 对象（VBO）

缓冲对象

```
unsigned int VBO;
glGenBuffers(1, &VBO);
```

VBO缓冲类型是GL_ARRAY_BUFFER

```
glBindBuffer(GL_ARRAY_BUFFER, VBO);  
```

操作GL_ARRAY_BUFFER，把顶点数据复制到缓冲的内存中（glBufferData用来复制数据）

```
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：

- GL_STATIC_DRAW ：数据不会或几乎不会改变。
- GL_DYNAMIC_DRAW：数据会被改变很多。
- GL_STREAM_DRAW ：数据每次绘制时都会改变。

### 着色器（VS、PS）

VS

```glsl
#version 330 core // 版本声明，和GL版本对应
layout (location = 0) in vec3 aPos; // 设定了输入变量的位置值(Location)

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

创建着色器，源码附加到着色器，编译

```
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);

glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);
```

PS

```
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```

编译PS

```
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);
```

链接

```
unsigned int shaderProgram;
shaderProgram = glCreateProgram();
```


