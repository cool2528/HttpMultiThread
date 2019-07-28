#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QMutex>
#include <QFile>
class DownLoadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownLoadManager(QObject *parent = nullptr);

signals:

public slots :
	void onDownloadProgress(const qint64 doneSize,const qint64 bytesTotal);
    void onDownloadFinish();
public:
    bool startDownload(const QString& szUrl,size_t nNumber);
private:
    bool getFileInfo(const QString &szUrl);
private:
    qint64 mFileTotalSize; //文件总大小
    QString mFilePath;  //  文件的保存路径
    QString mFileName;  //以及文件名字
    QMutex mMutex;  //多线程互斥
	QFile* mFile;
	qint64 mDownloadSize;
	size_t mThreadNumber;
	size_t mFinishThread;
};

#endif // DOWNLOADMANAGER_H
