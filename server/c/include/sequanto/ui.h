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

#include <sequanto/types.h>

#ifndef SEQUANTO_UI_H_
#define SEQUANTO_UI_H_

/**
 * \file ui.h
 * \brief Standard values and types used when exposing a UI tree.
 */

#ifdef __cplusplus
extern "C"
{
#endif

   typedef enum
   {
      SQ_WIDGET_UNKNOWN,
      SQ_WIDGET_WIDGET,
      SQ_WIDGET_WINDOW,
      SQ_WIDGET_CONTAINER,
      SQ_WIDGET_BUTTON,
      SQ_WIDGET_LABEL,
      SQ_WIDGET_MENU_BAR,
      SQ_WIDGET_MENU_ITEM,
   } SQWidgetType;
   
   extern const char * const SQ_WIDGET_UNKNOWN_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_WIDGET_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_WINDOW_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_CONTAINER_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_BUTTON_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_LABEL_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_MENU_BAR_STRING SQ_CONST_VARIABLE;
   extern const char * const SQ_WIDGET_MENU_ITEM_STRING SQ_CONST_VARIABLE;
   
#ifdef __cplusplus
}
#endif

#endif
