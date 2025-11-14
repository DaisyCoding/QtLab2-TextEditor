#include "searchdialog.h"
#include "ui_searchdialog.h"
#include<QMessageBox>

SearchDialog::SearchDialog(QWidget *parent ,QPlainTextEdit * textEdit) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;

    ui->raDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btnFindNext_clicked()
{
    QString target = ui->searchText->text();
    if(target==""||pTextEdit==nullptr)
    {
        return;
    }

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();
    int index = -1;

    if(ui->raDown->isChecked()){

        index = text.indexOf(target,c.position(),ui->btnCaps->isChecked()?Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index>=0)
        {
            c.setPosition(index);//找到，则移动光标到目标位置，再次点击可以接着找
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }
    else if(ui->raUp->isChecked())
    {
        index = text.lastIndexOf(target,c.position()-text.length()-1,ui->btnCaps->isChecked()?Qt::CaseSensitive  : Qt::CaseInsensitive);
        if(index>=0)
        {
            c.setPosition(index+target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);//找到，则移动光标到目标位置，再次点击可以接着找

            pTextEdit->setTextCursor(c);
        }
    }
    if(index<0)
    {
        QMessageBox msg(this);
        msg.setWindowTitle("提示");
        msg.setText(QString("找不到")+target);
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}


void SearchDialog::on_btnCancel_clicked()
{
    accept();
}

