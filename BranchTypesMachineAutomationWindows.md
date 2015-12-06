# Introduction #

Information about the windows shown on the screens of the machine.

All windows on the screen is modeled as an opaque byte-array which is
mapped to which ever data structure makes the most sense in the
implementation of the machine automation server.

The windows are reference counted, so when a function returns a new
byte-array, it has already been "ref'ed" by the callable - but the
caller must manually make sure to call unref on the window when it is
no longer needed - otherwise the machine automation server will have
to keep the window in memory forever.


# Properties #

| desktops | integer | Number of desktops (screens) attached to the machine. |
|:---------|:--------|:------------------------------------------------------|


# Callables #

| desktop | byte-array ( int ) | Get a reference to desktop with the given index. |
|:--------|:-------------------|:-------------------------------------------------|
| ref     | void ( byte-array ) | Increase the reference counter for the given window by one. |
| unref   | void ( byte-array ) | Decrement the reference counter for the given window by one. |
| name    | string ( byte-array ) | Get the name of the window.                      |
| process\_id | int ( byte-array ) | Get the id of the process which owns the window. |
| text    | string ( byte-array ) | Get the text of the window, e.g. the text on a button or label. |
| role    | string ( byte-array ) | Get the role of the window, is it a "window" or a "button" or a "label" (TODO: Specify which different roles we expect there to be). |
| x       | int ( byte-array ) | Get the top-left X coordinate of the window, in screen coordinates. |
| y       | int ( byte-array ) | Get the top-left Y coordinate of the window, in screen coordinates. |
| width   | int ( byte-array ) | Get width of the window.                         |
| height  | int ( byte-array ) | Get height of the window.                        |
| children | int ( byte-array ) | Get the number of child-windows for a given window. |
| child   | byte-array ( byte-array int ) | Get a reference to the child-window with the given index. |
| actions | int ( byte-array ) | Get the number of actions of the window.         |
| action\_name | string ( byte-array int ) | Get the name of the action with the given index. |
| action\_do | void ( byte-array int ) | Execute the action with the given index.         |
| capture\_screen (optional) | byte-array ( int ) | Capture the screen with the given index and return it in a "recognized" format (BMP, PNG, TGA, etc.) |