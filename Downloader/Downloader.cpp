#include "Downloader.h"

#include <qclipboard.h>
#include <qscrollbar.h>



Downloader::Downloader(QWidget *parent)
	: QMainWindow(parent),
	m_strURL("")
{
	ui.setupUi(this);
	m_DownLoadProcess = new QProcess(this);

	connect(m_DownLoadProcess, SIGNAL(started()), this, SLOT(processStarted()));
	connect(m_DownLoadProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
	connect(m_DownLoadProcess, SIGNAL(finished(int)), this, SLOT(processFinished()));
}


void Downloader::on_btnPaste_released()
{
	QClipboard *clipboard = QApplication::clipboard();
	m_strURL = clipboard->text(QClipboard::Clipboard);
	ui.lnURL->setText(m_strURL);
}

void Downloader::on_btnFormat_released()
{
	ui.cmbFormat->clear();
	
	QString searchTxt("format code  extension  resolution note");
	int lenghtSearchTxt(searchTxt.count()+1);
	QString strGetFormat = "youtube-dl.exe -F " + m_strURL;

	// Start the youtube-dl process to get the availabel formats
	m_DownLoadProcess->start(strGetFormat);
	m_DownLoadProcess->waitForFinished();
	
	
	QStringList outStrings = m_OutputString.split("\n");
	outStrings.removeAll(QString("")); 
	
	bool getFormats(false);
	
	for (auto string : outStrings)
	{
		if(getFormats)
		{
			QStringList lstFormat = string.split(" ");
			lstFormat.removeAll(QString(""));
			
			QVariant code(lstFormat[0]);
			QString strFormat(lstFormat[1] + " " + lstFormat[2]);
			ui.cmbFormat->addItem(strFormat,code);
		}
		else if (string == "format code  extension  resolution note")
		{
			getFormats = true;
		}
	}
	

}


void  Downloader::on_btnDownload_released()
{
	if (ui.cmbFormat->count())
	{
		ui.progressBar->setValue(0);
		QString format(ui.cmbFormat->currentData().toString());
		QString getVideo("youtube-dl.exe -f " + format + "  -o ""%(title)s.%(ext)s""  " + m_strURL);
		m_DownLoadProcess->start(getVideo);
	}
}

void Downloader::processStarted()
{
	m_lineCount = 0;
	m_OutputString = "";
	ui.txtOutput->setPlainText(m_OutputString);
	qApp->processEvents();
}

void Downloader::readyReadStandardOutput()
{

	QString newString(m_DownLoadProcess->readAllStandardOutput());
	QStringList lstString(newString.split(" "));
	lstString.removeAll(QString(""));
	for (auto string : lstString)
	{

		int pos = string.indexOf('%');

		if (pos > 0)
		{
			QString strPos(string.left(pos-2));
			int progress = strPos.toInt();
			ui.progressBar->setValue(progress);
		}
	}
	m_OutputString.append(newString);
	ui.txtOutput->setPlainText(m_OutputString);
	m_lineCount++;
	
	// put the slider at the bottom
	ui.txtOutput->verticalScrollBar()->setSliderPosition(ui.txtOutput->verticalScrollBar()->maximum());
	qApp->processEvents();
}

void Downloader::processFinished()
{

}
