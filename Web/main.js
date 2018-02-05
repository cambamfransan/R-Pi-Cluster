const express = require('express')
const app = express()

app.get('/', (req, res) => res.sendFile(__dirname + '/MainPage.html'));

app.get('/pis', (req, res) => res.sendFile(__dirname + '/PiPage.html'));

app.listen(80);