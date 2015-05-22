#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog_color.h"
#include "ui_dialog_color.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // New class items
    _parser = new parser;
    _scene = new GScene();
    _scene_repair = new GScene();
    _repair_data = new repair_data;
    _plc = new plc_omron;
    _client = new tcp_client;
    _client_info = new tcp_client;

    connect(    _client,
                SIGNAL( tcp_recd(QByteArray)),
                this,
                SLOT(tcp_recieved(QByteArray))
                );

    //Setting graphicViews and scenes

    //Main graphicsView

    ui->graphicsView->setScene(_scene);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->scale(1,-1);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(ShowContextMenu(const QPoint&)));    

    Graphics_view_zoom* z1 = new Graphics_view_zoom(ui->graphicsView);
    z1->set_modifiers(Qt::ControlModifier);

    //Repair graphicsView

    ui->graphicsView_repair->setScene(_scene_repair);
    ui->graphicsView_repair->scale(1,-1);
    ui->graphicsView_repair->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView_repair->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView_repair->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);


    Graphics_view_zoom* z2 = new Graphics_view_zoom(ui->graphicsView_repair);
    z2->set_modifiers(Qt::ControlModifier);

    //Setting table format
    ui->tableWidget_tooltable->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_tooltable->verticalHeader()->setStretchLastSection(true);
    ui->tableWidget_tooltable->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    ui->tableWidget_tooltable->verticalHeader()->resizeSections(QHeaderView::Stretch);


/************************ DRAWING PLOT

    QVector<double> x(10), y(10), z1(10); // initialize with entries 0..100

    for (int i=0; i<5; ++i)
    {
      x[i] = (i==0?0:x[i-1]) + ui->tableWidget->item(i,3)->text().toInt();
      y[i] = ui->tableWidget->item(i,2)->text().toInt();
      z1[i] = ui->tableWidget->item(i,1)->text().toInt();
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setData(x, z1);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 1000);
    ui->customPlot->yAxis->setRange(0, 3000);
    ui->customPlot->replot();
*/

    // Main graphicsView context menu
    action1 = myMenu.addAction("Repair selected");
    action2 = myMenu.addAction("Hide D02");
    action3 = myMenu.addAction("Show D02");
    action4 = myMenu.addAction("Zoom to rectangle");
    action4->setCheckable(true);
    gview_mode = SELECTION;

/*    // Logo
    QPixmap pmp(":/icons/icons/locked.bmp");
    ui->label_lock->setPixmap(pmp);
*/
}

MainWindow::~MainWindow()
{
    _client->close_client();
    _client_info->close_client();
    delete ui;
}

void MainWindow::on_openFileButton_clicked()
{
    if (this->read_file())
    {
        this->draw_all();
        this->zoom_to_fit();

        finished = false;
    }
}

void MainWindow::zoom_to_fit()
{
    ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::zoom_to_rect(QPainterPath zRect)
{    
    if (gview_mode == ZOOM_RECT)
    {
        ui->graphicsView->fitInView(zRect.boundingRect(), Qt::KeepAspectRatio);
    }
}

bool MainWindow::read_file()
{

    filename = QFileDialog::getOpenFileName(
                this,
                tr("Open"),
                "D:/!Transfer_16092014/GERBER",
                tr("All files (*.*)")
                );

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "Error: ", file.errorString());
        return false;
    }

    QTextStream in(&file);

    delete _parser;
    _parser = new parser;

    while(!in.atEnd())
    {
        QString line = in.readLine();
        _parser->textfile_in.append(line);
        if(!line.isEmpty()) _parser->Parse_Line(line);

    }

    file.close();

    return true;
}

void MainWindow::draw_all()
{
    // Clear all entities!

    _scene->clear();
    delete _scene;
    _scene = new GScene();
    ui->graphicsView->setScene(_scene);

    connect(_scene, SIGNAL(zoom_rect(QPainterPath)),
            this, SLOT(zoom_to_rect(QPainterPath)));

    itemlist.clear();
    item_match.clear();
    contour_match.clear();



    draw * drawall = new draw();
    drawall->draw_all(_scene, _parser, &item_match, &contour_match, &itemlist);
    delete drawall;

    ui->contours_left_lcdNumber->display(_parser->D01_count);

    ui->stats_textEdit->clear();
    ui->stats_textEdit->append("D01 count: ");
    ui->stats_textEdit->append(QString::number(_parser->D01_count));
    ui->stats_textEdit->append("D02 count: ");
    ui->stats_textEdit->append(QString::number(_parser->D02_count));

    ui->stats_textEdit->append("D01 path length: ");
    ui->stats_textEdit->append(QString::number(_parser->d01_pathlength));
    ui->stats_textEdit->append("D02 path length: ");
    ui->stats_textEdit->append(QString::number(_parser->d02_pathlength));



}

