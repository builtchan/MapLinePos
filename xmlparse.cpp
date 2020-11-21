

#include "xmlparse.h"

#pragma execution_character_set("utf-8")
CMyXmlParse::CMyXmlParse()
{
  iNum = 1;
  memset(&m_stLineStation,0,sizeof(m_stLineStation));
}

int CMyXmlParse::MapLineXmlParse(const char *pzFileName)
{
    qDebug() << pzFileName;
    if(NULL == pzFileName)
        return -1;

    //1、打开xml文件
    QFile file(pzFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open xml file failed" << pzFileName;
        return 1;
    }

    //2、将text文件以utf-8转成流数据
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("utf-8"));

    //3、转成String后再转换成文档
    QString string = stream.readAll();
    QDomDocument Domc;
    Domc.setContent(string);
    if(Domc.isNull() || file.size() <= 0)
    {
        qDebug() << "The xml file is Empty";
        return 2;
    }

    //根节点LineMap
    QDomElement Root = Domc.documentElement();
    //第一个Position元素
    QDomElement Position = Root.firstChildElement();


    ST_MAP_LINE_POSITION stPositon;
    memset(&stPositon,0,sizeof(stPositon));
    bool ok = true;
    int iCount = 0;
    while(!Position.isNull())
    {
        if(Position.hasAttribute("text_chs"))
            strcpy(stPositon.text_chs,Position.attributeNode("text_chs").value().toStdString().c_str());
        if(Position.hasAttribute("text_en"))
            strcpy(stPositon.text_en,Position.attributeNode("text_en").value().toStdString().c_str());
        if(Position.hasAttribute("id"))
            strcpy(stPositon.id,Position.attributeNode("id").value().toStdString().c_str());
        if(Position.hasAttribute("stationId"))
            strcpy(stPositon.stationId,Position.attributeNode("stationId").value().toStdString().c_str());
        if(Position.hasAttribute("lineId"))
            strcpy(stPositon.lineid,Position.attributeNode("lineId").value().toStdString().c_str());

        qDebug() << stPositon.id << stPositon.stationId << stPositon.lineid << stPositon.text_en;
        if(Position.hasAttribute("coords_chs"))
        {
            QString pos = Position.attributeNode("coords_chs").value();
             qDebug() <<pos;
            QStringList poslist = pos.split(",");
            if(4 == poslist.size())
            {
                stPositon.coords_chs.Top_x = poslist.at(0).toInt(&ok,10);
                stPositon.coords_chs.Top_y = poslist.at(1).toInt(&ok,10);
                stPositon.coords_chs.Under_x = poslist.at(2).toInt(&ok,10);
                stPositon.coords_chs.Under_y = poslist.at(3).toInt(&ok,10);
            }
            else
            {
                stPositon.coords_chs.Top_x = 0;
                stPositon.coords_chs.Top_y = 0;
                stPositon.coords_chs.Under_x = 0;
                stPositon.coords_chs.Under_y = 0;
            }
        }
        if(Position.hasAttribute("coords_en"))
        {
            QString pos = Position.attributeNode("coords_en").value();
            qDebug() <<pos;
            QStringList poslist = pos.split(",");
            if(4 == poslist.size())
            {
                stPositon.coords_en.Top_x = poslist.at(0).toInt(&ok,10);
                stPositon.coords_en.Top_y = poslist.at(1).toInt(&ok,10);
                stPositon.coords_en.Under_x = poslist.at(2).toInt(&ok,10);
                stPositon.coords_en.Under_y = poslist.at(3).toInt(&ok,10);
            }
            else
            {
                stPositon.coords_en.Top_x = 0;
                stPositon.coords_en.Top_y = 0;
                stPositon.coords_en.Under_x = 0;
                stPositon.coords_en.Under_y = 0;
            }
        }
        //下一个元素
        Position = Position.nextSiblingElement();
        //将解析到的内容加到列表里
        m_stPostionMap.insert(iCount,stPositon);
        m_stPostionTempMap.insert(iCount++,stPositon);
    }
    file.close();
    return 0;
}
//刷新参数Map表
int CMyXmlParse::RefleshParamMap(int iKey,ST_MAP_LINE_POSITION stPosition)
{
  m_stPostionTempMap[iKey] = stPosition;
  return 0;
}
//添加新站点
int CMyXmlParse::AddNewNode(int iKey,ST_MAP_LINE_POSITION stPosition)
{
    m_stPostionTempMap.insert(iKey,stPosition);
    return 0;
}

