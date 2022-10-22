import './style.css'
import javascriptLogo from './javascript.svg'
import {setupCounter} from './counter.js'


document.querySelector('#app').innerHTML = `
  <div>
    <a href="https://vitejs.dev" target="_blank">
      <img src="/vite.svg" class="logo" alt="Vite logo" />
    </a>
    <a href="https://developer.mozilla.org/en-US/docs/Web/JavaScript" target="_blank">
      <img src="${javascriptLogo}" class="logo vanilla" alt="JavaScript logo" />
    </a>
    <h1>Hello Vite!</h1>
    <div class="card">
      <button id="counter" type="button"></button>
    </div>
    <p class="read-the-docs">
      Click on the Vite logo to learn more
    </p>
    <div class="testarea">
        <h2>udp test area</h2>
    </div>
  </div>
`
setupCounter(document.querySelector('#counter'))

//socket.io
//https://blog.csdn.net/niulinbiao/article/details/122329293

import {createServer} from "http";
import Server from "socket.io"
const httpServer = createServer();
const websocserver = new Server(httpServer);
websocserver.on("connection", (socket) => {
    // 自定义接收消息事件
    socket.on('sendMsg',(data) => {
        console.log(data)
        //   定义发送消息事件
        // io表示广播出去，发送给全部的连接
        websocserver.emit('pushMsg',"服务端返回的消息："+data)
    })
});




//udp--
//https://www.yisu.com/zixun/151340.html
import dgram from 'dgram';
const server = dgram.createSocket("udp4");

server.on("message", (msg, rinfo) => {
    console.log("rinfo.address =  " + rinfo.address);
    console.log("rinfo.port =  " + rinfo.port);
    console.log(msg.toString());
});

server.on("listening", () => {
    console.log("address:" + server.address().address);
    console.log("port:" + server.address().port);
});

server.bind("41234");



