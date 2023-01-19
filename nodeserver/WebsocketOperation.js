//ws server set
//https://blog.csdn.net/LiMubai_CN/article/details/81844156

const WebSocket = require('ws');//引入模块
const devlop = require('./main.js');

let wss = null;

function setupserver(wsportname) {
    const portname = wsportname;
    //创建一个WebSocketServer的实例，监听端口8080
    wss = new WebSocket.Server({port: portname}, function (err) {
            if (err) {
                return console.log('Error: ', err.message)
            } else {
                console.log("WS server set " + portname);
                return wss;
            }
        }
    );
}

function listen(table, sendserialmsg) {
    wss.on('connection', function connection(ws) {
        ws.on('message', function incoming(message) {
            console.log('received: %s', message);

            //webdebugger消息转发
            if(message.toString().slice(0,3) === "[D]"){
                sendwsmsg(message.toString().replace("[D]",""));
            }
            //webdebugger消息转发:end

            if (message.toString() === table["AC ON"]) {
                console.log("[W]op: AC ON");
                sendwsmsg("[W]" + table["AC ON"]);
                if (devlop.open_serial) {
                    sendserialmsg(table["AC ON"]);
                }
            } else if (message.toString() === table["AC OFF"]) {
                console.log("[W]op: AC OFF");
                sendwsmsg("[W]" + table["AC OFF"]);
                if (devlop.open_serial) {
                    sendserialmsg(table["AC OFF"]);
                }
            }
        });//当收到消息时，在控制台打印出来，并回复一条信息
    });
}

function sendwsmsg(msg) {
    wss.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
            client.send(msg);
        }
    });
}

// Path: WebsocketOperation.js
module.exports = {
    setupserver,
    sendwsmsg,
    listen
}