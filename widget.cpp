#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Справочники");
    QVariantList data;
    Widget::connectToDB();
    Widget::getTables(data);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connectToDB()
{
    /*database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName(DATABASE_HOSTNAME);
    database.setPort(PORT);
    database.setDatabaseName(DATABASE_NAME);
    database.setPassword(PASSWORD);
    database.setUserName(USERNAME);*/

    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName(DATABASE_HOSTNAME);
    database.setDatabaseName(DATABASE_NAME);
    database.setUserName(USERNAME);
    database.setPassword(PASSWORD);
    if(!database.open()){
        message->critical(NULL, QObject::tr("Ошибка подключения к базе данных"),database.lastError().text());
        //qDebug() << database.driverName()<< database.tables();
    }else{
        this->show();
        qDebug() <<database.driverName()<< database.tables();
    }
}

bool Widget::getTables(const QVariantList &data)
{
    query = new QSqlQuery;
    excelButton = new QPushButton;
    editDataButton = new QPushButton;
    push_button_close = new QPushButton;
    layout = new QGridLayout;
    selectTable = new QComboBox;
    tableData = new QTableView;
    excelButton->setText("Выгрузить в Excel");
    editDataButton->setText("Внести изменения");
    push_button_close->setText("Выход");

    tables.append(database.tables());
    layout->setRowMinimumHeight(10,10);
    selectTable->setEditable(true);
    selectTable->lineEdit()->setPlaceholderText("Выберите таблицу...");
    layout->addWidget(selectTable,0,0,1,3);
    layout->addWidget(tableData,1,0,5,3);
    layout->addWidget(excelButton,6,1,1,1);
    layout->addWidget(editDataButton,6,0,1,1);
    layout->addWidget(push_button_close,6,2,1,1);
    setLayout(layout);

    for(int i = 0; i < tables.count(); ++i){
        query->exec("SELECT *FROM " + tables[i]);
        rec = query->record();
        for(int j = 0; j < rec.count(); j++)
            {
                fields.append(rec.fieldName(j));
               // qDebug()<<"filed name:"<</*rec.fieldName(j)<<
                 //       "type"<<rec.field(j).type() << */"relation: " << rec.keyValues(rec);
                   }
        selectTable->addItem(tables[i]);
    }
    connect(this->tableData, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showCorrectForm()));
    connect(this->selectTable, SIGNAL(activated(int)), this, SLOT(showTable()));
    connect(this->editDataButton, SIGNAL(clicked()), this, SLOT(setChanges()));
    connect(tableData, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuReauested(QPoint)));
    connect(this->push_button_close, SIGNAL(clicked()), this, SLOT(dir_exit()));
}


void Widget::setupModel(QString &tableName)
{
    model = new QSqlTableModel;
    model->setTable(tableName);



    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setSort(0,Qt::AscendingOrder);
    }
}

void Widget::setupMainModel(const QString &tableName)
{   
    modelMain = new QSqlRelationalTableModel;
    modelMain->setTable(tableName);
    modelMain->setRelation(1, QSqlRelation("exp_tab", "id_exp", "name_exp"));
   // modelMain->setRelation(1, QSqlRelation("model", "id_model", "name_model"));
   // modelMain->setRelation(2, QSqlRelation("properties", "id_prop", "name_prop"));
    modelMain->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //modelMain->setEditStrategy(QSqlTableModel::OnFieldChange);
    for(int i = 0, j = 0; i < modelMain->columnCount(); i++, j++){
        modelMain->setSort(0,Qt::AscendingOrder);
    }
}

void Widget::createUI()
{
    tableData->setModel(modelMain);
    tableData->setItemDelegate(new QSqlRelationalDelegate(tableData));
   // tableData->setItemDelegateForRow(1, new QSqlRelationalDelegate(tableData));
   // tableData->setItemDelegateForRow(2, new QSqlRelationalDelegate(tableData));
    tableData->setColumnHidden(0,true);
    // Разрешаем выделение строк
    tableData->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    tableData->setSelectionMode(QAbstractItemView::SingleSelection);
    //tableData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableData->horizontalHeader()->setStretchLastSection(true);
    tableData->setContextMenuPolicy(Qt::CustomContextMenu);

    //connect(tableData, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(addANote()));
    //connect(tableData, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuReauested(QPoint)));

    modelMain->select();
    // Устанавливаем размер колонок по содержимому
    tableData->resizeColumnsToContents();
}

void Widget::showTable()
{
    tableName = selectTable->currentText();
    qDebug()<<"selected table: "<<tableName;
    this->setupMainModel(tableName);
    this->createUI();
}


