#include <QCoreApplication>
#include "downloadmanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DownLoadManager Manager;
    Manager.startDownload("https://download.virtualbox.org/virtualbox/6.0.10/VirtualBox-6.0.10-132072-Win.exe",5);
    return a.exec();
}
