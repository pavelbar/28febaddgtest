#include "dialog.h"

#include <QCalendarWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDate>
#include <QApplication>
#include <QFileDialog>
#include <QTextStream>
#include <QTimer>

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    setWindowIcon(QIcon(":/icon/app_icon.png"));
    setWindowTitle(tr("Твой помощник"));
    createMenu();
    createGridGroupBox();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(gridGroupBox);
    setLayout(mainLayout);
    setMinimumWidth(700);
    setMinimumHeight(280);//высота

    keyF11 = new QShortcut(this);
    keyF11->setKey(Qt::Key_F11);
    connect(keyF11, SIGNAL(activated()), this, SLOT(slotShortcutF11()));
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

void Dialog::createMenu()
{
    menuBar = new QMenuBar;
    QMenu *MainMenu = new QMenu(tr("&Меню"), this);

    QAction *m_load = new QAction(tr("Загрузить"), this);
    m_load->setShortcut(tr("Ctrl+O"));
    m_load->setIcon(QIcon(":/icon/m_icon_load.png"));

    QAction *m_save = new QAction(tr("Save"), this);
    m_save->setShortcut(tr("Ctrl+S"));
    m_save->setIcon(QIcon(":/icon/m_icon_save.png"));

    QAction *m_exit = new QAction(tr("Exit"), this);
    m_exit->setShortcut(tr("Ctrl+Q"));
    m_exit->setIcon(QIcon(":/icon/m_icon_exit.png"));

    connect(m_load, SIGNAL(triggered()), this, SLOT(action_open()));
    connect(m_save, SIGNAL(triggered()), this, SLOT(action_save()));
    connect(m_exit, SIGNAL(triggered()), this, SLOT(action_exit()));

    MainMenu->addAction(m_load);
    MainMenu->addAction(m_save);

    MainMenu->addSeparator();

    MainMenu->addAction(m_exit);
    menuBar->addMenu(MainMenu);
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

void Dialog::createGridGroupBox()
{

    gridGroupBox = new QGroupBox(tr(""));

    QGridLayout *layoutUp = new QGridLayout;
    QLabel *ConstDate  = new QLabel(tr("Выбранная дата:"), this);
    label_selectedDate  = new QLabel("", this);
    label_selectedDate->setText(selectedDate.toString("yyyy-MM-dd"));
    layoutUp->addWidget(ConstDate, 0, 0);
    layoutUp->addWidget(label_selectedDate, 0, 1);

    QGridLayout *layout = new QGridLayout;
    QGridLayout *layoutLeft = new QGridLayout;
    QGridLayout *layoutRight = new QGridLayout;
    QGridLayout *layoutDown = new QGridLayout;

    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(2018, 1, 1));
    calendar->setMaximumDate(QDate(2018, 12, 31));
    calendar->setGridVisible(true);
    connect(calendar,SIGNAL(activated(QDate)),this,SLOT(slot_on_date_clicked(QDate)));

    tableWidget = new QTableWidget(0, 3, this);
    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Напоминание") << tr("Время") << tr("Дело"));
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(s_DelRow(int,int)));

    QPushButton *buttonAddRow = new QPushButton(tr("Добавить строку"), this);
    QPushButton *buttonWriteNote = new QPushButton(tr("Изменить страницу"), this);

    connect(buttonAddRow, SIGNAL(released()), this, SLOT(s_AddRow()));
    connect(buttonWriteNote, SIGNAL(released()), this, SLOT(s_WriteNote()));

    layoutLeft->addWidget(calendar, 0, 0);
    layoutRight->addLayout(layoutUp, 0, 0);
    layoutRight->addWidget(tableWidget, 1, 0);
    layoutRight->addLayout(layoutDown, 2, 0);
    layoutDown->addWidget(buttonAddRow, 0, 0);
    layoutDown->addWidget(buttonWriteNote, 0, 1);
    layout->addLayout(layoutLeft, 0, 0);
    layout->addLayout(layoutRight, 0, 1);

    gridGroupBox->setLayout(layout);

    open(0);
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

