var gadu = require('./build/Release/gadu.node')
console.log(gadu.version())

var s1 = new gadu.Session()
s1.login(process.env.UIN, process.env.PWD, function(event) {
	console.log('mam event')
})