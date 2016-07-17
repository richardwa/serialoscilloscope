var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var SerialPort = require("serialport");
    
app.use(express.static('webapp'));
app.get('/', function(req, res){
    res.sendFile(__dirname + '/webapp/scope.html');
});

var port = new SerialPort("/dev/ttyUSB0", {
    baudRate: 115200,
    parser: (function () {
        var data = '', delimiter = 'EOF\n';
        return function(emitter, buffer) {
            var b = new Uint8Array(buffer);
            data += String.fromCharCode.apply(null, b);
            // Split collected data by delimiter
            var parts = data.split(delimiter);
            data = parts.pop();
            parts.forEach(function(part) {
                emitter.emit('data', part);
            });
        };
    })()
});


io.on('connection', function(socket){
    socket.on('message', function(msg){
        port.write(msg);
        io.emit('message', msg);
    });
});

port.on('data', function(msg){
    console.log(msg.length, msg);
    if (msg  === 'READY'){
        port.write('s');
    } 
    
    if (msg.startsWith('DATA')){
        io.emit('data', msg.substring(4));
    } else if (msg.startsWith('{')){
        var fixedJSON = msg.replace(/(['"])?([a-zA-Z0-9_]+)(['"])?:/g, '"$2": ');
        io.emit('status', fixedJSON);
    } else {
        io.emit('message', msg);
    }
});

http.listen(3000, function(){
    console.log('listening on *:3000');
});
