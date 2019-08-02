#include <QCoreApplication>
#include "downloadmanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DownLoadManager Manager;
    Manager.startDownload("http://download.microsoft.com/download/B/8/9/B898E46E-CBAE-4045-A8E2-2D33DD36F3C4/vs2015.pro_chs.iso",5);
    return a.exec();
}
