var assert = require('assert')
  , gadu = require('../')
  , Gadu = gadu.Gadu

describe('library', function() {
	it('should return correct version', function() {
		assert.equal(Gadu.version(), '1.11.2')
	})
	describe('events', function() {
		it('has connection success', function() {
			assert.equal(Gadu.GG_EVENT_CONN_SUCCESS, 8)
		})
		it('has connection failed', function() {
			assert.equal(Gadu.GG_EVENT_CONN_FAILED, 7)
		})
	})
})
