/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#include "qrecentfileaction.h"

#include <QSettings>
#include <QMenu>
#include <QDebug>

class QRecentFileActionPrivate
{
public:
    QRecentFileActionPrivate(QAction *q)
        :qq(q)
    {
        maximumNumberOfRecentFile = 10;
    }
    QList<QAction*> listRecentAction;
    QStringList recentFiles;
    int maximumNumberOfRecentFile;
    QAction *noEntriesAction;
    QAction *clearSeparator;
    QAction *clearAction;

    QAction *qq;
};


QRecentFileAction::QRecentFileAction(QObject *parent)
    :QAction(parent), d(new QRecentFileActionPrivate(this))
{
    setText(tr("Recent Files..."));
    initMenu();
    loadRecentFile();
}

QRecentFileAction::~QRecentFileAction()
{
    delete d;
}

void QRecentFileAction::initMenu()
{
    delete menu();
    setMenu(new QMenu());
    d->noEntriesAction = menu()->addAction(tr("No Entries"));
    d->noEntriesAction->setEnabled(false);
    d->clearSeparator = menu()->addSeparator();
    d->clearSeparator->setVisible(false);
    d->clearAction = menu()->addAction(tr("Clear List"), this, SLOT(clear()));
    d->clearAction->setVisible(false);
    setEnabled(false);
    connect(menu(), SIGNAL(triggered(QAction*)), SLOT(fileSelected(QAction*)));
}

void QRecentFileAction::clear()
{
    QMenu *recentFileMenu = menu();
    delete recentFileMenu;
    setMenu(0);
    setEnabled(false);
    d->recentFiles.clear();
    Q_EMIT recentFileCleared();
}

void QRecentFileAction::fileSelected(QAction*action)
{
    if(action) {
        Q_EMIT recentFileSelected(action->text());
    }
}


void QRecentFileAction::fillRecentMenu()
{
    if(d->recentFiles.isEmpty())
        return;
    setEnabled(true);

}

void QRecentFileAction::addRecentFile(const QString&file)
{

    // remove file if already in list
    foreach (QAction* action, d->listRecentAction)
    {
      if ( action->text() == file )
      {
        menu()->removeAction(action);
        d->recentFiles.removeAll(file);
        d->listRecentAction.removeAll(action);
        break;
      }
    }
    if( d->maximumNumberOfRecentFile && d->listRecentAction.count() == d->maximumNumberOfRecentFile )
    {
        QAction *act = d->listRecentAction.first();
        menu()->removeAction(act);
        d->recentFiles.removeAll(act->text());
        d->listRecentAction.removeAll(act);
    }

    d->noEntriesAction->setVisible(false);
    d->clearSeparator->setVisible(true);
    d->clearAction->setVisible(true);
    setEnabled(true);
    d->recentFiles.append(file);

    QAction* action = new QAction(file,this);
    menu()->insertAction(menu()->actions().value(0), action);
}

void QRecentFileAction::removeRecentFile(const QString&file)
{
    if(!d->recentFiles.contains(file)) {
        d->recentFiles.removeAll(file);
    }

    foreach (QAction* action, d->listRecentAction)
    {
      if ( action->text() == file )
      {
        menu()->removeAction(action);
        d->recentFiles.removeAll(file);
        d->listRecentAction.removeAll(action);
        break;
      }
    }
    if(d->listRecentAction.isEmpty()) {
        d->noEntriesAction->setVisible(true);
        d->clearSeparator->setVisible(false);
        d->clearAction->setVisible(false);
        setEnabled(false);
    }
}

void QRecentFileAction::saveRecentFile()
{
    QSettings settings;
    if(settings.applicationName().isEmpty()) {
        qDebug()<<" application name empty";
        return; //Return ?
    }
    settings.setValue(QLatin1String("Recent Files"),d->recentFiles);
}

void QRecentFileAction::loadRecentFile()
{
    QSettings settings;
    if(settings.applicationName().isEmpty()) {
        qDebug()<<" application name empty";
        return; //Return ?
    }
    d->recentFiles = settings.value(QLatin1String("Recent Files"),QStringList()).toStringList();
    fillRecentMenu();
}


int QRecentFileAction::maximumNumberOfRecentFile() const
{
    return d->maximumNumberOfRecentFile;
}

void QRecentFileAction::setMaximumNumberOfRecentFile(int maximumRecentFile ) const
{
    if(d->maximumNumberOfRecentFile != maximumRecentFile) {
        d->maximumNumberOfRecentFile = maximumRecentFile;
        // remove all excess items
        while( d->listRecentAction.count() > d->maximumNumberOfRecentFile ) {
            QAction *act = d->listRecentAction.last();
            menu()->removeAction(act);
            d->recentFiles.removeAll(act->text());
            d->listRecentAction.removeAll(act);
        }
    }
}
