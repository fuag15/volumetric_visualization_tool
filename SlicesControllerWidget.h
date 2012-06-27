#ifndef __SlicesControllerWidget_h__
#define __SlicesControllerWidget_h__

#include <QGLWidget>
#include <math.h>
#include <QSlider>
#include <QDockWidget>
#include <QGridLayout>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QDialogButtonBox>
#include "ColorMap.h"
#include "SlicesControllerSliderGroup.h"

class SlicesControllerWidget : public QWidget // widget to hold other widgets
{
    Q_OBJECT
    
	signals:
		void toggleSlices();
		void toggleVols();
		void toggleIsos();
		void reloadFrag();
		void toggleCut();
		
	public slots:
		void sendReloadOut();
		void sendCutOut();
		void sendSlicesOut();
		void sendIsosOut();
		void sendVolsOut();
    
  public:
    QSlider* getDetailSlider();
    QSlider* getAlphaSlider();
    SlicesControllerWidget(QWidget * parent);
    void setSliceMap(ColorMap* smap);
    SlicesControllerSliderGroup* sgroup;

};

#endif