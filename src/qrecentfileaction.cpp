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
    connect(this, SIGNAL(triggered(QAction*)), SLOT(fileSelected(QAction*)));
}

void QRecentFileAction::clear()
{
    //TODO
}

void QRecentFileAction::fileSelected(QAction*)
{
    //TODO
}

void QRecentFileAction::clearRecentFile()
{
    QMenu *recentFileMenu = menu();
    delete recentFileMenu;
    setMenu(0);
    setEnabled(false);
    d->recentFiles.clear();
    Q_EMIT recentFileCleared();
}

void QRecentFileAction::fillRecentMenu()
{
    if(d->recentFiles.isEmpty())
        return;
    setEnabled(true);

}

void QRecentFileAction::addRecentFile(const QString&file)
{
    if(!d->recentFiles.contains(file)) {
        d->recentFiles.append(file);
    }
    //update Menu
}

void QRecentFileAction::removeRecentFile(const QString&file)
{
    if(!d->recentFiles.contains(file)) {
        d->recentFiles.removeAll(file);
    }
    //update Menu

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
        //update menu
    }
}
