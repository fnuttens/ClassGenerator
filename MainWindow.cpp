#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialog>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QRegExp>
#include <QFileDialog>
#include <QFile>
#include <QApplication>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("ClassGenerator");

    setGui();
    setValidators();
    setConnections();
}

MainWindow::~MainWindow()
{}

/**
 * \author fnuttens
 * \brief Met en place l'interface
 */
void MainWindow::setGui()
{
    QGroupBox* grpDefinition(new QGroupBox("Définition de la classe"));
        QFormLayout* defLayout(new QFormLayout);
            _ledtClassName = new QLineEdit;
            _ledtHeaderGuard = new QLineEdit;
            _ledtHeaderGuard->setEnabled(false);
            _ledtSuperClass = new QLineEdit;
            _lsIncludes = new ListManager;
        defLayout->addRow("&Nom :", _ledtClassName);
        defLayout->addRow("H&eader guard :", _ledtHeaderGuard);
        defLayout->addRow("Classe &mère :", _ledtSuperClass);
        defLayout->addRow("&Inclusions :", _lsIncludes);
    grpDefinition->setLayout(defLayout);

    QGroupBox* grpOptions(new QGroupBox("Options"));
        QVBoxLayout* optLayout(new QVBoxLayout);
            _chkProtect =
                new QCheckBox("Protéger le &header contre les inclusions multiples");
            _chkProtect->setChecked(true);
            _chkDefaultBuilder =
                new QCheckBox("Générer un &constructeur par défaut");
            _chkDefaultBuilder->setChecked(true);
            _chkDestructeur =
                new QCheckBox("Générer un &destructeur");
        optLayout->addWidget(_chkProtect);
        optLayout->addWidget(_chkDefaultBuilder);
        optLayout->addWidget(_chkDestructeur);
    grpOptions->setLayout(optLayout);

    _grpCommentaires = new QGroupBox("Ajouter des commentaires");
        _grpCommentaires->setCheckable(true);
        _grpCommentaires->setChecked(true);

        QFormLayout* commLayout(new QFormLayout);
            _ledtAuteur = new QLineEdit;
            _dtCreation = new QDateEdit(QDate::currentDate());
            _txtClassRole = new QTextEdit;
        commLayout->addRow("&Auteur :", _ledtAuteur);
        commLayout->addRow("Da&te de création :", _dtCreation);
        commLayout->addRow("&Rôle de la classe :", _txtClassRole);
    _grpCommentaires->setLayout(commLayout);

    QHBoxLayout* btnLayout(new QHBoxLayout);
        _btnGenerer = new QPushButton("&Générer");
        _btnQuitter = new QPushButton("&Quitter");
    btnLayout->addWidget(_btnGenerer, 1, Qt::AlignRight);
    btnLayout->addWidget(_btnQuitter, 0, Qt::AlignRight);

    QVBoxLayout* mainLayout(new QVBoxLayout);
        mainLayout->addWidget(grpDefinition);
        mainLayout->addWidget(grpOptions);
        mainLayout->addWidget(_grpCommentaires);
        mainLayout->addLayout(btnLayout);

    this->setLayout(mainLayout);
}

/**
 * \author fnuttens
 * \brief Crée les RegExpValidator à appliquer aux QLineEdit
 */
void MainWindow::setValidators()
{
    QRegExp regexClassName("^[A-Z][A-Za-z]*", Qt::CaseSensitive);
    QRegExpValidator* classNameValidator = new QRegExpValidator(regexClassName);

    _ledtClassName->setValidator(classNameValidator);
    _ledtSuperClass->setValidator(classNameValidator);
}

/**
 * \author fnuttens
 * \brief Effectue les connections nécessaires
 */
void MainWindow::setConnections()
{
    connect(_btnGenerer, SIGNAL(clicked()), this, SLOT(genererCode()));
    connect(_btnQuitter, SIGNAL(clicked()), this, SLOT(close()));
    connect(_ledtClassName, SIGNAL(textEdited(QString)), this, SLOT(genererHeaderGuard()));
}

/**
 * \author fnuttens
 * \brief Vérifie que le formulaire est bien rempli puis affiche la fenêtre avec le
 * code généré
 */
