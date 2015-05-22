#include "dialog_color.h"
#include "ui_dialog_color.h"



Dialog_color::Dialog_color(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_color)
{
    ui->setupUi(this);
}

Dialog_color::~Dialog_color()
{
    delete ui;
}

void Dialog_color::on_background_button_clicked()
{
    emit change_background(Qt::blue);
}

