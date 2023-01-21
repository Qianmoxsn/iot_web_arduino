let ws = new WebSocket("ws://localhost:8080");
var statetext = document.getElementById("text");
var messagetext = document.getElementById("mtext");
ws.onopen = function () {
  ws.send("[C]You are connected with ESP32(test)");
  statetext.innerHTML = "Connected";
};
ws.onmessage = function (evt) {
  var received_msg = evt.data;
  messagetext.innerHTML = received_msg;
  if (received_msg == "[W]5678") {
    document.getElementById("AC").style.backgroundColor = "#a9e089";
    document.getElementById("AC").innerHTML = "ON(WEB)";

    // document.getElementById("btn_off").style.backgroundColor = "#f9f9f9";
  } else if (received_msg == "[W]1234") {
    document.getElementById("AC").style.backgroundColor = "#ff9d42";
    document.getElementById("AC").innerHTML = "OFF(WEB)";
    // document.getElementById("btn_on").style.backgroundColor = "#f9f9f9";
  }
};

function send_on_order() {
  ws.send("[D][L]5678");
  messagetext.innerHTML = "[L]5678";
  document.getElementById("AC").style.backgroundColor = "#a9e089";
  document.getElementById("AC").innerHTML = "ON";
}

function send_off_order() {
  ws.send("[D][L]1234");
  messagetext.innerHTML = "[L]1234";
  document.getElementById("AC").style.backgroundColor = "#ff9d42";
  document.getElementById("AC").innerHTML = "OFF";
}

function send_tem_hum() {
  var tem = document.getElementById("tem").value;
  var hum = document.getElementById("hum").value;
  var msg = "[T]" + tem + "," + hum;
  console.log(msg);
  ws.send("[D]" + msg);
  messagetext.innerHTML = msg;
}

document.getElementById("btn_on").addEventListener("click", send_on_order);
document.getElementById("btn_off").addEventListener("click", send_off_order);
document.getElementById("btn_send").addEventListener("click", send_tem_hum);

document.getElementById("tem").onchange = send_tem_hum;
document.getElementById("hum").onchange = send_tem_hum;
