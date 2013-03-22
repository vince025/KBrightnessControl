#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
	int get_brightness();//get the brightness, useless.
	void set_brightess(int value);//start a process to set the system's brightness.

protected slots:
	void ProcessFinished(int exitCode);//do something after process finished, read value or refresh the old brightness
    
private slots:
	void on_btnReset_clicked();//reset

	void on_btnQuit_clicked();//quit

	void on_hSliderBrightness_valueChanged(int value);//reflesh the line edit

	void on_btnDefault_clicked();//set the brightness to system default value

	void on_btnApply_clicked();//apply the change

private:
    Ui::Widget *ui;

	QProcess process;//process to run the aticonfig command
	int iOldBrightness;
	int iBrightness;
	int iDefaultBrightness;
	bool bIsWriteProcess;
};

#endif // WIDGET_H
