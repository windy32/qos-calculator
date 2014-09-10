#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->btnCalculate, SIGNAL(clicked()), this, SLOT(onCalculate()));
    connect(ui->lstAddress, SIGNAL(itemSelectionChanged()),
        this, SLOT(onSelectionChanged()));
    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(onAddAddress()));
    connect(ui->btnRemove, SIGNAL(clicked()), this, SLOT(onRemoveAddress()));
    connect(ui->btnGenerate, SIGNAL(clicked()), this, SLOT(onGenerate()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::onSelectionChanged()
{
    if (ui->lstAddress->selectedItems().count() == 0)
    {
        ui->btnRemove->setEnabled(false);
    }
    else
    {
        ui->btnRemove->setEnabled(true);
    }
}

void Dialog::onCalculate()
{
    double bw = ui->spnBandwidth->value();
    double logbw = log10(bw);
    bufferSize = (int)(261 * bw + 4710);
    double reserved;

    // The equation is not accurate for range (1 Mbps, 2 Mbps), so there's fix
    if (bw < 2.1064)
        reserved = 0.24 * bw + 0.03;
    else
        reserved = 0.2668 * logbw * logbw + 1.2419 * logbw + 0.1058;
    tokenRate = bw - reserved;

    ui->lblTokenRate->setText(
        QString("Token rate: %1 Mbps").arg(tokenRate, 0, 'f', 2));
    ui->lblBufferSize->setText(
        QString("Buffer size: %1 Bytes").arg(bufferSize));

    ui->btnGenerate->setEnabled(true);
}

void Dialog::onAddAddress()
{
    QRegExp exp(
            "(0|[1-9][0-9]{0,2})\\.(0|[1-9][0-9]{0,2})\\."
            "(0|[1-9][0-9]{0,2})\\.(0|[1-9][0-9]{0,2})");
    if (exp.exactMatch(ui->txtAddress->text()))
    {
        int a = exp.cap(1).toInt();
        int b = exp.cap(2).toInt();
        int c = exp.cap(3).toInt();
        int d = exp.cap(4).toInt();

        if (a >= 0 && a <= 255 &&
            b >= 0 && b <= 255 &&
            c >= 0 && c <= 255 &&
            c >= 0 && d <= 255)
        {
            // Avoid duplicated items
            for (int i = 0; i < ui->lstAddress->count(); i++)
            {
                if (ui->txtAddress->text() == ui->lstAddress->item(i)->text())
                {
                    QMessageBox::warning(this,
                        "QoS Calculator", "This address has been added!");
                    return;
                }
            }

            ui->lstAddress->addItem(ui->txtAddress->text());
            ui->txtAddress->setText("");
            return;
        }
    }

    QMessageBox::warning(this, "QoS Calculator", "Illegal IP address!");
}

void Dialog::onRemoveAddress()
{
    // it's not very efficient but it works
    for (int i = 0; i < ui->lstAddress->count(); i++)
    {
        if (ui->lstAddress->item(i)->isSelected())
        {
            ui->lstAddress->takeItem(i);
            break;
        }
    }
}

void Dialog::onGenerate()
{
    QList<QString> lines;

    // 1. Initialize
    lines.append("# Initialize");
    lines.append("");
    lines.append("insmod sch_htb");
    lines.append("insmod ifb");
    lines.append("ifconfig ifb0 up");
    lines.append("tc qdisc add dev ppp0 handle ffff: ingress");
    lines.append("tc filter add dev ppp0 parent ffff: protocol ip pref 10 \\");
    lines.append("   u32 match u32 0 0 flowid 1:1 \\");
    lines.append("   action mirred egress redirect dev ifb0");
    lines.append("");

    // 2. Setup
    lines.append("# Setup");
    lines.append("");
    lines.append("tc qdisc add dev ifb0 root handle 1: htb default 1");
    lines.append(QString(
        "tc class add dev ifb0 parent 1: classid 1:1 htb"
          " rate %1kbit ceil %1kbit quantum 1540").arg((int)(tokenRate * 1000)));
    lines.append(QString(
        "tc qdisc add dev ifb0 parent 1:1 handle 10: bfifo limit %1")
        .arg(bufferSize));

    // 4. Write to screen
    QString script = "";
    for (int i = 0; i < lines.count(); i++)
    {
        script.append(lines[i] + "\n");
    }

    ui->txtScript->setPlainText(script);
}
