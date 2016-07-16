var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var SerialPort = require("serialport");

var port = new SerialPort("/dev/ttyUSB0", {
    baudRate: 115200,
    //require a custome parser to handle mixed data types
    parser: SerialPort.parsers.readline('EOF\n')
});

app.get('/', function(req, res){
  res.sendFile(__dirname + '/scope.html');
});

io.on('connection', function(socket){
  socket.on('message', function(msg){
      port.write(msg);
      io.emit('message', msg);
  });
});

port.on('data', function (data) {
    io.emit('message', data);
    console.log('serial: ',data.length, data);
    if (data === 'READY'){
        port.write('s');
    }
    debugger;
    //dataMode = data === "DATA";
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});