void MainWindow::genererCode()
{
    if(!checkFormulaire()) return;

    QDialog* fenCodeGenere(new QDialog(this));
    fenCodeGenere->setWindowTitle(_ledtClassName->text() + " - " + windowTitle());
    fenCodeGenere->resize(300, 450);

    QVBoxLayout* layout(new QVBoxLayout);
        _tabs = new QTabWidget;
            QTextEdit* header(new QTextEdit);
            header->setPlainText(genererHeader());
            header->setReadOnly(true);

            QTextEdit* implementation(new QTextEdit);
            implementation->setPlainText(genererImplementation());
            implementation->setReadOnly(true);

            _tabs->addTab(header, _ledtClassName->text() + ".h");
            _tabs->addTab(implementation, _ledtClassName->text() + ".cpp");

        QHBoxLayout* boutons(new QHBoxLayout);
            QPushButton* btnCopy(new QPushButton("&Copier"));
            connect(btnCopy, SIGNAL(clicked()), this, SLOT(copyText()));
            QPushButton* btnSave(new QPushButton("&Enregistrer"));
            connect(btnSave, SIGNAL(clicked()), this, SLOT(createFiles()));
            QPushButton* btnFermer(new QPushButton("&Fermer"));
            connect(btnFermer, SIGNAL(clicked()), fenCodeGenere, SLOT(close()));

            boutons->addWidget(btnCopy);
            boutons->addWidget(btnSave);
            boutons->addWidget(btnFermer);

        layout->addWidget(_tabs);
        layout->addLayout(boutons);
    fenCodeGenere->setLayout(layout);

    fenCodeGenere->show();
}

/**
 * \author fnuttens
 * \brief Copie le texte actuellement affiché dans le QTabWidget dans le presse-papier
 */
void MainWindow::copyText()
{
    QTextEdit* currentTxtEdit = (QTextEdit*)_tabs->currentWidget();
    QApplication::clipboard()->setText(currentTxtEdit->toPlainText());
}

/**
 * \author fnuttens
 * \brief Enregistre sous la forme de 2 fichiers le header et l'implémentation
 */
void MainWindow::createFiles()
{
    QString output =
        QFileDialog::getExistingDirectory(this, "Enregistrer sous...", QDir::homePath());

    //TODO: créer les fichiers de la classe à l'emplacement output.
}

/**
 * \author fnuttens
 * \brief Met à jour le champ correspondant au header guard qui sera utilisé
 */
void MainWindow::genererHeaderGuard()
{
    _ledtHeaderGuard->setEnabled(!_ledtClassName->text().isEmpty());

    if(_ledtClassName->text().isEmpty())    return;

    _ledtHeaderGuard->setText(_ledtClassName->text().toUpper() + "_H");
}

/**
 * \author fnuttens
 * \brief Génère le code de l'entête de la classe
 * \return QString code le texte généré
 */
QString MainWindow::genererHeader()
{
    QString code("");

    if(_grpCommentaires->isChecked())
    {
        code += "/*\n";

        if(!_ledtAuteur->text().isEmpty())
            code += "Auteur : " + _ledtAuteur->text() + "\n";

        code += "Date de création : " + _dtCreation->text() + "\n";

        if(!_txtClassRole->toPlainText().isEmpty())
            code += "\nRôle :\n" + _txtClassRole->toPlainText() + "\n*/\n\n";
        else
            code += "*/\n\n";
    }

    if(_chkProtect->isChecked())
    {
        code += "#ifndef " + _ledtHeaderGuard->text() + "\n";
        code += "#define " + _ledtHeaderGuard->text() + "\n\n";
    }

    code += "class " + _ledtClassName->text();

    if(!_ledtSuperClass->text().isEmpty())
        code += " : public " + _ledtSuperClass->text();

    code += "\n{\npublic:\n";

    if(_chkDefaultBuilder->isChecked())
        code += "\t" + _ledtClassName->text() + "();\n";

    if(_chkDestructeur->isChecked())
        code += "\t~" + _ledtClassName->text() + "();\n";

    code += "\nprotect:\n\nprivate:\n};";

    if(_chkProtect->isChecked())
        code += "\n\n#endif " + _ledtHeaderGuard->text();

    return code;
}

/**
 * \author fnuttens
 * \brief Génère le code de l'implémentation de la classe
 * \return QString code le code généré
 */
QString MainWindow::genererImplementation()
{
    QString code("");

    code += "#include \"" + _ledtClassName->text() + ".h\"\n\n";

    if(_chkDefaultBuilder->isChecked())
        code += _ledtClassName->text() + "::" + _ledtClassName->text() + "()\n{\n\n}\n\n";

    if(_chkDestructeur->isChecked())
        code += _ledtClassName->text() + "::~" + _ledtClassName->text() + "()\n{\n\n}";

    return code;
}

/**
 * \author fnuttens
 * \brief Vérifie la validité du formulaire
 * \return faux si le nom de la classe n'est pas renseigné, vrai sinon
 */
bool MainWindow::checkFormulaire()
{
    if(_ledtClassName->text().isEmpty())
    {
        QMessageBox::warning(this, "Formulaire invalide", "Vous n'avez pas \
renseigné de nom pour votre classe.");
        return false;
    }

    return true;
}
