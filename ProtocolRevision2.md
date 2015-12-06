# NOT PUBLISHED YET, WILL CHANGE BEFORE IT IS FINALIZED #

[Revision 2](https://code.google.com/p/sequanto-automation/source/detail?r=2) extends [revision 1](https://code.google.com/p/sequanto-automation/source/detail?r=1) with new commands, including some which makes it backwards incompatible with the old format.

You should read ProtocolRevision1 before reading this document.

## Monitors ##

Monitors are a new object type which is introduced in this revision. Monitors functions much like properties, except that they do not have a "current" value.

Monitors only sends !UPDATE asynchronous messages when a new value arrives, but the current value can not be retrieved using a GET request.

Monitors can be used to model e.g. a log messaging system, a log message is only sent when a new log message is available, but there is no current log message.

Monitors can be enabled and disabled, a disabled monitor does not send !UPDATE when they are updated. This is useful to avoid having the connection to flooded with !UPDATE messages for things which the client is not interested in. For instance if we have a monitor for the value of some analog signal which will fluctuate wildly, the monitor can just be disabled.

Monitors are disabled per default.

### ENABLE and DISABLE requests ###

These two new request have been added to enable/disable a monitor.

```
ENABLE /analog_input/value
+
```

```
DISABLE /analog_input/value
+
```

The server sends an acknowledgement as response to a ENABLE or DISABLE request.

### Changes to the INFO command after the addition of monitors ###

A monitor consists of one or more values, which makes them different from properties, which can only contain one value.

```
INFO /analog_input/value
+INFO Monitor Integer Float Enabled
```

This response tells us that the object in question is a monitor, it consists of an integer and a floating point value (e.g. timestamp and value) and the monitor is enabled. The corresponding response when the monitor is disabled is:

```
+INFO Monitor Integer Float Disabled
```


## ` !ADD ` and ` !REMOVE ` asynchronous messages ##

Previously the server could only send an ` !UPDATE ` message to notify the client about a change in the list of children of a node. But that is not an efficient way to synchronize a list of names, since the entire list must be sent for each add/remove of a child node.

In [revision 2](https://code.google.com/p/sequanto-automation/source/detail?r=2) we have added !ADD and !REMOVE messages which can be sent when a child node is added or removed, followed by the full name of the added or removed node.

  * node
    * a
    * b
    * c

```
!ADD /node/d
```

  * node
    * a
    * b
    * c
    * d

```
!REMOVE /node/b
```

  * node
    * a
    * c
    * d

## Parts from [revision 1](https://code.google.com/p/sequanto-automation/source/detail?r=1) which have been deprecated in [revision 2](https://code.google.com/p/sequanto-automation/source/detail?r=2) ##

### ` null ` removed ###

[Revision 1](https://code.google.com/p/sequanto-automation/source/detail?r=1) listed ` null ` as a possible value, this has been removed since it only adds confusion to the specification. It is never mentioned elsewhere in the protocol, so it is not clear where it is allowed.

### Valid types ###

Null is also removed here, what is the point of having a value which can only be null anyway?

The Any data type has been allowed to survive, even though we prefer the automation interface to be as statically typed as possible while still being usable.

The use of Any is not discouraged, we merely have never used it since our tools prefer strongly types values.