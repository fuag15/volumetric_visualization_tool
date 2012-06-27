#include "ColorMapItr.h"

ColorMapItr::ColorMapItr(MapNode* sNode, MapNode* eNode):QObject()
{
	startNode = sNode;
	endNode = eNode;
	cursor = sNode->getNext();
}

void ColorMapItr::reset()
{
	cursor = startNode->getNext();
}

MapNode* ColorMapItr::getCursor()
{
	return cursor;
}

void ColorMapItr::advance()
{
	cursor = cursor->getNext();
}

bool ColorMapItr::end()
{
	return cursor == endNode;
}