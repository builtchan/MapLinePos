#include "maplinepos.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  CMapLinePos w;
  w.show();

  return a.exec();
}
