<<<<<<< HEAD
var express = require('express');
var appMain = express(); var appReset = express();
var serverMain = require('http').Server(appMain);
var serverReset = require('http').Server(appReset);
var fs = require('fs');

appMain.get('/', function(req, res) {
    res.sendFile(__dirname + '/gui/index.html');
});
appMain.use('/gui', express.static(__dirname + '/gui'));

appReset.get('/', function(req, res) {
    res.sendFile(__dirname + '/reset.html');
});
appReset.use('/', express.static(__dirname + '/'));

serverMain.listen(process.env.PORT || 6246); //console.log('Start mainServer'); // MAIN = 6246
serverReset.listen(process.env.PORT || 7378); //console.log('Start resetServer'); // REST = 7378

var systemData;

fs.readFile('data/null.txt', 'utf8', function(error, data) {
    if (error) {
        //console.log('Read Error!');
        systemData = null;
    }
    else {
        //console.log('Read Success!');
        systemData = data;
    }
});

var ioMain = require('socket.io') (serverMain,{});
ioMain.sockets.on('connection', function(socket) {
    //console.log('mainSocket Connected');
    socket.emit('systemData', systemData);
    socket.on('systemData', function(data) {
        systemData = data;
        fs.writeFile('data/null.txt', JSON.stringify(data), function(error) {
            if (error) {
                //console.log('Write Error!');
            }
            else {
                //console.log('Write Success!');
            }
        });
        socket.emit('systemData', systemData);
    });
});

var ioReset = require('socket.io') (serverReset,{});
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
});
=======
const express = require('express');
const app = express();
// var bodyParser = require('body-parser');

// configure app to use bodyParser()
// this will let us get the data from a POST
// app.use(bodyParser.urlencoded({ extended: true }));
// app.use(bodyParser.json());

// var router = express.Router(); 

app.get('/', (req, res) => res.sendFile(__dirname + '/MainPage.html'));

app.get('/pis', (req, res) => res.sendFile(__dirname + '/PiPage.html'));

// app.get('/api/1', function(req, res) { // example api
//   res.json({ message: 'hooray! welcome to our api!', value: 1 });   
// });

var net = require('net');

var client = new net.Socket();
client.connect(process.argv[2], '127.0.0.1', function() {
    console.log('Connected');
    client.write(JSON.stringify({
        convId: '0',
        msg: 'Hello, server! Love, Client.'
    }));
    console.log(JSON.stringify({
        convId: '0',
        msg: 'Hello, server! Love, Client.'
    }));
});

client.on('data', function(data) {
    var msgs = data.toString();
    var msg;
    while (msgs) {
        var index = msgs.indexOf("~");
        if(index == -1)index = msgs.length;
        msg = msgs.substring(0, index);
        msgs = msgs.substring(index+1);
        var temp = JSON.parse(msg.toString());
        switch (temp.MsgType) {
            case "Heartbeat":
            console.log("Recieved Heartbeat");
                client.write(JSON.stringify({MsgType:'HeartbeatAck', convId: temp.convId}));
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

app.listen(8080);
>>>>>>> master
