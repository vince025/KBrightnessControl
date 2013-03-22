#include "widget.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	if(argc == 2)
	{
		qDebug()<<"argc="<<QString::number(argc)<<",argv[1]="<<argv[1];
		QTranslator translator;
		translator.load(argv[1]);
		a.installTranslator(&translator);//use translator to change to chinese, but failed.
	}
    Widget w;
    w.show();
    
    return a.exec();
}
