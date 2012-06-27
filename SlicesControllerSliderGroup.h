#ifndef __SlicesControllerSliderGroup_h__
#define __SlicesControllerSliderGroup_h__

#include <QGLWidget>
#include <math.h>
#include <QSlider>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>
#include "ColorMap.h"
#include "MapNode.h"

class SlicesControllerSliderGroup : public QWidget // widget to hold other widgets
{
    Q_OBJECT
    
	public slots:
		void addSliceNode();
    
  public:
    SlicesControllerSliderGroup(QWidget * parent);
    void setSlicesMapList(ColorMap* cmaplistin);
    QSlider* getDetail();
    void addSlicesMap(int x, int y, int z, int alpha, int value);
    QSlider* detail;
    QSlider* alpha;
    QSlider* getAlpha();
    ColorMap* cmaplist;
    QVBoxLayout* layout;

};

#endif