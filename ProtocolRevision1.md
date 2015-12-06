# Message format #

Each messages consist of a line terminated by a newline, newlines are
not specified directly since we want to support both \r\n and \n and
perhaps even other newlines. This is done because things like **nix's
telnet programs will write \n as newlines - and we wish to support
that without causing too much fuss.**

The newlines will not be printed in examples on this page, all example
protocol lines end with `\r\n`.

## Request ##

A request can only be sent by a client (SeqZap) and never by a server
(target), a request always gets a corresponding response, even if that
response is just an "success" response.

A request always starts with an ascii character.

```
GET /object/72
```

## Response ##

A response is only sent by the server in response to a request, a
response can be either positive or negative, known as success or error
in this document and the code.

A positive response always begins with a `+` and a negative
response always begins with a `-`.

```
GET /object/72
+ 123
GET /object/0
- "Objects are 1-indexed"
```

## Asynchronous ##

Asynchronous events can be sent by the server at any time to describe an event that happens out of order of the rest of the messages. Asynchronous messages always starts with a `!`.

# Message order #

When a request is made it _must_ be followed by a response for that message by the server. The only messages that can be sent by the server after a request has been made and until the response is sent, are asynchronous events.

Several request can be sent/queued by the client but the server must always send the responses in the correct order.

This is an example of a correct conversation:

```
GET /object/72
GET /object/0
+ 123
!UPDATE /object/12 "value"
- "Objects are 1-indexed"
```

# Object system #

The automation protocol is used to make a remote tree of objects available to the client.

Objects in this tree are either:
  * **property** Readable/writable or both
  * **callable** Callable by the client with zero or more parameters.
  * **list** List of sub-objects.

Every object in the tree can be uniquely identified by their path, this path is built up like a normal directory structure separated by `/`.

The root of the tree is called `/`.

Object names, also known as identifiers are case-sensitive strings of alphanumeric + '_' (underscore), '.' (dot) and '-' (dash) characters._

# Values #

All values are encoded in a format such that the type information can be extracted from a value string without having to know the values before hand.

  * **integer** Integers are encoded as a string of digits like ` 123 `, possible prefixed by a dash to notate negation.
  * **boolean** Booleans are encoded as either ` true ` or ` false `.
  * **null** Null can be encoded as ` null `.
  * **float** Floating point values are encoded as two strings of digits separated by a period, for example: ` 123.456 ` or ` 0.987 `.
  * **string** Strings are encoded as characters strings surrounded by " (quotes). Quotes and backslashes are escaped using backslash. Example ` "This is a test" ` or ` "Test \"this is\"" `.
  * **byte array** A byte stream or any other binary stream of data is represented in hexadecimal prefixed by "0x", for instance: ` 0xFFEEDDCCBBAA99887766554433221100 `.

# Request messages #

## PROTOCOL ##

Get the protocol used by the server, this allows the client to support older servers as well as new ones by adapting the commands used.

### Syntax ###
```
PROTOCOL
```

### Response ###
The server _must_ respond with a success-protocol message.

## GET ##

Get the value of a property.

### Syntax ###
```
GET <object path>
```

### Response ###
The server _must_ respond with a success-with-value message, or an error if it does not work.

### Example ###

```
GET /object/75
```


## SET ##

Set the value of a property.

### Syntax ###

```
SET <object path> <value>
```

### Response ###

The server must respond with either a success message, or an error message of some kind.

### Example ###

```
SET /object/75 456
```

## LIST ##

List the sub-objects of an object.

### Syntax ###
```
LIST <object path>
```

### Response ###
The server must respond with a success-list message, or an error message of some kind.

### Example ###
```
LIST /
+LIST object print
LIST /object
+LIST 12 64 64 75 74
```

## CALL ##

Call a callable and retrieve the result.

### Syntax ###
```
CALL <object path> (<parameter 1> <parameter 2> ... <parameter x>)?
```

### Response ###
The server must return a success-with-value message or a success message if the callable does not return any value. An error message can also be returned which means that the call was not successful.

### Example ###
```
CALL /print "This is a test" 0 3
+
CALL /print "This is a test" -5 3
- "x position must be positive."
```

## INFO ##

Retrieve information about an object in the tree, server must respond with an info response or an error response.

### Syntax ###

```
INFO <object path>
```

### Example ###
```
INFO /object/75
+INFO ReadWriteProperty Integer
```

# Response messages #

## Protocol ##

Is only sent in response to PROTOCOL requests.

### Syntax ###

```
+PROTOCOL <protocol version>
```

## Success ##

This is a simple acknowledgment message, used to say "the last request was successfully executed".

### Syntax ###

```
+
```

## Success-with-a-value ##

Used as a response to a property GET request or a CALL request for a function which returns something.

### Syntax ###

```
+ <value>
```

### Example ###

```
GET /object/75
+ "ostemad"
CALL /function/add 4 5
+ 9
```

## List ##

Sent in response to a LIST request.

### Syntax ###

```
+LIST <identifiers>
```

### Example ###
```
LIST /object
+LIST 12 64 64 75 74
```

## Info ##

Sent in response to a INFO request.

The format of the message is different depending on whether the requested object is a property, list or callable.

### Syntax ###

```
+INFO <object type> [return type] [parm1 type] [parm2 type] ... [parmX type]
```

Where object type is one of:
  * Callable
  * ReadableProperty
  * WritableProperty
  * ReadWriteProperty
  * List

Return type is only valid for properties and callables.

And parameter types are only valid for callables.

Valid types are:
  * Integer
  * Boolean
  * Float
  * String
  * Null
  * ByteArray
  * Any (Special value, used by dynamic languages such as python to indicate that any value goes).
  * Void (Special value, used to indicate callables without a return value.
# Asynchronous messages #

## UPDATE ##

Notifies the client that a property or a list has changed. The new
value is sent as part of the message (if it is a property) because the
value might change on the server before the client is able to retrieve
the value.

Updates can also be sent when a list changes, the new list is not sent
as part of this update, the client must manually make a LIST request
to get the new list.

### Syntax ###

```
!UPDATE <object path> <optional value>
```

### Example ###

```
!UPDATE /object/12 "new value"
!UPDATE /object
```