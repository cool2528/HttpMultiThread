#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QFile>
#include <QMutex>
class WorkThread : public QThread
{
	Q_OBJECT
public:
    explicit WorkThread(QThread* parent = nullptr);
public:
    void init(const QString& szUrl,const qint64 startPoint,const qint64 endPoint,QFile* FileManger,QMutex* mutex);
signals:
	void downloadProgress(const qint64 donesize, qint64 bytesTotal);
	void downloadFinish();
protected:
    void run();
private:
    qint64 mStartPoint;//下载的起点
    qint64 mEndPoint;   //下载结束的位置
    QFile* mFileManager; // 将下载的文件写入到文件
    QString mUrl;
    QMutex* mMutex;
	qint64 mHaveDoneSize;	//下载的文件大小
};

#endif // WORKTHREAD_H
