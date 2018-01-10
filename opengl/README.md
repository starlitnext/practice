# Opengl Demo

## Step 1. 编译opengl相关的库
### 下载glew、glfw以及freeglut源码
* glew提供opengl的核心功能API以及不同显卡驱动的扩展API
* glfw和freeglut用来通过渲染窗口，以及键盘输入等事件，这里我主要使用glfw
### msvc编译
* 使用CMake生成对应版本的解决方案，注意选择install的位置
* 使用vs进行编译安装

## Step2. vs中配置环境
* 添加以下几个环境变量
	- OPENGL_INCLUDE
	- OPENGL_BIN
	- OPENGL_LIB
* 在Path中添加\%OPENGL_BIN\%
* 在Project中配置
	- 包含目录中添加：\$(OPENG_INCLUDE)
	- 库目中中添加：\$(OPENGL_LIB)

## Step3. opengl 学习资料
### http://learnopengl-cn.readthedocs.io/zh/latest/
### http://www.opengl-tutorial.org/
