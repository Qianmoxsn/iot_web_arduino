const SerialPort = require("serialport").SerialPort;

SerialPort.list().then(ports => console.log(ports))
// keep console for 5 seconds
setTimeout(() => {}, 5000)
