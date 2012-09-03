/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#include "qpagedialog.h"
#include <QVBoxLayout>

class QPageDialogPrivate
{
public:
    QPageDialogPrivate(QPageDialog *q)
        :buttonBox(0), qq(q)
    {
        init();
    }

    void init();
    void setButtons(QDialogButtonBox::StandardButtons standardButtons);

    QDialogButtonBox *buttonBox;
    QPageDialog *qq;
};

void QPageDialogPrivate::init()
{
    QWidget *w = new QWidget(qq);
    QVBoxLayout *lay = new QVBoxLayout;
    w->setLayout(lay);
    buttonBox = new QDialogButtonBox;
    lay->addWidget(buttonBox);
}

void QPageDialogPrivate::setButtons(QDialogButtonBox::StandardButtons standardButtons)
{
    if(buttonBox) {
        buttonBox->clear();
        buttonBox->setStandardButtons(standardButtons);
    }
}

QPageDialog::QPageDialog(QWidget *parent)
    :QDialog(parent), d(new QPageDialogPrivate(this))
{
}

QPageDialog::~QPageDialog()
{
    delete d;
}


void QPageDialog::setButtons(QDialogButtonBox::StandardButtons standardButtons)
{
    d->setButtons(standardButtons);
}

QDialogButtonBox *QPageDialog::dialogButtonBox() const
{
    return d->buttonBox;
}
