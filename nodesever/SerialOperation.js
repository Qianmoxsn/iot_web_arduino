//serial read
//https://cloud.tencent.com/developer/article/1875599

const SerialPort = require("serialport").SerialPort;
let serialPort = null;

let WS = require('./WebsocketOperation.js');

function setupserial(portname) {
    serialPort = new SerialPort(
        {
            path: portname,
            baudRate: 9600, //波特率
            dataBits: 8, //数据位
            parity: 'none', //奇偶校验
            stopBits: 1, //停止位
            flowControl: false,

        }, function (err) {
            if (err) {
                return console.log('Error: ', err.message)
            } else {
                console.log("serial open at " + portname);
                console.log("--------------------------");
            }
        });

}

function readserial(table) {
    //open serial
    serialPort.on('data', function (data) {
        let order = data.toString('utf8');

        order = order.replace("\r\n", "");
        if (order.slice(1, 2) === "T") {
            console.log("T: " + order.slice(3, 7) + " H: " + order.slice(8, 10));
            //WS.sendmsg("[T]: " + order.slice(3, 7) + order.slice(8, 10));
        }
        else if (order.slice(1, 2) === "L") {
            order = order.slice(3, 7);
            if (order === table["AC ON"]) {
                console.log("[L]op: AC ON");
                WS.sendmsg("[L]op: AC ON");
            } else if (order === table["AC OFF"]) {
                console.log("[L]op: AC OFF");
                WS.sendmsg("[L]op: AC OFF");
            }
        }
    })
}


function writeserial(data) {
    serialPort.write(data, (err) => {
        if (err) return console.log('write Error: ', err.message);
    })
}

// Path: SerialOperation.js
exports.setupserial = setupserial;
exports.readserial = readserial;
exports.writeserial = writeserial;

