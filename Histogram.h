#ifndef __Histogram_h__
#define __Histogram_h__

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



class Histogram : public QGLWidget
{
	Q_OBJECT

	public:
		Histogram(QWidget * parent);
		//Histogram();
		QSize minimumSize() const; // min size of widget
		QSize maximumSize() const; // max size of widget
		QSize sizeHint() const;
		void readFile(float* fileData, int xDim, int yDim, int zDim, float maxValueIn);
		void toggleZoom();

	//private slots:
  //  void move();
		
		
	signals:
		void updatedMaxRangeInternal(int value);
		void setUpRange(float value);
		void setDownRange(float value);
		void updatedMaxIntensity(int value);
		void updatedMinRange(int value);
		void updatedMaxRange(int value);

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();
		void setUp();
		 // reads in the contents of MightyMax.boone
		QPoint lastPos;
		float maxValue;
		float minValue;
		float maxIntensity;
		float minIntensity;
		float viewHoriz;
		float viewVert;
		float horizOff;
		float vertOff;
		int intensitySize;
		float intensities[3000];
		float currentPos;
		void leftBarSet();
		void rightBarSet();
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		unsigned char* texture;
		bool collectedInfo;
		int leftBar;
		int rightBar;
		int zoomDraw;
		bool leftRight;
		int zoomIntensity;
};

#endif