#include "dialog.h"
#include "ui_dialog.h"

#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->fileOpenButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->convertButton, SIGNAL(clicked()), this, SLOT(convertFile()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::openFile()
{
    m_fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("XRD Files (*.*)"));

    ui->filePathLabel->setText(m_fileName);
}

void Dialog::convertFile()
{
    QFile inFile(m_fileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open '%1'").arg(m_fileName));
        return;
    }

    static QDir lastPath = QDir::current();
    QString outFileName = QFileDialog::getSaveFileName(this, tr("Save"), lastPath.absolutePath(), tr("FOX (*.dat)"));
    if (outFileName.isEmpty())
        return;

    QFileInfo info(outFileName);
    lastPath = info.dir();

    QFile outFile(outFileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Error"), tr("An error occured while saving '%1'").arg(outFileName));
        return;
    }

    QTextStream out(&outFile);
    out << ui->minThetaSpin->value() << " " << ui->dThetaSpin->value()
        << " " << ui->maxThetaSpin->value() << " formatted by XRD to FOX Converter\n";

    int i = 0;
    QTextStream in(&inFile);
    while (!in.atEnd()) {

        QString line = in.readLine();

        if (line[0] == '_' || line[0] == ';')
            continue;

        QStringList fields = line.simplified().split(" ");

        if (i == 10)
        {
            out << "\n";
            i = 0;
        }
        ++i;
        out << qSetFieldWidth(8) << fields[1];
    }
}
