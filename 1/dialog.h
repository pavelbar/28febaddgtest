#ifndef DIALOG_H
#define DIALOG_H

const int COUNT_COLUMN = 3;

#include <QDate>

#include <tlist.h>

#include <QDialog>
#include <QString>
#include <QMenuBar>
#include <QGroupBox>
#include <QTextEdit>
#include <QLabel>
#include <QTableWidget>
#include <QMessageBox>
#include <QShortcut>
#include <QCalendarWidget>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:

    struct звено
    {
     int row;
     QDate date;
     TList<QString> column1;
     TList<QString> column2;
     TList<QString> column3;
    };

    TList<звено> mainList;

    QDate selectedDate = QDate::currentDate();
    QShortcut *keyF11;
    QMenuBar *menuBar;
    QString open_FilePath;
    QString save_FilePath;
    QGroupBox *gridGroupBox;
    QLabel *label_selectedDate;
    QTextEdit *smallEditor;
    QTableWidget *tableWidget;
    QCalendarWidget *calendar;
    QTimer *tmr;
    QString timerstr;

    void createMenu();
    void createGridGroupBox();
    void clearTable();
    void paintCell(int flag, QDate Date);
    bool findTimer();
    bool open(int val);
    bool save(int val);
    bool on_date_clicked(QDate date);

private slots:
    void slot_on_date_clicked(QDate date)
    {
        on_date_clicked(date);
    }

    bool s_WriteNote();
    void timerOperation();

    void action_open()
     {
       open(1);
     }

    void action_save()
    {
       save(1);
    }

    bool  s_AddRow()
    {
        tableWidget->insertRow(tableWidget->rowCount());
        return true;
    }

    void  s_DelRow(int row,int col)
    {
            tableWidget->removeRow(row);
    }

    void action_exit()
    {
        save(0);
        QMessageBox msg;
        msg.setText(tr("Досвидания"));
        msg.exec();
        close();
    }

    void slotShortcutF11();

};

#endif // DIALOG_H
