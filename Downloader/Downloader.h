#pragma once

#include <QtWidgets/QMainWindow>
#include <QProcess>

#include "ui_Downloader.h"
#include <qsettings.h>
#include "AudioDlg.h"

class QSettings; 

class Downloader : public QMainWindow
{
	Q_OBJECT

public:
	Downloader(QWidget *parent = Q_NULLPTR);
	~Downloader();

private:
	Ui::DownloaderClass ui;
	QString m_strURL;
	QProcess* m_DownLoadProcess;
	QString m_OutputString;
	QString m_filePath;
	QString m_Options;
	AudioSettings m_AudioSettings;
	QString m_AudioFormat;
	QSettings m_Settings;
	int m_lineCount;

private slots:
	void on_btnPaste_released();
	void on_btnFormat_released();
	void on_btnDownload_released();
	void on_btnFolder_released();
	void on_btnAudio_released();
	void on_lnUrl_drop();
	void processStarted();
	void readyReadStandardOutput();
	void readyReadStandardError();
	void processFinished();
};
