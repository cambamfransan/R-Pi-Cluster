#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

signals:
  void signalClicked(std::string);

public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

  void setPort(quint16 port);

private slots:
  void onSendButton();

private:
  Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
