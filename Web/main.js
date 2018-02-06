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
  client.write('Hello, server! Love, Client.');
});

client.on('data', function(data) {
  var temp = JSON.parse(data.toString());
switch(temp.type) {
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

app.listen(80);