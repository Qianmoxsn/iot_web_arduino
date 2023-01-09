# 项目概述
## 项目文件简介
本项目分三个板块，`iot`文件夹内为`Arduino`相关代码，作为传感器数据处理。  
`nodesever`文件夹内为使用`node.js`运行的相关代码，主要是建立本地网络服务器，向浏览器端发送由`arduino`收集的串口数据。  
`broswertest`文件夹内为暂时代替`webgpu`的测试环境，可以显示需要上传至webgpu的数据和操作一些事件绑定函数
## 运行方法
### 环境准备
1. 安装`node.js`   
   link:https://nodejs.org/en/
2. 安装`vite`框架   
   link:https://cn.vitejs.dev/  

具体安装方法参照官方文档
### 运行方式
1. 在`nodesever`文件夹内找到`searchport.js`文件，使用nodejs运行，运行后会显示当前计算机连接的串口设备。找到`arduino`所连接的串口号，修改`nodesever/main.js`第11行函数`Serialop.setupserial("COM4");`的传入参数为正确的串口号。
2. 运行`nodesever/main.js`，运行成功后会显示
```
WS sever set 8080
serial open at COMx
--------------------------
```
3. 在`powershell`中打开`broswertest`文件夹，输入vite命令:`npm run dev`,成功后显示
```
  VITE v4.0.3  ready in 156 ms

  ➜  Local:   http://127.0.0.1:5173/
  ➜  Network: use --host to expose
  ➜  press h to show help
```
4. 浏览器打开链接 http://127.0.0.1:5173/ (以vite输出网址为准)