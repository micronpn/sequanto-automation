/*
 * Copyright 2010 Rasmus Toftdahl Olesen <rasmus@sequanto.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef SEQUANTO_UI_H_
#define SEQUANTO_UI_H_

#include <sequanto/macros.h>
#include <sequanto/types.h>

/**
 * \file ui.h
 * \brief Standard values and types used when exposing a UI tree.
 */

SQ_BEGIN_DECL

   typedef enum
   {
      SQ_WIDGET_TYPE_UNKNOWN,
      SQ_WIDGET_TYPE_WIDGET,
      SQ_WIDGET_TYPE_CONTAINER,
      SQ_WIDGET_TYPE_BUTTON,
      SQ_WIDGET_TYPE_LABEL,
      SQ_WIDGET_TYPE_CHECK_BOX,
      SQ_WIDGET_TYPE_RADIO_BUTTON,
      SQ_WIDGET_TYPE_TEXT_BOX,
      SQ_WIDGET_TYPE_MENU_BAR,
      SQ_WIDGET_TYPE_MENU_ITEM,
      SQ_WIDGET_TYPE_STATUS_BAR,
      SQ_WIDGET_TYPE_SLIDER,
      SQ_WIDGET_TYPE_SCROLL_AREA,
      SQ_WIDGET_TYPE_TABLE,
      SQ_WIDGET_TYPE_TABS,
      SQ_WIDGET_TYPE_TOOL_BAR,
      SQ_WIDGET_TYPE_PROGRESS_BAR,
      SQ_WIDGET_TYPE_LIST
   } SQWidgetType;

   extern const char * const SQ_WIDGET_TYPE_UNKNOWN_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_WIDGET_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_CONTAINER_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_BUTTON_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_LABEL_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_CHECK_BOX_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_RADIO_BUTTON_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_TEXT_BOX_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_MENU_BAR_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_MENU_ITEM_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_STATUS_BAR_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_SLIDER_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_SCROLL_AREA_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_TABLE_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_TABS_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_TOOL_BAR_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_PROGRESS_BAR_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_TYPE_LIST_STRING SQ_CONST_VARIABLE;

   SQ_DECL const char * sq_widget_type_to_string ( SQWidgetType _widgetType );

   extern const char * const SQ_UI_BUTTON_TEXT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_CHECK_BOX_TEXT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_CHECK_BOX_CHECKED SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_NATIVE_TYPE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_NATIVE_ID SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_WIDTH SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_HEIGHT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_VISIBLE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_CHILDREN SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_AUTO_GENERATED_NAME SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_COMMON_BASE_ENABLED SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_LABEL_TEXT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_LIST_LINES SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_LIST_UPDATE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_LIST_LINE_TEXT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_LIST_LINE_HEIGHT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_MOUSE_MOVE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_MOUSE_CLICK SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_RADIO_BUTTON_TEXT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_RADIO_BUTTON_CHECKED SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_ROOT_WINDOWS SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_ROOT_ACTIVE_WINDOW SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_ROOT_SCREEN SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_ROOT_MOUSE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_SCREEN_WIDTH SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_SCREEN_HEIGHT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_SLIDER_VALUE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_SLIDER_MINIMUM SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_SLIDER_MAXIMUM SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_ROWS SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_COLUMNS SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_ROW_HEADER_HEIGHT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_COLUMN_HEADER_WIDTH SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_UPDATE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_CELL_TEXT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_ROW_HEIGHT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_COLUMN_WIDTH SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_ROW_HEADER SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABLE_COLUMN_HEADER SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABS_COUNT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABS_ACTIVE_TAB SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TABS_TAB_NAME SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_TEXT_BOX_TEXT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_WIDGET_TYPE SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_WIDGET_X SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_WIDGET_Y SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_WIDGET_INPUT SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_WINDOW_SCREEN_X SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_WINDOW_SCREEN_Y SQ_CONST_VARIABLE;
   extern const char * const SQ_UI_WINDOW_BACKGROUND_COLOR SQ_CONST_VARIABLE;


SQ_END_DECL

#endif
