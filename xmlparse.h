#ifndef XMLPARSE
#define XMLPARSE
/*
 *  author  :chenjc
 *  date    :20170209
 *  function:解析特定xml文件
 *
*/
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QTextCodec>
#include <QFile>
#include <QDebug>
#include <QList>
#include <string.h>
#include <QMap>
#include <QList>
#include <QLabel>
#include "CRectLabel.h"
//StaionList
typedef struct _ST_STATION_LIST_
{
    char zName_CN[50];
    char zName_EN[50];
    int iId;
    int iLocationNum;
}ST_STATION_LIST;

//LineList
typedef struct _ST_LINE_LIST_
{
    char zId[4];
    int iLocationNum;
    int iCarrieroperatorCode;
    int iLineCenterId;
    int iLineId;
    char zName_CN[50];
    char zName_EN[50];
    char zShortEnName[10];
    char zUpBeginMoment[20];
    char zUpEndMoment[20];
    char zDownBeginMoment[20];
    char zDownEndMoment[20];
    int iStationCount;
    QList<ST_STATION_LIST> stStationList;
}ST_LINE_LIST;

//LineMap
typedef struct _ST_LINE_STATION_
{
    int iLineCount;
    QList<ST_LINE_LIST> stLineList;
}ST_LINE_STATION;

typedef struct _ST_XYWH_
{
  int Top_x;
  int Top_y;
  int Under_x;
  int Under_y;
}ST_XYWH;

//xml中Position的结构体
typedef struct _ST_MAP_LINE_POSITION_
{
  char text_chs[100];
  char text_en[256];
  char stationId[12];
  char id[4];
  char lineid[4];
  ST_XYWH coords_chs;
  ST_XYWH coords_en;
  CRectLabel *pLabel;
}ST_MAP_LINE_POSITION;

class CMyXmlParse:QObject
{
  Q_OBJECT
public:
  CMyXmlParse();
  ~CMyXmlParse();
public:
  int MapLineXmlParse(const char *pzFileName);
  int RefleshParamMap(int iKey,const ST_MAP_LINE_POSITION& stPosition);
  int AddNewNode(int iKey,ST_MAP_LINE_POSITION stPosition);
  int RewriteXml(const char *pzPath,const char*pNewFilename,float fMultiple);
  int LineStationParse(const char *pzPath);

public:
  void DebugPrint(QDomElement QElement);
public:
  QMap<int,ST_MAP_LINE_POSITION> m_stPostionMap;
  QMap<int,ST_MAP_LINE_POSITION> m_stPostionTempMap;
  QMap<int,QList<int> > m_LineStationListMap;
  int iNum;

  ST_LINE_STATION m_stLineStation;
};


#endif // XMLPARSE

