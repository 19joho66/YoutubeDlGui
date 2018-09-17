#pragma once

#include <QtWidgets/QMainWindow>
#include <QProcess>

#include "ui_Downloader.h"

class Downloader : public QMainWindow
{
	Q_OBJECT

public:
	Downloader(QWidget *parent = Q_NULLPTR);

private:
	Ui::DownloaderClass ui;
	QString m_strURL;
	QProcess* m_DownLoadProcess;
	QString m_OutputString;
	int m_lineCount;

private slots:
	void on_btnPaste_released();
	void on_btnFormat_released();
	void on_btnDownload_released();
	void processStarted();
	void readyReadStandardOutput();
	void processFinished();
};
