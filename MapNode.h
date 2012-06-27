#ifndef __MapNode_h__
#define __MapNode_h__
#include <QObject>
#include "MapNode.h"
#include <QDebug>
#include <QColor>
#include <QPushButton>

class MapNode : public QObject
{
	Q_OBJECT
	
	signals:
		void nodeChanged();
		void valueChanged(int value);
		void active(MapNode* active);
		void killMe(int value);
		void closeMe();
	
	public slots:
		void selfDestruct();
		void uRed(float x);
		void uGreen(float x);
		void uBlue(float x);
		void uAlpha(float x);
		void uValue(int x);
		void setX();
		void setY();
		void setZ();
		void setV();
		void setToggle();
		void setActive();
		void uColor(const QColor & color2);
	
	
	public:
		MapNode();
		void setRGB(float r, float g, float b);
		float getRed();
		float getGreen();
		float getBlue();
		float getAlpha();
		int getType();
		int getValue();
		void activate();
		MapNode* getNext();
		MapNode* getPrev();
		void setQColor(const QColor & color2);
		void setRed(float x);
		void setGreen(float x);
		void setBlue(float x);
		void setType(int x);
		
		void setRedI(int x);
		void setGreenI(int x);
		void setBlueI(int x);
		
		void setAlpha(float x);
		void setValue(int x);
		void setNext(MapNode* x);
		void setPrev(MapNode* x);
		
		void setButtons(QPushButton* x, QPushButton* y, QPushButton* z, QPushButton* v);
		//float red;
		//int green;
		//int blue;
		int alpha;
		int type;
		int value;
		void die();
		int visible;
		MapNode* next;
		MapNode* prev;
		QColor* color;
		QPushButton* butx;
		QPushButton* buty;
		QPushButton* butz;
		QPushButton* butv;
		int buttonSet;
};

#endif