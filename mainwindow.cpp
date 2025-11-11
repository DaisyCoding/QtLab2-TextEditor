#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged = false;
    on_actionNew_triggered();

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


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed())
        return;

    filePath = "";
    ui->TextEdit->clear();
    this->setWindowTitle(tr("新建文本文档 - 编辑器"));

    textChanged = false;
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())
        return;

    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",
                                                    tr("Text files(*.txt);;All(*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,"提示","打开文件失败");
        return;
    }
    filePath = filename;
    QTextStream in(&file);
    QString text = in.readAll();
    ui->TextEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionSave_triggered()
{
    if(filePath =="")
    {
        QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",
                                                        tr("Text files(*.txt)"));
        QFile file(filename);

        if(!file.open(QFile::WriteOnly|QFile::Text)){
            QMessageBox::warning(this,"提示","打开保存文件失败");
            return;
        }
        filePath = filename;

    }
    QFile file(filePath);


    // //如果文本框不为空 则通过判断filepath是否为空来判断要保存的是原有文件内容还是文本框里的内容
    // if(!ui->TextEdit->toPlainText().isEmpty())
    // {
    //     //不为空时，则保存原有文件内容
    //     if(!filePath.isEmpty())
    //     {
    //         //保存时，因为之前关闭了文件，所以要重新打开
    //         if(!file.open(QFile::WriteOnly|QFile::Text)){
    //             QMessageBox::warning(this,"提示","打开文件失败");

    //             QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",
    //                                                             tr("Text files(*.txt)"));
    //             QFile file(filename);

    //             if(!file.open(QFile::WriteOnly|QFile::Text)){
    //                 QMessageBox::warning(this,"提示","打开保存文件失败");
    //                 return;
    //             }
    //             filePath = filename;

    //         }
    //     }
    //     //为空，则保存文本框内容(没有路径则使用另存为)
    //     else
    //     {
    //         on_actionSaveAs_triggered();
    //         return;
    //     }
    // }
    // //文本框为空，证明没内容
    // else{
    //     QMessageBox::warning(this,"提示","文本为空");
    //     return;
    // }

    //保存文件的核心:
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    QMessageBox::information(this,"提示","保存成功");
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionSaveAs_triggered()
{

    QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",
                                                    tr("Text files(*.txt)"));
    QFile file(filename);

    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"提示","打开保存文件失败");
        return;
    }

    filePath = filename;

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    QMessageBox::information(this,"提示","保存成功");
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textChanged = false;
}


void MainWindow::on_TextEdit_textChanged()
{
    if(!textChanged)//如果文本改变了，将标题添加一个*，将textchanged置为true
    {
        this->setWindowTitle("*"+this->windowTitle());
        textChanged = true;
    }
}

bool MainWindow::userEditConfirmed()
{
    if(textChanged)
    {
        QString path = (filePath!="")?filePath:"无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("..");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n")+"\""+path+"\"?");
        msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

        int r = msg.exec();
        switch(r){
        case QMessageBox::Yes:
            on_actionSave_triggered();//如果同意保存的话 直接调用槽函数保存
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }
    return true;
}

