/*
 * This file is part of the syndication library
 *
 * Copyright (C) 2005 Frank Osterfeld <osterfeld@kde.org>
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

#include <rss2/item.h>
#include <rss2/category.h>
#include <rss2/enclosure.h>
#include <rss2/source.h>
#include <rss2/tools_p.h>
#include <constants.h>
#include <specificitem.h>
#include <specificitemvisitor.h>
#include <tools.h>

#include <QtXml/QDomElement>
#include <QtCore/QString>
#include <QtCore/QList>

namespace Syndication {
namespace RSS2 {

class Item::ItemPrivate
{
    public:
        
        boost::shared_ptr<Document> doc;
};

Item::Item(boost::shared_ptr<Document> doc) : ElementWrapper(), d(new ItemPrivate)
{
    d->doc = doc;
}

Item::Item(const QDomElement& element, boost::shared_ptr<Document> doc) : ElementWrapper(element), d(new ItemPrivate)
{
    d->doc = doc;
}

Item::~Item()
{
}

Item::Item(const Item& other) : ElementWrapper(other), SpecificItem(other)
{
    d = other.d;
}

Item& Item::operator=(const Item& other)
{
    ElementWrapper::operator=(other);
    SpecificItem::operator=(other);
    d = other.d;
    return *this;
}

QString Item::title() const
{
    if (!d->doc)
        return originalTitle();
    
    bool isCDATA = false;
    bool containsMarkup = false;
    d->doc->getItemTitleFormatInfo(&isCDATA, &containsMarkup);
    
    return normalize(originalTitle(), isCDATA, containsMarkup);
}


QString Item::originalDescription() const
{
    return extractElementTextNS(QString(), QString::fromUtf8("description"));
}
        
QString Item::originalTitle() const
{
    return extractElementTextNS(QString(), QString::fromUtf8("title"));
}

QString Item::link() const
{
    return extractElementTextNS(QString(), QString::fromUtf8("link") );
}

QString Item::description() const
{
    if (!d->doc)
        return originalDescription();

    bool isCDATA = false;
    bool containsMarkup = false;
    d->doc->getItemDescriptionFormatInfo(&isCDATA, &containsMarkup);
    
    return normalize(originalDescription(), isCDATA, containsMarkup);
}

QString Item::content() const
{
    // parse encoded stuff from content:encoded, xhtml:body and friends into content
    return extractContent(*this);
}

QList<Category> Item::categories() const
{
    QList<QDomElement> cats = elementsByTagNameNS(QString(),
            QString::fromUtf8("category"));

    QList<Category> categories;

    QList<QDomElement>::ConstIterator it = cats.constBegin();
    for ( ; it != cats.constEnd(); ++it)
    {
        categories.append(Category(*it));
    }
    return categories;
}

QString Item::comments() const
{
    return extractElementTextNS(QString(), QString::fromUtf8("comments") );
}

QString Item::author() const
{
    QString a = extractElementTextNS(QString(), QString::fromUtf8("author") );
    if (!a.isNull()) 
    {
        return a;
    }
    else
    {
        // if author is not available, fall back to dc:creator
        return extractElementTextNS(dublinCoreNamespace(),
                                    QString::fromUtf8("creator") );
    }
    
}

QList<Enclosure> Item::enclosures() const
{
    QList<QDomElement> encs = elementsByTagNameNS(QString(),
            QString::fromUtf8("enclosure"));

    QList<Enclosure> enclosures;

    QList<QDomElement>::ConstIterator it = encs.constBegin();
    for ( ; it != encs.constEnd(); ++it)
    {
        enclosures.append(Enclosure(*it));
    }
    return enclosures;
}

QString Item::guid() const
{
    return extractElementTextNS(QString(), QString::fromUtf8("guid") );
}

bool Item::guidIsPermaLink() const
{
    bool guidIsPermaLink = true;  // true is default

    QDomElement guidNode = firstElementByTagNameNS(QString(), 
            QString::fromUtf8("guid"));
    if (!guidNode.isNull())
    {
        if (guidNode.attribute(QString::fromUtf8("isPermaLink")) 
            == QString::fromUtf8("false"))
        {
            guidIsPermaLink = false;
        }
    }

    return guidIsPermaLink;
}

time_t Item::pubDate() const
{
    QString str = extractElementTextNS(QString(), QString::fromUtf8("pubDate"));
    
    if (!str.isNull())
    {
        return parseDate(str, RFCDate);
    }
    
    // if there is no pubDate, check for dc:date
    str = extractElementTextNS(dublinCoreNamespace(), QString::fromUtf8("date"));
    return parseDate(str, ISODate);
}
        
time_t Item::expirationDate() const
{
    QString str = extractElementTextNS(QString(), QString::fromUtf8("expirationDate"));
    return parseDate(str, RFCDate);
}

Source Item::source() const
{
    return Source(firstElementByTagNameNS(QString(), QString::fromUtf8("source")));
}

QString Item::rating() const
{
    return extractElementTextNS(QString(), QString::fromUtf8("rating") );
}

QString Item::debugInfo() const
{
    QString info;
    info += "### Item: ###################\n";
    if (!title().isNull())
        info += "title: #" + title() + "#\n";
    if (!link().isNull())
        info += "link: #" + link() + "#\n";
    if (!description().isNull())
        info += "description: #" + description() + "#\n";
    if (!content().isNull())
        info += "content: #" + content() + "#\n";
    if (!author().isNull())
        info += "author: #" + author() + "#\n";
    if (!comments().isNull())
        info += "comments: #" + comments() + "#\n";
    QString dpubdate = dateTimeToString(pubDate());
    if (!dpubdate.isNull())
        info += "pubDate: #" + dpubdate + "#\n";
    if (!guid().isNull())
        info += "guid: #" + guid() + "#\n";
    if (guidIsPermaLink())
        info += "guid is PL: #true#\n";
    if (!source().isNull())
         info += source().debugInfo();
    
    QList<Category> cats = categories();
    for (QList<Category>::ConstIterator it = cats.constBegin(); it != cats.constEnd(); ++it)
        info += (*it).debugInfo();
    QList<Enclosure> encs = enclosures();
    for (QList<Enclosure>::ConstIterator it = encs.constBegin(); it != encs.constEnd(); ++it)
        info += (*it).debugInfo();

    info += "### Item end ################\n";
    return info;
}

QList<QDomElement> Item::unhandledElements() const
{
    // TODO: do not hardcode this list here
    QList<ElementType> handled;
    handled.append(QString::fromUtf8("title"));
    handled.append(QString::fromUtf8("link"));
    handled.append(QString::fromUtf8("description"));
    handled.append(QString::fromUtf8("pubDate"));
    handled.append(QString::fromUtf8("expirationDate"));
    handled.append(QString::fromUtf8("rating"));
    handled.append(QString::fromUtf8("source"));
    handled.append(QString::fromUtf8("guid"));
    handled.append(QString::fromUtf8("comments"));
    handled.append(QString::fromUtf8("author"));
    handled.append(ElementType(QString::fromUtf8("date"), dublinCoreNamespace()));
    
    QList<QDomElement> notHandled;
    
    QDomNodeList children = element().childNodes();
    for (int i = 0; i < children.size(); ++i)
    {
        QDomElement el = children.at(i).toElement();
        if (!el.isNull() 
             && !handled.contains(ElementType(el.localName(), el.namespaceURI())))
        {
            notHandled.append(el);
        }
    }
    
    return notHandled;
}
        
bool Item::accept(SpecificItemVisitor* visitor)
{
    return visitor->visitRSS2Item(this);
}

} // namespace RSS2
} // namespace Syndication