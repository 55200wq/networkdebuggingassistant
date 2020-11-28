#include "testForm.h"
#include "ui_testForm.h"

testForm::testForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::testForm)
{
    ui->setupUi(this);
}

testForm::~testForm()
{
    delete ui;
}
