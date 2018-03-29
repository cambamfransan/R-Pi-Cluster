var express = require('express');
var appMain = express();
//var appReset = express();
var serverMain = require('http').Server(appMain);
//var serverReset = require('http').Server(appReset);
var fs = require('fs');

appMain.get('/', function(req, res) {
    res.sendFile(__dirname + '/gui/index.html');
});
appMain.use('/gui', express.static(__dirname + '/gui'));

// appReset.get('/', function(req, res) {
// res.sendFile(__dirname + '/reset.html');
// });
// appReset.use('/', express.static(__dirname + '/'));

var net = require('net');

var client = new net.Socket();
client.connect(process.argv[2], '127.0.0.1', function() {
    console.log('Connected');
    client.write('Hello, server! Love, Client.');
});
/*
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
//serverReset.listen(process.env.PORT || 7378); //console.log('Start resetServer'); // REST = 7378

//var systemData;

/*fs.readFile('data/null.txt', 'utf8', function(error, data) {
    if (error) {
        //console.log('Read Error!');
        systemData = null;
    }
    else {
        //console.log('Read Success!');
        systemData = data;
    }
});*/

var ioMain = require('socket.io')(serverMain, {});
ioMain.sockets.on('connection', function(socket) {
//console.log('mainSocket Connected');
//    socket.emit('systemData', systemData);
socket.on('systemData', function(data) {
    console.log("Received Data");
});
//        socket.emit('systemData', systemData);
});

/*var ioReset = require('socket.io') (serverReset,{});
ioReset.sockets.on('connection', function(socket) {
    //console.log('resetSocket Connected');
    socket.on('resetSystem', function(data) {
        fs.unlink('data/null.txt', function(error) {
            if (error) {
                //console.log('Delete Error!');
                socket.emit('clearSystem', false);
            }
            else {
                //console.log('Delete Success!');
                systemData = null;
                ioMain.emit('defaultSystem', systemData);
                socket.emit('clearSystem', true);
            }
        });
    });
});*/