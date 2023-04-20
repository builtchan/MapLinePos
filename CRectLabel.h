#ifndef CRECTLABEL_H
#define CRECTLABEL_H

#include <QLabel>
#include <QDebug>
class CRectLabel : public QLabel
{
    Q_OBJECT
public:
    CRectLabel(QWidget *p = NULL):QLabel(p)
    {
        this->setGeometry(0,0,0,0);
        this->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(39, 72, 98);background-color:rgba(0,0,0,0);");
    }
    ~CRectLabel()
    {

    }

    void focusIn(bool bFocus)
    {
        if(bFocus)
        {
            qDebug() << __FUNCTION__ << __LINE__ << bFocus;
            this->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 66, 93);background-color:rgba(0,0,0,0);");
        }
        else
        {
            qDebug() << __FUNCTION__ << __LINE__ << bFocus;
            this->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(107, 194, 53);background-color:rgba(0,0,0,0);");
        }
    }
};

#endif // CRECTLABEL_H
