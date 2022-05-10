#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contact.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}
#include <stack>
#include "Data.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui; 
    Data MyDataBase;



};

#endif // MAINWINDOW_H
