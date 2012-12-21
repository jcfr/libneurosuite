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
    QRecentFileActionPrivate(QAction *q)
        :qq(q), initialized(false)

    {
        maximumFileCount = 10;
        initMenu();

    }
    void initMenu();
    void loadRecentFile();
    void fillRecentMenu();
    void addAction(const QString& file);
    void removeAction(QAction* act);
    void updateActionsState();

    void initializeMenu();

    QStringList recentFiles;
    int maximumFileCount;
    QAction *noEntriesAction;
    QAction *clearSeparator;
    QAction *clearAction;
    QAction *qq;
    bool initialized;
};

void QRecentFileActionPrivate::initializeMenu()
{
    if(initialized)
        return;
    initialized = true;
    noEntriesAction = qq->menu()->addAction(qq->tr("No Entries"));
    noEntriesAction->setEnabled(false);

    clearSeparator = qq->menu()->addSeparator();
    clearSeparator->setVisible(false);

    clearAction = qq->menu()->addAction(qq->tr("Clear List"), qq, SLOT(clear()));
    clearAction->setVisible(false);

    qq->connect(qq->menu(), SIGNAL(triggered(QAction*)),qq, SLOT(fileSelected(QAction*)));
    loadRecentFile();
}

void QRecentFileActionPrivate::initMenu()
{
    if(initialized)
        return;

    delete qq->menu();
    QMenu *menu = new QMenu();
    qq->setMenu(menu);
    qq->connect(menu,SIGNAL(aboutToShow()),qq,SLOT(initializeMenu()));
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
    updateActionsState();
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
    QAction* action = new QAction(actionText,qq);
    action->setToolTip(file);
    action->setData(file);
    qq->menu()->insertAction(qq->menu()->actions().value(0), action);
}

void QRecentFileActionPrivate::removeAction(QAction* act)
{
    qq->menu()->removeAction(act);
    recentFiles.removeAll(act->data().toString());
}

void QRecentFileActionPrivate::updateActionsState()
{
    bool isMenuEmpty = qq->menu()->actions().isEmpty();
    noEntriesAction->setVisible(isMenuEmpty);
    clearSeparator->setVisible(!isMenuEmpty);
    clearAction->setVisible(!isMenuEmpty);
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

    foreach (QAction* action, menu()->actions()) {
        if((action != d->clearAction) && (action != d->noEntriesAction) && (action != d->clearSeparator))
            d->removeAction(action);
    }
    d->updateActionsState();
    save();
    Q_EMIT recentFileCleared();
}

void QRecentFileAction::fileSelected(QAction*action)
{
    if(action) {
        Q_EMIT recentFileSelected(action->data().toString());
    }
}

void QRecentFileAction::addRecentFile(const QString&file)
{
    if(file.isEmpty())
        return;

    // remove file if already in list
    foreach (QAction* action, menu()->actions())
    {
      if ( action->data().toString() == file )
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

    d->recentFiles.append(file);
    d->addAction(file);
    d->updateActionsState();
    save();
}

void QRecentFileAction::removeRecentFile(const QString&file)
{
    d->recentFiles.removeAll(file);

    foreach (QAction* action, menu()->actions())
    {
      if ( action->data().toString() == file )
      {
          d->removeAction(action);
          break;
      }
    }
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
