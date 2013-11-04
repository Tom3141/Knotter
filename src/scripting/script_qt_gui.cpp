/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "script_qt_gui.hpp"

#include "QTableWidget"

Script_Qt_GUI::Script_Qt_GUI(QObject *parent) :
    QObject(parent)
{
}

void Script_Qt_GUI::table_set(QObject *table_object, int row, int column, QString value)
{
    QTableWidget *table = qobject_cast<QTableWidget*>(table_object);
    if ( table ) //&& table->rowCount() > row && table->columnCount() > column  )
    {
        QTableWidgetItem* item = table->item(row, column);
        if ( !item )
        {
            item = new QTableWidgetItem(value);
            table->setItem(row,column,item);
        }
        else
            item->setText(value);
    }
}

QString Script_Qt_GUI::table_get(QObject *table_object, int row, int column)
{
    QTableWidget *table = qobject_cast<QTableWidget*>(table_object);
    if ( table ) //&& table->rowCount() > row && table->columnCount() > column  )
    {
        QTableWidgetItem* item = table->item(row, column);
        if ( item )
            return item->text();
    }

    return QString();
}

void Script_Qt_GUI::table_append_row(QObject *table_object, QStringList value)
{
    QTableWidget *table = qobject_cast<QTableWidget*>(table_object);
    if ( table )
    {
        table->setRowCount(table->rowCount()+1);
        for( int i = 0; i < value.size(); i++ )
        {
            table->setItem(table->rowCount()-1,i,new QTableWidgetItem(value[i]));
        }
    }
}

int Script_Qt_GUI::table_current_row(QObject *table_object)
{
    QTableWidget *table = qobject_cast<QTableWidget*>(table_object);
    if ( table )
        return table->currentRow();
    return -1;
}

int Script_Qt_GUI::table_current_column(QObject *table_object)
{
    QTableWidget *table = qobject_cast<QTableWidget*>(table_object);
    if ( table )
        return table->currentColumn();
    return -1;
}
