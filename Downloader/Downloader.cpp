#include "Downloader.h"

#include <qclipboard.h>
#include <qscrollbar.h>
#include <qcolor.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <qmessagebox.h>
#include "AudioDlg.h"



Downloader::Downloader(QWidget *parent)
	: QMainWindow(parent),
	m_strURL(""), 
	m_Settings("downloader.ini", QSettings::IniFormat),
	m_AudioSettings(),
	m_gettingFormats(false)

{
	ui.setupUi(this);
	m_DownLoadProcess = new QProcess(this);

	connect(m_DownLoadProcess, SIGNAL(started()), this, SLOT(processStarted()));
	connect(m_DownLoadProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
	connect(m_DownLoadProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
	connect(m_DownLoadProcess, SIGNAL(finished(int)), this, SLOT(processFinished()));
	m_filePath = m_Settings.value("Folder","").toString();
	m_Options = m_Settings.value("Options", "").toString();
	m_AudioFormat = m_Settings.value("AudioOptions", "").toString();
	ui.edtFolder->setText(m_filePath);
	ui.edtOptions->setText(m_Options);
	ui.cmbFormat->addItem("None");
	ui.edtAudioSettings->setText("");
	
}

Downloader::~Downloader()
{
	m_Options = ui.edtOptions->text();
	m_Settings.setValue("Options", m_Options);
}


void Downloader::on_btnPaste_released()
{
	QClipboard *clipboard = QApplication::clipboard();
	m_strURL = clipboard->text(QClipboard::Clipboard);
	ui.lnURL->setText(m_strURL);
}

void Downloader::on_btnFormat_released()
{
	m_gettingFormats = true;
	ui.cmbFormat->clear();

	QMessageBox msgBox;
	

	if (m_strURL == "")
	{
		m_strURL = ui.lnURL->text();
	}

	QString searchTxt("format code  extension  resolution note");
	int lenghtSearchTxt(searchTxt.count()+1);
	QString strGetFormat = "youtube-dl.exe -F " + m_strURL;

	m_DownLoadProcess->start(strGetFormat);

	if (false == m_DownLoadProcess->waitForFinished())
	{
		msgBox.setText("Timeout oder Youtube-dl.exe konnte nicht gestartet werden");
		msgBox.exec();
	}
	

	
	ui.cmbFormat->clear();
	
	QStringList outStrings = m_OutputString.split("\n");
	outStrings.removeAll(QString("")); 
	
	bool getFormats(false);
	ui.cmbFormat->addItem("None");

	for (auto string : outStrings)
	{
		if(getFormats)
		{
			QStringList lstFormat = string.split(" ");
			lstFormat.removeAll(QString(""));
			
			QVariant code(lstFormat[0]);
			QString strFormat(lstFormat[0] + " " + lstFormat[1] + " " + lstFormat[2]);
			ui.cmbFormat->addItem(strFormat,code);
		}
		else if (string == "format code  extension  resolution note")
		{
			getFormats = true;
		}
	}
	
	m_gettingFormats = false;

}

void Downloader::on_cmbFormat_currentIndexChanged(int index)
{
	if (m_gettingFormats)
	{
		return;
	}

	QString format("");

	if (ui.cmbFormat->currentText() != "None")
	{
		
		format.append("-f " + ui.cmbFormat->currentData().toString());
		
	}
	else
	{
		format.append("-f ");
	}

	ui.edtFormat->setText(format);


}

void  Downloader::on_btnDownload_released()
{

	if (ui.cmbFormat->count())
	{
		ui.progressBar->setValue(0);
		
		QString selectedFormat(ui.cmbFormat->currentData().toString());
		QString options(ui.edtOptions->text());
		QString output(m_filePath + "/""%(title)s (%(autonumber)s).%(ext)s""" );
		QString getVideo("youtube-dl.exe --newline -i --ignore-config " + ui.edtFormat->text() + " " + options + " " + m_AudioSettings.GetSettingsString() + "-o " + "\"" + output + "\" " + m_strURL);
		m_DownLoadProcess->start(getVideo);
	}
}

void Downloader::processStarted()
{
	m_lineCount = 0;
	m_OutputString = "";
	ui.txtOutput->setPlainText(m_OutputString);
	ui.txtEdit->setPlainText(m_OutputString);
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
			auto decPos = string.indexOf('.');
			QString strPos("");

			if (decPos > 0 && decPos < pos)
			{
				strPos=string.left(pos - 2);
			}
			else
			{
				strPos=string.left(pos);
			}

			int progress = strPos.toInt();
			if (progress > 100)
			{
				progress = 100;
			}
			else
			{
				ui.progressBar->setValue(progress);
			}
			
		}
	}
	m_OutputString.append(newString);
	ui.txtOutput->setPlainText(m_OutputString);
	m_lineCount++;
	
	// put the slider at the bottom
	ui.txtOutput->verticalScrollBar()->setValue(ui.txtOutput->verticalScrollBar()->maximum());
	qApp->processEvents();
}

void Downloader::readyReadStandardError()
{
	QString newString(m_DownLoadProcess->readAllStandardError());
	ui.txtEdit->setTextColor(Qt::red);
	m_OutputString.append(newString);
	ui.txtEdit->setPlainText(m_OutputString);
	ui.txtEdit->verticalScrollBar()->setValue(ui.txtEdit->verticalScrollBar()->maximum());
	qApp->processEvents();
}

void Downloader::processFinished()
{

}

void Downloader::on_btnFolder_released()
{

	m_filePath = QFileDialog::getExistingDirectory(this, "Get Any File");
	ui.edtFolder->setText(m_filePath);

	m_Settings.setValue("Folder", m_filePath);

}

void Downloader::on_lnUrl_drop() 
{
	m_strURL = ui.lnURL->text();
}

void Downloader::on_btnAudio_released()
{
	QString AudioString(m_AudioFormat);
	AudioSettings audioSettings(m_AudioSettings);
	auto audioDlg = new AudioDlg(audioSettings, this);
	if (audioDlg->exec())
	{
		m_AudioSettings = audioSettings;
	}

	ui.edtAudioSettings->setText(m_AudioSettings.GetSettingsString());

}
