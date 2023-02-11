//ws server set
//https://blog.csdn.net/LiMubai_CN/article/details/81844156

const WebSocket = require('ws');//引入模块
const devlop = require('./main.js');

let wss = null;
let status = "None";


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
            // console.log('received: %s', message);

            //esp/webdebugger消息转发
            if (message.toString().slice(0, 3) === "[D]") {
                let msg = message.toString().replace("[D]", "")
                sendwsmsg(msg);
                if (msg.slice(0, 3) === "[L]") {
                    if (msg.slice(3, 7) === table["AC ON"]) {
                        console.log("[L]op: AC ON");
                        status = "[L]" + table["AC ON"]
                    } else if (msg.slice(3, 7) === table["AC OFF"]) {
                        console.log("[L]op: AC OFF");
                        status = "[L]" + table["AC OFF"]
                    }

                }
            }
            if(message.toString() === "[C]You are connected with broswer(test)"){
                sendwsmsg(status);
                console.log("Refresh Status:" + status);
            }

            //连接信息显示
            if (message.toString().slice(0, 3) === "[C]") {
                console.log(message.toString().replace("[C]", ""));
            }

            if (message.toString() === table["AC ON"]) {
                console.log("[W]op: AC ON");
                status = "[W]" + table["AC ON"]
                sendwsmsg("[W]" + table["AC ON"]);
                if (devlop.open_serial) {
                    sendserialmsg(table["AC ON"]);
                }
            } else if (message.toString() === table["AC OFF"]) {
                console.log("[W]op: AC OFF");
                status = "[W]" + table["AC OFF"]
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