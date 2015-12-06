# Introduction #

The mouse node provides handles for clicking and moving the mouse.

# Callables #

| move  | void ( integer integer ) | Move the mouse to screen position x, y. |
|:------|:-------------------------|:----------------------------------------|
| click | void ( integer integer integer ) | Click the mouse at position x, y - it will click the mouse button described by the last parameter (0: left mouse button, 1: right mouse button, etc., 2: middle mouse button). |

# Monitors #

| capture (optional) | integer integer integer | Captures the clicks of the mouse ( x, y, button ) in the application so they can be captured by a recording tool. Only exists if the application supports capture. |
|:-------------------|:------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------|