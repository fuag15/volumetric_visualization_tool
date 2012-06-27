#ifndef __ColorMapper_h__
#define __ColorMapper_h__

#include <math.h>
#include <QSlider>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <QDir>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <iostream>
#define GL_GLEXT_PROTOTYPES
//#include <GL/glut.h>
#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glext.h>

#include <QGLWidget>
#include <QImage>
#include <QFileDialog>
#include "ColorMap.h"
#include "ColorMapItr.h"



class ColorMapper : public QGLWidget
{
	Q_OBJECT
	
	public:
		ColorMapper(QWidget * parent);
		//Histogram();
		QSize minimumSize() const; // min size of widget
		QSize maximumSize() const; // max size of widget
		QSize sizeHint() const;
		void setCmap(ColorMap* cmapt);
		void setAlphaMap(float* map);

	//private slots:
  //  void move();
		
	signals:
		void updatedMaxIntensity(int value);

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();
		void setUp();
		//void readFile(QString filename); // reads in the contents of MightyMax.boone
		QPoint lastPos;
		QTimer *timer;
		float maxValue;
		float minValue;
		float maxIntensity;
		float minIntensity;
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void setAlpha();
		float viewHoriz;
		float viewVert;
		float horizOff;
		float vertOff;
		float intensities[256];
		unsigned char* texture;
		bool collectedInfo;
		GLint fragmentShader;
		GLint program;
		ColorMap* cmap;
		float lastPosx;
		float lastPosy;
		float currentPosx;
		float currentPosy;
		float* alphaMap;
		int alphaMapSet;
};

#endif