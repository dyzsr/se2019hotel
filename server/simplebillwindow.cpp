#include "simplebillwindow.h"
#include "ui_simplebillwindow.h"

SimpleBillWindow::SimpleBillWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleBillWindow)
{
    ui->setupUi(this);
}

SimpleBillWindow::~SimpleBillWindow()
{
    delete ui;
}

void SimpleBillWindow::showSimpleBill(Room room)
{
    //To be continued
    this->show();
}
