////devlop use-----------------
const open_serial = false;
exports.open_serial = open_serial;
////devlop use end

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






