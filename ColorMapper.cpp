#include "ColorMapper.h"

// USAGE: start x, start y, start z, you will focus along the positive z 20 units away from this
using namespace std;

ColorMapper::ColorMapper(QWidget * parent) : QGLWidget(parent)
{
	for(int i = 0; i < 256; i++)
	{
		intensities[i] = 0;
	}
	currentPosx = 0;
	currentPosy = 0;
	cmap = 0;
	alphaMapSet = 0;
	collectedInfo = 0;
	
	updateGL();
}



void ColorMapper::setUp()
{
	return;
}

void ColorMapper::setAlphaMap(float* map)
{
	alphaMapSet = 1;
	alphaMap = map;
}

void ColorMapper::setCmap(ColorMap* cmapt)
{
	cmap = cmapt;
}

void ColorMapper::mousePressEvent(QMouseEvent *event)
{
	if((event -> x() / float(this->width())) > 1)
	{
		currentPosx = 1;
	}
	else if((event -> x() / float(this->width())) < 0)
	{
		currentPosx = 0;
	}
	else
	{
		currentPosx = event -> x() / float(this->width());
	}
	
	if((event -> y() / float(this->height())) > 1)
	{
		currentPosy = 1;
	}
	else if((event -> y() / float(this->height())) < 0)
	{
		currentPosy = 0;
	}
	else
	{
		currentPosy = event -> y() / float(this->height());
	}
	
	lastPosx = currentPosx;
	lastPosy = currentPosy;
	
	return;
}

void ColorMapper::setAlpha()
{
	int lowRange = lastPosx * 999;
	int hiRange = currentPosx * 999;
	int valueRange = hiRange - lowRange;
	
	if(valueRange < 0)
	{
		// right to left
		for(int i = hiRange; i< lowRange; i++)
		{
			alphaMap[i] = 1 - currentPosy;
		}
	}
	else
	{
		//left to right
		for(int i = lowRange; i< hiRange; i++)
		{
			alphaMap[i] = 1 - currentPosy;
		}
	}
	
	cmap->emitUpdate();
	
	updateGL();
	
	return;
}

void ColorMapper::mouseMoveEvent(QMouseEvent *event)
{
	if((event -> x() / float(this->width())) > 1)
	{
		currentPosx = 1;
	}
	else if((event -> x() / float(this->width())) < 0)
	{
		currentPosx = 0;
	}
	else
	{
		currentPosx = event -> x() / float(this->width());
	}
	
	if((event -> y() / float(this->height())) > 1)
	{
		currentPosy = 1;
	}
	else if((event -> y() / float(this->height())) < 0)
	{
		currentPosy = 0;
	}
	else
	{
		currentPosy = event -> y() / float(this->height());
	}
	
	if(alphaMapSet == 1)
	{
		setAlpha();
	}
	
	lastPosx = currentPosx;
	lastPosy = currentPosy;
	
	return;
}

void ColorMapper::initializeGL()
{
	
	glClearColor(1, 1, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_2D);
	
	return;
}

void ColorMapper::resizeGL(int w, int h) // sets gl up
{
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 255, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    return;
}

void ColorMapper::paintGL() // paints gl
{
	float pointX;
	float pointY;
	ColorMapItr* itr;
	MapNode* temp;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glBegin(GL_QUAD_STRIP);
		glColor3f(0, 0, 0);
		glVertex2f(-1, 0);
		glVertex2f(-1, 1);
		if(cmap != 0)
		{
			itr = cmap->getIter();
			itr->reset();
			while(!itr->end())
			{
				temp = itr->getCursor();
				glColor3f(temp->getRed(), temp->getGreen(),  temp->getBlue());
				glVertex2f(temp->getValue(), 0);
				glVertex2f(temp->getValue(), 1);
				itr->advance();
			}
		}
		glColor3f(0, 0, 0);
		glVertex2f(256, 0);
		glVertex2f(256, 1);
	glEnd();
	
	if(alphaMapSet == 1)
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(0,0,0);
		for(int i = 0; i < 1000; i++)
		{
			pointX = (i/float(999))*255;
			pointY = alphaMap[i];
			glVertex2f(pointX, pointY);
		}
		glEnd();
	}
	
	return;
}

QSize ColorMapper::minimumSize() const
{
     return QSize(50, 50);
}

QSize ColorMapper::maximumSize() const
{
	return QSize(50,50);
}

QSize ColorMapper::sizeHint() const
{
     return QSize(400, 400);
}