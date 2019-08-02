#include <QCoreApplication>
#include "downloadmanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DownLoadManager Manager;
    Manager.startDownload("https://dldir1.qq.com/qqfile/qq/PCTIM2.3.2/21158/TIM2.3.2.21158.exe",5);
    return a.exec();
}
