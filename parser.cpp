#include "parser.h"

parser::parser()
{

}

parser::~parser()
{

}

void  parser::Clear_All()
{
    x_parsed.clear();
    y_parsed.clear();
    d_parsed.clear();
    gcode_parsed.clear();

    textfile_in.clear();

    x_val_array.clear();
    y_val_array.clear();
    d_val_array.clear();
    g_val_array.clear();

    D01_count = 0;
    D02_count = 0;

}

int parser::Parse_Line(QString string)
{

    QByteArray ba;

    ba = string.toLatin1();


    int ret = 0x0F;

    int i = 0;
    int cmd = 0;
    char symb;
    int endofline = 0;
    int cmd_prev = 0;

    QString  x_string;
    int x_i = 0;
    QString  y_string;
    int y_i = 0;
    QString  d_string;
    int d_i = 0;
    QString  gcode_string;
    int g_i = 0;

    x_string[0] = '\0';
    y_string[0] = '\0';
    gcode_string[0] = '\0';
    d_string[0] = '\0';

    if (ba[0] == 'M') return 0x80;

    while (endofline == 0)
    {
     symb = ba[i];

     switch (symb)
     {
         case 'X':{cmd = 1;break;}
         case 'Y':{cmd = 2;break;}
         case 'G':{cmd = 7;break;}
         case 'D':{cmd = 6;break;}
         case '\r':{cmd = 3;break;}
         case '\n':{cmd = 3;break;}
         case '*':{cmd = 4;break;}
         case '%':{cmd = 3;break;} //changed from cmd = 6
         default:{cmd = 5;break;}
     }

     switch (cmd)
     {
         case 1:
             {
                 break;
             }
         case 2:
             {
                 break;
             }
         case 3:
             {
                 endofline = 1;
                 break;
             }
         case 4:
             {
                 endofline = 1;
                 break;
             }
         case 5:
             {
                 if (cmd_prev == 1)
                 {
                     x_string[x_i] = symb;
                     x_i++;
                     cmd = 1;
                 }
                 else if (cmd_prev == 2)
                 {
                     y_string[y_i] = symb;
                     y_i++;
                     cmd = 2;
                 }
                 else if (cmd_prev == 6)
                 {
                     d_string[d_i] = symb;
                     d_i++;
                     cmd = 6;
                 }
                 else if (cmd_prev == 7)
                 {
                     gcode_string[g_i] = symb;
                     g_i++;
                     cmd = 7;

                     if ((symb == '0') & (ba[i + 1] == '4'))
                     {
                         endofline = 1;
                         cmd = 3;
                     }

                 }


                 break;
             }
         case 6:
             {
                 break;
             }
         case 7:
             {
                 break;
             }

     }

     i++;
     cmd_prev = cmd;

    }


    if (cmd_prev == 3)
    {
     x_string[0] = '\0';
     y_string[0] = '\0';
     gcode_string[0] = '\0';
     d_string[0] = '\0';
     return 0;
    }

    x_parsed = x_string.toLatin1();
    y_parsed = y_string.toLatin1();
    d_parsed = d_string.toLatin1();
    gcode_parsed = gcode_string.toLatin1();

    if (x_string[0] == '\0')
    {
        if(x_val_array.isEmpty()) x_val_array.append(0);
        else    x_val_array.append(x_val_array.last());
    }
    else
    {
         x_val_array.append(x_parsed.toInt());
    }


    if (y_string[0] == '\0')
    {
        if(y_val_array.isEmpty()) y_val_array.append(0);
        else    y_val_array.append(y_val_array.last());
    }
    else
    {
         y_val_array.append(y_parsed.toInt());
    }


    if (d_string[0] == '\0')
    {
        if(d_val_array.isEmpty()) d_val_array.append(2);
        else    d_val_array.append(d_val_array.last());
    }
    else
    {
         d_val_array.append((d_parsed.toInt()==1)?1:2);
    }

    if (gcode_string[0] == '\0')
    {
        if(g_val_array.isEmpty()) g_val_array.append(0);
        else    g_val_array.append(g_val_array.last());
    }
    else
    {
        g_val_array.append(gcode_parsed.toInt());
    }


   /* x_val_array.append(x_parsed.toInt());
    y_val_array.append(y_parsed.toInt());
    g_val_array.append(d_parsed.toInt());
    d_val_array.append(gcode_parsed.toInt());*/


    return ret;
 }
