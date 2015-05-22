#ifndef GRB_H
#define GRB_H

#include <QString>
#include <QStringList>

#include <QList>

#include <QChar>
#include <QCharRef>

class parser
{
public:
    parser();
    ~parser();

    QStringList textfile_in;

    QList<int> x_val_array;
    QList<int> y_val_array;
    QList<int> d_val_array;
    QList<int> g_val_array;

    QByteArray x_parsed;
    QByteArray y_parsed;
    QByteArray d_parsed;
    QByteArray gcode_parsed;

    int Parse_Line(QString string);

    void Clear_All();

    int D01_count;
    int D02_count;

    int d01_pathlength;
    int d02_pathlength;


};

#endif // GRB_H
