#include "mapLinePos.h"
#include <QCoreApplication>
CMapLinePos::CMapLinePos(QWidget *parent)
    : QWidget(parent)
{
    Init();
    XMLShowInit();
    HideWidgets();
    m_pXmlParse = new CMyXmlParse();

    QObject::connect(m_Open_Btn,SIGNAL(clicked()),this,SLOT(on_click_OpenSlot()));
    QObject::connect(m_Path_Btn,SIGNAL(clicked()),this,SLOT(on_click_GetFilePath_slot()));
    QObject::connect(m_Prev_pict_btn,SIGNAL(clicked()),this,SLOT(on_click_Show_Prev_slot()));
    QObject::connect(m_Next_pict_btn,SIGNAL(clicked()),this,SLOT(on_click_Show_Next_slot()));
    QObject::connect(m_Prev_Elem_Btn,SIGNAL(clicked()),this,SLOT(on_click_Show_prev_Station_slot()));
    QObject::connect(m_Next_Elem_Btn,SIGNAL(clicked()),this,SLOT(on_click_Show_next_Station_slot()));
    QObject::connect(m_Prev_Line_Bit,SIGNAL(clicked()),this,SLOT(on_click_Show_prev_Line_slot()));
    QObject::connect(m_Next_Line_Bit,SIGNAL(clicked()),this,SLOT(on_click_Show_next_Line_slot()));
    QObject::connect(m_CreateNewNode_Btn,SIGNAL(clicked()),this,SLOT(on_click_AddNewNode_slot()));
    QObject::connect(m_CreateNewXml_Btn,SIGNAL(clicked()),this,SLOT(on_click_CreateNewXml_slot()));
    QObject::connect(m_Multiple_RadioBtn,SIGNAL(clicked()),this,SLOT(Choice_Use_Multiple_slot()));

    QObject::connect(m_text_chs_edit,SIGNAL(editingFinished()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_text_en_edit,SIGNAL(editingFinished()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_stationId_edit,SIGNAL(editingFinished()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_Id_edit,SIGNAL(editingFinished()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_lineId_edit,SIGNAL(editingFinished()),this,SLOT(Edit_changed_slot()));

    QObject::connect(m_Chs_Pos_x,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_Chs_Pos_y,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_Chs_Pos_w,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_Chs_Pos_h,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_EN_Pos_x,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_EN_Pos_y,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_EN_Pos_w,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));
    QObject::connect(m_EN_Pos_h,SIGNAL(returnPressed()),this,SLOT(Edit_changed_slot()));


}
void CMapLinePos::Init()
{
    m_iIndex = 0;
    m_mywidget = this;
    //确定加载路径中文件的按钮
    m_Open_Btn = new QPushButton(this);
    m_Open_Btn->setGeometry(0,40,200,30);
    m_Open_Btn->setText("加载文件路径");
    m_Path_Btn = new QPushButton(this);
    m_Path_Btn->setGeometry(205,40,95,30);
    m_Path_Btn->setText("打开文件");

    m_Next_pict_btn = new QPushButton(this);
    m_Next_pict_btn->setText("下一张");
    m_Next_pict_btn->hide();
    m_Prev_pict_btn = new QPushButton(this);
    m_Prev_pict_btn->setText("上一张");
    m_Prev_pict_btn->hide();

    //路径文本输入框
    m_Path_Test = new QLineEdit(this);
    m_Path_Test->setGeometry(0,0,300,30);

    QString strPath = QCoreApplication::applicationDirPath();
#ifdef WIN32
    strPath.replace("/","\\");
    strPath += "\\";
#elif
    strPath += "/";
#endif
    m_Path_Test->setText(strPath);
    //线路图片展示
    m_PictureLabel = new QLabel(this);
    m_PictureLabel->hide();
    //提示框
    m_WarningMessage = new QMessageBox(this);
    m_WarningMessage->hide();
}

void CMapLinePos::mousePressEvent(QMouseEvent *e)
{
    QPoint pt = e->pos();
    if(pt.x() > m_iPic_Width || pt.y() > m_iPic_Height)
        return;
    //qDebug() << pt.x()<< pt.y();
    if(m_bIsChinese)
    {
        m_stStation.coords_chs.Top_x = pt.x();
        m_stStation.coords_chs.Top_y = pt.y();
        if(m_SaveLocation_CheckBox->isChecked())
        {
            m_stStation.coords_en.Top_x = pt.x();
            m_stStation.coords_en.Top_y = pt.y();
        }
    }
    else
    {
        m_stStation.coords_en.Top_x = pt.x();
        m_stStation.coords_en.Top_y = pt.y();
    }
    m_pXmlParse->RefleshParamMap(m_iStation,m_stStation);
    ParamShowReflesh(m_stStation);
}
void CMapLinePos::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint pt = e->pos();
    if(pt.x() > m_iPic_Width || pt.y() > m_iPic_Height)
        return;
    //qDebug() << pt.x()<< pt.y();
    if(m_bIsChinese)
    {
        m_stStation.coords_chs.Under_x = pt.x();
        m_stStation.coords_chs.Under_y = pt.y();
        if(m_SaveLocation_CheckBox->isChecked())
        {
            m_stStation.coords_en.Under_x = pt.x();
            m_stStation.coords_en.Under_y = pt.y();
        }
    }
    else
    {
        m_stStation.coords_en.Under_x = pt.x();
        m_stStation.coords_en.Under_y = pt.y();
    }
    m_pXmlParse->RefleshParamMap(m_iStation,m_stStation);
    ParamShowReflesh(m_stStation);
}

void CMapLinePos::paintEvent(QPaintEvent *e)
{

}

//内容改变相关处理
void CMapLinePos::Edit_changed_slot()
{
    UpdateParam();
    //ParamShowReflesh(m_stStation);
    m_pXmlParse->RefleshParamMap(m_iStation,m_stStation);
}

//上一张图片相关处理
void CMapLinePos::on_click_Show_Prev_slot()
{
    m_iIndex --;
    if(m_iIndex < 0)
        m_iIndex = m_Picture_list.size() - 1;

    QString PicturePath = m_Path_Test->text()+m_Picture_list.at(m_iIndex);
    m_bIsChinese = !strstr(PicturePath.toStdString().c_str(),"_en");
    QPixmap map(PicturePath);
    QImage image(PicturePath);
    m_iPic_Width = image.width();
    m_iPic_Height = image.height();
    m_PictureLabel->setGeometry(0,0,image.width(),image.height());
    m_PictureLabel->setPixmap(map);
    m_PictureLabel->setScaledContents(true);
}

//下一张图片相关处理
void CMapLinePos::on_click_Show_Next_slot()
{
    m_iIndex ++;
    if(m_iIndex >= m_Picture_list.size())
        m_iIndex = 0;

    QString PicturePath = m_Path_Test->text()+m_Picture_list.at(m_iIndex);
    m_bIsChinese = !strstr(PicturePath.toStdString().c_str(),"_en");
    QPixmap map(PicturePath);
    QImage image(PicturePath);
    m_iPic_Width = image.width();
    m_iPic_Height = image.height();
    m_PictureLabel->setGeometry(0,0,image.width(),image.height());
    m_PictureLabel->setPixmap(map);
    m_PictureLabel->setScaledContents(true);
}

//获取文件夹路径
void CMapLinePos::on_click_GetFilePath_slot()
{
    QString file_path = QFileDialog::getExistingDirectory(this,"请选择模板保存路径...","./");
    if(file_path.isEmpty())
    {
        m_WarningMessage->setText("路径为空");
        m_WarningMessage->show();
        return;
    }
    else
    {
#ifdef WIN32
        m_Path_Test->setText(file_path.replace("/","\\") + "\\");
#elif
        m_Path_Test->setText(file_path + "/");
#endif
    }

}

//打开文件相关处理
void CMapLinePos::on_click_OpenSlot()
{
    if(m_Path_Test->text().isEmpty())
    {
        m_WarningMessage->setText("请输入地图参数路径");
        m_WarningMessage->show();
        return;
    }

    QDir dir(m_Path_Test->text()+ "/");
    QStringList names;
    names <<"*.**g" ;
    m_Picture_list = dir.entryList(names,QDir::NoFilter,QDir::Name);
    if(m_Picture_list.isEmpty())
    {
        m_WarningMessage->setText("该路径下没有jpg或者png图片");
        m_WarningMessage->show();
        return;
    }
    QString xmlname = m_Path_Test->text() + "MapLineConfig.xml";
    qDebug() << xmlname;
    int iRet = m_pXmlParse->MapLineXmlParse(xmlname.toStdString().c_str());
    if(0 != iRet)
    {
        switch(iRet)
        {
        case -1:
            m_WarningMessage->setText("无MapLineConfig.xml文件");
            m_WarningMessage->show();
            break;
        case 1:
            m_WarningMessage->setText("打开文件配置失败");
            m_WarningMessage->show();
            break;
        case 2:
            m_WarningMessage->setText("配置文件内容为空");
            m_WarningMessage->show();
            break;
        }
        return;
    }
    QString LineStationname = m_Path_Test->text() + "LineStation.xml";
    m_pXmlParse->LineStationParse(LineStationname.toStdString().c_str());

    memset(&m_stStation,0,sizeof(m_stStation));
    memcpy(&m_stStation,&(m_pXmlParse->m_stPostionMap.find(m_iStation).value()),sizeof(ST_MAP_LINE_POSITION));

    qDebug() << m_Picture_list.size()<< m_Path_Test->text()+m_Picture_list.at(m_iIndex);
    QString PicturePath = m_Path_Test->text()+m_Picture_list.at(m_iIndex);
    m_bIsChinese = strstr("_en",PicturePath.toStdString().c_str());
    m_bIsChinese = !strstr(PicturePath.toStdString().c_str(),"_en");
    QPixmap map(PicturePath);
    QImage image(PicturePath);
    m_iPic_Width = image.width();
    m_iPic_Height = image.height();
    m_PictureLabel->setGeometry(0,0,image.width(),image.height());
    m_PictureLabel->setPixmap(map);
    m_PictureLabel->setScaledContents(true);
    m_PictureLabel->show();

    //窗口大小
    m_mywidget->setGeometry(50,50,image.width()+400,image.height());
    //m_mywidget->showMaximized();

    m_Prev_pict_btn->setGeometry(image.width(),0,100,30);
    m_Prev_pict_btn->show();
    m_Next_pict_btn->setGeometry(image.width()+105,0,100,30);
    m_Next_pict_btn->show();


    m_Open_Btn->hide();
    m_Path_Btn->hide();
    m_Path_Test->hide();
    ShowWidgets(image.width(),image.height());
}

//上一个站点参数显示处理
void CMapLinePos::on_click_Show_prev_Station_slot()
{
    m_iStation--;
    if(m_iStation < 0)
        m_iStation = m_pXmlParse->m_stPostionTempMap.size() - 1;
    memset(&m_stStation,0,sizeof(m_stStation));
    memcpy(&m_stStation,&(m_pXmlParse->m_stPostionTempMap.find(m_iStation).value()),sizeof(ST_MAP_LINE_POSITION));
    ParamShowReflesh(m_stStation);
}

//下一个站点参数显示处理
void CMapLinePos::on_click_Show_next_Station_slot()
{
    m_iStation++;
    if(m_iStation >= m_pXmlParse->m_stPostionTempMap.size())
        m_iStation = 0;
    memset(&m_stStation,0,sizeof(m_stStation));
    memcpy(&m_stStation,&(m_pXmlParse->m_stPostionTempMap.find(m_iStation).value()),sizeof(ST_MAP_LINE_POSITION));
    ParamShowReflesh(m_stStation);
}
//转跳下一条线路
void CMapLinePos::on_click_Show_next_Line_slot()
{
    int iCurLine = atoi(m_pXmlParse->m_stPostionTempMap.find(m_iStation).value().lineid);

    for(; m_iStation < m_pXmlParse->m_stPostionTempMap.size();m_iStation++)
    {
        if(iCurLine != atoi(m_pXmlParse->m_stPostionTempMap.find(m_iStation).value().lineid))
            break;
    }
    if(m_iStation >= m_pXmlParse->m_stPostionTempMap.size())
        m_iStation = 0;
    memset(&m_stStation,0,sizeof(m_stStation));
    memcpy(&m_stStation,&(m_pXmlParse->m_stPostionTempMap.find(m_iStation).value()),sizeof(ST_MAP_LINE_POSITION));
    ParamShowReflesh(m_stStation);
}
//转跳上一条线路
void CMapLinePos::on_click_Show_prev_Line_slot()
{

}

//添加新站点
void CMapLinePos::on_click_AddNewNode_slot()
{
    int iNewStationNum = m_pXmlParse->m_stPostionTempMap.size();
    ST_MAP_LINE_POSITION stStation;
    memset(&stStation,0,sizeof(stStation));
    m_pXmlParse->AddNewNode(iNewStationNum,stStation);
    //转跳到新建站点并显示
    m_iStation = iNewStationNum;
    ParamShowReflesh(stStation);
}

//选择生成坐标倍数生效
void CMapLinePos::Choice_Use_Multiple_slot()
{
    if(m_Multiple_RadioBtn->isChecked())
        m_Multiple->show();
    else
        m_Multiple->hide();
}

//生成新站点参数
void CMapLinePos::on_click_CreateNewXml_slot()
{
    bool bOk = 0;
    if(m_Multiple_RadioBtn->isChecked())
        m_pXmlParse->RewriteXml(m_Path_Test->text().toStdString().c_str(),NULL,m_Multiple->text().toFloat(&bOk));
    else
        m_pXmlParse->RewriteXml(m_Path_Test->text().toStdString().c_str(),NULL,1);

}

//更新参数
void CMapLinePos::UpdateParam()
{
    strcpy(m_stStation.text_chs,m_text_chs_edit->text().toStdString().c_str());
    strcpy(m_stStation.text_en,m_text_en_edit->text().toStdString().c_str());
    strcpy(m_stStation.stationId,m_stationId_edit->text().toStdString().c_str());
    strcpy(m_stStation.id,m_Id_edit->text().toStdString().c_str());
    strcpy(m_stStation.lineid,m_lineId_edit->text().toStdString().c_str());
    m_stStation.coords_chs.Top_x = m_Chs_Pos_x->text().toInt();
    m_stStation.coords_chs.Top_y = m_Chs_Pos_y->text().toInt();
    m_stStation.coords_chs.Under_x = m_Chs_Pos_w->text().toInt();
    m_stStation.coords_chs.Under_y = m_Chs_Pos_h->text().toInt();
    m_stStation.coords_en.Top_x = m_EN_Pos_x->text().toInt();
    m_stStation.coords_en.Top_y = m_EN_Pos_y->text().toInt();
    m_stStation.coords_en.Under_x = m_EN_Pos_w->text().toInt();
    m_stStation.coords_en.Under_y = m_EN_Pos_h->text().toInt();

}

//刷新参数
void CMapLinePos::ParamShowReflesh(ST_MAP_LINE_POSITION stPosition)
{
    m_text_chs_edit->setText(stPosition.text_chs);
    m_text_en_edit->setText(stPosition.text_en);
    m_stationId_edit->setText(stPosition.stationId);
    m_Id_edit->setText(stPosition.id);
    m_lineId_edit->setText(stPosition.lineid);
    m_Chs_Pos_x->setText(QString("%1").arg(stPosition.coords_chs.Top_x));
    m_Chs_Pos_y->setText(QString("%1").arg(stPosition.coords_chs.Top_y));
    m_Chs_Pos_w->setText(QString("%1").arg(stPosition.coords_chs.Under_x));
    m_Chs_Pos_h->setText(QString("%1").arg(stPosition.coords_chs.Under_y));
    m_EN_Pos_x->setText(QString("%1").arg(stPosition.coords_en.Top_x));
    m_EN_Pos_y->setText(QString("%1").arg(stPosition.coords_en.Top_y));
    m_EN_Pos_w->setText(QString("%1").arg(stPosition.coords_en.Under_x));
    m_EN_Pos_h->setText(QString("%1").arg(stPosition.coords_en.Under_y));
}

//xml解析后显示的相关控件初始化
void CMapLinePos::XMLShowInit()
{
    m_iLineId = 0;

    m_text_chs_label = new QLabel(this);
    m_text_en_label = new QLabel(this);
    m_stationId_label = new QLabel(this);
    m_Id_label = new QLabel(this);
    m_lineId_label = new QLabel(this);

    m_Chs_Pos_label = new QLabel(this);
    m_EN_Pos_label = new QLabel(this);

    m_text_chs_edit = new QLineEdit(this);
    m_text_en_edit = new QLineEdit(this);
    m_stationId_edit = new QLineEdit(this);
    m_Id_edit = new QLineEdit(this);
    m_lineId_edit = new QLineEdit(this);

    m_Chs_Pos_x = new QLineEdit(this);
    m_Chs_Pos_y = new QLineEdit(this);
    m_Chs_Pos_w = new QLineEdit(this);
    m_Chs_Pos_h = new QLineEdit(this);
    m_EN_Pos_x = new QLineEdit(this);
    m_EN_Pos_y = new QLineEdit(this);
    m_EN_Pos_w = new QLineEdit(this);
    m_EN_Pos_h = new QLineEdit(this);

    m_Next_Elem_Btn = new QPushButton(this);
    m_Next_Elem_Btn->setText("下一个站点");
    m_Prev_Elem_Btn = new QPushButton(this);
    m_Prev_Elem_Btn->setText("上一个站点");
    m_Prev_Line_Bit = new QPushButton(this);
    m_Prev_Line_Bit->setText("上一条线路");
    m_Next_Line_Bit = new QPushButton(this);
    m_Next_Line_Bit->setText("下一条线路");

    m_CreateNewNode_Btn = new QPushButton(this);
    m_CreateNewNode_Btn->setText("添加新站点");
    m_CreateNewXml_Btn = new QPushButton(this);
    m_CreateNewXml_Btn->setText("生成新XML");

    m_Multiple_RadioBtn = new QRadioButton(this);
    m_Multiple_label = new QLabel(this);
    m_Multiple_label->setText("大图生成坐标倍数:");
    m_Multiple = new QLineEdit(this);

    m_SaveLocation_CheckBox = new QCheckBox(this);
    m_SaveLocation_label = new QLabel(this);
    m_SaveLocation_label->setText("中文和英文坐标是否一样");
    m_iStation = 0;

}
//隐藏某些控件
void CMapLinePos::HideWidgets()
{
    m_text_chs_label->hide();
    m_text_chs_label->setText("中文站点名");
    m_text_en_label->hide();
    m_text_en_label->setText("英文站点名");
    m_stationId_label->hide();
    m_stationId_label->setText("站点ID");
    m_Id_label->hide();
    m_Id_label->setText("ID");
    m_lineId_label->hide();
    m_lineId_label->setText("线路ID");
    m_Chs_Pos_label->hide();
    m_Chs_Pos_label->setText("中文坐标(Top_x,Top_y,Under_x,Under_y)");
    m_EN_Pos_label->hide();
    m_EN_Pos_label->setText("英文坐标(Top_x,Top_y,Under_x,Under_y)");

    m_text_chs_edit->hide();
    m_text_en_edit->hide();
    m_stationId_edit->hide();
    m_Id_edit->hide();
    m_lineId_edit->hide();
    m_Chs_Pos_x->hide();
    m_Chs_Pos_y->hide();
    m_Chs_Pos_w->hide();
    m_Chs_Pos_h->hide();
    m_EN_Pos_x->hide();
    m_EN_Pos_y->hide();
    m_EN_Pos_w->hide();
    m_EN_Pos_h->hide();

    m_Next_Elem_Btn->hide();
    m_Prev_Elem_Btn->hide();
    m_Prev_Line_Bit->hide();
    m_Next_Line_Bit->hide();
    m_CreateNewXml_Btn->hide();
    m_CreateNewNode_Btn->hide();

    m_Multiple_label->hide();
    m_Multiple->hide();
    m_SaveLocation_CheckBox->hide();
    m_SaveLocation_label->hide();
}
//展示某些控件
void CMapLinePos::ShowWidgets(int Width,int Height)
{
    QMap<int,ST_MAP_LINE_POSITION>::iterator it = m_pXmlParse->m_stPostionMap.find(0);
    memset(&m_stStation,0,sizeof(m_stStation));
    memcpy(&m_stStation,&(it.value()),sizeof(ST_MAP_LINE_POSITION));

    int iHeight = 30;
    //中文站点名
    m_text_chs_label->setGeometry(Width,iHeight,100,25);
    m_text_chs_label->show();
    iHeight+=25;
    m_text_chs_edit->setGeometry(Width,iHeight,400,25);
    m_text_chs_edit->show();
    iHeight+=25;
    //英文站点名
    m_text_en_label->setGeometry(Width,iHeight,100,25);
    m_text_en_label->show();
    iHeight+=25;
    m_text_en_edit->setGeometry(Width,iHeight,400,25);
    m_text_en_edit->show();
    iHeight+=25;

    //站点ID
    m_stationId_label->setGeometry(Width,iHeight,50,25);
    m_stationId_label->show();
    m_Id_label->setGeometry(Width + 60,iHeight,50,25);
    m_Id_label->show();
    m_lineId_label->setGeometry(Width + 120,iHeight,50,25);
    m_lineId_label->show();

    iHeight+=25;
    m_stationId_edit->setGeometry(Width,iHeight,50,25);
    m_stationId_edit->show();
    m_Id_edit->setGeometry(Width + 60,iHeight,50,25);
    m_Id_edit->show();
    m_lineId_edit->setGeometry(Width + 120,iHeight,50,25);
    m_lineId_edit->show();
    iHeight+=25;

    //中文坐标
    m_Chs_Pos_label->setGeometry(Width,iHeight,300,25);
    m_Chs_Pos_label->show();
    iHeight+=25;
    m_Chs_Pos_x->setGeometry(Width,iHeight,50,25);
    m_Chs_Pos_x->show();
    m_Chs_Pos_y->setGeometry(Width+60,iHeight,50,25);
    m_Chs_Pos_y->show();
    m_Chs_Pos_w->setGeometry(Width+120,iHeight,50,25);
    m_Chs_Pos_w->show();
    m_Chs_Pos_h->setGeometry(Width+180,iHeight,50,25);
    m_Chs_Pos_h->show();
    iHeight+=25;

    //英文坐标
    m_EN_Pos_label->setGeometry(Width,iHeight,300,25);
    m_EN_Pos_label->show();
    iHeight+=25;
    m_EN_Pos_x->setGeometry(Width,iHeight,50,25);
    m_EN_Pos_x->show();
    m_EN_Pos_y->setGeometry(Width+60,iHeight,50,25);
    m_EN_Pos_y->show();
    m_EN_Pos_w->setGeometry(Width+120,iHeight,50,25);
    m_EN_Pos_w->show();
    m_EN_Pos_h->setGeometry(Width+180,iHeight,50,25);
    m_EN_Pos_h->show();
    iHeight+=30;

    m_Prev_Elem_Btn->setGeometry(Width,iHeight,100,30);
    m_Prev_Elem_Btn->show();
    m_Next_Elem_Btn->setGeometry(Width + 105,iHeight,100,30);
    m_Next_Elem_Btn->show();
    iHeight+=30;
    m_Prev_Line_Bit->setGeometry(Width,iHeight,100,30);
    m_Prev_Line_Bit->show();
    m_Next_Line_Bit->setGeometry(Width+105,iHeight,100,30);
    m_Next_Line_Bit->show();
    iHeight+=30;
    m_CreateNewNode_Btn->setGeometry(Width,iHeight,100,30);
    m_CreateNewNode_Btn->show();
    m_CreateNewXml_Btn->setGeometry(Width + 105,iHeight,100,30);
    m_CreateNewXml_Btn->show();
    iHeight+=30;
    m_Multiple_label->setGeometry(Width,iHeight,100,30);
    m_Multiple_label->show();
    m_Multiple_RadioBtn->setGeometry(Width+100,iHeight,30,30);
    m_Multiple->setGeometry(Width+130,iHeight,50,30);

    iHeight+=30;
    m_SaveLocation_CheckBox->setGeometry(Width,iHeight,30,30);
    m_SaveLocation_CheckBox->show();
    m_SaveLocation_label->setGeometry(Width+30,iHeight,200,30);
    m_SaveLocation_label->show();
    ParamShowReflesh(m_stStation);
}

CMapLinePos::~CMapLinePos()
{
    delete m_PictureLabel;
    delete m_Open_Btn;
    delete m_Next_pict_btn;
    delete m_Path_Test;
    delete m_WarningMessage;

    delete m_text_chs_label;
    delete m_text_en_label;
    delete m_stationId_label;
    delete m_Id_label;
    delete m_lineId_label;
    delete m_Chs_Pos_label;
    delete m_EN_Pos_label;
    delete m_text_chs_edit;
    delete m_text_en_edit;
    delete m_Id_edit;
    delete m_lineId_edit;
    delete m_Chs_Pos_x;
    delete m_Chs_Pos_y;
    delete m_Chs_Pos_w;
    delete m_Chs_Pos_h;
    delete m_EN_Pos_x;
    delete m_EN_Pos_y;
    delete m_EN_Pos_w;
    delete m_EN_Pos_h;
    delete m_pXmlParse;
    delete m_Prev_Elem_Btn;
    delete m_Next_Elem_Btn;
    delete m_Next_Line_Bit;
    delete m_Prev_Line_Bit;
    delete m_CreateNewNode_Btn;
    delete m_CreateNewXml_Btn;
    delete m_Multiple_RadioBtn;
    delete m_Multiple_label;
    delete m_Multiple;
    delete m_SaveLocation_CheckBox;
    delete m_SaveLocation_label;
}


