#ifndef CMYLABEL_H
#define CMYLABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <CRectLabel.h>
class CMyLabel : public QLabel
{
    Q_OBJECT
public:
    CMyLabel(QWidget *p = NULL):QLabel(p)
    {
        m_pFocusRectLabel = NULL;
    }

    void setCurrentRectLabel(CRectLabel *p)
    {
        if(NULL != m_pFocusRectLabel)
            m_pFocusRectLabel->focusIn(false);
        m_pFocusRectLabel = p;
        if(NULL != m_pFocusRectLabel)
            m_pFocusRectLabel->focusIn(true);
    }

signals:
    void touchRectPos(int iTopX,int iTopY,int iUnderX,int iUnderY);

protected:
    void mousePressEvent(QMouseEvent *ev)
    {
        if(NULL != m_pFocusRectLabel)
        {
            m_pFocusRectLabel->setGeometry(ev->pos().x(),ev->pos().y(),0,0);
            m_iFixedX = ev->pos().x();
            m_iFixedY = ev->pos().y();
        }
    }

    void mouseMoveEvent(QMouseEvent *ev)
    {
        if(NULL == m_pFocusRectLabel)
            return;
        //左下角拉伸
        if(ev->pos().x() >= m_iFixedX && ev->pos().y() >= m_iFixedY)
            m_pFocusRectLabel->setGeometry(m_iFixedX,m_iFixedY,abs(m_iFixedX - ev->pos().x()),abs(m_iFixedY - ev->pos().y()));
        //左上角拉伸
        else if(ev->pos().x() >= m_iFixedX && ev->pos().y() < m_iFixedY)
            m_pFocusRectLabel->setGeometry(m_iFixedX,m_iFixedY-abs(m_iFixedY - ev->pos().y()),abs(m_iFixedX - ev->pos().x()),abs(m_iFixedY - ev->pos().y()));
        //右下角拉伸
        else if(ev->pos().x() < m_iFixedX && ev->pos().y() >= m_iFixedY)
            m_pFocusRectLabel->setGeometry(m_iFixedX-abs(m_iFixedX - ev->pos().x()),m_iFixedY,abs(m_iFixedX - ev->pos().x()),abs(m_iFixedY - ev->pos().y()));
        //右上角拉伸
        else if(ev->pos().x() < m_iFixedX && ev->pos().y() < m_iFixedY)
            m_pFocusRectLabel->setGeometry(m_iFixedX-abs(m_iFixedX - ev->pos().x()),m_iFixedY-abs(m_iFixedY - ev->pos().y()),abs(m_iFixedX - ev->pos().x()),abs(m_iFixedY - ev->pos().y()));
    }

    void mouseReleaseEvent(QMouseEvent *ev)
    {
        if(NULL == m_pFocusRectLabel)
            return;
        emit touchRectPos(m_pFocusRectLabel->x(),m_pFocusRectLabel->y(),m_pFocusRectLabel->x() + m_pFocusRectLabel->width(),m_pFocusRectLabel->y()+m_pFocusRectLabel->height());
    }

private:
    CRectLabel *m_pFocusRectLabel;
    int m_iFixedX;
    int m_iFixedY;
};



#endif // CMYLABEL_H
