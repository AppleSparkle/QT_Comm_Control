#ifndef REPAIR_DATA_H
#define REPAIR_DATA_H

#include <QList>

class repair_data
{
public:
    repair_data();
    ~repair_data();

    QList<int> x_val_array;
    QList<int> y_val_array;
    QList<int> d_val_array;
    QList<int> g_val_array;

};

#endif // REPAIR_DATA_H
