#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <random>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    filename = "";
    this->setFixedSize(500 ,400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Open Transport Stream file"), "", tr("TS Files (*.ts)"));
    if(!fname.isEmpty()){
        filename = fname;
        getPackets();
        ui->statusBar->showMessage("Input file loaded!", 4000);
    } else {
        ui->statusBar->showMessage("Nothing selected!", 4000);
    }
}

void MainWindow::getPackets(){
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        qint64 size = file.size();
        inputarray = file.readAll();
        file.close();
        if(inputarray[0] != 0x47){
            qDebug() << "Stream sa nezacina 0x47";
        }
        else {
            pkts = size/188;
            QString packets = QString::number(pkts);
            ui->label_4->setText(packets);
            ui->label_3->setText(QFileInfo(filename).fileName());
            ui->calcButton->setEnabled(true);
            ui->groupBox->setEnabled(true);
        }
}

void MainWindow::on_calcButton_clicked()
{
    outputarray.clear();
    std::default_random_engine generator1;
    std::default_random_engine generator2;
    std::uniform_real_distribution<double> distribution1(0.0,1.0);
    std::uniform_real_distribution<double> distribution2(0.0,1.0);

    double gTh = ui->lineEdit->text().toDouble();
    double bTh = ui->lineEdit_2->text().toDouble();

    bool state = true;
    int size = inputarray.size();
    int one = size/100;

    for (int i=0; i < pkts; i++) {
      double g = distribution1(generator1);
      double b = distribution2(generator2);
      if(state == true){
        if(g > gTh){
            state = false;
        }
      }
      else {
          if(b > bTh){
              state = true;
          }
      }

      if(state == true){
        const char *data = inputarray.constData();
        outputarray.append(data+(i*188), 188);
      }

      ui->progressBar->setValue((i*188)/one);
    }
    ui->progressBar->setValue(100);
    QString packets = QString::number(outputarray.size()/188);
    ui->label_9->setText(packets);
    ui->pushButton_2->setEnabled(true);
    ui->statusBar->showMessage("Calculated!", 4000);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString savefile = QFileDialog::getSaveFileName(this, tr("Save output TS file"), "", tr("TS Files (*.ts)"));
    QFile file(savefile);
    file.open(QIODevice::WriteOnly);
    file.write(outputarray);
    file.close();
    ui->statusBar->showMessage("File saved!", 4000);
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
