/*
  This file is part of the kcalcore library.

  Copyright (C) 2006,2008 Allen Winter <winter@kde.org>

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
#include "testevent.h"
#include "event.h"
#include "todo.h"

#include <QTest>
QTEST_MAIN(EventTest)

Q_DECLARE_METATYPE(KCalendarCore::Incidence::DateTimeRole)

using namespace KCalendarCore;

void EventTest::testSetRoles_data()
{
    QTest::addColumn<QDateTime>("originalDtStart");
    QTest::addColumn<QDateTime>("originalDtEnd");
    QTest::addColumn<bool>("allDayEvent");

    QTest::addColumn<KCalendarCore::Incidence::DateTimeRole>("setRole");
    QTest::addColumn<QDateTime>("dateTimeToSet");
    QTest::addColumn<QDateTime>("expectedDtStart");
    QTest::addColumn<QDateTime>("expectedDtEnd");

    const QDateTime todayDate(QDate::currentDate(), {});   // all day event
    const QDateTime todayDateTime = QDateTime::currentDateTimeUtc();

    QTest::newRow("dnd 0 duration") << todayDate << todayDate << true << KCalendarCore::Incidence::RoleDnD
                                    << todayDateTime << todayDateTime << todayDateTime.addSecs(3600);
}

void EventTest::testSetRoles()
{
    QFETCH(QDateTime, originalDtStart);
    QFETCH(QDateTime, originalDtEnd);
    QFETCH(bool, allDayEvent);
    QFETCH(KCalendarCore::Incidence::DateTimeRole, setRole);

    QFETCH(QDateTime, dateTimeToSet);
    QFETCH(QDateTime, expectedDtStart);
    QFETCH(QDateTime, expectedDtEnd);

    Event::Ptr event = Event::Ptr(new Event());
    event->setDtStart(originalDtStart);
    event->setDtEnd(originalDtEnd);
    event->setAllDay(allDayEvent);

    event->setDateTime(dateTimeToSet, setRole);
    QCOMPARE(event->dtStart(), expectedDtStart);
    QCOMPARE(event->dtEnd(), expectedDtEnd);
}

void EventTest::testValidity()
{
    QDate dt = QDate::currentDate();
    Event *event = new Event();
    event->setDtStart(QDateTime(dt, {}));
    event->setDtEnd(QDateTime(dt, {}).addDays(1));
    event->setSummary(QStringLiteral("Event1 Summary"));
    event->setDescription(QStringLiteral("This is a description of the first event"));
    event->setLocation(QStringLiteral("the place"));

    QCOMPARE(event->summary(), QStringLiteral("Event1 Summary"));
    QCOMPARE(event->location(), QStringLiteral("the place"));
    QCOMPARE(event->type(), Incidence::TypeEvent);
}

void EventTest::testCompare()
{
    QDate dt = QDate::currentDate();
    Event event1;
    event1.setDtStart(QDateTime(dt, {}));
    event1.setDtEnd(QDateTime(dt, {}).addDays(1));
    event1.setSummary(QStringLiteral("Event1 Summary"));
    event1.setDescription(QStringLiteral("This is a description of the first event"));
    event1.setLocation(QStringLiteral("the place"));

    Event event2;
    event2.setDtStart(QDateTime(dt, {}).addDays(1));
    event2.setDtEnd(QDateTime(dt, {}).addDays(2));
    event2.setSummary(QStringLiteral("Event2 Summary"));
    event2.setDescription(QStringLiteral("This is a description of the second event"));
    event2.setLocation(QStringLiteral("the other place"));

    QVERIFY(!(event1 == event2));
    QCOMPARE(event1.dtEnd(), event2.dtStart());
    QCOMPARE(event2.summary(), QStringLiteral("Event2 Summary"));
}

void EventTest::testClone()
{
    QDate dt = QDate::currentDate();
    Event event1;
    event1.setDtStart(QDateTime(dt, {}));
    event1.setDtEnd(QDateTime(dt, {}).addDays(1));
    event1.setSummary(QStringLiteral("Event1 Summary"));
    event1.setDescription(QStringLiteral("This is a description of the first event"));
    event1.setLocation(QStringLiteral("the place"));

    Event *event2 = event1.clone();
    QCOMPARE(event1.summary(), event2->summary());
    QCOMPARE(event1.dtStart(), event2->dtStart());
    QCOMPARE(event1.dtEnd(), event2->dtEnd());
    QCOMPARE(event1.description(), event2->description());
    QCOMPARE(event1.location(), event2->location());
}

void EventTest::testCopy()
{
    QDate dt = QDate::currentDate();
    Event event1;
    event1.setDtStart(QDateTime(dt, {}));
    event1.setDtEnd(QDateTime(dt, {}).addDays(1));
    event1.setSummary(QStringLiteral("Event1 Summary"));
    event1.setDescription(QStringLiteral("This is a description of the first event"));
    event1.setLocation(QStringLiteral("the place"));
    event1.setTransparency(Event::Transparent);

    Event event2 = event1;
    QCOMPARE(event1.summary(), event2.summary());
    QCOMPARE(event1.dtStart(), event2.dtStart());
    QCOMPARE(event1.dtEnd(), event2.dtEnd());
    QCOMPARE(event1.description(), event2.description());
    QCOMPARE(event1.location(), event2.location());
}

void EventTest::testCopyIncidence()
{
    QDate dt = QDate::currentDate();
    Todo todo;
    todo.setDtStart(QDateTime(dt, {}));
    todo.setSummary(QStringLiteral("Event1 Summary"));
    todo.setDescription(QStringLiteral("This is a description of the first event"));
    todo.setLocation(QStringLiteral("the place"));

    Event event(todo);
    QCOMPARE(event.uid(), todo.uid());
    QCOMPARE(event.dtStart(), todo.dtStart());
    QCOMPARE(event.summary(), todo.summary());
    QCOMPARE(event.description(), todo.description());
    QCOMPARE(event.location(), todo.location());
}

void EventTest::testAssign()
{
    QDate dt = QDate::currentDate();
    Event event1;
    event1.setDtStart(QDateTime(dt, {}));
    event1.setDtEnd(QDateTime(dt, {}).addDays(1));
    event1.setSummary(QStringLiteral("Event1 Summary"));
    event1.setDescription(QStringLiteral("This is a description of the first event"));
    event1.setLocation(QStringLiteral("the place"));
    event1.setTransparency(Event::Transparent);

    Event event2 = event1;
    QCOMPARE(event1, event2);
}

void EventTest::testSerializer_data()
{
    QTest::addColumn<KCalendarCore::Event::Ptr>("event");
    QDateTime today = QDateTime::currentDateTimeUtc();
    QDateTime yesterday = today.addDays(-1);

    Event::Ptr event1 = Event::Ptr(new Event());
    Attendee attendee1(QStringLiteral("fred"), QStringLiteral("fred@flintstone.com"));
    event1->addAttendee(attendee1);
    event1->setDtStart(yesterday);
    event1->setDtEnd(today);

    Event::Ptr event2 = Event::Ptr(new Event());
    Attendee attendee2(QStringLiteral("fred"), QStringLiteral("fred@flintstone.com"));
    event2->addAttendee(attendee2);
    event2->setDtStart(yesterday);
    event2->setDtEnd(today);
    event2->setAllDay(true);

    event2->addComment(QStringLiteral("comment1"));
    event2->setUrl(QUrl(QStringLiteral("http://someurl")));

    event2->setCustomProperty("app", "key", QStringLiteral("value"));

    // Remaining properties tested in testtodo.cpp

    QTest::newRow("event") << event1;
    QTest::newRow("event2") << event2;
}

void EventTest::testSerializer()
{
    QFETCH(KCalendarCore::Event::Ptr, event);
    IncidenceBase::Ptr incidenceBase = event.staticCast<KCalendarCore::IncidenceBase>();

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream << incidenceBase;

    Event::Ptr event2 = Event::Ptr(new Event());
    IncidenceBase::Ptr incidenceBase2 = event2.staticCast<KCalendarCore::IncidenceBase>();
    QVERIFY(*event != *event2);
    QDataStream stream2(&array, QIODevice::ReadOnly);
    stream2 >> incidenceBase2;
    QCOMPARE(*event, *event2);
}

void EventTest::testDurationDtEnd()
{
    const QDate dt = QDate::currentDate();

    {
        Event event;
        event.setDtStart(QDateTime(dt, {}));
        event.setDtEnd(QDateTime(dt, {}).addDays(1));
        QCOMPARE(event.hasEndDate(), true);
        QCOMPARE(event.hasDuration(), false);
    }
    {
        Event event;
        event.setDtStart(QDateTime(dt, {}));
        event.setDuration(Duration(QDateTime(dt, {}), QDateTime(dt, {}).addDays(1)));
        QCOMPARE(event.hasDuration(), true);
        QCOMPARE(event.hasEndDate(), false);
    }
}

void EventTest::testDtEndChange()
{
    QDate dt = QDate::currentDate();
    Event event1;
    event1.setDtStart(QDateTime(dt, {}));
    event1.setDtEnd(QDateTime(dt, {}).addDays(1));
    event1.resetDirtyFields();

    event1.setDtEnd(QDateTime(dt, {}).addDays(1));
    QVERIFY(event1.dirtyFields().empty());

    event1.setDtEnd(QDateTime(dt, {}).addDays(2));
    QCOMPARE(event1.dirtyFields(), QSet<IncidenceBase::Field>() << IncidenceBase::FieldDtEnd);
    event1.resetDirtyFields();

    event1.setDtEnd(QDateTime());
    QCOMPARE(event1.dirtyFields(), QSet<IncidenceBase::Field>() << IncidenceBase::FieldDtEnd);
    event1.resetDirtyFields();

    event1.setDtEnd(QDateTime(dt, {}).addDays(2));
    QCOMPARE(event1.dirtyFields(), QSet<IncidenceBase::Field>() << IncidenceBase::FieldDtEnd);
}

void EventTest::testIsMultiDay_data()
{
    QTest::addColumn<QDateTime>("start");
    QTest::addColumn<QDateTime>("end");
    QTest::addColumn<bool>("isMultiDay");

    QTest::newRow("event0") << QDateTime(QDate(2016, 7, 9), QTime(12, 0, 0))
                            << QDateTime(QDate(2016, 7, 9), QTime(13, 0, 0)) << false;

    QTest::newRow("event1") << QDateTime(QDate(2016, 7, 9), QTime(12, 0, 0))
                            << QDateTime(QDate(2016, 7, 10), QTime(0, 0, 0)) << false;

    QTest::newRow("event2") << QDateTime(QDate(2016, 7, 9), QTime(12, 0, 0))
                            << QDateTime(QDate(2016, 7, 10), QTime(12, 0, 0)) << true;

    QTest::newRow("event3") << QDateTime(QDate(2016, 12, 31), QTime(0, 0, 0))
                            << QDateTime(QDate(2017, 1, 1), QTime(0, 0, 0)) << false;

    QTest::newRow("event4") << QDateTime(QDate(2016, 12, 31), QTime(0, 0, 1))
                            << QDateTime(QDate(2017, 1, 1), QTime(0, 0, 1)) << true;

    QTest::newRow("event5") << QDateTime(QDate(2016, 12, 31), QTime(12, 0, 0))
                            << QDateTime(QDate(2017, 1, 1), QTime(12, 0, 0)) << true;

    QTest::newRow("event6") << QDateTime(QDate(2016, 12, 24), QTime(12, 0, 0))
                            << QDateTime(QDate(2017, 1, 1), QTime(0, 0, 0)) << true;
}

void EventTest::testIsMultiDay()
{
    QFETCH(QDateTime, start);
    QFETCH(QDateTime, end);
    QFETCH(bool, isMultiDay);

    Event event;
    event.setDtStart(start);
    event.setDtEnd(end);

    QCOMPARE(event.isMultiDay(), isMultiDay);
}
