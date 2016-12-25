# node-gadu [![Build Status](https://travis-ci.org/mpapierski/node-gadu.png?branch=master)](https://travis-ci.org/mpapierski/node-gadu)
Asynchronous, non-blocking libgadu bindings.

## Requirements

 * libgadu (>= 1.12.1)
 * libuv (>= 1.10.1)

# Installing

	npm install gadu
	
# Hacking

See [http://toxygen.net/libgadu/](http://toxygen.net/libgadu/).

## TODO

 * ~~Implement `libuv`'s asynchronous DNS resolver. Forking might cause problems when multiple threads are present.~~
 * Implement timeouts for the libuv implementation of resolver.

## Documentation

### Gadu.version()

Gives information about `libgadu`.

	> Gadu.version()
	'1.12.1'

### Events

List of events. The event idents are numeric constants.

#### Gadu.GG_EVENT_CONN_SUCCESS

Connected successfuly.

#### Gadu.GG_EVENT_CONN_FAILED

Connection failure.
	
### Gadu.Session

Gadu-Gadu session object. Use it for server connection.

#### Gadu.Session.login(uin, password, callback)

Connects to Gadu-Gadu server. `callback` is called every time a event is received. In case of an error `callback` is *undefined*.

The `event` passed as a first parameter is object with a attribute `type`.

## Author

Michał Papierski <[michal@papierski.net](michal@papierski.net)>
