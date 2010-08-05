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

#include <sequanto/ui.h>
#include "config.h"

const char * const SQ_WIDGET_TYPE_UNKNOWN_STRING SQ_CONST_VARIABLE = "Unknown";
const char * const SQ_WIDGET_TYPE_WIDGET_STRING SQ_CONST_VARIABLE = "Widget";
const char * const SQ_WIDGET_TYPE_WINDOW_STRING SQ_CONST_VARIABLE = "Window";
const char * const SQ_WIDGET_TYPE_CONTAINER_STRING SQ_CONST_VARIABLE = "Container";
const char * const SQ_WIDGET_TYPE_BUTTON_STRING SQ_CONST_VARIABLE = "Button";
const char * const SQ_WIDGET_TYPE_LABEL_STRING SQ_CONST_VARIABLE = "Label";
const char * const SQ_WIDGET_TYPE_MENU_BAR_STRING SQ_CONST_VARIABLE = "MenuBar";
const char * const SQ_WIDGET_TYPE_MENU_ITEM_STRING SQ_CONST_VARIABLE = "MenuItem";

const char * const sq_widget_type_to_string ( SQWidgetType _type )
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

   default:
      return SQ_WIDGET_TYPE_UNKNOWN_STRING;
   }
}
