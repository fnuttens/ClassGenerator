#include "ListManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QString>
#include <QRegExpValidator>
#include <QRegExp>

ListManager::ListManager(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout(new QVBoxLayout);
        _liste = new QListWidget;
        connect(_liste, SIGNAL(doubleClicked(QModelIndex)),
                _liste, SLOT(edit(QModelIndex)));
        _editItem = new QLineEdit;
        connect(_editItem, SIGNAL(editingFinished()), this, SLOT(addItem()));
        setValidator();

        QHBoxLayout* btnLayout(new QHBoxLayout);
            _btnAdd = new QPushButton("Ajouter");
            connect(_btnAdd, SIGNAL(clicked()), this, SLOT(addItem()));
            _btnRemove = new QPushButton("Retirer");
            connect(_btnRemove, SIGNAL(clicked()), this, SLOT(removeItem()));
            btnLayout->addWidget(_btnAdd);
            btnLayout->addWidget(_btnRemove);

        layout->addWidget(_liste);
        layout->addWidget(_editItem);
        layout->addLayout(btnLayout);

    setLayout(layout);
}

void ListManager::addItem()
{
    QListWidgetItem* newItem;

    if(sender() == _editItem || sender() == _btnAdd)
    {
        if(_editItem->text().isEmpty() || existingItem(_editItem->text())) return;

        newItem = new QListWidgetItem(_editItem->text());
        _editItem->setText("");
    }
    else
    {
        QLineEdit* item = (QLineEdit*)(sender());
        QString itemName = item->text();
        if(itemName.isEmpty() || existingItem(itemName))  return;

        newItem = new QListWidgetItem(itemName);
    }

    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    _liste->addItem(newItem);

}

void ListManager::removeItem()
{
    delete _liste->takeItem(_liste->currentRow());
}

bool ListManager::existingItem(QString item)
{
    for(int i(0); i < _liste->count(); ++i)
    {
        if(item == _liste->item(i)->text())
            return true;
    }

    return false;
}

void ListManager::setValidator()
{
    QRegExp regex("^[A-Za-z]+\.?[A-Za-z]+$", Qt::CaseSensitive);
    QRegExpValidator* validator(new QRegExpValidator(regex));

    _editItem->setValidator(validator);
}
