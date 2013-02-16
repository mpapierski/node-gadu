var gadu = require('../lib/gadu.js').Gadu
console.log(gadu.version())
console.log(gadu.GG_EVENT_CONN_SUCCESS);
console.log(gadu.GG_EVENT_CONN_FAILED);
var s1 = new gadu.Session()
s1.login(process.env.UIN, process.env.PASSWORD, function(ev) {
	if (!ev) {
		console.log('error')
		return;
	}
	console.log('event', ev)
	if (ev.type == gadu.GG_EVENT_CONN_SUCCESS) {
		console.log('connection success')
	}
	else if (ev.type == gadu.GG_EVENT_CONN_FAILED) {
		console.log('connection failed')
	}
})