void MainWindow::on_Connect_TCP_Button_clicked()
{
    QString address;
    QStringList address_port;
    QHostAddress addr;

    address = ui->address_lineEdit->text();
    address_port = address.split(":");

    addr = address_port[0];
    int port = address_port[1].toInt();

    if (_client->connect_client(addr,port))
    {
        QString info;

        info = "Connected to ";
        info += address;

        ui->net_status_lineEdit->setText(info);

        ui->address_lineEdit->setEnabled(false);

        _client_info->connect_client(addr,port + 1);
    }
    else
    {
        ui->net_status_lineEdit->setText("Not connected");
    }
}

void MainWindow::on_Disconnect_TCP_Button_clicked()
{
    _client->close_client();
    _client_info->close_client();

    ui->net_status_lineEdit->setText("Not connected");
    ui->address_lineEdit->setEnabled(true);
}

void MainWindow::tcp_recieved(QByteArray recieved_data)
{
    while (comm.RecieveCommand(recieved_data))
    {
        recieved_data = recieved_data.remove(0, comm.bytecount);

        // execute commands

        if (comm.CMD.at(0) == 0x1)
        {
            int n;
            n = comm.paint(&itemlist, &item_match, &comm.DAT, _parser->d_val_array.length());

            ui->contours_left_lcdNumber->display(_parser->D01_count - contour_match.at(n==0?0:n-1));

            if ((n >= _parser->d_val_array.length()-1)&!finished)
            {
                QDateTime datetime;

                ui->log_textEdit->insertPlainText(datetime.currentDateTime().toString("dd.MM HH:mm:ss -> "));
                ui->log_textEdit->insertPlainText("Program finished.\r\n");

                finished = true;

            }
        }
        else {}
    }

}

// Color settings

void MainWindow::on_color_settings_button_clicked()
{
    QDialog * dialog_col = new Dialog_color(this);

    connect(dialog_col,
            SIGNAL(change_background(QBrush)),
            this,
            SLOT(change_background_color(QBrush))
            );

    dialog_col->show();
}

void MainWindow::change_background_color(QBrush color)
{
   _scene->setBackgroundBrush(color);
}



void MainWindow::on_send_program_button_clicked()
{
    comm.Send_Program(_parser, _client);
}

void MainWindow::on_start_pushButton_clicked()
{
    QDateTime datetime;

    ui->log_textEdit->insertPlainText(datetime.currentDateTime().toString("dd.MM HH:mm:ss -> "));
    ui->log_textEdit->insertPlainText(filename);
    ui->log_textEdit->insertPlainText(" Program started.\r\n");
}

void MainWindow::on_pause_pushButton_clicked()
{
    QDateTime datetime;

    ui->log_textEdit->insertPlainText(datetime.currentDateTime().toString("dd.MM HH:mm:ss -> "));
    ui->log_textEdit->insertPlainText("Program paused.\r\n");
}

void MainWindow::on_stop_pushButton_clicked()
{
    QDateTime datetime;

    ui->log_textEdit->insertPlainText(datetime.currentDateTime().toString("dd.MM HH:mm:ss -> "));
    ui->log_textEdit->insertPlainText("Program stopped.\r\n");
}

void MainWindow::ShowContextMenu(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = ui->graphicsView->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QAction* selectedItem = myMenu.exec(globalPos);

    if (selectedItem == action1)
    {
        QList <QPointF> list_points;

        _scene_repair->clear();

        draw * drawing = new draw;
        list_points = drawing->transfer_elements(_scene, _repair_data);
        drawing->draw_all(_scene_repair,_repair_data);
        delete drawing;

        ui->graphicsView_repair->fitInView(_scene_repair->sceneRect(), Qt::KeepAspectRatio);

        ui->tabWidget->setCurrentIndex(1);
    }
    else if (selectedItem == action2)
    {
        draw * tmp = new draw();
        tmp->hideD02(_scene);
        delete tmp;
    }
    else if (selectedItem == action3)
    {
        draw * tmp = new draw();
        tmp->showD02(_scene);
        delete tmp;
    }
    else if (selectedItem == action4)
    {
        if(action4->isChecked() == true)
        {
            this->setCursor(Qt::CrossCursor);
            gview_mode = ZOOM_RECT;

        }
        else
        {
            gview_mode = SELECTION;
            this->setCursor(Qt::ArrowCursor);

        }
    }
    else
    {
        // nothing was chosen
    }

}

