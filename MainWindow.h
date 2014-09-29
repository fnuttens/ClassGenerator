#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QString>
#include <QTabWidget>
#include "ListManager.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void genererCode();
    void copyText();
    void createFiles();
    void genererHeaderGuard();

private:
    QGroupBox* _grpCommentaires;

    QPushButton* _btnGenerer;
    QPushButton* _btnQuitter;

    QLineEdit* _ledtClassName;
    QLineEdit* _ledtHeaderGuard;
    QLineEdit* _ledtSuperClass;
    QLineEdit* _ledtAuteur;

    ListManager* _lsIncludes;

    QCheckBox* _chkProtect;
    QCheckBox* _chkDefaultBuilder;
    QCheckBox* _chkDestructeur;

    QDateEdit* _dtCreation;

    QTextEdit* _txtClassRole;
    QTextEdit* _txtCodeGenere;

    QTabWidget* _tabs;

    void setGui();
    void setValidators();
    void setConnections();
    QString genererHeader();
    QString genererImplementation();
    bool checkFormulaire();
    void addIncludes(QString& code);
};

#endif // MAINWINDOW_H
