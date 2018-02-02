
var http = require('http');
var url = require('url');
var fs = require("fs");

var mainPage = function(response) {
  return 'Main Page';
}

var piPage = function() {
  return 'Raspberry pis';
}

var server = function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});
  var q = url.parse(req.url, true).query;
  var txt;
  if(req.url === '/')txt = mainPage(res);
  else if(req.url == '/pi')txt = piPage();
  res.end(txt);
};

http.createServer(server).listen(8080); //the server object listens on port 8080
