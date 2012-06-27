#include "Histogram.h"

using namespace std;

Histogram::Histogram(QWidget * parent) : QGLWidget(parent)
{
	intensitySize = 3000;
	for(int i = 0; i < intensitySize; i++)
	{
		intensities[i] = 0;
	}
	maxValue = 1;
	minValue = 0;
	maxIntensity = 1;
	minIntensity = 0;
	collectedInfo = 0;
	leftBar = 0;
	rightBar = 1;
	zoomDraw = 0;
	
	updateGL();
}

void Histogram::toggleZoom()
{
	if(zoomDraw == 0)
	{
		for(int i = 0; i < intensitySize; i++)
		{
			if( i <= rightBar && i >= leftBar)
			{
				if(i == leftBar)
				{
					zoomIntensity = intensities[i];
				}
				else if(zoomIntensity < intensities[i])
				{
					zoomIntensity = intensities[i];
				}
			}
		}
		
		emit(updatedMaxRangeInternal(int(rightBar)));
		emit(updatedMinRange(int(leftBar)));
		emit(updatedMaxIntensity(zoomIntensity));
		zoomDraw = 1;
	}
	else
	{
		emit(updatedMaxRangeInternal(maxValue));
		emit(updatedMinRange(minValue));
		emit(updatedMaxIntensity(maxIntensity));
		zoomDraw = 0;
	}
	
	
	updateGL();
	
	return;
}

void Histogram::setUp()
{
	return;
}

void Histogram::initializeGL()
{
	
	glClearColor(1, 1, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	
	return;
}

void Histogram::resizeGL(int w, int h) // sets gl up
{
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(collectedInfo == 1)
    {
	    gluOrtho2D(0, maxValue, 0, maxIntensity + 20);
    }
    else
    {
	    gluOrtho2D(0,1,0,1);
    }
    glMatrixMode(GL_MODELVIEW);
    
    emit updatedMaxIntensity(int(maxIntensity));

    return;
}

void Histogram::mousePressEvent(QMouseEvent *event)
{
	if((event -> x() / float(this->width())) > 1)
	{
		currentPos = 1;
	}
	else if((event -> x() / float(this->width())) < 0)
	{
		currentPos = 0;
	}
	else
	{
		currentPos = event -> x() / float(this->width());
	}
	
	if (event->button() == Qt::LeftButton)
	{
		leftRight = 0;
		leftBarSet();
	}
	else if (event -> button() == Qt::RightButton)
	{
		leftRight = 1;
		rightBarSet();
	}
	
	return;
}

void Histogram::mouseMoveEvent(QMouseEvent *event)
{
	if((event -> x() / float(this->width())) > 1)
	{
		currentPos = 1;
	}
	else if((event -> x() / float(this->width())) < 0)
	{
		currentPos = 0;
	}
	else
	{
		currentPos = event -> x() / float(this->width());
	}
	
	if (leftRight == 0)
	{
		leftBarSet();
	}
	else if (leftRight == 1)
	{
		rightBarSet();
	}
	
	//moveNode();
	
	return;
}

void Histogram::leftBarSet()
{
	if(currentPos * maxValue < rightBar - 22)
	{
		leftBar = currentPos * maxValue;
		emit(setDownRange(currentPos));
	}
	updateGL();
	
	return;
}

void Histogram::rightBarSet()
{
	if(currentPos * maxValue > leftBar + 22)
	{
		rightBar = currentPos * maxValue;
		emit(setUpRange(currentPos));
	}
	updateGL();
	return;
}

void Histogram::paintGL() // paints gl
{
	float lineHeightLeft, lineHeightRight;
	float zoomValue;
	float zoomPos;
	float zoomOffset = rightBar - leftBar;
	float polygonWidth = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(0, 0, 1.9, 0, 0, 0, 0, 1, 0);
	if(collectedInfo == 1)
	{
		if(zoomDraw == 0)
		{
			glColor3f(0, 0, 0);
			for(int i = 0; i < intensitySize; i++)
			{
				glBegin(GL_POLYGON);
					glVertex2f(i, 0);
					glVertex2f(i, intensities[i]);
					glVertex2f(i + 1, intensities[i+1]);
					glVertex2f(i + 1, 0);
				glEnd();
			}
			
			glBegin(GL_LINES);
			{
				glColor3f(0.5, 0.5, 0.5);
				glVertex2f(leftBar ,0);
				glVertex2f(leftBar ,maxIntensity);
			}
			glEnd();
			
			glBegin(GL_LINES);
			{
				glColor3f(0.5, 0.5, 0.5);
				glVertex2f(rightBar ,0);
				glVertex2f(rightBar ,maxIntensity);
			}
			glEnd();
		}
		else
		{
			glColor3f(0, 0, 0);
			for(int i = 0; i < intensitySize; i++)
			{
				if( i <= rightBar && i >= leftBar)
				{
					lineHeightLeft = (intensities[i] / float(zoomIntensity)) * maxIntensity;
					lineHeightRight = (intensities[i+1] / float(zoomIntensity)) * maxIntensity;
					zoomPos = i - leftBar;
					zoomValue = zoomPos/zoomOffset;
					zoomValue = zoomValue * maxValue;
					polygonWidth = maxValue / zoomOffset;
					glBegin(GL_POLYGON);
						glVertex2f(zoomValue, 0);
						glVertex2f(zoomValue, lineHeightLeft);
						glVertex2f(zoomValue + polygonWidth, lineHeightRight);
						glVertex2f(zoomValue + polygonWidth, 0);
					glEnd();
				}
			}
		}
	}
	return;
}

QSize Histogram::minimumSize() const
{
     return QSize(50, 50);
}

QSize Histogram::maximumSize() const
{
	return QSize(50,50);
}

QSize Histogram::sizeHint() const
{
     return QSize(400, 400);
}

void Histogram::readFile(float* fileData, int xDim, int yDim, int zDim, float maxValueIn) // reads a file asssumes it recieved a header
{
	for(int i = 0; i < intensitySize; i++)
	{
		intensities[i] = 0;
	}
	maxValue = maxValueIn;
	float currentNumber;
	for(int i = 0; i < (xDim*yDim*zDim); i++)
	{
		currentNumber = fileData[i];
		currentNumber *= maxValue;
		intensities[int(currentNumber)]++;
	}
	for(int i = 0; i < maxValue; i++)
	{
		if(i == 0)
		{
			minIntensity = intensities[i];
			maxIntensity = intensities[i];
		}
		else
		{
			if(intensities[i] > maxIntensity)
			{
				maxIntensity = intensities[i];
			}
			if(intensities[i] < minIntensity)
			{
				minIntensity = intensities[i];
			}
		}
	}
	
	rightBar = maxValue;
	leftBar = 0;
	//qDebug() << intensitySize << endl;
	//qDebug() << rightBar << endl;
 	emit updatedMinRange(0);
// 	//qDebug() << "got here 2" << endl;
 	emit updatedMaxRange(int(maxValue));
// 	//qDebug() << "got here 3" << endl;
 	emit updatedMaxIntensity(int(maxIntensity) + 20);
// 	//qDebug() << "got here 4" << endl;
 	collectedInfo = 1;
// 	//qDebug() << "got here 5" << endl;
 	updateGL();
 	return;
}