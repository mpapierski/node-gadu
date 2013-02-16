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
	describe('message classes', function() {
		it('has queued', function() {
			assert.equal(Gadu.GG_CLASS_QUEUED, 0x0001)
		})
		it('has offline', function() {
			assert.equal(Gadu.GG_CLASS_OFFLINE, 0x0001)
		})
		it('has msg', function() {
			assert.equal(Gadu.GG_CLASS_MSG, 0x0004)
		})
		it('has chat', function() {
			assert.equal(Gadu.GG_CLASS_CHAT, 0x0008)
		})
		it('has ctcp', function() {
			assert.equal(Gadu.GG_CLASS_CTCP, 0x0010)
		})
		it('has ack', function() {
			assert.equal(Gadu.GG_CLASS_ACK, 0x0020)
		})
		it('has ext', function() {
			assert.equal(Gadu.GG_CLASS_EXT, 0x0020)
		})
	})
	describe('status', function() {
		it('has not avail', function() {
			assert.equal(Gadu.GG_STATUS_NOT_AVAIL, 0x0001);
		})
		it('has not avail descr', function() {
			assert.equal(Gadu.GG_STATUS_NOT_AVAIL_DESCR, 0x0015);
		})
		it('has ffc', function() {
			assert.equal(Gadu.GG_STATUS_FFC, 0x0017);
		})
		it('has ffc descr', function() {
			assert.equal(Gadu.GG_STATUS_FFC_DESCR, 0x0018);
		})
		it('has avail', function() {
			assert.equal(Gadu.GG_STATUS_AVAIL, 0x0002);
		})
		it('has avail descr', function() {
			assert.equal(Gadu.GG_STATUS_AVAIL_DESCR, 0x0004);
		})
		it('has busy', function() {
			assert.equal(Gadu.GG_STATUS_BUSY, 0x0003);
		})
		it('has busy descr', function() {
			assert.equal(Gadu.GG_STATUS_BUSY_DESCR, 0x0005);
		})
		it('has dnd', function() {
			assert.equal(Gadu.GG_STATUS_DND, 0x0021);
		})
		it('has dnd descr', function() {
			assert.equal(Gadu.GG_STATUS_DND_DESCR, 0x0022);
		})
		it('has invisible', function() {
			assert.equal(Gadu.GG_STATUS_INVISIBLE, 0x0014);
		})
		it('has invisible descr', function() {
			assert.equal(Gadu.GG_STATUS_INVISIBLE_DESCR, 0x0016);
		})
		it('has blocked', function() {
			assert.equal(Gadu.GG_STATUS_BLOCKED, 0x0006);
		})
		it('has image mask', function() {
			assert.equal(Gadu.GG_STATUS_IMAGE_MASK, 0x0100)
		})
		it('has descr mask', function() {
			assert.equal(Gadu.GG_STATUS_DESCR_MASK, 0x4000)
		})
		it('has friends mask', function() {
			assert.equal(Gadu.GG_STATUS_FRIENDS_MASK, 0x8000)
		})
		it('has flag unknown', function() {
			assert.equal(Gadu.GG_STATUS_FLAG_UNKNOWN, 0x00000001)
		})
		it('has flag video', function() {
			assert.equal(Gadu.GG_STATUS_FLAG_VIDEO, 0x00000002)
		})
		it('has flag mobile', function() {
			assert.equal(Gadu.GG_STATUS_FLAG_MOBILE, 0x00100000)
		})
		it('has flag spam', function() {
			assert.equal(Gadu.GG_STATUS_FLAG_SPAM, 0x00800000)
		})
	})
})
