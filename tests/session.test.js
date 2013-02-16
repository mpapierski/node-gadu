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
			assert(session.login)
		})
		it('is able to login', function(done) {
			session.login(1234, "test", function(ev) {
				assert(ev)
				assert('type' in ev)
				done()
			})
		})
	})
})