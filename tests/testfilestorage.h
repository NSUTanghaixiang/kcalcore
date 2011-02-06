/*
  This file is part of the kcalcore library.
  Copyright (c) 2007 Allen Winter <winter@kde.org>

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

#ifndef FILESTORAGETEST_H
#define FILESTORAGETEST_H

#include <QtCore/QObject>

class FileStorageTest : public QObject
{
  Q_OBJECT
  private Q_SLOTS:
    void testValidity();
    void testSave();
    void testSaveLoadSave();

    /** Saves an incidence with éèü chars, then reads the file into a second incidence
        and compares both incidences. The comparison should yeld true.
    */
    void testSpecialChars();
};

#endif