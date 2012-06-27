#ifndef __ColorMapItr_h__
#define __ColorMapItr_h__
#include <QObject>
#include "MapNode.h"

class ColorMapItr : public QObject
{
	Q_OBJECT
	
	public:
    
		ColorMapItr(MapNode* sNode, MapNode* eNode);
		void reset();
		MapNode* getCursor();
		void advance();
		bool end();
		MapNode* startNode;
		MapNode* endNode;
		MapNode* cursor;
};

#endif