#ifndef DIALOG_COLOR_H
#define DIALOG_COLOR_H

#include <QDialog>
#include <QColor>

namespace Ui {
class Dialog_color;
}

class Dialog_color : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_color(QWidget *parent = 0);
    ~Dialog_color();

signals:
    void change_background(QBrush color);

private slots:
    void on_background_button_clicked();

private:
    Ui::Dialog_color *ui;
};

#endif // DIALOG_COLOR_H
