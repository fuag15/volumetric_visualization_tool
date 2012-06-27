#ifndef __MainWindow_h__
#define __MainWindow_h__
#include <QApplication>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
		MainWindow(QApplication * appIn);
		void closeEvent(QCloseEvent * event);
		QApplication* app;
};

#endif