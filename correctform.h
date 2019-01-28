#ifndef CORRECTFORM_H
#define CORRECTFORM_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QVector>
#include <QLineEdit>
#include <QTableView>
#include <QSqlTableModel>

namespace Ui {
class correctForm;
}

class correctForm : public QWidget
{
    Q_OBJECT

public:
    explicit correctForm(QWidget *parent = nullptr);
    ~correctForm();
    QVariantList data_lst;
    QVector <QLineEdit*> edits;
signals:
    void mySignal(QTableView*);
public slots:
    void Data();
    void getData(QTableView*);
private:
    Ui::correctForm *ui;
};

#endif // CORRECTFORM_H
