////devlop use-----------------
const open_serial = false;
const enable_user_sever = true;
exports.open_serial = open_serial;
////devlop use end

//set up user broswer(same function with broswertest but in build env)
//web source files(html.css.js) in user_broswer
if (enable_user_sever) {
    const express = require('express');
    const app = express();
    //port is vite default port 5173
    const PORT = 5173;
    app.use(express.static('user_broswer'));
    app.listen(PORT, () => console.log(`user sever on port: ${PORT}`));
}

var serialparam = {
    portname: "COM4",//串口号
    baudRate: 9600, //波特率
    dataBits: 8, //数据位
    parity: 'none', //奇偶校验
    stopBits: 1, //停止位
    flowControl: false,//流控制
};

var wsportname = 8080;

var key2operation = {
    "AC ON": "5678",
    "AC OFF": "1234",
};

let Serialop = require('./SerialOperation.js');
let WSop = require('./WebsocketOperation.js');
//串口启动与串口监听
if (open_serial) {
    Serialop.setupserial(serialparam);
    Serialop.listen(key2operation, WSop.sendwsmsg);
}
//websocket启动与websocket监听
WSop.setupserver(wsportname);
WSop.listen(key2operation, Serialop.sendserialmsg);






