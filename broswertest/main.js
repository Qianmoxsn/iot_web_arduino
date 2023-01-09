let ws = new WebSocket("ws://localhost:8080");
ws.onopen = function () {
  ws.send("Here is the client! You are connected!");
};
ws.onmessage = function (evt) {
  console.log(evt.data);
  if (evt.data.slice(1, 2) === "T") {
    let temp = evt.data.slice(4, 8);
    document.getElementById("temp").innerHTML = temp;
    let hum = evt.data.slice(9, 11);
    document.getElementById("hum").innerHTML = hum;
  } else if (evt.data.slice(1, 2) === "L" || evt.data.slice(1, 2) === "W") {
    if (evt.data.slice(3,7) == "5678") {
      document.getElementById("btnon").style.backgroundColor = "#a9e089";
      document.getElementById("btnoff").style.backgroundColor = "#f9f9f9";
    } else if (evt.data.slice(3,7) == "1234") {
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
