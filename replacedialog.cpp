#include "replacedialog.h"
#include "ui_replacedialog.h"

#include<QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit *textEdit) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;

    ui->raDown->setChecked(true);

}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_btnSearchNext_clicked()
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


void ReplaceDialog::on_btnReplace_clicked()
{
    QString text = pTextEdit->toPlainText();
    QString to = ui->targetText->text();
    QString target = ui->searchText->text();
    if((pTextEdit!=nullptr)&&(target!="")&&(to!="")){
        QString selText = pTextEdit->textCursor().selectedText();
        if(selText == target)
        {
            pTextEdit->insertPlainText(to);
        }
    }

}


void ReplaceDialog::on_btnReplaceAll_clicked()
{
    QString target = ui->searchText->text();
    QString to = ui->targetText->text();

    if((pTextEdit!=nullptr)&&(target!="")&&(to!="")){
        QString text = pTextEdit->toPlainText();

        text.replace(target,to,ui->btnCaps->isChecked()?Qt::CaseInsensitive:Qt::CaseInsensitive);
        pTextEdit->clear();
        pTextEdit->insertPlainText(text);
    }

}


void ReplaceDialog::on_btnCancel_clicked()
{
    accept();
}

