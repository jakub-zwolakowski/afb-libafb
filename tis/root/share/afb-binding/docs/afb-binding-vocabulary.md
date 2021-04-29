# Vocabulary for bindings

## Binding

A micro-service using the current API. A such service is included in
a system in a flexible way, mainly managed outside of it.

Binding made for services can have specific entry points called after
initialization and before serving.

## Event

Messages with data propagated from the services to the client and not expecting
any reply.

The current implementation allows either to widely broadcast events to all clients
or either to client that are interested in specific events.

## Level of assurance (LOA)

This level that can be from 0 to 3 represent the level of
assurance that the services can expect from the session.

The exact definition of the meaning of these levels and how to use it remains to
be achieved.

## Request

A request is an invocation by a client to a binding method using a message
transferred through some protocol.

## Reply/Response

This is a message sent to client as the result of the request.

## Service

Service are made of bindings running on a binder
The binder is in charge of connecting services and applications.
A service can serve many clients.

The framework establishes connection between the services and the clients.
Using sockets currently but other protocols are considered.

The term of service is tightly bound to the notion of API.

## Session

A session is meant to be the unique instance context of a client,
which identify that instance across requests.

When a session is closed or disappears, data attached to that session
are released.
