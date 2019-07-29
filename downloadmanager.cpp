#include "downloadmanager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QEventLoop>
#include <QUrl>
#include <QFileInfo>
#include <QDebug>
#include "workthread.h"
DownLoadManager::DownLoadManager(QObject *parent) : QObject(parent)
,mFilePath("d:/pdf/")
, mDownloadSize(0)
,mFileTotalSize(0)
, mFinishThread(0)
{

}

void DownLoadManager::onDownloadProgress(const qint64 doneSize, const qint64 bytesTotal)
{
	if (doneSize >= bytesTotal)
	{
		mDownloadSize += doneSize;
		if (mDownloadSize >= mFileTotalSize)
		{
			qDebug() << "download finish" << endl;
		}
		mDownloadSize = mDownloadSize > mFileTotalSize ? mFileTotalSize : mDownloadSize;
    }else{
		mDownloadSize = mDownloadSize > mFileTotalSize ? mFileTotalSize : mDownloadSize;
		float progress = 0;
		if (mDownloadSize >= mFileTotalSize) {
			progress = ((double)(mDownloadSize) / (double)mFileTotalSize) * 100;
		}
		else
		{
			progress = ((double)(doneSize + mDownloadSize) / (double)mFileTotalSize) * 100;
		}
        qDebug() << QString("当前进度 %1").arg(progress) << endl;
    }
}

void DownLoadManager::onDownloadFinish()
{
	mFinishThread++;
	if (mFinishThread >= mThreadNumber)
	{
		mFile->close();
	}
}

bool DownLoadManager::startDownload(const QString &szUrl, size_t nNumber)
{
    bool bResult = false;
    if(!getFileInfo(szUrl)){
        return bResult;
    }
    mFile = new QFile(this);
	mFile->setFileName(mFilePath + mFileName);
	mFile->open(QIODevice::WriteOnly);
	mThreadNumber = nNumber;
	mDownloadSize = 0;
	mFinishThread = 0;
    for(size_t i =0;i<nNumber;i++){
        qint64 nstart = mFileTotalSize * i / nNumber;
        qint64 nEnd = mFileTotalSize * (i+1) / nNumber;
        auto work = new WorkThread;
        work->init(szUrl,nstart,nEnd, mFile,&mMutex);
        connect(work,&WorkThread::finished,work,&WorkThread::deleteLater);
		connect(work, &WorkThread::downloadFinish, this, &DownLoadManager::onDownloadFinish);
		connect(work, &WorkThread::downloadProgress, this, &DownLoadManager::onDownloadProgress);
        work->start();
    }
    bResult = true;
    return bResult;
}

bool DownLoadManager::getFileInfo(const QString &szUrl)
{
    bool bResult = false;
    if(szUrl.isEmpty())
        return bResult;
    QUrl url (szUrl);
    if(!url.isValid()){
        return bResult;
    }
    QFileInfo fileinfo(url.path());
    mFileName = fileinfo.fileName();
    if(mFileName.isEmpty()){
        mFileName = "noFileName.html";
    }
    QNetworkRequest Request(url);
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	Request.setSslConfiguration(config);
    auto Manager = new QNetworkAccessManager(this);
    auto reply = Manager->head(Request);
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
	QNetworkReply::NetworkError error = reply->error();
    if(QNetworkReply::NoError == error){
        mFileTotalSize = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
        bResult = true;
    }
    reply->deleteLater();
    return bResult;
}
