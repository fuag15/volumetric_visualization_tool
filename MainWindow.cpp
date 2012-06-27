#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(QApplication * appIn) : QMainWindow()
{
	app = appIn;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	app -> quit();
}