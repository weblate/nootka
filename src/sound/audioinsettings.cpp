/***************************************************************************
 *   Copyright (C) 2011-2013 by Tomasz Bojczuk                             *
 *   tomaszbojczuk@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/


#include "audioinsettings.h"
#include "tpitchview.h"
#include "tpitchfinder.h"
#include <QtGui>
#include "taudioparams.h"
#include "trtaudioin.h"
#include "tintonationview.h"
#include "tvolumeslider.h"
#include "ttipchart.h"
#include <tkeysignature.h>
#include <ttune.h>


AudioInSettings::AudioInSettings(TaudioParams* params, QString path, Ttune* tune, QWidget* parent) :
  QWidget(parent),
  m_audioIn(0),
  m_glParams(params),
  m_listGenerated(false),
  m_tune(tune)
{
	QString styleTxt = "background-color: palette(base); border: 1px solid palette(Text); border-radius: 5px;";
	
  m_tmpParams = new TaudioParams();
  *m_tmpParams = *m_glParams;
	
	QToolBox *m_toolBox = new QToolBox(this);
	QWidget *m_1_device, *m_2_params, *m_3_middleA, *m_4_test;
  
//################### 1. 	Input device & pitch detection #################################
	m_1_device = new QWidget();
	m_toolBox->addItem(m_1_device, tr("1. Input device and pitch detection"));  
  
  QLabel *devLab = new QLabel(tr("input device"), m_1_device);
  m_inDeviceCombo = new QComboBox(m_1_device);
		m_inDeviceCombo->setStatusTip(tr("Be sure your input device (microphone, webcam, instrument, etc.) is plugged in, properly configured, and working."));
  
  voiceRadio = new QRadioButton(m_1_device);
  QLabel *voiceLab = new QLabel(tr("for singing") + 
    " <span style=\"font-family: nootka; font-size: 25px;\">v</span>", m_1_device);
		voiceLab->setStatusTip(tr("This mode is more accurate but slower. It is recommended for singing and for instruments with \"wobbly\" intonation."));
	instrRadio = new QRadioButton(m_1_device);
  QButtonGroup *butGr = new QButtonGroup(m_1_device);
	butGr->addButton(voiceRadio);
  butGr->addButton(instrRadio);
  QLabel *instrLab = new QLabel(tr("for playing") + 
        " <span style=\"font-family: nootka; font-size: 25px;\">g</span>", m_1_device);
  instrLab->setStatusTip(tr("This mode is faster and good enough for guitars and other instruments."));
	
	durHeadLab = new QLabel(tr("minimum note duration"), m_1_device);
	durationSpin = new QSpinBox(m_1_device);
		durationSpin->setMinimum(10);
		durationSpin->setMaximum(1000);
		durationSpin->setSuffix("  "  + tr("[milliseconds]"));
		durationSpin->setSingleStep(50);
		durationSpin->setValue(m_glParams->minDuration * 1000); // minimal duration is stored in seconds but displayed in milliseconds
		durationSpin->setStatusTip(tr("Only sounds longer than the selected time will be pitch-detected.<br>Selecting a longer minimum note duration helps avoid capturing fret noise or other unexpected sounds but decreases responsiveness."));
  if (m_glParams->isVoice)
			voiceRadio->setChecked(true);
	else
			instrRadio->setChecked(true);
	// 1. Layout
	QVBoxLayout *deviceLay = new QVBoxLayout();
		deviceLay->addWidget(devLab);
		deviceLay->addWidget(m_inDeviceCombo);
		deviceLay->addStretch(1);
	QGridLayout *modeButtonsLay = new QGridLayout();
		modeButtonsLay->addWidget(voiceRadio, 0, 0, Qt::AlignRight);
		modeButtonsLay->addWidget(voiceLab, 0, 1, Qt::AlignLeft);
		modeButtonsLay->addWidget(instrRadio, 1, 0, Qt::AlignRight);
		modeButtonsLay->addWidget(instrLab, 1, 1, Qt::AlignLeft);
	QVBoxLayout *modeLay = new QVBoxLayout;
		modeLay->addLayout(modeButtonsLay);
		modeLay->addStretch(1);
		QHBoxLayout *durLay = new QHBoxLayout;
			durLay->addStretch(2);
			durLay->addWidget(durHeadLab);
			durLay->addStretch(1);
			durLay->addWidget(durationSpin);
			durLay->addStretch(2);
		modeLay->addLayout(durLay);
	modeGr = new QGroupBox(tr("pitch detection mode"), m_1_device);
		modeGr->setLayout(modeLay);
	deviceLay->addWidget(modeGr);
  deviceLay->addStretch(1);
  m_1_device->setLayout(deviceLay);
	
//################### 2. Pitch detection parameters #################################
	m_2_params = new QWidget();
	m_toolBox->addItem(m_2_params, tr("2. Pitch detection parameters"));
	
	QLabel *volLabel = new QLabel(tr("minimum volume"), m_2_params);
	volumeSlider = new TvolumeSlider(m_2_params);
		volumeSlider->setValue(m_glParams->minimalVol);
		volumeSlider->setStatusTip(tr("Minimal volume of a sound to be pitch-detected"));
	
	lowRadio = new QRadioButton(tr("low"), m_2_params);
		lowRadio->setStatusTip(tr("The lowest notes.<br>Suitable for bass guitar, double bass, etc."));
	middleRadio = new QRadioButton(tr("middle"), m_2_params);
		middleRadio->setStatusTip(tr("Notes above <b>A contra</b>.<br>Suitable for guitar, cello, human voice, etc."));
	highRadio = new QRadioButton(tr("high"), m_2_params);
		highRadio->setStatusTip(tr("Notes above <b>small g</b>.<br>Suitable for high pitched instruments such as flute, piccolo, etc."));
	QButtonGroup *rangeGr = new QButtonGroup(m_2_params);
		rangeGr->addButton(lowRadio);
		rangeGr->addButton(middleRadio);
		rangeGr->addButton(highRadio);
	if (m_glParams->range == TaudioParams::e_low)
		lowRadio->setChecked(true);
	else if (m_glParams->range == TaudioParams::e_middle)
		middleRadio->setChecked(true);
	else
		highRadio->setChecked(true);
	
	TintonationCombo *intoCombo = new TintonationCombo(m_2_params);
		m_intonationCombo = intoCombo->accuracyCombo;
		m_intonationCombo->setCurrentIndex(m_glParams->intonation);
	// 2. Layout
	QVBoxLayout *paramsLay = new QVBoxLayout;
	QHBoxLayout *volLay = new QHBoxLayout;
		volLay->addWidget(volLabel);
		volLay->addWidget(volumeSlider);
	paramsLay->addLayout(volLay);
	QHBoxLayout *rangeLay = new QHBoxLayout;
		rangeLay->addWidget(lowRadio);
		rangeLay->addWidget(middleRadio);
		rangeLay->addWidget(highRadio);
	QGroupBox *rangeBox = new QGroupBox(tr("Range of note pitches:"), m_2_params);
		rangeBox->setLayout(rangeLay);
	paramsLay->addWidget(rangeBox);
	paramsLay->addWidget(intoCombo, 1, Qt::AlignCenter);
	m_2_params->setLayout(paramsLay);
		
//################### 3. Middle a & transposition #################################
	m_3_middleA= new QWidget();
	m_toolBox->addItem(m_3_middleA, tr("3. 'Middle a' and transposition"));
  
  QLabel *headLab = new QLabel("<table><tr><td valign=\"middle\">" + tr("middle A") + QString("&nbsp;&nbsp;&nbsp;%1</td></tr></table>").arg(TtipChart::wrapPixToHtml(Tnote(6, 1, 0), Tclef::e_treble_G, TkeySignature(0), 4.0)), m_3_middleA);
  QLabel *frLab = new QLabel(tr("frequency:"), m_3_middleA);
  
  freqSpin = new QSpinBox(m_3_middleA);
		freqSpin->setStatusTip(tr("The base frequency of <i>middle a</i>.<br>Detection of the proper pitch of notes is relative to this value. This also affects the pitch of played sounds."));
		freqSpin->setMinimum(200);
		freqSpin->setMaximum(900);
		freqSpin->setValue(getFreq(440.0));
		freqSpin->setSuffix(" Hz");
  frLab->setStatusTip(freqSpin->statusTip());
  
  QLabel *intervalLab = new QLabel(tr("interval:"), m_3_middleA);  
  m_intervalCombo = new QComboBox(m_3_middleA);
		m_intervalCombo->addItem(tr("semitone up"));
		m_intervalCombo->addItem(tr("none"));
		m_intervalCombo->addItem(tr("semitone down"));
		m_intervalCombo->setStatusTip(tr("Shifts the frequency of <i>middle a</i> one semitone."));
  if (freqSpin->value() <= 415)
      m_intervalCombo->setCurrentIndex(2);
    else if (freqSpin->value() >= 465)
      m_intervalCombo->setCurrentIndex(0);
    else
      m_intervalCombo->setCurrentIndex(1);
  intervalLab->setStatusTip(m_intervalCombo->statusTip());
  
	tuneFreqlab = new QLabel(m_3_middleA);
		tuneFreqlab->setAlignment(Qt::AlignCenter);
		tuneFreqlab->setStyleSheet(styleTxt);
		QFont ff = tuneFreqlab->font();
		ff.setPixelSize(fontMetrics().boundingRect("A").height() * 1.5);
		tuneFreqlab->setFont(ff);
  
  // 3. Layout
	QGridLayout *midGrLay = new QGridLayout;
		midGrLay->addWidget(frLab, 0, 0);
		midGrLay->addWidget(freqSpin, 0, 1);
		midGrLay->addWidget(intervalLab, 1, 0);
		midGrLay->addWidget(m_intervalCombo, 1, 1);
	QHBoxLayout *aLay = new QHBoxLayout;
		aLay->addWidget(headLab, 0, Qt::AlignCenter);
		aLay->addLayout(midGrLay);
	QVBoxLayout *middleAlay = new QVBoxLayout();
		middleAlay->addLayout(aLay);
		middleAlay->addWidget(tuneFreqlab);
	m_3_middleA->setLayout(middleAlay);
	
//################### 4. Test the settings #################################
	m_4_test= new QWidget();
	m_toolBox->addItem(m_4_test, tr("4. Test the settings"));  
  
  testTxt = tr("Test");
  stopTxt = tr("Stop");
  
  testButt = new QPushButton(testTxt, m_4_test);
		testButt->setStatusTip(tr("Check, are your audio input settings appropriate?<br>And how well does pitch detection work for your selected settings?"));
  pitchView = new TpitchView(m_audioIn, m_4_test, false);
		pitchView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		pitchView->setPitchColor(Qt::darkGreen);
		pitchView->setMinimalVolume(m_glParams->minimalVol);
	
  pitchLab = new QLabel("--", this);
		pitchLab->setFixedWidth(70);
		pitchLab->setStyleSheet(styleTxt);
		pitchLab->setStatusTip(tr("Detected pitch"));
		pitchLab->setAlignment(Qt::AlignCenter);
  
  freqLab = new QLabel("--", this);
		freqLab->setFixedWidth(70);
		freqLab->setStyleSheet(styleTxt);
		freqLab->setAlignment(Qt::AlignCenter);
  getFreqStatusTip();
  
	// 4. Layout
	QHBoxLayout *testLay = new QHBoxLayout();
		testLay->addWidget(testButt);
		testLay->addWidget(pitchView);
	QVBoxLayout *freqLay = new QVBoxLayout();
		freqLay->setAlignment(Qt::AlignCenter);
		freqLay->addWidget(pitchLab);
		freqLay->addWidget(freqLab);
		testLay->addLayout(freqLay);
	m_4_test->setLayout(testLay);
  
  // Whole layout
	QVBoxLayout *inLay = new QVBoxLayout();
		inLay->addWidget(m_toolBox);
	enableInBox = new QGroupBox(tr("enable pitch detection"), this);
  enableInBox->setLayout(inLay);
	QVBoxLayout *lay = new QVBoxLayout();
  lay->addWidget(enableInBox);
  setLayout(lay);
  
  setTestDisabled(true);
	enableInBox->setCheckable(true);
  enableInBox->setChecked(m_glParams->INenabled);
  
  connect(testButt, SIGNAL(clicked()), this, SLOT(testSlot()));
  connect(m_intervalCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(intervalChanged(int)));
  connect(freqSpin, SIGNAL(valueChanged(int)), this, SLOT(baseFreqChanged(int)));
  connect(volumeSlider, SIGNAL(valueChanged(float)), this, SLOT(minimalVolChanged(float)));
	connect(voiceRadio, SIGNAL(clicked(bool)), this, SLOT(voiceOrInstrumentChanged()));
  connect(instrRadio, SIGNAL(clicked(bool)), this, SLOT(voiceOrInstrumentChanged()));
}

AudioInSettings::~AudioInSettings()
{
  pitchView->stopVolume();
  if (m_audioIn) {
    m_audioIn->stopListening();
    delete m_audioIn;
  }
  delete m_tmpParams;
}

//------------------------------------------------------------------------------------
//------------          methods       ------------------------------------------------
//------------------------------------------------------------------------------------
void AudioInSettings::setTestDisabled(bool disabled) {
  m_testDisabled = disabled;
  if (disabled) {
			pitchLab->setText("--");
			freqLab->setText("--");
			pitchLab->setDisabled(true);
			freqLab->setDisabled(true);
			pitchView->setDisabled(true);
	// enable the rest of widget
			m_inDeviceCombo->setDisabled(false);
			modeGr->setDisabled(false);
// 			midABox->setDisabled(false);
			volumeSlider->setDisabled(false);
			lowRadio->setDisabled(false);
			middleRadio->setDisabled(false);
			highRadio->setDisabled(false);
			m_intonationCombo->setDisabled(false);
			voiceOrInstrumentChanged();
  } else {
			pitchLab->setDisabled(false);
			freqLab->setDisabled(false);
			pitchView->setDisabled(false);
	// disable the rest of widget
			m_inDeviceCombo->setDisabled(true);
// 			midABox->setDisabled(true);
			modeGr->setDisabled(true);
			volumeSlider->setDisabled(true);
			lowRadio->setDisabled(true);
			middleRadio->setDisabled(true);
			highRadio->setDisabled(true);
			durationSpin->setDisabled(true);
			m_intonationCombo->setDisabled(true);
  }
}


void AudioInSettings::grabParams(TaudioParams *params) {
  if (freqSpin->value() == 440 )
      params->a440diff = 0.0;
  else
      params->a440diff = getDiff(freqSpin->value());
  params->INdevName = m_inDeviceCombo->currentText();
  if (voiceRadio->isChecked())
      params->isVoice = true;
  else
      params->isVoice = false;
  params->INenabled = enableInBox->isChecked();
  params->minimalVol = volumeSlider->value();
	if (lowRadio->isChecked())
			params->range = TaudioParams::e_low;
	else if (middleRadio->isChecked())
			params->range = TaudioParams::e_middle;
	else
			params->range = TaudioParams::e_high;
	params->minDuration = (float)durationSpin->value() / 1000.0f;
	params->intonation = m_intonationCombo->currentIndex();
}


void AudioInSettings::restoreDefaults() {
	if (!m_testDisabled)
		testSlot();
	enableInBox->setChecked(true);
	freqSpin->setValue(440);
	m_intervalCombo->setCurrentIndex(1); // none
	m_inDeviceCombo->setCurrentIndex(0);
	instrRadio->setChecked(true);
	volumeSlider->setValue(0.4); // It is multipled by 100
	middleRadio->setChecked(true);
	durationSpin->setValue(90);
	m_intonationCombo->setCurrentIndex(3); // normal
}



void AudioInSettings::saveSettings() {
  if (m_listGenerated)
      grabParams(m_glParams);
}


void AudioInSettings::generateDevicesList() {
  if (m_listGenerated)
    return;
  setDevicesCombo();
  m_listGenerated = true;
}


void AudioInSettings::setDevicesCombo() {
  m_inDeviceCombo->clear();
  m_inDeviceCombo->addItems(TaudioIN::getAudioDevicesList());
  if (m_inDeviceCombo->count()) {
        int id = m_inDeviceCombo->findText(m_glParams->INdevName);
        if (id != -1)
            m_inDeviceCombo->setCurrentIndex(id);
				m_inDeviceCombo->setDisabled(false);
    } else {
        m_inDeviceCombo->addItem(tr("no devices found"));
        m_inDeviceCombo->setDisabled(true);
  }
}


float AudioInSettings::offPitch(float pitch) {
  return pitch2freq(pitch - m_tmpParams->a440diff);
}


void AudioInSettings::getFreqStatusTip() {
		QString freqTxt = "";
		for (int i = 1; i <= m_tune->stringNr(); i++) {
				freqTxt += QString("<span style=\"font-family: nootka;\">%1</span>%2 = %3 Hz, ").arg(i).
// 				arg(TnoteName::noteToRichText(m_tune->str(i))).
				arg((m_tune->str(i)).toRichText()).
				arg(offPitch((float)m_tune->str(i).getChromaticNrOfNote() + 47), 0, 'f', 1);
				if (i % 2 == 0 && i < 6)
						freqTxt += "<br>"; // two entries per line 
				else if (i % 3 == 0 && i < 6)
						freqTxt += "ALT_BR"; // three entries per line
		}
    QString freq2 = freqTxt;
    freqLab->setStatusTip(tr("Frequency of detected note. You can use this for tuning.") + "<br>" +
          freq2.replace("<br>", "").replace("ALT_BR", "<br>")); // three entries per line in status tip
    tuneFreqlab->setText(freqTxt.replace("ALT_BR", "")); // two entries per line on the label
}


int AudioInSettings::getFreq(double freq) {
    return qRound((pitch2freq(freq2pitch(freq) + m_tmpParams->a440diff)));
}


float AudioInSettings::getDiff(int freq) {
   return float(freq2pitch((double)freq) - freq2pitch(440.0)); // in semitones
}


//------------------------------------------------------------------------------------
//------------          slots       --------------------------------------------------
//------------------------------------------------------------------------------------

void AudioInSettings::tuneWasChanged(Ttune* tune) {
		m_tune = tune;
		getFreqStatusTip();
}



void AudioInSettings::minimalVolChanged(float vol) {
  pitchView->setMinimalVolume(vol);
}


void AudioInSettings::testSlot() {
  setTestDisabled(!m_testDisabled);
  if (!m_testDisabled) { // start a test
    grabParams(m_tmpParams);
    if (!m_audioIn)
      m_audioIn = new TaudioIN(m_tmpParams, this);
    else 
      m_audioIn->setParameters(m_tmpParams);
    testButt->setText(stopTxt);
    pitchView->setAudioInput(m_audioIn);
    m_audioIn->startListening();
    pitchView->startVolume();
		pitchView->setIntonationAccuracy(m_tmpParams->intonation);
    connect(m_audioIn, SIGNAL(noteDetected(Tnote)), this, SLOT(noteSlot(Tnote)));
    connect(m_audioIn, SIGNAL(fundamentalFreq(float)), this, SLOT(freqSlot(float)));
  } 
  else { // stop a test
    pitchView->stopVolume();
    m_audioIn->stopListening();
    testButt->setText(testTxt);
    setTestDisabled(true);
  }
}


void AudioInSettings::noteSlot(Tnote note) {
		pitchLab->setText("<b>" + note.toRichText() + "</b>");
//   pitchLab->setText("<b>" + TnoteName::noteToRichText(note) + "</b>");
}


void AudioInSettings::freqSlot(float freq) {
	freqLab->setText(QString("%1 Hz").arg(freq, 0, 'f', 1, '0'));
}


void AudioInSettings::intervalChanged(int index) {
  if (m_intervalCombo->hasFocus()) {
		switch (index) {
      case 0 : freqSpin->setValue(pitch2freq(70)); break;
      case 1 : freqSpin->setValue(pitch2freq(69)); break;
      case 2 : freqSpin->setValue(pitch2freq(68)); break;
		}
    m_tmpParams->a440diff = getDiff(freqSpin->value());
		getFreqStatusTip();
  }
}


void AudioInSettings::baseFreqChanged(int bFreq) {
  if (freqSpin->hasFocus()) {
    if (freqSpin->value() <= 415)
      m_intervalCombo->setCurrentIndex(2);
    else if (freqSpin->value() >= 465)
      m_intervalCombo->setCurrentIndex(0);
    else
      m_intervalCombo->setCurrentIndex(1);
    m_tmpParams->a440diff = getDiff(freqSpin->value());
    getFreqStatusTip();
  }
}

/** This is not so pretty (piano staff invokes low range) */
void AudioInSettings::whenLowestNoteChanges(Tnote loNote) {
	char noteNr = loNote.getChromaticNrOfNote();
	if (noteNr > Tnote(6, 0, 0).getChromaticNrOfNote())
		highRadio->setChecked(true);
	else if (noteNr > Tnote(5, -2, 0).getChromaticNrOfNote())
		middleRadio->setChecked(true);
	else
		lowRadio->setChecked(true);
}


void AudioInSettings::voiceOrInstrumentChanged() {
	if (instrRadio->isChecked())
			durationSpin->setDisabled(false);
	else
			durationSpin->setDisabled(true);
}





