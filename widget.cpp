#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
	ui->hSliderBrightness->setRange(-100, 100);
	ui->hSliderBrightness->setPageStep(10);
	ui->hSliderBrightness->setSingleStep(1);
	ui->hSliderBrightness->setValue(0);
	ui->lineEditBrightness->setReadOnly(true);
    ui->lineEditBrightness->setText(tr("0"));

	iOldBrightness = 0;
	connect(&process, SIGNAL(finished(int)), this, SLOT(ProcessFinished(int)));
	//aticonfig --query-dispattrib=lvds,brightness
	QStringList args;
	args<<"--query-dispattrib=lvds,brightness";
	bIsWriteProcess = false;
	process.start("aticonfig", args, QIODevice::ReadOnly);
	//get the brightness from system.
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_hSliderBrightness_valueChanged(int value)
{
    iBrightness = value;
    ui->lineEditBrightness->setText(QString::number(iBrightness));
}

int Widget::get_brightness()
{
	return iOldBrightness;
}

void Widget::set_brightess(int value)
{
	if(value < ui->hSliderBrightness->minimum()) value = ui->hSliderBrightness->minimum();
	if(value > ui->hSliderBrightness->maximum()) value = ui->hSliderBrightness->maximum();
	if(process.state()) return ;
	QStringList args;
	args << "--set-dispattrib=lvds,brightness:" + QString::number(value);
	bIsWriteProcess = true;
	process.start("aticonfig", args, QIODevice::ReadOnly);
	//run command: aticonfig --set-dispattrib=lvds,brightness:value
	//set brightness
}

void Widget::ProcessFinished(int exitCode)
{
	if(exitCode == 0)//process is normal finished
	{
		if(bIsWriteProcess)//if is writing the brightness value to system
		{
			iOldBrightness = iBrightness;//reflesh old brightness, reset need it.
			return ;
		}
		//if is getting system brightness value.
		QByteArray output;
		QString strOutput;
		QStringList listOutput;
		output = process.readAllStandardOutput();
		strOutput.clear();
		strOutput.append(output);
		listOutput = strOutput.split(' ');
		/**
		 * the listOutput value is :
			"0" = ""
			"1" = "brightness"
			"2" = "attribute"
			"3" = "information"
			"4" = "of"
			"5" = "monitor"
			"6" = "lvds"
			"7" = ":
			"
			"8" = "default:0,"
			"9" = "value:-50,"
			"10" = "min:-100,"
			"11" = "max:100,"
			"12" = "step:1
			"
		 */
		//for(int i = 0; i < listOutput.size(); i++) qDebug()<<QString::number(i)<<"="<<listOutput.at(i);
		iDefaultBrightness = listOutput.at(8).split(':').at(1).toInt();//get default value
		iBrightness = iOldBrightness = listOutput.at(9).split(':').at(1).toInt();//get value
		ui->hSliderBrightness->setValue(iBrightness);//update to UI.
	}
}

void Widget::on_btnDefault_clicked()
{
	iBrightness = iDefaultBrightness;
	set_brightess(iDefaultBrightness);
	ui->hSliderBrightness->setValue(iBrightness);
}

void Widget::on_btnReset_clicked()
{
	iBrightness = iOldBrightness;
	ui->hSliderBrightness->setValue(iBrightness);
}

void Widget::on_btnApply_clicked()
{
	iBrightness = ui->hSliderBrightness->value();
	set_brightess(iBrightness);
}

void Widget::on_btnQuit_clicked()
{
	while(process.state())//waiting for process finishing
	{
		sleep(1);
		qApp->processEvents();
	}
	qApp->quit();
}
