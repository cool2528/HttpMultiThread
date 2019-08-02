#include "workthread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QUrl>
#include <QEventLoop>
#include <QMutexLocker>
#include <QDebug>
WorkThread::WorkThread(QThread *parent):QThread (parent)
, mHavedonesize(0)
{

}

void WorkThread::init(const QString &szUrl, const qint64 startPoint, const qint64 endPoint, QFile *FileManger, QMutex* mutex)
{
    if(szUrl.isEmpty() || !FileManger){
        return;
    }
    if(!endPoint){
        mEndPoint = -1;
    }else{
            mEndPoint = endPoint;
    }
    mUrl = szUrl;
    mStartPoint = startPoint;
    mFileManager = FileManger;
	mMutex = mutex;
}

void WorkThread::run()
{
	qDebug() << "WorkThread::run()" << QThread::currentThreadId() << endl;
    //在这个线程函数中执行我们的下载任务
    auto DownloadManagerPtr = new QNetworkAccessManager();
    QUrl url(mUrl);
    QNetworkRequest Request(url);
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	Request.setSslConfiguration(config);
    QString strRange =QString("bytes=%1-%2").arg(mStartPoint).arg(mEndPoint);
	Request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36");
    Request.setRawHeader("Range",strRange.toLatin1());
    auto reply = DownloadManagerPtr->get(Request);
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
	connect(reply,&QNetworkReply::downloadProgress, this, [this](qint64 bytesReceived, qint64 bytesTotal) {
        emit downloadProgress(bytesReceived, bytesTotal);
	});
	connect(reply, &QNetworkReply::readyRead, this, [&]() {
		mMutex->lock();
		if (mFileManager->seek(mStartPoint + mHavedonesize)) {
			QByteArray data = reply->readAll();
			qint64&& size = mFileManager->write(data);
			if (size != -1)
			{
				mHavedonesize += size;
			}
		}
		mMutex->unlock();
	}, Qt::DirectConnection);
    loop.exec();
	emit downloadFinish();
    reply->deleteLater();
	delete DownloadManagerPtr;
    DownloadManagerPtr = nullptr;
	qDebug() << "WorkThread::run()  end " << QThread::currentThreadId() << endl;
}



















