/*
  This file is part of the kcalcore library.

  Copyright (c) 2006,2008 Allen Winter <winter@kde.org>

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

#ifndef TESTTODO_H
#define TESTTODO_H

#include <QObject>

class TodoTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();

    void testValidity();
    void testCompare();
    void testClone();
    void testCopyIncidence();
    void testAssign();
    void testSetCompleted();
    void testStatus();
    void testSerializer_data();
    void testSerializer();
    void testRoles();
};

#endif
