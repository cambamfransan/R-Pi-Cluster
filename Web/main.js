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
