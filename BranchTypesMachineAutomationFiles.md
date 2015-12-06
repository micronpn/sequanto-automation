# Introduction #

Information about the files on the machine.

# Properties #

| separator | string | The string used to separate path elements. |
|:----------|:-------|:-------------------------------------------|


# Callables #

| number\_of\_directories | int ( string ) | Number of directories in the given path to a directory. |
|:------------------------|:---------------|:--------------------------------------------------------|
| number\_of\_files       | int ( string ) | Number of files in the given path to a directory.       |
| directory\_name         | string ( string int ) | Get the name of directory number "index" in the given path to a directory. |
| file\_name              | string ( string int ) | Get the name of file number "index" in the given path to a directory. |
| size                    | int ( string ) | Get the size  ( in bytes ) of the given file.           |
| read                    | byte-array ( string ) | Get the entire contents of the given file.              |
| write                   | void ( string byte-array ) | Write the given data as the file "path".                |
| create\_directory       | void ( string ) | Create the given directory.                             |
| remove\_directory       | void ( string ) | Remove the given directory path.                        |
| remove\_file            | void ( string ) | Remove the give file.                                   |
