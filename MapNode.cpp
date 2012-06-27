#include "MapNode.h"

MapNode::MapNode() : QObject()
{
	color = new QColor(255, 255, 255);
	visible = 1;
	return;
	butx = 0;
	buty = 0;
	butz = 0;
	type = 0;
	butv = 0;
	buttonSet = 0;
}

void MapNode::setButtons(QPushButton* x, QPushButton* y, QPushButton* z, QPushButton* v)
{
	butx = x;
	buty = y;
	butz = z;
	butv = v;
	buttonSet = 1;
	
	return;
}

int MapNode::getType()
{
	return type;
}

void MapNode::setType(int x)
{
	type = x;
	emit(nodeChanged());
}

void MapNode::uRed(float x)
{
	color->setRedF(x);
	emit(nodeChanged());
}

void MapNode::uGreen(float x)
{
	color->setGreenF(x);
	emit(nodeChanged());
}

void MapNode::die()
{
	emit(closeMe());
	emit(killMe(value));
}

void MapNode::setX()
{
	if(buttonSet == 1)
	{
		buty -> setChecked(0);
		butz -> setChecked(0);
		butv -> setChecked(0);
	}
	color->setRedF(1);
	color->setGreenF(0);
	color->setBlueF(0);
	emit(nodeChanged());
}

void MapNode::setV()
{
	if(buttonSet == 1)
	{
		buty -> setChecked(0);
		butz -> setChecked(0);
		butx -> setChecked(0);
	}
	color->setRedF(0);
	color->setGreenF(0);
	color->setBlueF(0);
	emit(nodeChanged());
}

void MapNode::setToggle()
{
	if(visible == 1)
	{
		alpha = 0;
		visible = 0;
	}
	else
	{
		alpha = 1;
		visible = 1;
	}
	emit(nodeChanged());
}

void MapNode::setY()
{
	if(buttonSet == 1)
	{
		butx -> setChecked(0);
		butz -> setChecked(0);
		butv -> setChecked(0);
	}
	color->setRedF(0);
	color->setGreenF(1);
	color->setBlueF(0);
	emit(nodeChanged());
}

void MapNode::setZ()
{
	if(buttonSet == 1)
	{
		buty -> setChecked(0);
		butx -> setChecked(0);
		butv -> setChecked(0);
	}
	color->setRedF(0);
	color->setGreenF(0);
	color->setBlueF(1);
	emit(nodeChanged());
}

void MapNode::uBlue(float x)
{
	color->setBlueF(x);
	emit(nodeChanged());
}

void MapNode::selfDestruct()
{
	emit(killMe(value));
}

void MapNode::setRGB(float r, float g, float b)
{
	color->setRedF(r);
	color->setGreenF(g);
	color->setBlueF(b);
	emit(nodeChanged());
}

void MapNode::setActive()
{
	emit(active(this));
}

void MapNode::activate()
{
	emit(active(this));
}

void MapNode::uColor(const QColor & color2)
{
	*color = color2;
	emit(nodeChanged());
}

void MapNode::uAlpha(float x)
{
	alpha = x;
	emit(nodeChanged());
}

void MapNode::uValue(int x)
{
	value = x;
	emit(valueChanged(value));
}

void MapNode::setQColor(const QColor & color2)
{
	*color = color2;
	emit(nodeChanged());
}

float MapNode::getRed()
{
	return color->redF();
}
float MapNode::getGreen()
{
	return color->greenF();
}
float MapNode::getBlue()
{
	return color->blueF();
}
float MapNode::getAlpha()
{
	return alpha;
}
int MapNode::getValue()
{
	return value;
}
MapNode* MapNode::getNext()
{
	return next;
}
MapNode* MapNode::getPrev()
{
	return prev;
}

void MapNode::setRed(float x)
{
	color->setRedF(x);
	emit(nodeChanged());
}
void MapNode::setGreen(float x)
{
	color->setGreenF(x);
	emit(nodeChanged());
}
void MapNode::setBlue(float x)
{
	color->setBlueF(x);
	emit(nodeChanged());
}

void MapNode::setRedI(int x)
{
	color->setRed(x);
	emit(nodeChanged());
	return;
}
void MapNode::setGreenI(int x)
{
	color->setGreen(x);
	emit(nodeChanged());
	return;
}
void MapNode::setBlueI(int x)
{
	color->setBlue(x);
	emit(nodeChanged());
	return;
}

void MapNode::setAlpha(float x)
{
	alpha = x;
	emit(nodeChanged());
}
void MapNode::setValue(int x)
{
	value = x;
	emit(valueChanged(value));
}
void MapNode::setNext(MapNode* x)
{
	next = x;
	emit(nodeChanged());
}
void MapNode::setPrev(MapNode* x)
{
	prev = x;
	emit(nodeChanged());
}