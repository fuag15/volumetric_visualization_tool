#ifndef __ColorMapperBar_h__
#define __ColorMapperBar_h__

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


class ColorMapperBar : public QGLWidget
{
	Q_OBJECT

	public:
		ColorMapperBar(QWidget * parent);
		//Histogram();
		QSize minimumSize() const; // min size of widget
		QSize maximumSize() const; // max size of widget
		QSize sizeHint() const;
		void setCmap(ColorMap* cmapr);
		void reValue();
		void convert();

	//private slots:
  //  void move();
		
	signals:
		void updatedMaxIntensity(int value);
		
	public slots:
		void removeCurrent();
		void setUpperBound(float up);
		void setLowerBound(float down);
	
	public:
		bool setLimit;

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();
		void setUp();
		//void readFile(QString filename); // reads in the contents of MightyMax.boone
		QPoint lastPos;
		float maxValue;
		float minValue;
		float maxIntensity;
		float minIntensity;
		float viewHoriz;
		float viewVert;
		float horizOff;
		float vertOff;
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void moveNode();
		void addNode();
		void makeFirstCurrent();
		float intensities[256];
		unsigned char* texture;
		bool collectedInfo;
		float currentPos;
		int convertTime;
		void activateNode();
		MapNode* activeMN;
		float upperBound;
		float oldLowerBound;
		float oldUpperBound;
		float lowerBound;
		
		ColorMap* cmap;
};

#endif