(function(){const n=document.createElement("link").relList;if(n&&n.supports&&n.supports("modulepreload"))return;for(const e of document.querySelectorAll('link[rel="modulepreload"]'))c(e);new MutationObserver(e=>{for(const t of e)if(t.type==="childList")for(const l of t.addedNodes)l.tagName==="LINK"&&l.rel==="modulepreload"&&c(l)}).observe(document,{childList:!0,subtree:!0});function s(e){const t={};return e.integrity&&(t.integrity=e.integrity),e.referrerpolicy&&(t.referrerPolicy=e.referrerpolicy),e.crossorigin==="use-credentials"?t.credentials="include":e.crossorigin==="anonymous"?t.credentials="omit":t.credentials="same-origin",t}function c(e){if(e.ep)return;e.ep=!0;const t=s(e);fetch(e.href,t)}})();const i="10.122.199.209",d="10.122.234.77",f="10.122.220.130",u="127.0.0.1",a={esp32_local_ip:i,ws_server_ip:d,ws_server_ip_dev:f,ws_server_ip_dev_local:u};var m=a.ws_server_ip;let r=new WebSocket("ws://"+m+":8080");r.onopen=function(){r.send("[C]You are connected with broswer(test)")};r.onmessage=function(o){if(console.log(o.data),o.data.slice(1,2)==="T"){let n=o.data.slice(3,7);document.getElementById("temp").innerHTML=n;let s=o.data.slice(8,10);document.getElementById("hum").innerHTML=s}else(o.data.slice(1,2)==="L"||o.data.slice(1,2)==="W")&&(o.data.slice(3,7)=="5678"?(document.getElementById("btnon").style.backgroundColor="#a9e089",document.getElementById("btnoff").style.backgroundColor="#f9f9f9"):o.data.slice(3,7)=="1234"&&(document.getElementById("btnoff").style.backgroundColor="#ff9d42",document.getElementById("btnon").style.backgroundColor="#f9f9f9"))};document.getElementById("btnon").onclick=function(){r.send("5678")};document.getElementById("btnoff").onclick=function(){r.send("1234")};
