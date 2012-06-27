#include "ColorMapperBar.h"

// USAGE: start x, start y, start z, you will focus along the positive z 20 units away from this
using namespace std;

ColorMapperBar::ColorMapperBar(QWidget * parent) : QGLWidget(parent)
{
	currentPos = -100;
	for(int i = 0; i < 256; i++)
	{
		intensities[i] = 0;
	}
	//texture = textin;
	cmap = 0;
	collectedInfo = 0;
	
	oldUpperBound = 1;
	oldLowerBound = 0;
	upperBound = 1;
	lowerBound = 0;
	convertTime = 0;
	setLimit = 0;
	
	updateGL();
}

void ColorMapperBar::makeFirstCurrent()
{
	activeMN = 0;
	ColorMapItr* itr;
	MapNode* temp;
	
	if(cmap != 0 )
	{
		itr = cmap->getIter();
		itr->reset();
		
		if(!itr->end())
		{
			temp = itr->getCursor();
			activeMN = temp;
			activeMN -> activate();
			itr->advance();
		}
		
	}
	
	return;
}

void ColorMapperBar::mousePressEvent(QMouseEvent *event)
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
	
	activateNode();
	return;
}

void ColorMapperBar::addNode()
{
	if( cmap == 0)
	{
		return;
	}
	if( setLimit == 1)
	{
		if(cmap->length() == 1)
		{
			return;
		}
	}
	activeMN = cmap -> addMap(1, 1, 1, 1, ((currentPos * ((upperBound * 256) - (lowerBound * 256))) + lowerBound * 256));
	
	updateGL();
	
	return;
}

void ColorMapperBar::removeCurrent()
{
	MapNode* temp;
	if(activeMN == 0)
	{
		return;
	}
	temp = activeMN;
	activeMN = 0;
	temp -> selfDestruct();
	
	makeFirstCurrent();
	
	updateGL();
	
	return;
}

void ColorMapperBar::activateNode()
{
	float currentWidth = ((upperBound * float(256)) - (lowerBound * float(256)));
	float currentPoint;
	activeMN = 0;
	ColorMapItr* itr;
	MapNode* temp;
	
	if(cmap != 0 )
	{
		itr = cmap->getIter();
		itr->reset();
		
		while(!itr->end())
		{
			temp = itr->getCursor();
			currentPoint = (((temp->getValue() - (256 * lowerBound)) / currentWidth) * 256);
			if((currentPoint - 10 < (currentPos * 256)) && (currentPoint + 10 > (currentPos * 256)))
			{
				activeMN = temp;
				activeMN -> activate();
			}
			itr->advance();
		}
		
	}
	
	if(activeMN == 0)
	{
		addNode();
	}
	
	return;
}

void ColorMapperBar::moveNode()
{
	if(activeMN == 0)
	{
		return;
	}
	
	activeMN -> setValue(currentPos * (((upperBound * 256) - (lowerBound * 256))) + (lowerBound * 256));
	
	return;
}

void ColorMapperBar::setUpperBound(float up)
{
	convertTime++;
	oldUpperBound = upperBound;
	upperBound = up;
	convert();
	
	return;
}
void ColorMapperBar::setLowerBound(float down)
{
	convertTime++;
	oldLowerBound = lowerBound;
	lowerBound = down;
	convert();
	
	return;
}

void ColorMapperBar::convert()
{
	if(convertTime == 2)
	{
		convertTime = 0;
		reValue();
	}
	
	return;
}

void ColorMapperBar::mouseMoveEvent(QMouseEvent *event)
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
	
	moveNode();
	
	
	
	return;
}

void ColorMapperBar::setUp()
{
	return;
}

void ColorMapperBar::setCmap(ColorMap* cmapr)
{
	cmap = cmapr;
	
	return;
}

void ColorMapperBar::initializeGL()
{
	
	glClearColor(1, 1, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	
	return;
}

void ColorMapperBar::resizeGL(int w, int h) // sets gl up
{
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 255, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    return;
}

void ColorMapperBar::paintGL() // paints gl
{
	float currentWidth = ((upperBound * float(256)) - (lowerBound * float(256)));
	float currentPoint;
	ColorMapItr* itr;
	MapNode* temp;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(0, .49);
	glVertex2f(0, .51);
	glVertex2f(255, .51);
	glVertex2f(255, .49);
	glEnd();
	
	if(cmap != 0 )
	{
		itr = cmap->getIter();
		itr->reset();
		
		while(!itr->end())
		{
			temp = itr->getCursor();
			currentPoint = (((temp->getValue() - (256 * lowerBound)) / currentWidth) * 256);
			glBegin(GL_TRIANGLES);
				glColor3f(temp->getRed(), temp->getGreen(),  temp->getBlue());
				glVertex2f(currentPoint - 10, .25);
				glVertex2f(currentPoint + 10, .25);
				glVertex2f(currentPoint, .75);
				
			glEnd();
			
			glBegin(GL_LINES);
			{
				if(temp == activeMN)
				{
					glColor3f(0,0,0);
				}
				else
				{
					glColor3f(0.5,0.5,0.5);
				}
				glVertex2f(currentPoint - 10, .25);
				glVertex2f(currentPoint + 10, .25);
				glVertex2f(currentPoint + 10, .25);
				glVertex2f(currentPoint, .75);
				glVertex2f(currentPoint, .75);
				glVertex2f(currentPoint - 10, .25);
			}
			glEnd();
			itr->advance();
		}
		
	}
	
	return;
}

void ColorMapperBar::reValue()
{
	ColorMapItr* itr;
	MapNode* temp;
	float oldValue;
	//float width = 256;
	//float oldMin = oldLowerBound * float(256);
	//float oldWidth = (oldUpperBound * float(256)) - (oldLowerBound * float(256));
	float newWidth = (upperBound * float(256)) - (lowerBound * float(256));
	float newMin = lowerBound * float(256);
	float count;
	float countInc;
	
	countInc = 10.0;
	float countAdd = newWidth / countInc;
	count = countAdd;
	
	if(cmap != 0 )
	{
		itr = cmap->getIter();
		itr->reset();
		
		while(!itr->end())
		{
			if(newMin + count >= upperBound * float(256))
			{
				countInc *= 2;
				countAdd = newWidth / countInc;
				count = countAdd;
			}
			temp = itr->getCursor();
			oldValue = temp->getValue();
			
			//temp->setValue((((oldValue - oldMin)/oldWidth) * newWidth) + newMin);
			temp->setValue(newMin + count);
			//temp->setValue(((temp->getValue() / float(256)) * ((upperBound * float(256)) - (lowerBound * float(256)))) + (lowerBound * float(256)));
			itr->advance();
			
			count += countAdd;
		}
		
	}
	
	updateGL();
}

QSize ColorMapperBar::minimumSize() const
{
     return QSize(50, 50);
}

QSize ColorMapperBar::maximumSize() const
{
	return QSize(50,50);
}

QSize ColorMapperBar::sizeHint() const
{
     return QSize(400, 400);
}