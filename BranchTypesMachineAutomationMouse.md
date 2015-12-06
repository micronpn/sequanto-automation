# Introduction #

Simulating mouse events.

# Callables #

| click\_at | void ( int int int ) | Simulate a mouse click at positon (x, y) with the button number as the third argument (0 is left-click, 1 is right-click, 2 is middle-click) |
|:----------|:---------------------|:---------------------------------------------------------------------------------------------------------------------------------------------|
| hold\_at (optional) | void ( integer integer integer ) | Press and hold the mouse button at position x, y - the last parameter is the button to press and hold.                                       |
| release\_at (optional) | void ( integer integer integer ) | Release the previously held mouse button at position x, y - the last parameter is the button to release.                                     |