#ifndef MAPLINEPOS_H
#define MAPLINEPOS_H
/*
 *  author  :chenjc
 *  date    :20170208
 *  function:通过点击线路图来生成或者修改MapLineConfig.xml
 *
*/
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDir>
#include <QKeyEvent>
#include <QTimer>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include "xmlparse.h"
#include <QFileDialog>
#include <QRadioButton>

class CMapLinePos : public QWidget
{
  Q_OBJECT

private:

    QLabel *m_PictureLabel;         //图片展示
    QStringList m_Picture_list;     //图片列表
    QStringList m_XML_list;         //xml配置文件列表
    QPushButton *m_Open_Btn;        //确认加载路径
    QPushButton *m_Path_Btn;        //触发打开文件夹路径
    QPushButton *m_Prev_pict_btn;   //上一张图片
    QPushButton *m_Next_pict_btn;   //下一张图片
    QLineEdit *m_Path_Test;         //路径输入文本框
    QMessageBox *m_WarningMessage;  //提示框
    QWidget *m_mywidget;            //当前窗口

    int m_iIndex;                   //当前图片索引
    bool m_bIsChinese;              //当前图片是否是中文
    int m_iPic_Width;
    int m_iPic_Height;

//站点相关
private:
    int m_iLineId;                  //线路id
    int m_iStation;                 //站点索引
    ST_MAP_LINE_POSITION m_stStation;

    QLabel *m_text_chs_label;
    QLineEdit *m_text_chs_edit;     //中文站点名称
    QLabel *m_text_en_label;
    QLineEdit *m_text_en_edit;      //英文站点名称
    QLabel *m_stationId_label;
    QLineEdit *m_stationId_edit;    //站点id
    QLabel *m_Id_label;
    QLineEdit *m_Id_edit;           //id
    QLabel *m_lineId_label;
    QLineEdit *m_lineId_edit;       //线路id

    QLabel *m_Chs_Pos_label;        //中文站点坐标
    QLineEdit *m_Chs_Pos_x;
    QLineEdit *m_Chs_Pos_y;
    QLineEdit *m_Chs_Pos_w;
    QLineEdit *m_Chs_Pos_h;

    QLabel *m_EN_Pos_label;         //英文站点坐标
    QLineEdit *m_EN_Pos_x;
    QLineEdit *m_EN_Pos_y;
    QLineEdit *m_EN_Pos_w;
    QLineEdit *m_EN_Pos_h;

    QPushButton *m_Next_Elem_Btn;    //下一个站点参数展示按钮
    QPushButton *m_Prev_Elem_Btn;    //上一个站点参数展示按钮
    QPushButton *m_CreateNewNode_Btn; //生成新节点
    QPushButton *m_CreateNewXml_Btn; //更新站点参数

    QRadioButton *m_Multiple_RadioBtn;  //是否使用倍数
    QLabel *m_Multiple_label;           //倍数
    QLineEdit *m_Multiple;


public:
    CMyXmlParse *m_pXmlParse;

public:
    CMapLinePos(QWidget *parent = 0);
    ~CMapLinePos();

    void Init();
    void XMLShowInit();
    void HideWidgets();
    void ShowWidgets(int Width,int Height);
    void ParseXml(QString XmlPath);
    void UpdateParam();                                       //刷新参数
    void ParamShowReflesh(ST_MAP_LINE_POSITION stPosition);  //刷新显示参数


protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    //void changeEvent(QEvent *e);

public slots:
    void on_click_Show_Next_slot();
    void on_click_Show_Prev_slot();
    void on_click_OpenSlot();
    void on_click_Show_prev_Station_slot();
    void on_click_Show_next_Station_slot();
    void on_click_AddNewNode_slot();
    void on_click_CreateNewXml_slot();
    void on_click_GetFilePath_slot();
    void Edit_changed_slot();
    void Choice_Use_Multiple_slot();

};

#endif // MAPLINEPOS_H
