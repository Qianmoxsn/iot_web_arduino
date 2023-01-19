import ipconfig from "../ipconfig.json" assert { type: "json" };

var ws_server_ip_forbroswer = ipconfig.ws_server_ip_forbroswer;
let ws = new WebSocket("ws://" + ws_server_ip_forbroswer + ":8080");

ws.onopen = function () {
  ws.send("[C]You are connected with broswer(test)");
};
ws.onmessage = function (evt) {
  console.log(evt.data);
  if (evt.data.slice(1, 2) === "T") {
    let temp = evt.data.slice(3, 7);
    document.getElementById("temp").innerHTML = temp;
    let hum = evt.data.slice(8, 10);
    document.getElementById("hum").innerHTML = hum;
  } else if (evt.data.slice(1, 2) === "L" || evt.data.slice(1, 2) === "W") {
    if (evt.data.slice(3, 7) == "5678") {
      document.getElementById("btnon").style.backgroundColor = "#a9e089";
      document.getElementById("btnoff").style.backgroundColor = "#f9f9f9";
    } else if (evt.data.slice(3, 7) == "1234") {
      document.getElementById("btnoff").style.backgroundColor = "#ff9d42";
      document.getElementById("btnon").style.backgroundColor = "#f9f9f9";
    }
  }
};

document.getElementById("btnon").onclick = function () {
  ws.send("5678");
};
document.getElementById("btnoff").onclick = function () {
  ws.send("1234");
};
