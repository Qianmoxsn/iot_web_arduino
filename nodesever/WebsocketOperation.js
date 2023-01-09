//ws sever set
//https://blog.csdn.net/LiMubai_CN/article/details/81844156
const WebSocket = require('ws');//引入模块
let Serialop = require('./SerialOperation.js');
let wss = null;

function setsever(table) {
    const portname = 8080;
    wss = new WebSocket.Server({port: portname}, function (err) {
            if (err) {
                return console.log('Error: ', err.message)
            } else {
                console.log("WS sever set " + portname);
            }
        }
    )
    ;//创建一个WebSocketServer的实例，监听端口8080


    wss.on('connection', function connection(ws) {

        ws.on('message', function incoming(message) {
            // console.log('received: %s', message);
            if (message.toString() === table["AC ON"]) {
                console.log("[W]op: AC ON");
                sendmsg("[W]"+table["AC ON"]);
                Serialop.writeserial(table["AC ON"]);
            } else if (message.toString() === table["AC OFF"]) {
                console.log("[W]op: AC OFF");
                sendmsg("[W]"+table["AC OFF"]);
                Serialop.writeserial(table["AC OFF"]);
            }
        });//当收到消息时，在控制台打印出来，并回复一条信息


    });
}

function sendmsg(msg) {
    wss.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
            client.send(msg);
        }
    });
}

// Path: WebsocketOperation.js
exports.setsever = setsever;
exports.sendmsg = sendmsg;