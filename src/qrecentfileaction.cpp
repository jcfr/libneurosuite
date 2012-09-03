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
    saveRecentFile();
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
    menu()->setEnabled(false);
    connect(menu(), SIGNAL(triggered(QAction*)), SLOT(fileSelected(QAction*)));
}

void QRecentFileAction::clear()
{
    menu()->setEnabled(false);
    d->noEntriesAction->setVisible(true);
    d->clearSeparator->setVisible(false);
    d->clearAction->setVisible(false);

    const QList<QAction*> actions = d->listRecentAction;
    const int numberOfAction(actions.count());
    for (int i = 0; i < numberOfAction; ++i)
    {
        menu()->removeAction(actions.at(i));
        actions.at(i)->deleteLater();
    }

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
    menu()->setEnabled(true);
    const int numberOfRecentFile(d->recentFiles.count());
    for( int i = 0 ; i < numberOfRecentFile; i++ )
    {
        QAction* action = new QAction(d->recentFiles.at(i),this);
        menu()->insertAction(menu()->actions().value(0), action);
        if( i == d->maximumNumberOfRecentFile) {
            break;
        }
    }
    d->noEntriesAction->setVisible(false);
    d->clearSeparator->setVisible(true);
    d->clearAction->setVisible(true);
    menu()->setEnabled(true);
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
    menu()->setEnabled(true);
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
        menu()->setEnabled(false);
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
    qDebug()<<"saveRecentFile d->recentFile"<<d->recentFiles;
}

void QRecentFileAction::loadRecentFile()
{
    QSettings settings;
    qDebug()<<" void QRecentFileAction::loadRecentFile()";
    if(settings.applicationName().isEmpty()) {
        qDebug()<<" application name empty";
        return; //Return ?
    }
    d->recentFiles = settings.value(QLatin1String("Recent Files"),QStringList()).toStringList();
    qDebug()<<" d->recentFile"<<d->recentFiles;
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
