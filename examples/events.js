var gadu = require('../')
  , Connection = gadu.Connection;

var connection = new Connection({
	uin: process.env.UIN,
	password: process.env.PASSWORD
})

connection.connect(function() {
	console.log('connected')
	connection.on('error', function() {
		console.log('received event error')
	})
	connection.on('message', function(message) {
		console.log('received message', message)
	})
})
