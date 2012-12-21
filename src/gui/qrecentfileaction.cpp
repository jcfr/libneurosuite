/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#include "qrecentfileaction.h"

#include <QSettings>
#include <QMenu>
#include <QDebug>
#include <QFileInfo>

class QRecentFileActionPrivate
{
public:
    QRecentFileActionPrivate(QAction *qq)
        : maximumFileCount(10),
          q(qq),
          initialized(false)

    {
        initialize();
    }
    void initialize();

    void addAction(const QString& file);
    void removeAction(QAction* act);
    void removeAction(const QString &file);
    void updateActionsState();

    void initializeMenu();

    QStringList recentFiles;
    int maximumFileCount;
    QAction *noEntriesAction;
    QAction *clearSeparator;
    QAction *clearAction;
    QAction *q;
    bool initialized;
};

void QRecentFileActionPrivate::initializeMenu()
{
    if(initialized) {
        return;
    }

    QSettings settings;
    if(settings.applicationName().isEmpty()) {
        qDebug()<<" application name empty";
        return;
    }

    initialized = true;

    noEntriesAction = q->menu()->addAction(q->tr("No Entries"));
    noEntriesAction->setEnabled(false);
    clearSeparator = q->menu()->addSeparator();
    clearAction = q->menu()->addAction(q->tr("Clear List"), q, SLOT(clear()));

    q->connect(q->menu(), SIGNAL(triggered(QAction*)),q, SLOT(fileSelected(QAction*)));

    recentFiles = settings.value(QLatin1String("Recent Files"),QStringList()).toStringList();

    if(!recentFiles.isEmpty()) {
        const int numberOfRecentFile(recentFiles.count());
        for( int i = 0 ; i < numberOfRecentFile; ++i ) {
            addAction(recentFiles.at(i));
            if( i == maximumFileCount) {
                break;
            }
        }
    }
    updateActionsState();

}

void QRecentFileActionPrivate::initialize()
{
    if(initialized) {
        return;
    }

    delete q->menu();
    QMenu *menu = new QMenu();
    q->setMenu(menu);
    q->connect(menu,SIGNAL(aboutToShow()),q,SLOT(initializeMenu()));
}

void QRecentFileActionPrivate::addAction(const QString& file)
{
    if(file.isEmpty()) {
        return;
    }

    QString troncateFileName = file;
    if(file.length() > 30) {
        troncateFileName = file.left(15) + QLatin1String("...") + file.right(15);
    }

    const QString actionText = QString::fromLatin1("%1 [%2]").arg(QFileInfo(file).fileName()).arg(troncateFileName);
    QAction* action = new QAction(actionText,q);
    action->setToolTip(file);
    action->setData(file);
    q->menu()->insertAction(q->menu()->actions().value(0), action);
}

void QRecentFileActionPrivate::removeAction(QAction* act)
{
    q->menu()->removeAction(act);
    recentFiles.removeAll(act->data().toString());
}

void QRecentFileActionPrivate::updateActionsState()
{
    //We have noEntriesAction/clearSeparator/clearAction action by default
    //=> an empty menu has number action <4
    const bool isMenuEmpty = (q->menu()->actions().count() < 4);
    noEntriesAction->setVisible(isMenuEmpty);
    clearSeparator->setVisible(!isMenuEmpty);
    clearAction->setVisible(!isMenuEmpty);
}

void QRecentFileActionPrivate::removeAction(const QString &file)
{
    Q_FOREACH(QAction* action, q->menu()->actions()) {
      if ( action->data().toString() == file ) {
          removeAction(action);
          break;
      }
    }
}


QRecentFileAction::QRecentFileAction(QObject *parent)
    :QAction(parent), d(new QRecentFileActionPrivate(this))
{
    setText(tr("Recent Files..."));
}

QRecentFileAction::~QRecentFileAction()
{
    if(d->initialized)
        save();
    delete d;
}

void QRecentFileAction::clear()
{
    Q_FOREACH (QAction* action, menu()->actions()) {
        if((action != d->clearAction) && (action != d->noEntriesAction) && (action != d->clearSeparator))
            d->removeAction(action);
    }
    d->updateActionsState();
    save();
    Q_EMIT recentFileCleared();
}

void QRecentFileAction::fileSelected(QAction*action)
{
    if(action && (action != d->clearAction) && (action != d->noEntriesAction) && (action != d->clearSeparator)) {
        Q_EMIT recentFileSelected(action->data().toString());
    }
}

void QRecentFileAction::addRecentFile(const QString&file)
{
    if(file.isEmpty())
        return;

    // remove file if already in list
    d->removeAction(file);
    if( d->maximumFileCount && menu()->actions().count() == d->maximumFileCount ) {
        QAction *act = menu()->actions().first();
        d->removeAction(act);
    }

    d->recentFiles.append(file);
    d->addAction(file);
    d->updateActionsState();
    save();
}

void QRecentFileAction::removeRecentFile(const QString&file)
{
    d->recentFiles.removeAll(file);
    d->removeAction(file);
    d->updateActionsState();
    save();
}

void QRecentFileAction::save()
{
    QSettings settings;
    if(settings.applicationName().isEmpty()) {
        qWarning()<<" application name empty";
        return; //Return ?
    }
    settings.setValue(QLatin1String("Recent Files"),d->recentFiles);
}

int QRecentFileAction::maximumFileCount() const
{
    return d->maximumFileCount;
}

void QRecentFileAction::setMaximumFileCount(int maximumRecentFile ) const
{
    if(d->maximumFileCount != maximumRecentFile) {
        d->maximumFileCount = maximumRecentFile;
        // remove all excess items
        while( menu()->actions().count() > d->maximumFileCount ) {
            QAction *act = menu()->actions().last();
            d->removeAction(act);
        }
    }
}

void QRecentFileAction::initializeMenu()
{
    d->initializeMenu();
}
