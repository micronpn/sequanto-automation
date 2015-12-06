# Introduction #

A simple list of text items with variable height.

# Base #

[Widget](BranchTypesUiWidget.md)

# Properties #

| lines | integer | Number of lines. |
|:------|:--------|:-----------------|
| update | integer | Simple integer which tracks when the list changes, the value is increased every time the list is changed so the other end of the connection knows that all the some of the content in the list changed. |

# Callables #

| lineText | string ( integer ) | Get the text of the given line. |
|:---------|:-------------------|:--------------------------------|
| lineHeight | integer ( integer ) | Get the height of the given line. |