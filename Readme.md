# 项目概述
## 项目文件简介
本项目分四个板块，  
`iot`文件夹内为`Arduino`相关代码，作为传感器数据处理。  
`nodesever`文件夹内为使用`node.js`运行的相关代码，主要是建立本地网络服务器，向浏览器端发送由`arduino`收集的串口数据。  
`broswertest`文件夹内为暂时代替`webgpu`的测试环境。  
`webgpu`文件夹内为使用`orillusion`引擎实现的可视化环境。
## 运行方法
### 环境准备
1. 安装`node.js`   
   link:https://nodejs.org/en/
2. 安装`vite`框架(可以参考后文在项目中安装，不必全局安装)   
   link:https://cn.vitejs.dev/  
3. 安装`chrome canary`浏览器   
   link:https://www.google.com/chrome/canary/

具体安装方法参照官方文档
### 运行方式
#### npm安装依赖库
> `npm install`所安装的依赖库为同文件夹下`package.json`文件中写明的依赖库
1. 在`nodeserver`文件夹下，运行`npm install`安装依赖库
2. 在`broswertest`文件夹下，运行`npm install`安装依赖库
3. 在`webgpu`文件夹下，运行`npm install`安装依赖库
#### 浏览器设置（使用webgpu）
1. 在`chrome`浏览器中输入`chrome://flags/#enable-unsafe-webgpu`，将`Enable unsafe WebGPU`设置为`Enabled`
#### 运行程序
1. 在`nodesever`文件夹内找到`searchport.js`文件，使用nodejs运行，运行后会显示当前计算机连接的串口设备。
2. 找到`arduino`所连接的串口号，修改`nodesever/main.js`第7行`portname: "COM4",//串口号`参数为正确的串口号。
> 6-15行参数可按需修改
> ```javascript
> var serialparam = {
> portname: "COM4",//串口号
> baudRate: 9600, //波特率
> dataBits: 8, //数据位
> parity: 'none', //奇偶校验
> stopBits: 1, //停止位
> flowControl: false,//流控制
> };
>
> var wsportname = 8080;
> ```
3. 运行`nodesever/main.js`，运行成功后会显示
```
WS sever set 8080
serial open at COMx
--------------------------
```
4. 在`powershell`中打开`broswertest`文件夹，输入vite命令:`npm run dev`,成功后显示
```
  VITE v4.0.3  ready in 156 ms

  ➜  Local:   http://127.0.0.1:5173/
  ➜  Network: use --host to expose
  ➜  press h to show help
```
5. 浏览器打开链接 http://127.0.0.1:5173/ (以vite输出网址为准)
> 注意：显示WebGPU界面要求浏览器必须是`chrome canary`且打开`Enable unsafe WebGPU`设置