/*
 * This file is part of the syndication library
 *
 * Copyright (C) 2006 Frank Osterfeld <osterfeld@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "source.h"
#include "category.h"
#include "constants.h"
#include "generator.h"
#include "link.h"
#include "person.h"
#include "atomtools.h"

#include <tools.h>

#include <QtXml/QDomElement>
#include <QtCore/QList>
#include <QtCore/QString>

namespace Syndication {
namespace Atom {

Source::Source() : ElementWrapper()
{
}

Source::Source(const QDomElement& element) : ElementWrapper(element)
{
}

QList<Person> Source::authors() const
{
    QList<QDomElement> a = 
            elementsByTagNameNS(atom1Namespace(),
                                QString::fromUtf8("author"));
    QList<Person> list;
                                       
    QList<QDomElement>::ConstIterator it = a.constBegin();
    QList<QDomElement>::ConstIterator end = a.constEnd();
    
    
    for ( ; it != end; ++it)
    {
        list.append(Person(*it));
    }
        
    return list;
}

QList<Person> Source::contributors() const
{
        QList<QDomElement> a = 
                elementsByTagNameNS(atom1Namespace(),
                                    QString::fromUtf8("contributor"));
    QList<Person> list;
                                       
    QList<QDomElement>::ConstIterator it = a.constBegin();
    QList<QDomElement>::ConstIterator end = a.constEnd();
    
    
    for ( ; it != end; ++it)
    {
        list.append(Person(*it));
    }
        
    return list;
}

QList<Category> Source::categories() const
{
    QList<QDomElement> a = 
            elementsByTagNameNS(atom1Namespace(),
                                QString::fromUtf8("category"));
    QList<Category> list;
    
    QList<QDomElement>::ConstIterator it = a.constBegin();
    QList<QDomElement>::ConstIterator end = a.constEnd();


    for ( ; it != end; ++it)
    {
        list.append(Category(*it));
    }

    return list;
}

Generator Source::generator() const
{
    return Generator(firstElementByTagNameNS(atom1Namespace(),
                     QString::fromUtf8("generator")));
}

QString Source::icon() const
{
    return extractElementTextNS(atom1Namespace(),
                                QString::fromUtf8("icon"));
}

QString Source::id() const
{
    return extractElementTextNS(atom1Namespace(),
                                QString::fromUtf8("id"));
}

QList<Link> Source::links() const
{
    QList<QDomElement> a = 
            elementsByTagNameNS(atom1Namespace(),
                                QString::fromUtf8("link"));
    QList<Link> list;

    QList<QDomElement>::ConstIterator it = a.constBegin();
    QList<QDomElement>::ConstIterator end = a.constEnd();


    for ( ; it != end; ++it)
    {
        list.append(Link(*it));
    }

    return list;
}

QString Source::logo() const
{
    return extractElementTextNS(atom1Namespace(),
                                QString::fromUtf8("logo"));
}

QString Source::rights() const
{
    return extractAtomText(*this, QString::fromUtf8("rights"));
}

QString Source::subtitle() const
{
    return extractAtomText(*this, QString::fromUtf8("subtitle"));
}

QString Source::title() const
{
    return extractAtomText(*this, QString::fromUtf8("title"));
}

time_t Source::updated() const
{
    QString upd = extractElementTextNS(atom1Namespace(),
                                       QString::fromUtf8("updated"));
    return parseDate(upd, ISODate);
}

QString Source::debugInfo() const
{
    QString info;
    info += "### Source: ###################\n";
    if (!title().isEmpty())
        info += "title: #" + title() + "#\n";
    if (!subtitle().isEmpty())
        info += "subtitle: #" + subtitle() + "#\n";
    if (!id().isEmpty())
        info += "id: #" + id() + "#\n";

    if (!rights().isEmpty())
        info += "rights: #" + rights() + "#\n";
    if (!icon().isEmpty())
        info += "icon: #" + icon() + "#\n";
    if (!logo().isEmpty())
        info += "logo: #" + logo() + "#\n";
    if (!generator().isNull())
        info += generator().debugInfo();
    
    
    QString dupdated = dateTimeToString(updated());
    if (!dupdated.isNull())
        info += "updated: #" + dupdated + "#\n";
    
    QList<Link> dlinks = links();
    QList<Link>::ConstIterator endlinks = dlinks.constEnd();
    for (QList<Link>::ConstIterator it = dlinks.constBegin(); it != endlinks; ++it)
        info += (*it).debugInfo();
    
    QList<Category> dcats = categories();
    QList<Category>::ConstIterator endcats = dcats.constEnd();
    for (QList<Category>::ConstIterator it = dcats.constBegin(); it != endcats; ++it)
        info += (*it).debugInfo();

    info += "### Authors: ###################\n";
    
    QList<Person> dauthors = authors();
    QList<Person>::ConstIterator endauthors = dauthors.constEnd();
    for (QList<Person>::ConstIterator it = dauthors.constBegin(); it != endauthors; ++it)
        info += (*it).debugInfo();

    info += "### Contributors: ###################\n";
    
    QList<Person> dcontri = contributors();
    QList<Person>::ConstIterator endcontri = dcontri.constEnd();
    for (QList<Person>::ConstIterator it = dcontri.constBegin(); it != endcontri; ++it)
        info += (*it).debugInfo();
    
    info += "### Source end ################\n";

    return info;
}

} // namespace Atom
} //namespace Syndication