//重新生成MapLineConfig文件
int CMyXmlParse::RewriteXml(const char *pzPath,const char*pNewFilename,float fMultiple)
{
  QString NewFileName;
  if(NULL != pNewFilename)
    NewFileName = QString("%1").arg(pzPath) + QString("%1").arg(pNewFilename);
  else
    NewFileName = QString("%1").arg(pzPath) + "MapLineConfig_new.xml";

  //1、打开xml文件
  QFile file(NewFileName);
  if(!file.open(QFile::WriteOnly | QFile::Text))
  {
      qDebug() << "open xml file failed" <<NewFileName;
      return 1;
  }

  QDomDocument Domc;
  QTextStream stream(&file);

  QDomElement root_elem = Domc.createElement("LineMap");

  ST_MAP_LINE_POSITION stPosition;

  for(int i = 0 ; i < m_stPostionTempMap.size(); i++)
  {
    memset(&stPosition,0,sizeof(stPosition));
    memcpy(&stPosition,&(m_stPostionTempMap.find(i).value()),sizeof(stPosition));
    QDomElement position = Domc.createElement("Position");

    position.setAttribute("text_chs",QString("%1").arg(stPosition.text_chs));
    position.setAttribute("text_en",QString("%1").arg(stPosition.text_en));
    position.setAttribute("stationId",QString("%1").arg(stPosition.stationId));
    position.setAttribute("id",QString("%1").arg(stPosition.id));
    position.setAttribute("lineId",QString("%1").arg(stPosition.lineid));

    //big pictrue need Multiple
    if(0 == stPosition.lineid)
    {
        QString ChsPos = QString("%1,%2,%3,%4").arg((int)(stPosition.coords_chs.Top_x * fMultiple)).arg((int)(stPosition.coords_chs.Top_y * fMultiple)).arg((int)(stPosition.coords_chs.Under_x * fMultiple)).arg((int)(stPosition.coords_chs.Under_y * fMultiple));
        position.setAttribute("coords_chs",ChsPos);
        QString EngPos = QString("%1,%2,%3,%4").arg((int)(stPosition.coords_en.Top_x * fMultiple)).arg((int)(stPosition.coords_en.Top_y *fMultiple)).arg((int)(stPosition.coords_en.Under_x * fMultiple)).arg((int)(stPosition.coords_en.Under_y * fMultiple));
        position.setAttribute("coords_en",EngPos);
    }
    else
    {
        QString ChsPos = QString("%1,%2,%3,%4").arg(stPosition.coords_chs.Top_x).arg(stPosition.coords_chs.Top_y).arg(stPosition.coords_chs.Under_x).arg(stPosition.coords_chs.Under_y);
        position.setAttribute("coords_chs",ChsPos);
        QString EngPos = QString("%1,%2,%3,%4").arg(stPosition.coords_en.Top_x).arg(stPosition.coords_en.Top_y).arg(stPosition.coords_en.Under_x).arg(stPosition.coords_en.Under_y);
        position.setAttribute("coords_en",EngPos);
    }

    root_elem.appendChild(position);
  }
  Domc.appendChild(root_elem);

  file.seek(0);
  Domc.save(stream,4);
  file.close();

  //CreatLineStation(pzPath);
}