void MainWindow::on_write_config_pushButton_clicked()
{
    // Collect all parameter values and write

/*    if (client.isOpen())
    {
        comm.Send_Confing_Parameter(&client, ui->Kp_axis_0->value());

        comm.Send_Confing_Parameter(&client, ui->Kp_axis_1->value());

        comm.Send_Confing_Parameter(&client, ui->Ki_axis_0->value());
        comm.Send_Confing_Parameter(&client, ui->Ki_axis_1->value());

        comm.Send_Confing_Parameter(&client, ui->Kd_axis_0->value());
        comm.Send_Confing_Parameter(&client, ui->Kd_axis_1->value());

        comm.Send_Confing_Parameter(&client, ui->amax_d1_axis_0->value());
        comm.Send_Confing_Parameter(&client, ui->amax_d1_axis_1->value());

        comm.Send_Confing_Parameter(&client, ui->amax_d2_axis_0->value());
        comm.Send_Confing_Parameter(&client, ui->amax_d2_axis_1->value());

        comm.Send_Confing_Parameter(&client, ui->vmax_d1_axis_0->value());
        comm.Send_Confing_Parameter(&client, ui->vmax_d1_axis_1->value());

        comm.Send_Confing_Parameter(&client, ui->vmax_d2_axis_0->value());
        comm.Send_Confing_Parameter(&client, ui->vmax_d2_axis_1->value());
    }
    else
    {
        QMessageBox::information(0, "Error: ", client.errorString());
    }

*/
}

void MainWindow::on_export_Tooltable_Button_clicked()
{
    QString tt_filename = QFileDialog::getSaveFileName(
                this,
                tr("Save"),
                "D:/!123",
                tr("Comma Separated Custom (*.*)")
                );
    QFile tt_file(tt_filename);
    tt_file.remove();

    if ( tt_file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &tt_file );
        QTableWidgetItem *	item;

        stream<<"Tool Code"<<",";

        for (int i = 0; i < ui->tableWidget_tooltable->columnCount(); i++)
        {
            item = ui->tableWidget_tooltable->horizontalHeaderItem(i);
            stream<<(item==0?"":item->text())<<",";
        }

        stream << endl;

        for (int i = 0; i < ui->tableWidget_tooltable->rowCount(); i++)
        {
            item = ui->tableWidget_tooltable->verticalHeaderItem(i);
            stream<<(item==0?"":item->text())<<",";

            for (int j = 0; j<ui->tableWidget_tooltable->columnCount();j++)
            {
                item = ui->tableWidget_tooltable->item(i, j);
                stream<<(item==0?"":item->text())<<",";
            }
            stream << endl;
        }
    }
    tt_file.close();
}

void MainWindow::on_import_Tooltable_Button_clicked()
{
    QString tt_filename = QFileDialog::getOpenFileName(
                            this,
                            tr("Open"),
                            "D:/!123",
                            tr("Comma Separated Custom (*.*)")
                            );
    QFile tt_file(tt_filename);

    if(!tt_file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "Error: ", tt_file.errorString());
        return;
    }

    QTextStream in(&tt_file);
    QTableWidgetItem * item_wr;
    int i = 0;
    int j = 0;

    in.readLine();

    while(!in.atEnd())
    {
        QString line = in.readLine();

        if(!line.isEmpty())
        {
            QStringList list = line.split(',');

            foreach (QString str, list)
            {
                if (j==0){j++;continue;}
                if (j>ui->tableWidget_tooltable->columnCount()) continue;
                item_wr = ui->tableWidget_tooltable->item(i,j-1);
                if (item_wr != 0)
                {
                    item_wr->setText(str.isEmpty()?"_":str);
                }
                j++;
            }
            i++;
            j=0;
        }
        else break;

    }

    tt_file.close();

}

void MainWindow::on_plc_Connect_Button_clicked()
{
    _plc->connect_plc(ui->lineEdit->text(), ui->comboBox->currentText().toInt());
}

void MainWindow::on_plc_Runstop_Button_clicked()
{
    if (ui->plc_Runstop_Button->isChecked())
    {
        ui->plc_Runstop_Button->setText("STOP");
        _plc->plc_run();

    }
    else
    {
        ui->plc_Runstop_Button->setText("RUN");
        _plc->plc_stop();
    }
}

void MainWindow::on_save_as_gbr_button_clicked()
{
    QString tt_filename = QFileDialog::getSaveFileName(
                this,
                tr("Save"),
                "D:/!123",
                tr("Gerber (*.gbr)")
                );
    QFile tt_file(tt_filename);
    tt_file.remove();

    if ( tt_file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &tt_file );
        QDateTime datetime;

        stream << "% Auto generated repair file" << "*" << endl;
        stream << "% Created: " << datetime.currentDateTime().toString("dd.MM HH:mm:ss") << "*" << endl;
        stream << "% Source file: " << filename << "*" << endl;

        for (int i = 0; i < _repair_data->x_val_array.count(); i++)
        {
            stream << "X" << _repair_data->x_val_array.at(i);
            stream << "Y" << _repair_data->y_val_array.at(i);
            stream << "D" << _repair_data->d_val_array.at(i);
            stream << "*" <<endl;
        }

        stream << "M02*" << endl;



    }
    tt_file.close();
}
