#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QDockWidget>
#include <QSlider>
#include <QObject>
#include "ControlWindow.h"
#include "Viewer.h"
#include "ColorMap.h"
#include "HistogramWidget.h"
#include "IsoControlWindow.h"
#include "lightControlWindow.h"
#include "SlicesControllerWidget.h"
#include "vectorWindow.h"
#include "MainWindow.h"

int main(int argc, char **argv)
{
	float alphaArray[1000];
	for(int i = 0; i < 1000; i++)
	{
		alphaArray[i] = 1;
	}
	
	QApplication app(argc, argv);
	
	ColorMap* cmap = new ColorMap;
	ColorMap* smap = new ColorMap;
	ColorMap* imap = new ColorMap;
	
	MainWindow window(&app);
	Viewer *viewer = new Viewer(&window);
	viewer ->setApp(&app);
	viewer-> setSlicesMap(smap);
	ColorMap* gmap = new ColorMap;
	ColorMap* lmap = new ColorMap;
	viewer-> setGlyphSlicesMap(gmap);
	viewer-> setLicSlicesMap(lmap);
	viewer-> setColorMap(cmap);
	viewer-> setAlphaMap(alphaArray);
	viewer-> setIsoMap(imap);
	window.setWindowTitle("Volumetric Viewer");
	window.setCentralWidget(viewer);
	
	
	QDockWidget* sideDock = new QDockWidget("Slices");
	SlicesControllerWidget* sliceCon = new SlicesControllerWidget(sideDock);
	QSlider* isoDetail = sliceCon -> getDetailSlider();
	QSlider* alphaDetail = sliceCon -> getAlphaSlider();
	sliceCon->setSliceMap(smap);
	QObject::connect(isoDetail, SIGNAL(valueChanged(int)), viewer, SLOT(setIsoDetail(int)));
	QObject::connect(alphaDetail, SIGNAL(valueChanged(int)), viewer, SLOT(setAlphaDetail(int)));
	sideDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	sideDock->setWidget(sliceCon);
	
	window.addDockWidget(Qt::RightDockWidgetArea, sideDock);
	
	QMenuBar *menuBar = new QMenuBar(&window);
	menuBar->addAction("Quit", &app, SLOT(quit()));
	menuBar->addAction("Open File", viewer, SLOT(readFile()));
	
	ControlWindow cwindow(0);
	cwindow.setWindowTitle("Color Mapping Controls");
	cwindow.setCmap(cmap);
	cwindow.setSmap(smap);
	cwindow.setAlphaArray(alphaArray);
	
	vectorWindow vwindow(0);
	vwindow.setWindowTitle("Vector Visualization Controls");
	vwindow.setGlyphSliceMap(gmap);
	vwindow.setLicSliceMap(lmap);
	
	IsoControlWindow iwindow(0);
	iwindow.setWindowTitle("Iso Surface Controls");
	iwindow.setCmap(cmap);
	iwindow.setImap(imap);
	cwindow.setSmap(smap);
	cwindow.setAlphaArray(alphaArray);
	
// 	LightControlWindow lwindow(0);
// 	lwindow.setWindowTitle("Light Controls");
	
	menuBar->addAction("Color Map", &cwindow, SLOT(show()));
	menuBar->addAction("Iso Surface", &iwindow, SLOT(show()));
	//menuBar->addAction("Light Controls", &lwindow, SLOT(show()));
	menuBar->addAction("Vector Visualization", &vwindow, SLOT(show()));
	
	window.setMenuBar(menuBar);
	QObject::connect(viewer, SIGNAL(newFileLoaded(float*, int, int, int, float)), &cwindow, SLOT(fileOpened(float*, int, int, int, float)));
	QObject::connect(viewer, SIGNAL(newFileLoaded(float*, int, int, int, float)), &iwindow, SLOT(fileOpened(float*, int, int, int, float)));
	QObject::connect(&cwindow, SIGNAL(maxRange(int)), viewer, SLOT(setMaxRange(int)));
	QObject::connect(&cwindow, SIGNAL(needUpdate()), &cwindow, SLOT(update()));
	QObject::connect(viewer, SIGNAL(killOtherGl()), &cwindow, SLOT(close()));
	QObject::connect(sliceCon, SIGNAL(reloadFrag()), viewer, SLOT(reloadShaders()));
	QObject::connect(sliceCon, SIGNAL(toggleSlices()), viewer, SLOT(toggleSlices()));
	QObject::connect(sliceCon, SIGNAL(toggleIsos()), viewer, SLOT(toggleIsoSurfaces()));
	QObject::connect(sliceCon, SIGNAL(toggleVols()), viewer, SLOT(toggleVolSurfaces()));
	QObject::connect(sliceCon, SIGNAL(toggleCut()), viewer, SLOT(toggleCuts()));
	
	window.show();
	//cwindow.show();
	return app.exec();
}


