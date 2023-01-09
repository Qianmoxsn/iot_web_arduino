const WebSocket = require('ws');


const ws = new WebSocket('ws://localhost:8080');

ws.on('open', function open() {
  ws.send('Here is the client! You are connected!');
});//在连接创建完成后发送一条信息

ws.on('message', function incoming(data) {
  console.log(data.toString('utf8'));
});//当收到消息时，在控制台打印出来

