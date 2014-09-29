#include "ListManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>

ListManager::ListManager(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout(new QVBoxLayout);
        _liste = new QListWidget;
        connect(_liste, SIGNAL(doubleClicked(QModelIndex)), _liste, SLOT(edit(QModelIndex)));
        _editItem = new QLineEdit;

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
    QListWidgetItem* newItem(new QListWidgetItem(
        _editItem->text().isEmpty() ? "[Include item]" :
                                      _editItem->text()));
    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);

    _liste->addItem(newItem);
}

void ListManager::removeItem()
{
    //TODO: faire fonctionner la suppression
    _liste->removeItemWidget(_liste->currentItem());
}
