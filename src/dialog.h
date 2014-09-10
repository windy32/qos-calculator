#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QRegExp>
#include <QMessageBox>
#include <math.h>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Dialog *ui;

private:
    // token bucket parameters
    double tokenRate; // in mbps
    int bufferSize; // in bytes

private slots:
    void onCalculate();
    void onSelectionChanged();
    void onAddAddress();
    void onRemoveAddress();
    void onGenerate();
};

#endif // DIALOG_H
