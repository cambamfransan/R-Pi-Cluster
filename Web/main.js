var express = require('express');
var appMain = express();
var serverMain = require('http').Server(appMain);
var fs = require('fs');
var mySockets = [];

appMain.get('/', function(req, res) {
    res.sendFile(__dirname + '/gui/index.html');
});
appMain.use('/gui', express.static(__dirname + '/gui'));

var net = require('net');
var nextConvId = 1;

var client = new net.Socket();
client.connect(process.argv[2], '127.0.0.1', function() {
    console.log('Connected');
    client.write(JSON.stringify({
        convId: '0',
        msg: 'Hello, server! Love, Client.'
    }));
});

client.on('data', function(data) {
    var msgs = data.toString();
    var msg;
    console.log("received data" + mySockets.length);
    while (msgs) {
        var index = msgs.indexOf("~");
        if(index == -1)index = msgs.length;
        msg = msgs.substring(0, index);
        msgs = msgs.substring(index+1);
        var temp = JSON.parse(msg.toString());
        switch (temp.MsgType) {
            case "Heartbeat":
                console.log("Recieved Heartbeat");
                client.write(JSON.stringify({MsgType:'HeartbeatAck', convId: temp.convId}) + '~');
                break;
            case 'AddJobAck':
                console.log("received AddJobAck" + mySockets.length);
                for(var i = 0; i < mySockets.length; i++)
                    mySockets[i].emit('AddJobAck', temp.JobId);
                break;
            default:
                console.log("bad message received");
        }
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
    mySockets.push(socket);
    console.log('connected');
    socket.on('systemData', function(data) {
        switch(data.MsgType) {
            case 'AddJob':
                data.convId = nextConvId++;
                client.write(JSON.stringify(data) + '~');
                console.log("Adding Job");
                break;
        }
    });
});
