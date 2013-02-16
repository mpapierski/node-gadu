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
		it('has msg', function() {
			assert.equal(Gadu.GG_EVENT_MSG, 1)
		})
	})
	describe('message ack', function() {
		it('has value for delivered', function() {
			assert.equal(Gadu.GG_ACK_DELIVERED, 0x0002)	
		})
		it('has value for queued', function() {
			assert.equal(Gadu.GG_ACK_QUEUED, 0x0003)
		})
		it('has value for blocked', function() {
			assert.equal(Gadu.GG_ACK_BLOCKED, 0x0001)
		})
		it('has value for mboxfull', function() {
			assert.equal(Gadu.GG_ACK_MBOXFULL, 0x0004)	
		})
		it('has value for not delivered', function() {
			assert.equal(Gadu.GG_ACK_NOT_DELIVERED, 0x0006)	
		})
	})
})
