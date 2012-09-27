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
        maximumFileCount = 10;
        initMenu();
        loadRecentFile();
    }
    void initMenu();
    void loadRecentFile();
    void fillRecentMenu();
    void addAction(const QString& file);
    void removeAction(QAction* act);
    void menuIsEmpty();

    QStringList recentFiles;
    int maximumFileCount;
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
        if( i == maximumFileCount) {
            break;
        }
    }
    noEntriesAction->setVisible(false);
    clearSeparator->setVisible(true);
    clearAction->setVisible(true);
}

void QRecentFileActionPrivate::addAction(const QString& file)
{
    if(file.isEmpty()) {
        return;
    }
    QAction* action = new QAction(file,qq);
    qq->menu()->insertAction(qq->menu()->actions().value(0), action);
}

void QRecentFileActionPrivate::removeAction(QAction* act)
{
    qq->menu()->removeAction(act);
    recentFiles.removeAll(act->text());
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
    save();
    delete d;
}

void QRecentFileAction::clear()
{
    d->menuIsEmpty();

    foreach (QAction* action, menu()->actions()) {
        if((action != d->clearAction) && (action != d->noEntriesAction) && (action != d->clearSeparator))
            d->removeAction(action);
    }

    save();
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
    if(file.isEmpty())
        return;

    // remove file if already in list
    foreach (QAction* action, menu()->actions())
    {
      if ( action->text() == file )
      {
          d->removeAction(action);
          break;
      }
    }
    if( d->maximumFileCount && menu()->actions().count() == d->maximumFileCount )
    {
        QAction *act = menu()->actions().first();
        d->removeAction(act);
    }

    d->noEntriesAction->setVisible(false);
    d->clearSeparator->setVisible(true);
    d->clearAction->setVisible(true);
    d->recentFiles.append(file);

    d->addAction(file);
    save();
}

void QRecentFileAction::removeRecentFile(const QString&file)
{
    d->recentFiles.removeAll(file);

    foreach (QAction* action, menu()->actions())
    {
      if ( action->text() == file )
      {
          d->removeAction(action);
          break;
      }
    }
    if(menu()->actions().isEmpty()) {
        d->menuIsEmpty();
    }
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
