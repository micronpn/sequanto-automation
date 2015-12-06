# Introduction #

A simple 2D table with optional row and column headers.

The table below should give a overview of what the different measurements mean:

|                      | _rowHeaderWidth_ | _columnWidth(0)_  | ... | _columnWidth(column)_  |
|:---------------------|:-----------------|:------------------|:----|:-----------------------|
| _columnHeaderHeight_ |                  | **columnHeader(0)** | ... | **columnHeader(column)** |
| _rowHeight(0)_       | **rowHeader(0)**   | cellText(0,0)     | ... | cellText(0,cloumn)     |
| ...                  | ...              |  ...              |     | ...                    |
| _rowHeight(row)_     | **rowHeader(row)** | cellText(row,0)   | ... | cellText(row,column)   |

  * _italic_ values designate sizes (widths and heights).
  * **bold** values are header texts for either rows or columns.

![http://sequanto-automation.googlecode.com/svn/wiki/images/BranchTypesUiTable.png](http://sequanto-automation.googlecode.com/svn/wiki/images/BranchTypesUiTable.png)

# Base #

[Widget](BranchTypesUiWidget.md)

# Properties #

| rows | integer | Number of rows. |
|:-----|:--------|:----------------|
| columns | integer | Number of columns. |
| rowHeaderWidth | integer | Get the width of the row header (0 if there is no row header). |
| columnHeaderHeight | integer | Get the height of the column header (0 if theres is no column header). |
| update | integer | Simple integer which tracks when the table changed, the value is increased every time the table is changed so the other end of the connection knows that all the callables should be refreshed. |

# Callables #

| cellText | string ( integer integer ) | Get the text in cell row, column. |
|:---------|:---------------------------|:----------------------------------|
| rowHeight | integer ( integer )        | Get the height of the given row number. |
| columnWidth | integer ( integer )        | Get the width of the given column number. |
| rowHeader | string ( integer )         | Get the header of the given row number. |
| columnHeader | string ( integer )         | Get the header of the given column number. |