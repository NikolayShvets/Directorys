#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include "correctform.h"
#include <QSqlDatabase>
#include <QSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QMessageBox>
#include <QStringList>
#include <QGridLayout>
#include <QComboBox>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QDebug>
#include <QVariantList>
#include <QDate>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QMenu>

/*#define DATABASE_HOSTNAME "89.179.240.185"
#define PORT 5432
#define DATABASE_NAME "postgres"
#define USERNAME "postgres"
#define PASSWORD "sVQpV2EScrt7HsdLX0Jw"*/

#define DATABASE_HOSTNAME "Localhost"
#define DATABASE_NAME "nikonikoni"
#define USERNAME "nikonikoni"
#define PASSWORD "123"
#define TABLE "mytable"
#define TABLE_NAME "name"
#define TABLE_SECOND_NAME "second_name"
#define TABLE_INN "inn"
#define TABLE_BIRTHDATE "birthdate"
#define TABLE_POSITION "position"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QMessageBox* message = new QMessageBox;
    //QVariantList data_lst;
private:
    correctForm *myForm;
    QSqlRecord rec;
    QPushButton *excelButton;
    QPushButton *editDataButton;
    QPushButton *push_button_close;
    QPushButton *push_button_cancel;
    QSqlTableModel  *model;
    QGridLayout *layout;
    QGridLayout *formConstructor;
    QComboBox *selectTable;
    QTableView *tableData;
    QSqlDatabase database;
    QStringList tables, fields, data;
    QSqlQuery *query;
    QSqlRelationalTableModel *modelMain;
    QString tableName;
    QMenu *menu;
signals:
    void itemChanged();
private slots:
    bool getTables(const QVariantList &data);
    void createUI();
    void connectToDB();
    void setupModel(QString &tableName);
    void setupMainModel(const QString &tableName);
    void showTable();
    void showCorrectForm();
    void setChanges();
    void addANote();
    void dellANote();
    void slotCustomMenuReauested(QPoint pos);
    void dir_exit();
    void Data();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
