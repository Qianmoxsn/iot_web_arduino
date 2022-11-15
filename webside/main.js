import {createRequire} from 'module';

const require = createRequire(import.meta.url);

//http server
//https://zhuanlan.zhihu.com/p/262907776

import http from "http";

const port = 5678;

const server = http.createServer((req, res) => {
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/plain');
    res.end('你好世界\n');
})

server.listen(port, () => {
    console.log("HTTP sever port:" + port);
});

//serial read
//https://cloud.tencent.com/developer/article/1875599
var SerialPort = require("serialport").SerialPort;
var serialPort = new SerialPort(
    {
        path: 'COM3',
        baudRate: 9600, //波特率
        dataBits: 8, //数据位
        parity: 'none', //奇偶校验
        stopBits: 1, //停止位
        flowControl: false
    }, function (err) {
        if (err) {
            return console.log('Error: ', err.message)
        }
    })

serialPort.on('data', function (data) {
    console.log('serial received: ', data);//data就是你要读取的数据

})


