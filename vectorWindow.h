#ifndef _VectorWindow_h_
#define _VectorWindow_h_

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
#include "LicSliderGroup.h"
#include "GlyphSliderGroup.h"

class vectorWindow : public QWidget // widget to hold other widgets
{
    Q_OBJECT
    
	signals:
// 		void toggleSlices();
// 		void toggleVols();
// 		void toggleIsos();
// 		void reloadFrag();
// 		void toggleCut();
		
	public slots:
// 		void sendReloadOut();
// 		void sendCutOut();
// 		void sendSlicesOut();
// 		void sendIsosOut();
// 		void sendVolsOut();
    
  public:
//     QSlider* getDetailSlider();
//     QSlider* getAlphaSlider();
    vectorWindow(QWidget * parent);
    void setGlyphSliceMap(ColorMap* smap);
    void setLicSliceMap(ColorMap* smap);
//     SlicesControllerSliderGroup* sgroup;
    LicSliderGroup* lgroup;
    GlyphSliderGroup* ggroup;

};

#endif