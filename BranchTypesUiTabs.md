# Introduction #

Tab pages.

# Base #

[Widget](BranchTypesUiWidget.md)

# Properties #

| count | integer | Number of tab pages. |
|:------|:--------|:---------------------|
| activeTab | string  | Name of the currently active tab page, or the empty string if none is active. |
| tabHeight | integer | The height of the part of the widget where the tabs are written. |
| tabPosition | string  | North or South based on where the tabs are located. |

# Callables #

| tabName | string ( integer ) | Get the name of the tab page with the given index. |
|:--------|:-------------------|:---------------------------------------------------|
| tabWidth | integer ( integer ) | With of the tab with the given index.              |