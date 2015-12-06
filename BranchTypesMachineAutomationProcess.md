# Introduction #

Information about the running processes on the machine.

The process information is only updated/fetched when refresh is called, otherwise the information is static.

# Properties #

| count | int | Number of processes. |
|:------|:----|:---------------------|


# Callables #

| refresh | void() | Refresh the cached process list. |
|:--------|:-------|:---------------------------------|
| id      | int(int) | Get the ID of the process with the given index. |
| name    | string(int) | Get the name of the process with the given index. |
| filename | string(int) | Get the filename of the process with the given index. |
| owner   | int(int) | Get the numeric owner of the process. |
| cmdline | string(int) | Get the command line used to start the process. |
| memory  | int(int) | Get the memory usage of the process. |