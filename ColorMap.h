#ifndef __ColorMap_h__
#define __ColorMap_h__
#include <QObject>
#include "ColorMapItr.h"
#include <QColor>
#include "MapNode.h"

class ColorMap : public QObject
{
	Q_OBJECT
	
	signals:
		void changed();
		void sizeChanged();
		
	public slots:
		void affected();
		void removeSlot(int index);
		void valueAffected(int index);
	
	public:
		ColorMap();
		MapNode* addMap(float x, float y, float z, float alpha, int value);
		int length();
		void emitUpdate();
		MapNode* start();
		MapNode* end();
		void remove(int index);
		void print();
		ColorMapItr* getIter();
		int size;
		MapNode* startNode;
		MapNode* endNode;
		~ColorMap();
};

#endif