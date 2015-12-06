The automation protocol is used to expose a tree of callables and
properties on the automation server. The goal is to create a protocol which is both easy to automate using computers and relatively easy to use for humans.

Another important point is that the tree can be browsed completely just using the protocol, there is no need for an on-disk representation of the tree you can just use the automation protocol to browse it. This also means that once you have a client who can talk to an automation interface it should be able to talk to all automation interfaces, without modification.

  * svn\_revision
  * get\_name
  * clock
    * now
    * nowtext
    * weekday
    * hour
    * minute
    * second
  * temp
    * current
    * heater
    * readtemp
    * readvoltage
    * setpoint
    * day
      * temp
      * clock
    * night
      * used
      * temp
      * clock

The protocol is limited to simple types (integers, doubles, string, etc.) and the protocol format is character based.

The tree above can be used in the following way using the automation protocol:

```
PROTOCOL
+PROTOCOL 1
LIST /
+ svn_revision get_name clock temp
LIST /clock
+ now nowtext weekday hour minute second
GET /clock/hour
+ 6
SET /clock/hour 7
+
GET /clock/hour
+ 7
CALL /svn_revision
+ "r1234"
```

As can be seen the protocol is designed such that requests _always_ gets a response.