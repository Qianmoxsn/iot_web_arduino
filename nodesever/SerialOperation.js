//serial read
//https://cloud.tencent.com/developer/article/1875599

const SerialPort = require("serialport").SerialPort;

let serialPort = null;


function setupserial(param) {
    serialPort = new SerialPort(
        {
            path: param.portname,
            baudRate: param.baudRate, //波特率
            dataBits: param.dataBits, //数据位
            parity: param.parity, //奇偶校验
            stopBits: param.stopBits, //停止位
            flowControl: param.flowControl,//流控制

        }, function (err) {
            if (err) {
                return console.log('Error: ', err.message)
            } else {
                console.log("serial open at " + param.portname);
                console.log("--------------------------");
            }
        });

}

function listen(table, sendwsmsg) {
    //open serial
    serialPort.on('data', function (data) {
        let order = data.toString('utf8');

        order = order.replace("\r\n", "");
        if (order.slice(1, 2) === "T") {
            console.log("T: " + order.slice(3, 7) + " H: " + order.slice(8, 10));
            sendwsmsg("[T]:" + order.slice(3, 7) + " " + order.slice(8, 10));
        } else if (order.slice(1, 2) === "L") {
            order = order.slice(3, 7);
            if (order === table["AC ON"]) {
                console.log("[L]op: AC ON");
                sendwsmsg("[L]" + table["AC ON"]);
            } else if (order === table["AC OFF"]) {
                console.log("[L]op: AC OFF");
                sendwsmsg("[L]" + table["AC OFF"]);
            }
        }
    })
}


function sendserialmsg(data) {
    serialPort.write(data, (err) => {
        if (err) return console.log('write Error: ', err.message);
    });
}

// Path: SerialOperation.js
module.exports = {
    setupserial,
    sendserialmsg,
    listen
}

