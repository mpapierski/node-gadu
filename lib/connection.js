/**
 * Connection class with a base `driver`.
 */
var gadu = require('./bindings')
  , events = require('events')
  , util = require('util');

/**
 * Connection class.
 */
var Connection = function(options) {
	'use strict';
	events.EventEmitter.call(this);
	if (!options.uin || !options.password) {
		throw new Error('Missing uin and password')
	}
	
	var _this = this;

	_this.uin = options.uin;
	_this.password = options.password;
	_this.contacts = options.contacts || [];
	// Gadu-Gadu session object.
	var session
	// Message seq -> callback mapping.
	var outgoingMessages = {};
	// Callback to call when connection is made succesfully.
	var connectCallback;
	/**
	 * Event processor.
	 */
	var eventCallback = function(ev) {
		if (!ev) {
			// Got error
			_this.emit('error', _this);
			return;
		}
		switch (ev.type) {
		case gadu.GG_EVENT_CONN_SUCCESS:
			// Connection successfuly
			session && session.notify(_this.contacts);
			_this.emit('connected', _this.session);
			connectCallback && connectCallback()
			break;
		case gadu.GG_EVENT_CONN_FAILED:
			_this.emit('error');
			break;
		case gadu.GG_EVENT_MSG:
			_this.emit('message', ev.target);
			break;
		case gadu.GG_EVENT_ACK:
			_this.emit('ack', ev.target);
			if (ev.target.seq in outgoingMessages) {
				// Need to execute callback for this seq.
				try {
					outgoingMessages[ev.target.seq](ev.target);
				} finally {
					delete outgoingMessages[ev.target.seq];
				}
			}
			break;
		}
	}
	/**
	 * Connect to.
	 */
	this.connect = function(callback) {
		connectCallback = callback;
		session = new gadu.Session();
		session.login(this.uin, this.password, eventCallback);
	}
	/**
	 * Send a message.
	 */
	this.send = function(uin, text, callback) {
		if (!session) {
			throw new Error('Not connected to a server.')
		}
		var seq = session.send(uin, text);
		if (callback) {
			// Save callback
			outgoingMessages[seq] = callback;
		}
	}
}

Connection.prototype.co = function(){}

util.inherits(Connection, events.EventEmitter);

module.exports = Connection