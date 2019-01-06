#include "AudioDlg.h"


FormatSelect::FormatSelect(QRadioButton* ptrButton, QString strFormat, QString strOption) :
	m_strFormat(strFormat),
	m_strOption(strOption),
	m_pButton(ptrButton)
{

	m_pButton->setText(m_strFormat);
}

AudioSettings::AudioSettings() :
	Format("Best"),
	Quality(5),
	AudioExtract(false),
	AudioOnly(false)
{}

AudioSettings::AudioSettings(AudioSettings &lhs) :
	Format(lhs.Format),
	Quality(lhs.Quality),
	AudioExtract(lhs.AudioExtract),
	AudioOnly(lhs.AudioOnly)
{

}

QString AudioSettings::GetSettingsString()
{
	QString strSettings("");
	if (AudioExtract)
	{
		strSettings += "-x ";
	}
	else
	{
		return strSettings;
	}

	if (AudioOnly)
	{
		strSettings += "-k ";
	}

	strSettings += "--audio-format " + Format +" ";
	strSettings += "--audio-quality " + QString::number(Quality)+" ";

	return strSettings;
}



AudioDlg::AudioDlg(AudioSettings& audioSettings, QWidget * parent) :
	QDialog(parent),
	m_AudioSettings(audioSettings)
{
	setupUi(this);
	
	connect(this, SIGNAL(accepted()), this, SLOT(on_accept()));

	vecAudioFormat.push_back(new FormatSelect(radioButton, "Best", "Best"));
	vecAudioFormat.push_back(new FormatSelect(radioButton_2, "MP3", "mp3"));
	vecAudioFormat.push_back(new FormatSelect(radioButton_3, "AAC", "aac"));
	vecAudioFormat.push_back(new FormatSelect(radioButton_4, "FLAC", "flac"));
	vecAudioFormat.push_back(new FormatSelect(radioButton_5, "M4A", "m4a"));
	vecAudioFormat.push_back(new FormatSelect(radioButton_6, "VORBIS", "vorbis"));
	vecAudioFormat.push_back(new FormatSelect(radioButton_7, "OPUS", "opus"));
	vecAudioFormat.push_back(new FormatSelect(radioButton_8, "WAV", "wav"));

	for (auto FormatSelect : vecAudioFormat)
	{
		if (FormatSelect->GetOption() == audioSettings.Format)
		{
			FormatSelect->GetButton()->setChecked(true);
		}

		FormatSelect->GetButton()->setText(FormatSelect->GetString());
	}

	

	chkAudioExtract->setChecked(audioSettings.AudioExtract);
	chkAudioOnly->setChecked(audioSettings.AudioOnly);
	sldQuality->setSliderPosition(audioSettings.Quality);
	
}

void AudioDlg::on_chkAudioExtract_clicked()
{
	m_AudioSettings.AudioExtract=chkAudioExtract->isChecked();
}

void AudioDlg::on_chkAudioOnly_clicked()
{
	m_AudioSettings.AudioOnly = chkAudioOnly->isChecked();
}

void AudioDlg::on_sldQuality_valueChanged()
{
	m_AudioSettings.Quality = sldQuality->sliderPosition();
}

void AudioDlg::on_accept()
{
	for (auto FormatSelect : vecAudioFormat)
	{
		if (FormatSelect->GetButton()->isChecked())
		{
			m_AudioSettings.Format = FormatSelect->GetOption();
			break;
		}
	}
}