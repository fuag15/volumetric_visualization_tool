#ifndef _LicSliderGroup_h_
#define _LicSliderGroup_h_

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

class LicSliderGroup : public QWidget
{
	Q_OBJECT
    
	public slots:
		void addSliceNode();
		
	 public:
		LicSliderGroup(QWidget * parent);
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