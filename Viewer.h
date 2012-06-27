#ifndef __Viewer_h__
#define __Viewer_h__

#include <math.h>
#include <QSlider>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <QDir>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <iostream>
#define GL_GLEXT_PROTOTYPES
#include <QHBoxLayout>
#include <GL/gl.h>
#include <QApplication>

#include <QGLWidget>
#include <QImage>
#include <QFileDialog>
#include <QFile>
#include "ColorMap.h"
#include "ColorMapItr.h"
#include "MapNode.h"
#include <stdio.h>
#include <stdlib.h>
#include "arcball.h"



class Viewer : public QGLWidget
{
    Q_OBJECT

	public:
		Viewer(QWidget * parent);
		QSize minimumSizeHint() const; // min size of widget
		QSize sizeHint() const; // start size ideal of widget
		void setSlicesMap(ColorMap* smapin);
		void setGlyphSlicesMap(ColorMap* smapin);
		void setLicSlicesMap(ColorMap* smapin);
		void setColorMap(ColorMap* cmapin);
		void setIsoMap(ColorMap* imapin);
		void setAlphaMap(float* map);
		void setApp(QApplication* QappIn);

  //private slots:
  //  void move();
	signals:
		void killOtherGl();
		void newFileLoaded(float* fileData, int xDim, int yDim, int zDim, float maxValue);
		void windowClosed();
  
	public slots:
		void setIsoDetail(int value);
		void setAlphaDetail(int value);
		void toggleSlices();
		void toggleIsoSurfaces();
		void toggleCuts();
		void toggleVolSurfaces();
		void readFile();
		void setMaxRange(int value);
		void regenerateTransfer();
		void reloadShaders();

  protected:
    void drawBox();
    void drawGlyphSurfaces();
    void drawViewSlice();
    void drawLicSurfaces();
    void drawIsoSurfaces();
    void drawVolSurfaces();
    void initializeGL();
    void createNoiseTexture(int x, int y, int z);
    void resizeGL(int w, int h);
    void paintGL();
    float vecX(int x, int y, int z);
    float vecY(int x, int y, int z);
    float vecZ(int x, int y, int z);
    void plotPicture(); // applies camera transform
    //void readFile(QString filename); // reads in the contents of MightyMax.boone
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent ( QCloseEvent * event );
    float interpolate(float lowValue, float lowPos, float hiValue, float hiPos, int pos);
    void triTerpolate(float x, float y, float z);
    QPoint lastPos;
    QTimer *timer;
    float zoomAmount;
    float xOff;
    float yOff;
    void generateTex1D();
    bool zoom;
    float xrot;
    float currentVecX;
    float currentVecY;
    float currentVecZ;
    float yrot;
    int sizex;
    int sizey;
    int sizez;
    int size;
    int pan;
    void drawGlyphX(float xIn);
    void drawGlyphY(float yIn);
    void drawGlyphZ(float zIn);
    int cuts;
    int drawnView;
    bool fileSelected;
    unsigned int texname;
    void generateTransferImage();
    unsigned char* texture;
    unsigned int texname2;
    unsigned int texname3;
    float* convertedData;
    float* convertedVectorData;
    void drawSlices();
    int maxRange;
    float isoDetail;
    float alphaDetail;
    QFile* theSource;
    ColorMap* smap;
    ColorMap* cmap;
    ColorMap* gmap;
    ColorMap* lmap;
    ColorMap* imap;
    float* alphaMap;
    float* vectorMap;
    int gotVectors;
    int alphaMapSet;
    int transferFunctionSet;
    GLuint shaderTexture[1];
    int textureGenerated;
    GLuint geometryShader;
    GLuint fragmentShader;
    GLuint vertexShader;
    GLuint program;
    GLuint program2;
    GLuint program3;
    GLuint program4;
    GLuint program5;
    GLuint program6;
    arcball* mainball;
    arcball* texball;
    QApplication* Qapp;
    int drawSlicesToggle;
    int drawIsos;
    int drawVols;
    int shouldCombine;
};

#endif