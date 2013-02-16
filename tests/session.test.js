var assert = require('assert')
  , gadu = require('../')
  , Gadu = gadu.Gadu

describe('session', function() {
	describe('prototype', function() {
		it('could be invoked', function() {
			assert(Gadu.Session)
		})
	});
	describe('instance', function() {
		var session;
		beforeEach(function() {
			session = new Gadu.Session()
		})
		it('has login function', function() {
			assert(session.login instanceof Function)
		})
		it('has logoff function', function() {
			assert(session.logoff instanceof Function)
		})
		it('has notify function', function() {
			assert(session.notify instanceof Function)
		})
	})
})