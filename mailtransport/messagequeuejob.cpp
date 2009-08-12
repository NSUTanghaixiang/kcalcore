/*
    Copyright (c) 2009 Constantin Berzan <exit3219@gmail.com>

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

#include "messagequeuejob.h"

#include "transport.h"
#include "transportattribute.h"
#include "transportmanager.h"

#include <KDebug>
#include <KLocalizedString>

#include <akonadi/collection.h>
#include <akonadi/item.h>
#include <akonadi/itemcreatejob.h>
#include <akonadi/kmime/addressattribute.h>
#include <akonadi/kmime/localfolders.h>

using namespace Akonadi;
using namespace KMime;
using namespace MailTransport;

/**
  @internal
*/
class MailTransport::MessageQueueJob::Private
{
  public:
    Private( MessageQueueJob *qq )
      : q( qq )
    {
      transport = -1;
      dispatchMode = DispatchModeAttribute::Immediately;
      sentBehaviour = SentBehaviourAttribute::MoveToDefaultSentCollection;
      moveToCollection = -1;
      started = false;
    }

    MessageQueueJob *const q;

    Message::Ptr message;
    int transport;
    DispatchModeAttribute::DispatchMode dispatchMode;
    QDateTime dueDate;
    SentBehaviourAttribute::SentBehaviour sentBehaviour;
    Collection::Id moveToCollection;
    QString from;
    QStringList to;
    QStringList cc;
    QStringList bcc;
    bool started;

    /**
      Returns true if this message has everything it needs and is ready to be
      sent.
    */
    bool validate();

    // slot
    void doStart();

};

bool MessageQueueJob::Private::validate()
{
  if( !message ) {
    q->setError( UserDefinedError );
    q->setErrorText( i18n( "Empty message." ) );
    q->emitResult();
    return false;
  }

  if( to.count() + cc.count() + bcc.count() == 0 ) {
    q->setError( UserDefinedError );
    q->setErrorText( i18n( "Message has no recipients." ) );
    q->emitResult();
    return false;
  }

  if( dispatchMode == DispatchModeAttribute::AfterDueDate && !dueDate.isValid() ) {
    q->setError( UserDefinedError );
    q->setErrorText( i18n( "Message has invalid due date." ) );
    q->emitResult();
    return false;
  }

  if( TransportManager::self()->transportById( transport, false ) == 0 ) {
    q->setError( UserDefinedError );
    q->setErrorText( i18n( "Message has invalid transport." ) );
    q->emitResult();
    return false;
  }

  if( sentBehaviour == SentBehaviourAttribute::MoveToCollection && moveToCollection < 0 ) {
    q->setError( UserDefinedError );
    q->setErrorText( i18n( "Message has invalid sent-mail folder." ) );
    q->emitResult();
    return false;
  } else if( sentBehaviour == SentBehaviourAttribute::MoveToDefaultSentCollection ) {
    Q_ASSERT( LocalFolders::self()->isReady() );
    Q_ASSERT( LocalFolders::self()->sentMail().isValid() );
  }

  return true; // all ok
}

void MessageQueueJob::Private::doStart()
{
  LocalFolders::self()->disconnect( q );
  Q_ASSERT( !started );
  started = true;

  if( !validate() ) {
    // The error has been set; the result has been emitted.
    return;
  }

  // Create item.
  Item item;
  item.setMimeType( QLatin1String( "message/rfc822" ) );
  item.setPayload<Message::Ptr>( message );

  // Set attributes.
  AddressAttribute *addrA = new AddressAttribute( from, to, cc, bcc );
  DispatchModeAttribute *dmA = new DispatchModeAttribute( dispatchMode, dueDate );
  SentBehaviourAttribute *sA = new SentBehaviourAttribute( sentBehaviour, moveToCollection );
  TransportAttribute *tA = new TransportAttribute( transport );
  item.addAttribute( addrA );
  item.addAttribute( dmA );
  item.addAttribute( sA );
  item.addAttribute( tA );

  // Set flags.
  item.setFlag( "queued" );

  // Store the item in the outbox.
  Q_ASSERT( LocalFolders::self()->isReady() );
  Collection col = LocalFolders::self()->outbox();
  ItemCreateJob *job = new ItemCreateJob( item, col ); // job autostarts
  q->addSubjob( job );
}



MessageQueueJob::MessageQueueJob( QObject *parent )
  : KCompositeJob( parent )
  , d( new Private( this ) )
{
}

MessageQueueJob::~MessageQueueJob()
{
  delete d;
}

Message::Ptr MessageQueueJob::message() const
{
  return d->message;
}

int MessageQueueJob::transportId() const
{
  return d->transport;
}

DispatchModeAttribute::DispatchMode MessageQueueJob::dispatchMode() const
{
  return d->dispatchMode;
}

QDateTime MessageQueueJob::sendDueDate() const
{
  if( d->dispatchMode != DispatchModeAttribute::AfterDueDate ) {
    kWarning() << "Called when dispatchMode is not AfterDueDate.";
  }
  return d->dueDate;
}

Collection::Id MessageQueueJob::moveToCollection() const
{
  if( d->sentBehaviour != SentBehaviourAttribute::MoveToCollection ) {
    kWarning() << "Called when sentBehaviour is not MoveToCollection.";
  }
  return d->moveToCollection;
}

QString MessageQueueJob::from() const
{
  return d->from;
}

QStringList MessageQueueJob::to() const
{
  return d->to;
}

QStringList MessageQueueJob::cc() const
{
  return d->cc;
}

QStringList MessageQueueJob::bcc() const
{
  return d->bcc;
}

void MessageQueueJob::setMessage( Message::Ptr message )
{
  d->message = message;
}

void MessageQueueJob::setTransportId( int id )
{
  d->transport = id;
}

void MessageQueueJob::setDispatchMode( DispatchModeAttribute::DispatchMode mode )
{
  d->dispatchMode = mode;
}

void MessageQueueJob::setDueDate( const QDateTime &date )
{
  d->dueDate = date;
}

void MessageQueueJob::setSentBehaviour( SentBehaviourAttribute::SentBehaviour beh )
{
  d->sentBehaviour = beh;
}

void MessageQueueJob::setMoveToCollection( Collection::Id cid )
{
  d->moveToCollection = cid;
}

void MessageQueueJob::setFrom( const QString &from )
{
  d->from = from;
}

void MessageQueueJob::setTo( const QStringList &to )
{
  d->to = to;
}

void MessageQueueJob::setCc( const QStringList &cc )
{
  d->cc = cc;
}

void MessageQueueJob::setBcc( const QStringList &bcc )
{
  d->bcc = bcc;
}

void MessageQueueJob::start()
{
  LocalFolders *folders = LocalFolders::self();
  connect( folders, SIGNAL( foldersReady() ), this, SLOT( doStart() ) );
  folders->fetch(); // will emit foldersReady()
}

void MessageQueueJob::slotResult( KJob *job )
{
  // error handling
  KCompositeJob::slotResult( job );

  if( !error() ) {
    emitResult();
  }
}

#include "messagequeuejob.moc"