# node-gadu
Asynchronous, non-blocking libgadu bindings.

## Requirements

 * libgadu (>= 1.11.2)

# Installing

	npm install gadu
	
# Hacking

See [http://toxygen.net/libgadu/](http://toxygen.net/libgadu/).

## TODO

 * Implement `libuv`'s asynchronous DNS resolver. Forking might cause problems when multiple threads are present.

## Documentation

### Gadu.version()

Gives information about `libgadu`.

	> Gadu.version()
	'1.11.2'

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