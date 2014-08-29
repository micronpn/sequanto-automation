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
##SQ_WIDGET_TYPES##
   } SQWidgetType;

##SQ_WIDGET_TYPES_VARIABLES##

   SQ_DECL const char * sq_widget_type_to_string ( SQWidgetType _widgetType );

##SQ_WIDGET_NODES_VARIABLES##

SQ_END_DECL

#endif
