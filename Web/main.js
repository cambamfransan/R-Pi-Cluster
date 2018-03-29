var express = require('express');
var appMain = express();
var serverMain = require('http').Server(appMain);
var fs = require('fs');

appMain.get('/', function(req, res) {
    res.sendFile(__dirname + '/gui/index.html');
});
appMain.use('/gui', express.static(__dirname + '/gui'));

var net = require('net');

var client = new net.Socket();
client.connect(process.argv[2], '127.0.0.1', function() {
    console.log('Connected');
    client.write('Hello, server! Love, Client.');
});

client.on('data', function(data) {
    var temp = JSON.parse(data.toString());
    switch (temp.type) {
        case "Heartbeat":
            break;
        default:
            console.log("bad message received");
    }
});

client.on("error", function(err) {
    console.log(err.stack)
});

client.on('close', function() {
    console.log('Connection closed');
});

serverMain.listen(process.env.PORT || 8080); //console.log('Start mainServer'); // MAIN = 6246

var ioMain = require('socket.io')(serverMain, {});
ioMain.sockets.on('connection', function(socket) {
    socket.on('systemData', function(data) {
        console.log("Received Data");
    });
});