//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    varnotifier.cpp
  \brief   C++ Implementation: varnotifier
  \author  Author Name, 2009
*/
//========================================================================
#include "VarNotifier.h"

namespace VarTypes {
  
  VarNotifier::VarNotifier()
  {
    changed=false;
  }
  
  
  VarNotifier::~VarNotifier()
  {
  }
  
  void VarNotifier::clear() {
    mutex.lock();
    QHash<VarType *, VarNotificationType>::const_iterator iter;
    for (iter = senders.constBegin();iter != senders.constEnd();iter++) {
      internalNonMutexedRemoveItem(iter.key());
    }
    senders.clear();
    mutex.unlock();
  }
  
  bool VarNotifier::hasChanged() {
    bool result=false;
    mutex.lock();
    if (changed) {
      result=true;
      changed=false;
    }
    mutex.unlock();
    return result;
  }
  
  bool VarNotifier::hasChangedNoReset() {
    bool result;
    mutex.lock();
    result=changed;
    mutex.unlock();
    return result;
  }
  
  void VarNotifier::changeSlotOtherChange() {
    mutex.lock();
    changed=true;
    mutex.unlock();
    emit changeOccurred(VarPtr());
  }
  
  void VarNotifier::changeSlot(VarPtr item) {
    mutex.lock();
    changed=true;
    mutex.unlock();
    emit changeOccurred(item);
  }
  
  void VarNotifier::setChanged(bool value) {
    mutex.lock();
    changed=value;
    mutex.unlock();
  }
  
  
  void VarNotifier::internalNonMutexedRemoveItem(VarType * item) {
    if (item==0) return;
    QHash<VarType *, VarNotificationType>::iterator iter;
    iter = senders.find(item);
    if (iter != senders.constEnd()) {
      disconnect(item, 0, this, SLOT(changeSlot(VarPtr)));
      senders.erase(iter);
    }
  }
  
  void VarNotifier::internalNonMutexedAddItem(VarType * item, VarNotificationType notification_type) {
    if (item==0) return;
    QHash<VarType *, VarNotificationType>::iterator iter;
    iter = senders.find(item);
    if (iter != senders.constEnd()) {
      if (iter.value() == notification_type) {
        //this exact item already exists.
        return;
      } else {
        //item exists, but with wrong notification type:
        //fix it:
        disconnect(item, 0, this, SLOT(changeSlot(VarPtr)));
        iter.value() = notification_type;
      }
    } else {
      iter=senders.insert(item,notification_type);
    }
    //connect it with correct notification type:
    if (notification_type==VarNotificationChanged) {
      connect(item, SIGNAL(hasChanged(VarPtr)), this, SLOT(changeSlot(VarPtr)));
    } else {
      connect(item, SIGNAL(wasEdited(VarType*)), this, SLOT(changeSlot(VarPtr)));
    }
  }
  
  void VarNotifier::addItem(VarPtr item, VarNotificationType notification_type) {
    mutex.lock();
    internalNonMutexedAddItem(item.get(), notification_type);
    mutex.unlock();
  }
  
  void VarNotifier::addRecursive(VarPtr item, VarNotificationType notification_type, bool include_root) {
    mutex.lock();
    QQueue<VarType *> queue;
    if (item.get()!=0) queue.enqueue(item.get());
    while(queue.isEmpty()==false) {
      VarType * d = queue.dequeue();
      if ((d!=item.get()) || include_root) internalNonMutexedAddItem(d,notification_type);
      vector<VarPtr> children = d->getChildren();
      int s=children.size();
      for (int i=0;i<s;i++) {
        if (children[i].get()!=0) queue.enqueue(children[i].get());
      }
    }
    mutex.unlock();
  }
  
  void VarNotifier::removeItem(VarPtr item) {
    mutex.lock();
    internalNonMutexedRemoveItem(item.get());
    mutex.unlock();
  }
  
  void VarNotifier::removeRecursive(VarPtr item, bool include_root) {
    mutex.lock();
    QQueue<VarType *> queue;
    if (item.get()!=0) queue.enqueue(item.get());
    while(queue.isEmpty()==false) {
      VarType * d = queue.dequeue();
      if ((d!=item.get()) || include_root) internalNonMutexedRemoveItem(item.get());
      vector<VarPtr> children = d->getChildren();
      int s=children.size();
      for (int i=0;i<s;i++) {
        if (children[i].get()!=0) queue.enqueue(children[i].get());
      }
    }
    mutex.unlock();
  }
};
