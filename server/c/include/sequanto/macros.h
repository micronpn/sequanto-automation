/*
 * Copyright 2010-2011 Rasmus Toftdahl Olesen <rasmus@sequanto.com>
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

#ifndef SEQUANTO_MACROS_H_
#define SEQUANTO_MACROS_H_

#ifdef __cplusplus
#   define SQ_BEGIN_DECL extern "C" {
#   define SQ_END_DECL }
#else
#   define SQ_BEGIN_DECL
#   define SQ_END_DECL
#endif

#ifdef SQ_SEQUANTO_AUTOMATION_SHARED
#   ifdef _WIN32
#      ifdef SQ_SEQUANTO_AUTOMATION_SOURCE
#         define SQ_DECL __declspec(dllexport)
#      else
#         define SQ_DECL __declspec(dllimport)
#      endif
#   else
#      define SQ_DECL
#   endif
#else
#   define SQ_DECL
#endif

#endif  /* SEQUANTO_MACROS_H_ */
