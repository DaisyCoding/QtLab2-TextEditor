#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel.setMinimumWidth(120);
    statusLabel.setText(("length:  "+QString::number(0)+"  lines:  "+QString::number(1)));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMinimumWidth(90);
    statusCursorLabel.setText(("Ln:  "+QString::number(0)+"  Col:  "+QString::number(1)));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("章皖俊"));
    ui->statusbar->addPermanentWidget(author);
}

MainWindow::~MainWindow()
{
    delete ui;
}

