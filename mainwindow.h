#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

#include "parser.h"
#include "zoom.h"
#include "command.h"
#include "gscene.h"
#include "draw.h"
#include "plc_omron.h"
#include "repair_data.h"
#include "tcp_client.h"

#include <QFile>
#include <QFileDialog>

#include <QTextStream>

#include <QMessageBox>


#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainterPath>
#include <QPixmap>

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>

#include <QColor>
#include <QColorDialog>

#include <QVector>

#include <QDateTime>
#include <QTime>

#include <QMenu>

#include <math.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void change_background_color(QBrush color);

    void zoom_to_rect(QPainterPath zRect);

    void tcp_recieved(QByteArray recieved_data);

private slots:
    void zoom_to_fit();

    bool read_file();

    void draw_all();

    void on_openFileButton_clicked();

    void on_Connect_TCP_Button_clicked();

    void on_Disconnect_TCP_Button_clicked();

    void on_color_settings_button_clicked();

    void on_send_program_button_clicked();

    void on_start_pushButton_clicked();

    void on_pause_pushButton_clicked();

    void on_stop_pushButton_clicked();

    void ShowContextMenu(const QPoint& pos);

    void on_write_config_pushButton_clicked();

    void on_export_Tooltable_Button_clicked();

    void on_import_Tooltable_Button_clicked();

    void on_plc_Connect_Button_clicked();

    void on_plc_Runstop_Button_clicked();

private:
    Ui::MainWindow *ui;

    GScene * _scene;
    GScene * _scene_repair;

    parser * _parser;

    repair_data * _repair_data;

    QList <QGraphicsPathItem*> itemlist;
    QList <int> item_match;
    QList <int> contour_match;    
    QGraphicsItemGroup group_D1;
    QGraphicsItemGroup group_D2;

    tcp_client * _client;
    tcp_client * _client_info;

    plc_omron * _plc;

    command comm;

    QString filename;

    bool finished;

    enum gview_modes { ZOOM_RECT, SELECTION};
    gview_modes gview_mode;

    QMenu myMenu;
    QAction* action1;
    QAction* action2;
    QAction* action3;
    QAction* action4;


};

#endif // MAINWINDOW_H
