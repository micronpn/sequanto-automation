
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
 *
 */

#ifndef SEQUANTO_QT_SCREEN_POSITION_PROPERTY_H_
#define SEQUANTO_QT_SCREEN_POSITION_PROPERTY_H_

#include <sequanto/readwritepropertynode.h>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class QtScreenPositionProperty : public IntegerPropertyNode
      {
      private:
         QWidget * m_object;
      
      public:
         QtScreenPositionProperty ( const std::string & _name, QWidget * _object );

         virtual const NodeInfo & Info () const;
         virtual int GetValue ( const QPoint & _point ) = 0;
         virtual int GetValue ();
         virtual void SetValue ( int _newValue );
         
         virtual ~QtScreenPositionProperty();
      };
      
      class QtScreenXProperty : public QtScreenPositionProperty
      {
      public:
         QtScreenXProperty ( QWidget * _object );
         virtual int GetValue ( const QPoint & _point );
      };
      
      class QtScreenYProperty : public QtScreenPositionProperty
      {
      public:
         QtScreenYProperty ( QWidget * _object );
         virtual int GetValue ( const QPoint & _point );
      };
   }
}

#endif
