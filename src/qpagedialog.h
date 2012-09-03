/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#ifndef QPAGEDIALOG_H
#define QPAGEDIALOG_H

#include "libklustersshared_export.h"
#include <QDialogButtonBox>
#include <QDialog>

class QPageDialogPrivate;


class KLUSTERSSHARED_EXPORT QPageDialog : public QDialog
{
public:
    explicit QPageDialog(QWidget *parent);
    ~QPageDialog();

    enum PageType {
        IconType = 0,
        TreeType = 1,
        ListType = 2
    };

    QDialogButtonBox *dialogButtonBox() const;
    void setButtons(QDialogButtonBox::StandardButtons standardButtons);

    void setType(QPageDialog::PageType type);
    QPageDialog::PageType type() const;

    void addPage(QWidget *page);
private:
    QPageDialogPrivate *d;
    Q_DISABLE_COPY(QPageDialog)
};

#endif // QPAGEDIALOG_H
