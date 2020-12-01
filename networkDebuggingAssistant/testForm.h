#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>

namespace Ui {
class testForm;
}

class testForm : public QWidget
{
    Q_OBJECT

public:
    explicit testForm(QWidget *parent = nullptr);
    ~testForm();
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE ;

private:
    Ui::testForm *ui;
    QImage img;
};

#endif // TESTFORM_H
