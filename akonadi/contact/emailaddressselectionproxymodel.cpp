/*
    This file is part of Akonadi Contact.

    Copyright (c) 2010 KDAB
    Author: Tobias Koenig <tokoe@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "emailaddressselectionproxymodel_p.h"

#include <akonadi/item.h>
#include <kabc/addressee.h>
#include <kabc/contactgroup.h>
#include <klocale.h>

using namespace Akonadi;

EmailAddressSelectionProxyModel::EmailAddressSelectionProxyModel( QObject *parent )
  : LeafExtensionProxyModel( parent )
{
}

EmailAddressSelectionProxyModel::~EmailAddressSelectionProxyModel()
{
}

QVariant EmailAddressSelectionProxyModel::data( const QModelIndex &index, int role ) const
{
  const QVariant value = LeafExtensionProxyModel::data( index, role );

  if ( !value.isValid() ) { // index is not a leaf child
    if ( role == NameRole ) {
      const Akonadi::Item item = index.data( ContactsTreeModel::ItemRole ).value<Akonadi::Item>();
      if ( item.hasPayload<KABC::Addressee>() ) {
        const KABC::Addressee contact = item.payload<KABC::Addressee>();
        return contact.realName();
      } else if ( item.hasPayload<KABC::ContactGroup>() ) {
        const KABC::ContactGroup group = item.payload<KABC::ContactGroup>();
        return group.name();
      }
    } else if ( role == EmailAddressRole ) {
      const Akonadi::Item item = index.data( ContactsTreeModel::ItemRole ).value<Akonadi::Item>();
      if ( item.hasPayload<KABC::Addressee>() ) {
        const KABC::Addressee contact = item.payload<KABC::Addressee>();
        return contact.preferredEmail();
      } else if ( item.hasPayload<KABC::ContactGroup>() ) {
        const KABC::ContactGroup group = item.payload<KABC::ContactGroup>();
        return group.name(); // the name must be resolved by the caller
      }
    }
  }

  return value;
}

int EmailAddressSelectionProxyModel::leafRowCount( const QModelIndex &index ) const
{
  const Akonadi::Item item = index.data( ContactsTreeModel::ItemRole ).value<Akonadi::Item>();
  if ( item.hasPayload<KABC::Addressee>() ) {
    const KABC::Addressee contact = item.payload<KABC::Addressee>();
    if ( contact.emails().count() == 1 )
      return 0;
    else
      return contact.emails().count();
  } else if ( item.hasPayload<KABC::ContactGroup>() ) {
    const KABC::ContactGroup group = item.payload<KABC::ContactGroup>();
    return group.dataCount();
  } else {
    return 0;
  }
}

int EmailAddressSelectionProxyModel::leafColumnCount( const QModelIndex &index ) const
{
  const Akonadi::Item item = index.data( ContactsTreeModel::ItemRole ).value<Akonadi::Item>();
  if ( item.hasPayload<KABC::Addressee>() )
    return 1;
  else if ( item.hasPayload<KABC::ContactGroup>() )
    return 1;
  else
    return 0;
}

QVariant EmailAddressSelectionProxyModel::leafData( const QModelIndex &index, int row, int, int role ) const
{
  if ( role == Qt::DisplayRole ) {
    const Akonadi::Item item = index.data( ContactsTreeModel::ItemRole ).value<Akonadi::Item>();
    if ( item.hasPayload<KABC::Addressee>() ) {
      const KABC::Addressee contact = item.payload<KABC::Addressee>();
      if ( row >= 0 && row < contact.emails().count() )
        return contact.emails().at( row );
    } else if ( item.hasPayload<KABC::ContactGroup>() ) {
      const KABC::ContactGroup group = item.payload<KABC::ContactGroup>();
      if ( row >= 0 && row < (int)group.dataCount() )
        return i18nc( "name <email>", "%1 <%2>", group.data( row ).name(), group.data( row ).email() );
    }
  } else if ( role == NameRole ) {
    const Akonadi::Item item = index.data( ContactsTreeModel::ItemRole ).value<Akonadi::Item>();
    if ( item.hasPayload<KABC::Addressee>() ) {
      const KABC::Addressee contact = item.payload<KABC::Addressee>();
      return contact.realName();
    } else if ( item.hasPayload<KABC::ContactGroup>() ) {
      const KABC::ContactGroup group = item.payload<KABC::ContactGroup>();
      if ( row >= 0 && row < (int)group.dataCount() )
        return group.data( row ).name();
    }
  } else if ( role == EmailAddressRole ) {
    const Akonadi::Item item = index.data( ContactsTreeModel::ItemRole ).value<Akonadi::Item>();
    if ( item.hasPayload<KABC::Addressee>() ) {
      const KABC::Addressee contact = item.payload<KABC::Addressee>();
      if ( row >= 0 && row < contact.emails().count() )
        return contact.emails().at( row );
    } else if ( item.hasPayload<KABC::ContactGroup>() ) {
      const KABC::ContactGroup group = item.payload<KABC::ContactGroup>();
      if ( row >= 0 && row < (int)group.dataCount() )
        return group.data( row ).email();
    }
  } else
    return index.data( role );

  return QVariant();
}