#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(
    ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(onSendButton()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setPort(quint16 port)
{
  ui->label_port->setText("Port: " + QString::number(port));
}

void MainWindow::onSendButton()
{
  ui->label_connected->setText(QString("Clicked"));
  emit signalClicked(ui->lineEdit_toSend->text().toStdString());
}
