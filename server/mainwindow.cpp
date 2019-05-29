#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->stackedWidget->setCurrentWidget(ui->page_frontdesk);
  ui->lb_roomIdnotValid->hide();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_bt_callManager_clicked()
{
  emit sgn_openNewWindow(this);
}

bool MainWindow::checkRoomIdValid()
{
    bool isValid = true;
    QString str(ui->le_roomId->text());

    /*
    if (str == "")
    {
        isValid = false;
    }
    else
    {
        const char *s = str.toLatin1().data();
        while (*s && *s>='0' && *s<='9') s++;
        if (*s)
        {
            isValid = false;
        }
        else
        {
            isValid = true;
        }
    }
    */
    int roomId = str.toInt(&isValid);
    if (isValid)
    {
        isValid = emit sgn_checkRoomIdValid(roomId);
    }

    if (isValid)
    {
        ui->lb_roomIdnotValid->hide();
    }
    else
    {
        ui->lb_roomIdnotValid->show();
    }
    return isValid;
}

void MainWindow::on_bt_checkout_clicked()
{
    if (!checkRoomIdValid())
        return;
    emit sgn_checkout(ui->le_roomId->text().toInt());
}

void MainWindow::on_bt_simpleBill_clicked()
{
    if (!checkRoomIdValid())
        return;
    emit sgn_askSimpleBill_clicked();
}

void MainWindow::on_bt_detailedBill_clicked()
{
    if (!checkRoomIdValid())
        return;
    emit sgn_askDetailedBill_clicked();
}
