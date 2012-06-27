#include "ColorMap.h"

ColorMap::ColorMap() : QObject()
{
	size = 0;
	startNode = new MapNode();
	endNode =  new MapNode();
	startNode->setValue(-1);
	startNode -> setNext(endNode);
	endNode -> setValue(9999);
	endNode -> setPrev(startNode);
}

void ColorMap::emitUpdate()
{
	emit(affected());
	return;
}

void ColorMap::affected()
{
	emit(changed());
	return;
}

ColorMapItr* ColorMap::getIter()
{
	return new ColorMapItr(startNode, endNode);
}

void ColorMap::valueAffected(int index)
{
	MapNode* cursorNode = start();
	MapNode* temp;
	MapNode* temp2;
	while(cursorNode != endNode && cursorNode->getValue() != index)
	{
		cursorNode = cursorNode -> getNext();
	}
	if(cursorNode->getValue() == index)
	{
		temp = cursorNode->getPrev();
		while(temp != startNode && cursorNode->getValue() < temp->getValue())
		{
			//swap down
			temp = cursorNode->getPrev();
			temp2 = cursorNode->getNext();
			temp2->setPrev(temp);
			temp->setNext(temp2);
			temp2 = temp->getPrev();
			cursorNode->setPrev(temp2);
			cursorNode->setNext(temp);
			temp->setPrev(cursorNode);
			temp2->setNext(cursorNode);
			temp = cursorNode->getPrev();
		}
		temp = cursorNode->getNext();
		while(temp != endNode && cursorNode->getValue() > temp->getValue())
		{
			//swap up
			temp = cursorNode->getNext();
			temp2 = cursorNode->getPrev();
			temp->setPrev(temp2);
			temp2->setNext(temp);
			
			
			temp2 = temp->getNext();
			
			cursorNode->setPrev(temp);
			cursorNode->setNext(temp2);
			temp2->setPrev(cursorNode);
			temp->setNext(cursorNode);
			
			temp = cursorNode->getNext();
		}
	}
	else
	{
		qDebug() << "BIG FUCKING PROBLEM";
	}
	emit(changed());
	return;
}
void ColorMap::print()
{
	MapNode* cursorNode = startNode;
	while(cursorNode != endNode)
	{
		qDebug() << cursorNode->getValue() << "addres" << cursorNode;
		cursorNode = cursorNode->getNext();
	}
	
	qDebug() << cursorNode->getValue() << "addres" << cursorNode;
}

MapNode* ColorMap::addMap(float x, float y, float z, float alpha, int value)
{
	MapNode* newn = new MapNode();
	newn->setRed(x);
	newn->setGreen(y);
	newn->setBlue(z);
	newn->setAlpha(alpha);
	newn->setValue(value);
	MapNode* temp;
	MapNode* temp2;
	if(size == 0)// if empty put one in
	{
		temp = newn;
		temp -> setNext(endNode);
		temp -> setPrev(startNode);
		endNode->setPrev(temp);
		startNode->setNext(temp);
	}
	else// if not empty
	{
		MapNode* cursorNode = start();
		while(cursorNode != endNode && cursorNode->getValue() <= value)
		{
			cursorNode = cursorNode->getNext();
		}
		//we are now where we should put prev is where node shoudl be put
		newn->setNext(cursorNode);
		temp = cursorNode->getPrev();
		newn->setPrev(temp);
		cursorNode->setPrev(newn);
		temp->setNext(newn);
	}
	
	connect(newn, SIGNAL(nodeChanged()), this, SLOT(affected()));
	connect(newn, SIGNAL(killMe(int)), this, SLOT(removeSlot(int)));
	connect(newn, SIGNAL(valueChanged(int)), this, SLOT(valueAffected(int)));
	size++;
	
	emit(sizeChanged());
	emit(changed());
	newn -> activate();
	return newn;
}

void ColorMap::removeSlot(int index)
{
	remove(index);
}

int ColorMap::length()
{
	return size;
}
MapNode* ColorMap::start()
{
	return startNode;
}

MapNode* ColorMap::end()
{
	return endNode;
}

void ColorMap::remove(int value)
{
	MapNode* temp;
	MapNode* temp2;
	MapNode* cursorNode = start();
	while((cursorNode != endNode) && (cursorNode->getValue() != value))
	{
		cursorNode = cursorNode->getNext();
	}
	if(cursorNode->value == value)
	{
		temp = cursorNode;
		temp2 = cursorNode->getPrev();
		cursorNode = cursorNode->getNext();
		temp2->setNext(cursorNode);
		cursorNode->setPrev(temp2);
		delete(temp);
	}
	size--;	
	emit(sizeChanged());
	emit(changed());
}

ColorMap::~ColorMap()
{
	MapNode* temp;
	MapNode* cursorNode = start();
	while(cursorNode != endNode)
	{
		temp = cursorNode;
		cursorNode = cursorNode->getNext();
		delete(temp);
	}
	delete(endNode);
}