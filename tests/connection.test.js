var gadu = require('../')
  , Session = gadu.Gadu.Session
  , sinon = require('sinon')
  , assert = require('assert');

describe('connection', function() {
	'use strict'
	var connection;
	var orig
	// Mocked Gadu
	beforeEach(function() {
		orig = Session.prototype
		connection = new gadu.Connection({
			uin: 1234,
			password: 'password'
		})
	})
	afterEach(function() {
		Session.prototype = orig
	})
	it('connects', function(done) {
		Session.prototype.login = sinon.spy();
		connection.connect(done)
		assert(Session.prototype.login.calledWith(1234, 'password', sinon.match.any))
		// get callback
		var args = Session.prototype.login.getCall(0).args;
		assert.equal(args.length, 3)
		args[2]({
		 	type: gadu.Gadu.GG_EVENT_CONN_SUCCESS,
		 	target: {}
		});
	})
})