#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bencode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Bencode *bencode = new Bencode(this);
    bencode->fromBEncodedString(ui->lineEdit->text());
}