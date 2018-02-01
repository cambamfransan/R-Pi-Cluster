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

void MainWindow::addConnection(QHostAddress ip, quint16 port)
{
  m_connections.emplace_back(ip, port);
  QString toOutput;
  for (auto&& p : m_connections)
  {
    auto s = p.first.toString();
    if (s.contains("::ffff:"))s=s.mid(7, s.size() - 7);
    toOutput = toOutput + s + ": " + QString::number(p.second) + "\n";
  }
  ui->label_connected->setText(toOutput);
}

void MainWindow::receivedMsg(std::string s, QHostAddress ip, quint16 port)
{
  auto t = ip.toString();
  if (t.contains("::ffff:"))t = t.mid(7, static_cast<int>(s.size()) - 7);
  QString toOutput = t + ": " + QString::number(port) + "\n" + QString::fromStdString(s);
  ui->label_received->setText(toOutput);
}

void MainWindow::onSendButton()
{
  emit signalClicked(ui->lineEdit_toSend->text().toStdString());
}
