#pragma once

#include <QDialog>
#include "ui_AudioOptions.h"

enum class AudioFormat {Best, Mp3};

class FormatSelect
{
public:
	FormatSelect(QRadioButton* ptrButton, QString strFormat, QString strOption);
	QRadioButton * GetButton() { return m_pButton; }
	QString  GetOption() { return m_strOption; }
	QString  GetString() { return m_strFormat; }

private:
	QRadioButton * m_pButton;
	QString	m_strFormat;
	QString m_strOption;
};

class AudioSettings
{

public:
	AudioSettings();
	AudioSettings(AudioSettings &lhs);

	QString GetSettingsString();

	QString Format;
	int     Quality;
	bool    AudioOnly;
	bool    AudioExtract;
};

class AudioDlg : public QDialog, public Ui::AudioOptionsDlg {
	Q_OBJECT

public:
	AudioDlg(AudioSettings& audioSettings, QWidget * parent = 0);

private:
	Ui::AudioOptionsDlg ui;
	std::vector<FormatSelect*> vecAudioFormat;
	AudioSettings& m_AudioSettings;

private slots:
	void on_chkAudioExtract_clicked();
	void on_chkAudioOnly_clicked();
	void on_accept();
	void on_sldQuality_valueChanged();
};