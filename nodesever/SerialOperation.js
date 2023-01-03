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
            }
        });

}

function readserial(table) {
    //open serial
    serialPort.on('data', function (data) {
        let order = data.toString('utf8');

        order = order.replace("\r\n", "");

        if (order === table["AC ON"]) {
            console.log("operation: AC ON");
            WS.sendmsg("sending order: AC ON");
        } else if (order === table["AC OFF"]) {
            console.log("operation: AC OFF");
            WS.sendmsg("sending order: AC OFF");
        }
    })
}


function writeserial(data) {
    serialPort.write(data, function (err, results) {
        console.log('serial send:', '>>', data.toString('utf8'));
    });
}

// Path: SerialOperation.js
exports.setupserial = setupserial;
exports.readserial = readserial;
exports.writeserial = writeserial;