//解析LineStation.xml
int CMyXmlParse::LineStationParse(const char *pzFileName)
{
    if(NULL == pzFileName)
      return -1;

    //1、打开xml文件
    QFile file(pzFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
      qDebug() << "open xml file failed" << pzFileName;
      return 1;
    }

    //2、将text文件以utf-8转成流数据
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("utf-8"));

    //3、转成String后再转换成文档
    QString string = stream.readAll();
    QDomDocument Domc;
    Domc.setContent(string);
    if(Domc.isNull() || file.size() <= 0)
    {
       qDebug() << "The xml file is Empty";
       return 2;
    }

    QDomElement QERoot = Domc.firstChildElement();
    QDomElement QELineCount = QERoot.firstChildElement();
    QDomElement QELineList = QELineCount.nextSiblingElement();
    QDomElement QELine = QELineList.firstChildElement();
    //线路
    ST_LINE_LIST stLine;
    //站点
    ST_STATION_LIST stStation;
    m_stLineStation.iLineCount = QELineCount.text().toInt();
    DebugPrint(QELineCount);
    for(int iLineCnt = 0; iLineCnt < m_stLineStation.iLineCount; iLineCnt++)
    {
        memset(&stLine,0,sizeof(stLine));
        static QDomElement QEId = QELine.firstChildElement();
        strcpy(stLine.zId,QEId.text().toStdString().c_str());
        DebugPrint(QEId);
        static QDomElement QELocationNum = QEId.nextSiblingElement();
        stLine.iLocationNum = QELocationNum.text().toInt();
        DebugPrint(QELocationNum);
        static QDomElement QELineCenterId = QELocationNum.nextSiblingElement();
        stLine.iLineCenterId = QELineCenterId.text().toInt();
        DebugPrint(QELineCenterId);
        static QDomElement QELineId = QELineCenterId.nextSiblingElement();
        stLine.iLineId = QELineId.text().toInt();
        DebugPrint(QELineId);
        static QDomElement QELineName_CN = QELineId.nextSiblingElement();
        strcpy(stLine.zName_CN,QELineName_CN.text().toStdString().c_str());
        DebugPrint(QELineName_CN);
        static QDomElement QELineName_EN = QELineName_CN.nextSiblingElement();
        strcpy(stLine.zName_EN,QELineName_EN.text().toStdString().c_str());
        DebugPrint(QELineName_EN);
        static QDomElement QELneShortName_EN = QELineName_EN.nextSiblingElement();
        strcpy(stLine.zShortEnName,QELneShortName_EN.text().toStdString().c_str());
        DebugPrint(QELneShortName_EN);
        static QDomElement QEUpBeginMoment = QELneShortName_EN.nextSiblingElement();
        strcpy(stLine.zUpBeginMoment,QEUpBeginMoment.text().toStdString().c_str());
        DebugPrint(QEUpBeginMoment);
        static QDomElement QEUpEndMoment = QEUpBeginMoment.nextSiblingElement();
        strcpy(stLine.zUpEndMoment,QEUpEndMoment.text().toStdString().c_str());
        DebugPrint(QEUpEndMoment);
        static QDomElement QEDownBeginMoment = QEUpEndMoment.nextSiblingElement();
        strcpy(stLine.zDownBeginMoment,QEDownBeginMoment.text().toStdString().c_str());
        DebugPrint(QEDownBeginMoment);
        static QDomElement QEDownEndMoment = QEDownBeginMoment.nextSiblingElement();
        strcpy(stLine.zDownEndMoment,QEDownEndMoment.text().toStdString().c_str());
        DebugPrint(QEDownEndMoment);
        static QDomElement QEStationCount = QEDownEndMoment.nextSiblingElement();
        stLine.iStationCount = QEStationCount.text().toInt();
        DebugPrint(QEStationCount);

    }
    file.close();
    return 0;
}
 void CMyXmlParse::DebugPrint(QDomElement QElement)
 {
     qDebug() <<QElement.nodeName()<<QElement.text();
 }

CMyXmlParse::~CMyXmlParse()
{

}
