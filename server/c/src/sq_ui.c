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

#include "config.h"

#include "sequanto/ui.h"

const char * const SQ_WIDGET_TYPE_UNKNOWN_STRING SQ_CONST_VARIABLE = "Unknown";
const char * const SQ_WIDGET_TYPE_WIDGET_STRING SQ_CONST_VARIABLE = "Widget";
const char * const SQ_WIDGET_TYPE_WINDOW_STRING SQ_CONST_VARIABLE = "Window";
const char * const SQ_WIDGET_TYPE_CONTAINER_STRING SQ_CONST_VARIABLE = "Container";
const char * const SQ_WIDGET_TYPE_BUTTON_STRING SQ_CONST_VARIABLE = "Button";
const char * const SQ_WIDGET_TYPE_LABEL_STRING SQ_CONST_VARIABLE = "Label";
const char * const SQ_WIDGET_TYPE_CHECK_BOX_STRING SQ_CONST_VARIABLE = "CheckBox";
const char * const SQ_WIDGET_TYPE_MENU_BAR_STRING SQ_CONST_VARIABLE = "MenuBar";
const char * const SQ_WIDGET_TYPE_MENU_ITEM_STRING SQ_CONST_VARIABLE = "MenuItem";

const char * const SQ_UI_NODE_WINDOWS SQ_CONST_VARIABLE = "windows";
const char * const SQ_UI_NODE_ACTIVE_WINDOW SQ_CONST_VARIABLE = "activeWindow";
const char * const SQ_UI_NODE_SCREEN SQ_CONST_VARIABLE = "screen";
const char * const SQ_UI_NODE_MOUSE SQ_CONST_VARIABLE = "mouse";
const char * const SQ_UI_NODE_CLICK SQ_CONST_VARIABLE = "click";
const char * const SQ_UI_NODE_CHILDREN SQ_CONST_VARIABLE = "children";
const char * const SQ_UI_NODE_TYPE SQ_CONST_VARIABLE = "type";
const char * const SQ_UI_NODE_X SQ_CONST_VARIABLE = "x";
const char * const SQ_UI_NODE_Y SQ_CONST_VARIABLE = "y";
const char * const SQ_UI_NODE_WIDTH SQ_CONST_VARIABLE = "width";
const char * const SQ_UI_NODE_HEIGHT SQ_CONST_VARIABLE = "height";
const char * const SQ_UI_NODE_ENABLED SQ_CONST_VARIABLE = "enabled";
const char * const SQ_UI_NODE_MOVE SQ_CONST_VARIABLE = "move";
const char * const SQ_UI_NODE_RESIZE SQ_CONST_VARIABLE = "resize";
const char * const SQ_UI_NODE_NATIVE_TYPE SQ_CONST_VARIABLE = "nativeType";
const char * const SQ_UI_NODE_UNNAMED_OBJECTS SQ_CONST_VARIABLE = "unnamedObjects";

const char * const SQ_UI_WINDOW_SCREEN_X SQ_CONST_VARIABLE = "screenX";
const char * const SQ_UI_WINDOW_SCREEN_Y SQ_CONST_VARIABLE = "screenY";

const char * const SQ_UI_NODE_TEXT SQ_CONST_VARIABLE = "text";
const char * const SQ_UI_NODE_CHECKED SQ_CONST_VARIABLE = "checked";

const char * sq_widget_type_to_string ( SQWidgetType _type )
{
   switch ( _type )
   {
   case SQ_WIDGET_TYPE_WIDGET:
      return SQ_WIDGET_TYPE_WIDGET_STRING;

   case SQ_WIDGET_TYPE_WINDOW:
      return SQ_WIDGET_TYPE_WINDOW_STRING;

   case SQ_WIDGET_TYPE_CONTAINER:
      return SQ_WIDGET_TYPE_CONTAINER_STRING;

   case SQ_WIDGET_TYPE_BUTTON:
      return SQ_WIDGET_TYPE_BUTTON_STRING;

   case SQ_WIDGET_TYPE_LABEL:
      return SQ_WIDGET_TYPE_LABEL_STRING;

   case SQ_WIDGET_TYPE_MENU_BAR:
      return SQ_WIDGET_TYPE_MENU_BAR_STRING;

   case SQ_WIDGET_TYPE_MENU_ITEM:
      return SQ_WIDGET_TYPE_MENU_ITEM_STRING;

   case SQ_WIDGET_TYPE_CHECK_BOX:
      return SQ_WIDGET_TYPE_CHECK_BOX_STRING;

   default:
      return SQ_WIDGET_TYPE_UNKNOWN_STRING;
   }
}
