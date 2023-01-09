let key2operation = {
    "AC ON": "5678",
    "AC OFF": "1234",
    "5678": "AC ON",
    "1234": "AC OFF"
};

let Serialop = require('./SerialOperation.js');
let WS = require('./WebsocketOperation.js');

Serialop.setupserial("COM4");

WS.setsever(key2operation);

Serialop.readserial(key2operation);