void Widget::showCorrectForm()
{
    myForm = new correctForm;
    formConstructor = new QGridLayout(myForm);
    QPushButton *pushButton_setChanges = new QPushButton(myForm);
    myForm->setWindowTitle("Редактирование записи");
    QModelIndex indexes = tableData->currentIndex();
    int n = tableData->model()->columnCount();
    QLabel *labels[n];

    connect(pushButton_setChanges, SIGNAL(clicked()), this, SLOT(Data()));
    connect(myForm, SIGNAL(mySignal(QTableView*)), myForm, SLOT(getData(QTableView*)));

    formConstructor->setColumnMinimumWidth(tableData->model()->columnCount(), 1);
    formConstructor->setRowMinimumHeight(tableData->model()->rowCount(), 1);

    for(int i = 0; i < n; ++i){
        labels[i] = new QLabel;
        labels[i]->setText(tableData->model()->headerData(i, Qt::Horizontal).toString() + ":");
    }


    for(int i = 0; i < n; ++i){
        myForm->edits.append(new QLineEdit);
        myForm->edits[i]->setText((tableData->model()->data(tableData->model()->
                                                                     index(indexes.row(),i),0)).toString());
    }

    for(int i = 0; i < n ; i++){
        formConstructor->addWidget(myForm->edits[i], i, 1, 1, 1);
        formConstructor->addWidget(labels[i], i, 0, 1, 1);
        if(i + 1 == n){
            pushButton_setChanges->setText("Set changes");
            formConstructor->addWidget(pushButton_setChanges,i + 1, 1, 1, 2);
        }
    }
    for (int i = 0; i < n; ++i){
        myForm->data_lst.append(myForm->edits[i]->text());
    }
    myForm->show();
   // emit myForm->mySignal(i);
}

void Widget::setChanges()
{
    modelMain->submitAll();
    if(!modelMain->submitAll()){
        qDebug()<<"False: "<<modelMain->lastError();
    }else{
        qDebug()<<"true"<<modelMain->lastError();
    }
    modelMain->select();
    tableData->resizeColumnsToContents();
}
void Widget::addANote()
{
    int row = tableData->selectionModel()->currentIndex().row() + 1;
    modelMain->insertRow(row);
    qDebug()<<row;
}
void Widget::dellANote()
{
    /* получаем индекс выбранной строки */
    int row = tableData->selectionModel()->currentIndex().row();
    qDebug()<<row;

    /* проверяем была ли строка действительно выбрана */
    if (row >= 0){
        /* спрашиваем удалить запись или нет */
        if(QMessageBox::warning(this, trUtf8("Удаление записи"), trUtf8("Удалить запись?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            /* При отрицательном ответе делаем откат действий
             * и закрываем диалог без удаления записи
             * */
            database.rollback();
            qDebug()<<"NO";
            return;
        }else{
            /* Иначе удаляем запись
             * при успешном удалении обновляем таблицу
             * */
            tableData->model()->removeRow(row);
            qDebug()<<"YES";
            if(!modelMain->removeRow(row)){
                QMessageBox::warning(this, trUtf8("Уведомление"), trUtf8("Не удалось удалить запись\n"
                                                                         "Возможно она используется другими таблицами\n"
                                                                         "Проверьте зависимости и повторите попытку"));
            }
            //modelMain->select();
            tableData->setCurrentIndex(modelMain->index(-1,-1));

        }
    }
}
void Widget::slotCustomMenuReauested(QPoint pos)
{
    /* создаем объект контекстного меню*/
    menu = new QMenu(this);
    /* добавляем действия */
    QAction *addanote = new QAction(trUtf8("Добавить запсиь"), this);
    QAction *delanote = new QAction(trUtf8("Удалить запись"), this);
    QAction *alteration = new QAction(trUtf8("Редактировать запись"), this);
    /* Подключаем слоты обработки действий контекстного меню */
    connect(addanote, SIGNAL(triggered()), this, SLOT(addANote()));
    connect(delanote, SIGNAL(triggered()), this, SLOT(dellANote()));
    connect(alteration, SIGNAL(triggered()), this, SLOT(showCorrectForm()));
    /* утсновка действий в меню */
    menu->addAction(addanote);
    menu->addAction(delanote);
    menu->addAction(alteration);
    /* вызов контекстного меню */
    //tableData->viewport()->update();
    menu->popup(tableData->viewport()->mapToGlobal(pos));
    //menu->close();
}
void Widget::dir_exit()
{
   // this->close();

    if(QMessageBox::question(this, trUtf8("Выход"), trUtf8("Завершить работу?\nВсе несохраненные изменения не будут внесены в базу"),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        return;
    }else{
            database.rollback();
            this->close();
         }
}
void Widget::Data()
{
  emit myForm->mySignal(tableData);
        // SQL UPDATE
}

