/*=========================================================================

Copyright (c) 1998-2003 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/
#include "qtItem.h"

#include "qtUIManager.h"
#include "attribute/Item.h"
#include "attribute/ItemDefinition.h"

#include <QPointer>
#include <QLayout>
#include <QWidget>

using namespace slctk::attribute;

//----------------------------------------------------------------------------
class qtItemInternals
{
public:
  qtItemInternals(slctk::AttributeItemPtr dataObject, QWidget* p)
  {
  this->ParentWidget = p;
  this->DataObject = dataObject;
  }
  ~qtItemInternals()
  {
  }
 slctk::WeakAttributeItemPtr DataObject;
 QPointer<QWidget> ParentWidget;
 QList<slctk::attribute::qtItem*> ChildItems;
};


//----------------------------------------------------------------------------
qtItem::qtItem(slctk::AttributeItemPtr dataObject, QWidget* p)
{ 
  this->Internals  = new qtItemInternals(dataObject, p); 
  this->Widget = NULL;
  this->IsLeafItem = false;
  //this->Internals->DataConnect = NULL;
  //this->createWidget();
}

//----------------------------------------------------------------------------
qtItem::~qtItem()
{
  this->clearChildItems();
  if (this->Internals)
    {
    if(this->Internals->ParentWidget && this->Widget
      && this->Internals->ParentWidget->layout())
      {
      this->Internals->ParentWidget->layout()->removeWidget(this->Widget);
      }
    delete this->Internals;
    }
}

//----------------------------------------------------------------------------
void qtItem::addChildItem(qtItem* child)
{
  if(!this->Internals->ChildItems.contains(child))
    {
    this->Internals->ChildItems.append(child);
    }
}
//----------------------------------------------------------------------------
QList<qtItem*>& qtItem::childItems() const
{
  return this->Internals->ChildItems;
} 
//----------------------------------------------------------------------------
void qtItem::clearChildItems()
{
  for(int i=0; i < this->Internals->ChildItems.count(); i++)
    {
    delete this->Internals->ChildItems.value(i);
    }
  this->Internals->ChildItems.clear();
}

//----------------------------------------------------------------------------
slctk::AttributeItemPtr qtItem::getObject()
{
  return this->Internals->DataObject.lock();
}

//----------------------------------------------------------------------------
QWidget* qtItem::parentWidget()
{
  return this->Internals->ParentWidget;
}

//----------------------------------------------------------------------------
bool qtItem::passAdvancedCheck(bool advancedContainer)
{
  slctk::AttributeItemPtr dataObj = this->getObject();
  return qtUIManager::instance()->passItemAdvancedCheck(
    advancedContainer,dataObj->definition()->advanceLevel());
}