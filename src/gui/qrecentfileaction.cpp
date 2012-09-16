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
        initMenu();
        loadRecentFile();
    }
    void initMenu();
    void loadRecentFile();
    void fillRecentMenu();
    void addAction(const QString& file);
    void removeAction(QAction* act);
    void menuIsEmpty();

    QList<QAction*> listRecentAction;
    QStringList recentFiles;
    int maximumNumberOfRecentFile;
    QAction *noEntriesAction;
    QAction *clearSeparator;
    QAction *clearAction;
    QAction *qq;
};

void QRecentFileActionPrivate::initMenu()
{
    delete qq->menu();
    qq->setMenu(new QMenu());
    noEntriesAction = qq->menu()->addAction(qq->tr("No Entries"));
    noEntriesAction->setEnabled(false);
    clearSeparator = qq->menu()->addSeparator();
    clearSeparator->setVisible(false);
    clearAction = qq->menu()->addAction(qq->tr("Clear List"), qq, SLOT(clear()));
    clearAction->setVisible(false);
    qq->connect(qq->menu(), SIGNAL(triggered(QAction*)),qq, SLOT(fileSelected(QAction*)));
}

void QRecentFileActionPrivate::loadRecentFile()
{
    QSettings settings;
    if(settings.applicationName().isEmpty()) {
        qDebug()<<" application name empty";
        return; //Return ?
    }
    recentFiles = settings.value(QLatin1String("Recent Files"),QStringList()).toStringList();
    fillRecentMenu();
}

void QRecentFileActionPrivate::fillRecentMenu()
{
    if(recentFiles.isEmpty()) {
        noEntriesAction->setVisible(true);
        return;
    }
    const int numberOfRecentFile(recentFiles.count());
    for( int i = 0 ; i < numberOfRecentFile; i++ )
    {
        addAction(recentFiles.at(i));
        if( i == maximumNumberOfRecentFile) {
            break;
        }
    }
    noEntriesAction->setVisible(false);
    clearSeparator->setVisible(true);
    clearAction->setVisible(true);
}

void QRecentFileActionPrivate::addAction(const QString& file)
{
    QAction* action = new QAction(file,qq);
    qq->menu()->insertAction(qq->menu()->actions().value(0), action);
    listRecentAction.append(action);
}

void QRecentFileActionPrivate::removeAction(QAction* act)
{
    qq->menu()->removeAction(act);
    recentFiles.removeAll(act->text());
    listRecentAction.removeAll(act);
}

void QRecentFileActionPrivate::menuIsEmpty()
{
    noEntriesAction->setVisible(true);
    clearSeparator->setVisible(false);
    clearAction->setVisible(false);
}


QRecentFileAction::QRecentFileAction(QObject *parent)
    :QAction(parent), d(new QRecentFileActionPrivate(this))
{
    setText(tr("Recent Files..."));
}

QRecentFileAction::~QRecentFileAction()
{
    saveRecentFile();
    delete d;
}

void QRecentFileAction::clear()
{
    d->menuIsEmpty();

    foreach (QAction* action, d->listRecentAction) {
        d->removeAction(action);
    }

    saveRecentFile();
    Q_EMIT recentFileCleared();
}

void QRecentFileAction::fileSelected(QAction*action)
{
    if(action) {
        Q_EMIT recentFileSelected(action->text());
    }
}

void QRecentFileAction::addRecentFile(const QString&file)
{
    // remove file if already in list
    foreach (QAction* action, d->listRecentAction)
    {
      if ( action->text() == file )
      {
          d->removeAction(action);
          break;
      }
    }
    if( d->maximumNumberOfRecentFile && d->listRecentAction.count() == d->maximumNumberOfRecentFile )
    {
        QAction *act = d->listRecentAction.first();
        d->removeAction(act);
    }

    d->noEntriesAction->setVisible(false);
    d->clearSeparator->setVisible(true);
    d->clearAction->setVisible(true);
    d->recentFiles.append(file);

    d->addAction(file);
    saveRecentFile();
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
          d->removeAction(action);
          break;
      }
    }
    if(d->listRecentAction.isEmpty()) {
        d->menuIsEmpty();
    }
    saveRecentFile();
}

void QRecentFileAction::saveRecentFile()
{
    QSettings settings;
    if(settings.applicationName().isEmpty()) {
        qWarning()<<" application name empty";
        return; //Return ?
    }
    settings.setValue(QLatin1String("Recent Files"),d->recentFiles);
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
            d->removeAction(act);
        }
    }
}
