const WebSocket = require("ws");

const ws = new WebSocket("ws://localhost:8080");

ws.on("open", function open() {
  ws.send("5678");
});
//close the connection 1s after the message is sent
setTimeout(function timeout() {
  ws.close();
}, 1000);
