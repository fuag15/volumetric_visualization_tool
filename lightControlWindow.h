#ifndef __LightControlWindow_h__
#define __LightControlWindow_h__

#include <QGLWidget>
#include <math.h>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QLabel>
#include "ColorMap.h"
#include "HistogramWidget.h"
#include "MapNode.h"
#include <QPushButton>
#include "ColorMapItr.h"
#include "SlicesControllerWidget.h"
#include "ColorMapper.h"
#include "ColorMapperBar.h"

class LightControlWindow : public QWidget // widget to hold other widgets
{
    Q_OBJECT
    
  public:
    QSlider *dSlider;
    QSlider *aSlider; 
    QSlider *sSlider;
    QSlider *xSlider;
    QSlider *ySlider; 
    QSlider *zSlider;
    QSlider *dSlider2, *aSlider2, *sSlider2, *xSlider2, *ySlider2, *zSlider2;
    LightControlWindow(QWidget * parent);

};

#endif