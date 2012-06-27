#include "lightControlWindow.h"


LightControlWindow::LightControlWindow(QWidget * parent):QWidget(parent)
{
	QLabel* LightOne = new QLabel("Light One Controls");
	QLabel* LightTwo = new QLabel("Light Two Controls");
	QLabel* diffuse = new QLabel("Diffuse");
	QLabel* specular = new QLabel("Specular");
	QLabel* ambient = new QLabel("Ambient");
	QLabel* x = new QLabel("x");
	QLabel* y = new QLabel("y");
	QLabel* z = new QLabel("z");
	
	QLabel* diffuse2 = new QLabel("Diffuse");
	QLabel* specular2 = new QLabel("Specular");
	QLabel* ambient2 = new QLabel("Ambient");
	QLabel* x2 = new QLabel("x");
	QLabel* y2 = new QLabel("y");
	QLabel* z2 = new QLabel("z");
	
	/*aSlider = new QSlider();
	aSlider -> setMaximum(256);
	aSlider -> setMinimum(1);
	aSlider -> setOrientation(Qt::Horizontal);
	
	dSlider = new QSlider();
	dSlider -> setMaximum(256);
	dSlider -> setMinimum(1);
	dSlider -> setOrientation(Qt::Horizontal);
	
	sSlider = new QSlider();
	sSlider -> setMaximum(256);
	sSlider -> setMinimum(1);
	sSlider -> setOrientation(Qt::Horizontal);
	
	xSlider = new QSlider();
	xSlider -> setMaximum(256);
	xSlider -> setMinimum(1);
	xSlider -> setOrientation(Qt::Horizontal);
	
	ySlider = new QSlider();
	ySlider -> setMaximum(256);
	ySlider -> setMinimum(1);
	ySlider -> setOrientation(Qt::Horizontal);
	
	zSlider = new QSlider();
	zSlider -> setMaximum(256);
	zSlider -> setMinimum(1);
	zSlider -> setOrientation(Qt::Horizontal);
	qDebug() << "got here" << endl;
	aSlider2 = new QSlider();
	aSlider2 -> setMaximum(256);
	aSlider2 -> setMinimum(1);
	aSlider2 -> setOrientation(Qt::Horizontal);
	
	dSlider2 = new QSlider();
	dSlider2 -> setMaximum(256);
	dSlider2 -> setMinimum(1);
	dSlider2 -> setOrientation(Qt::Horizontal);
	
	sSlider2 = new QSlider();
	sSlider2 -> setMaximum(256);
	sSlider2 -> setMinimum(1);
	sSlider2 -> setOrientation(Qt::Horizontal);
	
	xSlider2 = new QSlider();
	xSlider2 -> setMaximum(256);
	xSlider2 -> setMinimum(1);
	xSlider2 -> setOrientation(Qt::Horizontal);
	
	ySlider2 = new QSlider();
	ySlider2 -> setMaximum(256);
	ySlider2 -> setMinimum(1);
	ySlider2 -> setOrientation(Qt::Horizontal);
	
	zSlider2 = new QSlider();
	zSlider2 -> setMaximum(256);
	zSlider2 -> setMinimum(1);
	zSlider2 -> setOrientation(Qt::Horizontal);
	
	
	
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(LightOne);
	
	layout->addWidget(ambient);
	layout->addWidget(aSlider);
	
	layout->addWidget(diffuse);
	layout->addWidget(dSlider);
	
	layout->addWidget(specular);
	layout->addWidget(sSlider);
	
	layout->addWidget(x);
	layout->addWidget(xSlider);
	
	layout->addWidget(y);
	layout->addWidget(ySlider);
	
	layout->addWidget(z);
	layout->addWidget(zSlider);
	
	layout->addWidget(LightTwo);
	layout->addWidget(ambient2);
	layout->addWidget(aSlider2);
	
	layout->addWidget(diffuse2);
	layout->addWidget(dSlider2);
	
	layout->addWidget(specular2);
	layout->addWidget(sSlider2);
	
	layout->addWidget(x2);
	layout->addWidget(xSlider2);
	
	layout->addWidget(y2);
	layout->addWidget(ySlider2);
	
	layout->addWidget(z2);
	layout->addWidget(zSlider2);
	
	setLayout(layout);*/
	
	
	resize(600, 800);
	
	return;
}

