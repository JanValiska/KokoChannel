#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();

    void getPackets();

    void on_calcButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString filename;
    QByteArray inputarray;
    QByteArray outputarray;
    int pkts;
};

#endif // MAINWINDOW_H
