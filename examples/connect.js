var libgadu = require('../lib/gadu.js')
   , gadu = libgadu.Gadu
var s1 = new gadu.Session()

s1.login(process.env.UIN, process.env.PASSWORD, function(ev) {
	if (!ev) {
		console.log('error')
		return;
	}
	console.log('event', ev)
	if (ev.type == gadu.GG_EVENT_CONN_SUCCESS) {
		console.log('connection success')
		s1.notify();
	}
	else if (ev.type == gadu.GG_EVENT_CONN_FAILED) {
		console.log('connection failed')
	}
	else if (ev.type == gadu.GG_EVENT_MSG) {
		var text = ev.target.message;
		if (text == "busy1") {
			s1.changeStatus(gadu.GG_STATUS_BUSY);
		} else if (text == "busy2") {
			s1.changeStatus(gadu.GG_STATUS_BUSY_DESCR, "hello world");
		}
	}
});
