#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void openFile();
    void convertFile();
    
private:
    Ui::Dialog *ui;

    QString m_fileName;
};

#endif // DIALOG_H
