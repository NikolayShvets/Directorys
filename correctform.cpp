#include "correctform.h"
#include "ui_correctform.h"
#include <QTableView>
#include <QModelIndex>
#include <QSqlTableModel>
#include <QString>

correctForm::correctForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::correctForm)
{
    ui->setupUi(this);
}

correctForm::~correctForm()
{
    delete ui;
}

void correctForm::getData(QTableView* table)
{
    for(int i = 0; i < data_lst.size(); ++i){
            qDebug()<< edits[i]->text();
            table->model()->setData(table->selectionModel()->currentIndex(), edits[i]->text());
        }
    qDebug()<<(table->selectionModel()->currentIndex());
}
