var express = require('express');
var appMain = express(); var appReset = express();
var serverMain = require('http').Server(appMain);
var serverReset = require('http').Server(appReset);
var fs = require('fs');

appMain.get('/', function(req, res) {
    res.sendFile(__dirname + '/client/index.html');
});
appMain.use('/client', express.static(__dirname + '/client'));

appReset.get('/', function(req, res) {
    res.sendFile(__dirname + '/reset.html');
});
appReset.use('/', express.static(__dirname + '/'));

serverMain.listen(process.env.PORT || 6246); //console.log('Start mainServer'); // MAIN = 6246
serverReset.listen(process.env.PORT || 7378); //console.log('Start resetServer'); // REST = 7378

var gameScores;

fs.readFile('data/scores.txt', 'utf8', function(error, data) {
    if (error) {
        //console.log('Read Error!');
        gameScores = null;
    }
    else {
        //console.log('Read Success!');
        gameScores = data;
    }
});

var ioMain = require('socket.io') (serverMain,{});
ioMain.sockets.on('connection', function(socket) {
    //console.log('mainSocket Connected');
    socket.emit('gameScores', gameScores);
    socket.on('gameScores', function(data) {
        gameScores = data;
        fs.writeFile('data/scores.txt', JSON.stringify(data), function(error) {
            if (error) {
                //console.log('Write Error!');
            }
            else {
                //console.log('Write Success!');
            }
        });
        socket.emit('gameScores', gameScores);
    });
});

var ioReset = require('socket.io') (serverReset,{});
ioReset.sockets.on('connection', function(socket) {
    //console.log('resetSocket Connected');
    socket.on('resetScores', function(data) {
        fs.unlink('data/scores.txt', function(error) {
            if (error) {
                //console.log('Delete Error!');
                socket.emit('clearScores', false);
            }
            else {
                //console.log('Delete Success!');
                gameScores = null;
                ioMain.emit('defaultScores', gameScores);
                socket.emit('clearScores', true);
            }
        });
    });
});