#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>

class ListManager : public QWidget
{
    Q_OBJECT

public:
    explicit ListManager(QWidget *parent = 0);
    QListWidget* getListWidget() const { return _liste; }

public slots:
    void addItem();
    void removeItem();

private:
    QListWidget* _liste;

    QLineEdit* _editItem;

    QPushButton* _btnAdd;
    QPushButton* _btnRemove;

    bool existingItem(QString item);
    void setValidator();
};

#endif // LISTMANAGER_H
