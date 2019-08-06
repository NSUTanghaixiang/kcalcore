/*
  This file is part of the kcalcore library.

  Copyright (c) 2002,2003 Cornelius Schumacher <schumacher@kde.org>

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
/**
  @file
  This file is part of the API for handling calendar data and
  defines the FileStorage class.

  @author Cornelius Schumacher \<schumacher@kde.org\>
*/

#ifndef KCALCORE_FILESTORAGE_H
#define KCALCORE_FILESTORAGE_H

#include "kcalendarcore_export.h"
#include "calstorage.h"

namespace KCalendarCore
{

class CalFormat;
class Calendar;

/**
  @brief
  This class provides a calendar storage as a local file.
*/
class KCALENDARCORE_EXPORT FileStorage : public CalStorage
{
    Q_OBJECT
public:

    /**
      A shared pointer to a FileStorage.
    */
    typedef QSharedPointer<FileStorage> Ptr;

    /**
      Constructs a new FileStorage object for Calendar @p calendar with format
      @p format, and storage to file @p fileName.

      @param calendar is a pointer to a valid Calendar object.
      @param fileName is the name of the disk file containing the calendar data.
      @param format is a pointer to a valid CalFormat object that specifies
      the calendar format to be used. FileStorage takes ownership; i.e., the
      memory for @p format is deleted by this destructor. If no format is
      specified, then iCalendar format is assumed.
    */
    explicit FileStorage(const Calendar::Ptr &calendar,
                         const QString &fileName = QString(),
                         KCalendarCore::CalFormat *format = nullptr);

    /**
      Destructor.
    */
    ~FileStorage() override;

    /**
      Sets the name of the file that contains the calendar data.

      @param fileName is the name of the disk file containing the calendar data.
      @see fileName().
    */
    void setFileName(const QString &fileName);

    /**
      Returns the calendar file name.
      @return A QString with the name of the calendar file for this storge.
      @see setFileName().
    */
    Q_REQUIRED_RESULT QString fileName() const;

    /**
      Sets the CalFormat object to use for this storage.

      @param format is a pointer to a valid CalFormat object that specifies
      the calendar format to be used. FileStorage takes ownership.
      @see saveFormat().
    */
    void setSaveFormat(KCalendarCore::CalFormat *format);

    /**
      Returns the CalFormat object used by this storage.
      @return A pointer to the CalFormat object used by this storage.
      @see setSaveFormat().
    */
    CalFormat *saveFormat() const;

    /**
      @copydoc CalStorage::open()
    */
    Q_REQUIRED_RESULT bool open() override;

    /**
      @copydoc CalStorage::load()
    */
    Q_REQUIRED_RESULT bool load() override;

    /**
      @copydoc CalStorage::save()
    */
    Q_REQUIRED_RESULT bool save() override;

    /**
      @copydoc CalStorage::close()
    */
    Q_REQUIRED_RESULT bool close() override;

private:
    //@cond PRIVATE
    Q_DISABLE_COPY(FileStorage)
    class Private;
    Private *const d;
    //@endcond
};

}

#endif
