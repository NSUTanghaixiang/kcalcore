/*
    This file is part of the kcal library.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef KCAL_RESOURCECACHEDCONFIG_H
#define KCAL_RESOURCECACHEDCONFIG_H

#include <QWidget>
#include "kcal.h"

class Q3ButtonGroup;
class QSpinBox;

namespace KCal {

class ResourceCached;

/**
  Configuration widget for reload policy

  @see ResourceCached
*/
class KCAL_EXPORT ResourceCachedReloadConfig : public QWidget
{
  Q_OBJECT
  public:
    explicit ResourceCachedReloadConfig( QWidget *parent = 0 );

  public Q_SLOTS:
    void loadSettings( ResourceCached *resource );
    void saveSettings( ResourceCached *resource );

  protected Q_SLOTS:
    void slotIntervalStateChanged( int );

  private:
    Q3ButtonGroup *mGroup;
    QSpinBox *mIntervalSpin;

    class Private;
    Private *const d;
};

/**
  Configuration widget for save policy

  @see ResourceCached
*/
class KCAL_EXPORT ResourceCachedSaveConfig : public QWidget
{
    Q_OBJECT
  public:
    explicit ResourceCachedSaveConfig( QWidget *parent = 0 );

  public Q_SLOTS:
    void loadSettings( ResourceCached *resource );
    void saveSettings( ResourceCached *resource );

  protected Q_SLOTS:
    void slotIntervalStateChanged( int );

  private:
    Q3ButtonGroup *mGroup;
    QSpinBox *mIntervalSpin;

    class Private;
    Private *const d;
};

}

#endif