bool Dialog::open(int val)// 0 - авто
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    звено clearRed;
    for (int i = 0; i < mainList.GetSize(); i++)
    {
        clearRed = mainList.GetElem(i);
        if (clearRed.row == 0)
        paintCell(0, clearRed.date);
    }


    mainList.DeleteAll();

    if (val == 0)
        open_FilePath = "default.txt";
    else
        open_FilePath = QFileDialog::getOpenFileName(0, tr("Open text file"), "", tr("Text Files (*.txt)"));

    QFile file(open_FilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (val !=0)
        QMessageBox::warning(this, tr("Error"),tr("Cannot read file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return false;
    }

    clearTable();

    QTextStream in(&file);        // Создаем потоковый объект типа QDadaStream, так же в данный поток мы передаем объект типа QFile file.
    in.setCodec("UTF-8");
    QString str;

    while (!in.atEnd())
    {
        звено TMPзвено;

        str = file.readLine();
        str = str.simplified();
        TMPзвено.row = str.toInt();

        str = file.readLine();
        str = str.simplified();
        QDate dateDate= QDate::fromString(str,"yyyy-MM-dd");
        TMPзвено.date = dateDate;

        if (TMPзвено.row != 0)
        paintCell(1, dateDate);

        int tmp = 0;
        for (int i = 0; i < TMPзвено.row*3; i++)
        {
            if (tmp == 0)
            {
                str = file.readLine();
                str = str.simplified();
                TMPзвено.column1.InSertLast(str.simplified());
            }

            if (tmp == 1)
            {
                str = file.readLine();
                str = str.simplified();
                TMPзвено.column2.InSertLast(str.simplified());
            }

            if (tmp ==2)
            {
                str = file.readLine();
                str = str.simplified();
                TMPзвено.column3.InSertLast(str.simplified());
            }
            tmp ++;
            if (tmp == 3) tmp = 0;
        }
        mainList.InSertLast(TMPзвено);
    }

    label_selectedDate->setText(selectedDate.toString("yyyy-MM-dd"));
    on_date_clicked(selectedDate);
    QApplication::restoreOverrideCursor();        // Возвращаем стандартный вид курсора мышки пользователя
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

bool Dialog::on_date_clicked(QDate date)
{
    clearTable();
    selectedDate = date;
    QString s_selectedDate = QString("%1").arg(date.toString("yyyy-MM-dd"));//Ставим кликнутую дату в лейбл
    label_selectedDate->setText(s_selectedDate);//Ставим кликнутую дату в лейбл

    if (mainList.GetSize() == 0) return false;

    int index = -1;
    for (int i = 0; i < mainList.GetSize(); i++)
    {
        звено TMPзвено = mainList.GetElem(i);
        if ( TMPзвено.date == selectedDate)
            index = i;
    }

    if (index == -1) return false;

    звено TMPзвено = mainList.GetElem(index);
    int countRow  = TMPзвено.row;

    tableWidget->setRowCount(countRow);
    tableWidget->setColumnCount(COUNT_COLUMN);
    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Напоминание") << tr("Время") << tr("Дело"));


    for (int row = 0; row < countRow; ++row)
    {
        for (int column = 0; column < COUNT_COLUMN; ++column)//0..2
        {
            if (column == 0)
            {
                QTableWidgetItem *newItem;
                newItem = new QTableWidgetItem(tr(qPrintable(TMPзвено.column1.GetElem(row))));
                tableWidget->setItem(row, column, newItem);

            }

            if (column == 1)
            {
                QTableWidgetItem *newItem;
                newItem = new QTableWidgetItem(tr(qPrintable(TMPзвено.column2.GetElem(row))));
                tableWidget->setItem(row, column, newItem);

            }

            if (column == 2)
            {
                QTableWidgetItem *newItem;
                newItem = new QTableWidgetItem(tr(qPrintable(TMPзвено.column3.GetElem(row))));
                tableWidget->setItem(row, column, newItem);

            }
        }

    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------


bool Dialog::save(int val)//0 - auto
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if( mainList.GetSize() == 0)
    {
        if (val !=0)
        QMessageBox::warning(this, tr("Error"),tr("Вы не создали ни одной задачи. Сохранение невозможно"));
        return false;
    }

    for (int row = 0; row < tableWidget->rowCount(); ++row)
        for (int column = 0; column < COUNT_COLUMN; ++column)
            if( tableWidget->item(row, column) == 0)
            {
                if (val !=0)
                QMessageBox::warning(this, tr("Error"),tr("Есть пустые ячейки. Сохранение невозможно"));
                return false;
            }

    if (val == 0)
        save_FilePath = "default.txt";
    else
        save_FilePath = QFileDialog::getSaveFileName(0, tr("Save text file"), "", tr("Text Files (*.txt)"));

    QFile file(save_FilePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        if (val !=0)
        QMessageBox::warning(this, tr("Error"),tr("%1 \n %2").arg(file.fileName()).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    //--------------------------------------------------------------------------------------------------------------

    QString str;

    for (int index = 0; index < mainList.GetSize(); index++)
    {
        звено TMPзвено;
        TMPзвено = mainList.GetElem(index);

        out << TMPзвено.row;
        out << "\r\n";

        QString s_selectedDate = QString("%1").arg(TMPзвено.date.toString("yyyy-MM-dd"));//Ставим кликнутую дату в лейбл
        s_selectedDate = s_selectedDate.simplified();
        out << s_selectedDate;
        out << "\r\n";

        for (int row = 0; row <  TMPзвено.row; ++row)
        {
            for (int column = 0; column < COUNT_COLUMN; ++column)
            {
                if (column == 0)
                {
                    str = TMPзвено.column1.GetElem(row);
                    str = str.simplified();
                    if (str.isEmpty()) return false;
                    out <<str;
                    out << "\r\n";
                }

                if (column == 1)
                {
                    str = TMPзвено.column2.GetElem(row);
                    str = str.simplified();
                    if (str.isEmpty()) return false;
                    out <<str;
                    out << "\r\n";
                }

                if (column == 2)
                {
                    str = TMPзвено.column3.GetElem(row);
                    str = str.simplified();
                    if (str.isEmpty()) return false;
                    out <<str;
                    out << "\r\n";
                }
            }
        }

    }

    //----------------------------------

    QApplication::restoreOverrideCursor();

    if (val !=0)
    {
    QMessageBox msg;
    msg.setText(tr("Успешно сохранено"));
    msg.exec();
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

bool  Dialog::s_WriteNote()
{
    int index = -1;
    звено TMPзвено;
    for (int row = 0; row < tableWidget->rowCount(); ++row)
        for (int column = 0; column < COUNT_COLUMN; ++column)//0..2
            if (tableWidget->item(row, column) == 0)
            {
                QMessageBox::warning(this, tr("Error"),tr("Есть пустые ячейки. Изменение невозможно"));
                return false;
            }

    for (int row = 0; row < tableWidget->rowCount(); ++row)
        if ((tableWidget->item(row, 0)->text().simplified() == "0")||tableWidget->item(row, 0)->text().simplified() == "1") true;
        else
        {
            QMessageBox::warning(this, tr("Error"),tr("Поле <Напоминание> может содержать только одно из двух значений: 1 (вкл) или 0 (выкл). Изменение невозможно"));
            return false;
        }

        for (int i = 0; i < mainList.GetSize(); i++)
        {
            TMPзвено = mainList.GetElem(i);
            if ( TMPзвено.date == selectedDate)
                index = i;
        }

        TMPзвено.row = tableWidget->rowCount();
        TMPзвено.date = selectedDate;
        if (TMPзвено.row != 0)
                paintCell(1, selectedDate);
        else
            paintCell(0, selectedDate);

        for (int row = 0; row < tableWidget->rowCount(); ++row)
        {
            for (int column = 0; column < COUNT_COLUMN; ++column)//0..2
            {
                if (column == 0)
                {
                    TMPзвено.column1.InSertLast(tableWidget->item(row, column)->text().simplified());
                }

                if (column == 1)
                {
                    TMPзвено.column2.InSertLast(tableWidget->item(row, column)->text().simplified());
                }

                if (column == 2)
                {
                    TMPзвено.column3.InSertLast(tableWidget->item(row, column)->text().simplified());
                }
            }
        }
                mainList.SetElem(index, TMPзвено);
    findTimer();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

void Dialog::slotShortcutF11()
{
    if(this->isFullScreen())
    {
        this->showNormal();
    }
    else
    {
        this->showFullScreen();
    }
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

void Dialog::clearTable()
{
    if ( tableWidget->columnCount() != COUNT_COLUMN)
        tableWidget->setColumnCount(COUNT_COLUMN);
    if ( tableWidget->rowCount() != 0)
        tableWidget->setRowCount(0);
    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Напоминание") << tr("Время") << tr("Дело"));
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

void Dialog::paintCell(int flag, QDate Date)
{
    //QDate dateDate = QDate::fromString(strDate.simplified(),"yyyy-MM-dd");
    QTextCharFormat format;
    QBrush brush;
    QColor color;
    int r=0,g=0,b=0,a=0;
    if (flag == 1)
        r=200,g=145,b=234,a=120;

    color.setRgb(r,g,b,a);;
    brush.setColor(color);
    format.setBackground(brush);

    calendar->setDateTextFormat(Date,format);
}
//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

bool Dialog::findTimer()
{
    if (mainList.GetSize() == 0)
        return false;

    if (timerstr.isNull() == false)
    {
        //QMessageBox::warning(this, tr("Error"),tr("Запрешено устанавливать два уведомления одновременно. Подождеите %1 минут").arg(QString::number(0.00001666*tmr->remainingTime())));
        return false;
    }

    QTime minTime;
    QTime curTime = QTime::currentTime();

    //QMessageBox::warning(this, tr("тек время"),tr("тек время %1").arg(QTime::currentTime().toString()));
    //QMessageBox::warning(this, tr("тек дата"),tr("тек дата %1").arg(QDate::currentDate().toString()));

    звено TMPзвено;
    bool flagActiv = false;
    bool flagTime = false;
/*
    for (int index = 0; index < mainList.GetSize(); index++)
    {
        TMPзвено = mainList.GetElem(index);
        flagActiv = false;

        if (TMPзвено.date == QDate::currentDate())
            for (int row = 0; row <  TMPзвено.row; ++row)
            {
                for (int column = 0; column < COUNT_COLUMN; ++column)
                {
                    if (column == 0)
                    {
                        QString str = TMPзвено.column1.GetElem(row);
                        str = str.simplified();
                        if (str[0] == '1')
                            flagActiv = true;
                    }

                    if (column == 1)
                    {
                        if ( (flagActiv == true) && (QTime::fromString(TMPзвено.column2.GetElem(row).simplified(),"hh-mm-ss") > curTime) )
                            minTime = QTime::fromString(TMPзвено.column2.GetElem(row).simplified(),"hh-mm-ss");
                    }
                }
            }
    }
    */
    //-----
/*
    if (minTime.isNull())
    {
        QMessageBox::warning(this, tr("er"),tr("дата не нашлась").arg(""));
        return false;
    }
    */

    //--------------------

    for (int index = 0; index < mainList.GetSize(); index++)
    {
        TMPзвено = mainList.GetElem(index);
        flagActiv = false;
        flagTime = false;

        if (TMPзвено.date == QDate::currentDate())
            for (int row = 0; row <  TMPзвено.row; ++row)
            {
                for (int column = 0; column < COUNT_COLUMN; ++column)
                {
                    if (column == 0)
                    {
                        QString str = TMPзвено.column1.GetElem(row);
                        str = str.simplified();
                        if (str[0] == '1')
                            flagActiv = true;
                    }

                    if (column == 1)
                    {
                       // if ( (flagActiv == true) && (QTime::fromString(TMPзвено.column2.GetElem(row).simplified(),"hh-mm-ss") < minTime) && (QTime::fromString(TMPзвено.column2.GetElem(row).simplified(),"hh-mm-ss") > curTime) )
                        if ( (flagActiv == true) && (QTime::fromString(TMPзвено.column2.GetElem(row).simplified(),"hh-mm-ss") > curTime) )
                        minTime = QTime::fromString(TMPзвено.column2.GetElem(row).simplified(),"hh-mm-ss");
                        flagTime = true;

                    }

                    if (column == 2)
                    {
                        if ((flagActiv == true) && (flagTime == true))
                            timerstr = TMPзвено.column3.GetElem(row);
                        timerstr = timerstr.simplified();
                    }
                }
            }

    }

    if (minTime.isNull())
    {
        //QMessageBox::warning(this, tr("er"),tr("дата не нашлась").arg(""));
        return false;
    }

    if ((flagActiv == true) && (flagTime == true) && (minTime.isNull() == false))
    {
        long int timeMS = -1;

        int i_result_hour = minTime.hour() - curTime.hour();
        int i_result_minute = minTime.minute() - curTime.minute();
        int i_result_seconds = minTime.second() - curTime.second();

        timeMS = 3600000*i_result_hour + 60000*i_result_minute + 1000*i_result_seconds;
        QMessageBox::warning(this, tr("Предупреждение"),tr("О деле <%1> будет напомненно через %2 минут").arg(timerstr).arg(QString::number(double(timeMS)*0.00001666)));

        tmr = new QTimer;
        tmr->setInterval(timeMS);
        connect(tmr, SIGNAL(timeout()), this, SLOT(timerOperation())); // Подключаем сигнал таймера к слоту
        tmr->start(); // Запускаем таймер
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

void Dialog::timerOperation()
{
    QMessageBox::warning(this, tr("Напоминание"),tr("%1").arg(timerstr));
    delete tmr;
    timerstr.clear();
}

//---------------------------------------------------------------------------------------------------------------------------

///--------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------

Dialog::~Dialog()
{
}

