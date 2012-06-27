#include "Viewer.h"
#include <time.h>
#include <math.h>

// USAGE: start x, start y, start z, you will focus along the positive z 20 units away from this
using namespace std;

Viewer::Viewer(QWidget * parent) : QGLWidget(parent)
{
	srand( (unsigned)time( NULL ) );
	mainball = new arcball();
	texball = new arcball();
	mainball->arcball_reset();
	texball->arcball_reset();
	size = 200;
	sizex = 200;
	sizey = 200;
	lmap = 0;
	gmap = 0;
	isoDetail = 0.9;
	alphaDetail = 0.9;
	sizez = 200;
	drawnView = 0;
	shouldCombine = 0;
	zoomAmount = size * 2.5;
	zoom = 0;
	maxRange = 10;
	xrot = 0;
	yrot = 0;
	fileSelected = 0;
	smap = 0;
	cmap = 0;
	transferFunctionSet = 0;
	textureGenerated = 0;
	alphaMapSet = 0;
	texball->arcball_setzoom(1, vec(0,0,zoomAmount), vec(0,1,0));
	mainball->arcball_setzoom(1, vec(0,0,zoomAmount), vec(0,1,0));
	updateGL();
	Qapp = 0;
	drawSlicesToggle = 1;
	drawIsos = 1;
	drawVols = 1;
	imap = 0;
	xOff = 0;
	yOff = 0;
	pan = 0;
	cuts = 0;
	resize(600, 600);
	
	//readFile();
}

void Viewer::toggleCuts()
{
	//qDebug() << "got Called 2" << endl;
	if(cuts == 0)
	{
		cuts = 1;
	}
	else
	{
		cuts = 0;
	}
	updateGL();
}

void Viewer::setIsoDetail(int value)
{
	isoDetail = ((float)value)/256.0;
	isoDetail = 0.9 - (isoDetail*0.8);
	updateGL();
}

void Viewer::setAlphaDetail(int value)
{
	//qDebug() << "got Called" << endl;
	alphaDetail = ((float)value)/256.0;
	isoDetail = 0.9 - (isoDetail*0.8);
	updateGL();
}

void Viewer::toggleSlices()
{
	if(drawSlicesToggle == 1)
	{
		drawSlicesToggle = 0;
	}
	else
	{
		drawSlicesToggle = 1;
	}
	updateGL();
}
void Viewer::toggleIsoSurfaces()
{
	if(drawIsos == 1)
	{
		drawIsos = 0;
	}
	else
	{
		drawIsos = 1;
	}
	updateGL();
}

void Viewer::toggleVolSurfaces()
{
	if(drawVols == 1)
	{
		drawVols = 0;
	}
	else
	{
		drawVols = 1;
	}
	updateGL();
}

void Viewer::closeEvent(QCloseEvent * event)
{
	event = 0;
	if(Qapp != 0)
	{
		Qapp->quit();
		qDebug() << "I TRIED TO QUIT";
	}
}

void Viewer::setApp(QApplication* QappIn)
{
	Qapp = QappIn;
}

void Viewer::setAlphaMap(float* map)
{
	alphaMap = map;
	alphaMapSet = 1;
}

void Viewer::regenerateTransfer()
{
	generateTransferImage();
}

void Viewer::generateTransferImage()
{
		int pos = 0;
		float normPos;
		float redV, greenV, blueV;
		ColorMapItr* itr;
		
		itr = cmap -> getIter();
		MapNode* lowerBound;
		MapNode* upperBound;
		
		if(cmap == 0 || maxRange == 0 || alphaMapSet == 0)
		{
			qDebug() << "got here";
			return;
		}
		glActiveTexture(GL_TEXTURE0);
		if(transferFunctionSet == 0)//set up texture
		{
			//glEnable(GL_TEXTURE_1D);
			transferFunctionSet = 1;
			glGenTextures (1, &shaderTexture[0]);
			glBindTexture (GL_TEXTURE_1D, shaderTexture[0]);
			glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		float shaderData[maxRange][4];
		//make 1d texture  of floats and linearly interpolate between cmap points and paint the texture
		itr->reset();
		if(!itr->end())//there is data in cmap
		{
			upperBound = itr->getCursor();
			while(pos <= upperBound->getValue() && pos < maxRange)//if were not at upperbound yet
			{
				normPos = float(pos) / float(maxRange);
				redV = interpolate(0, 0, upperBound->getRed(), upperBound->getValue()/float(256) * maxRange, pos);
				greenV = interpolate(0, 0, upperBound->getGreen(), upperBound->getValue()/float(256) * maxRange, pos);
				blueV = interpolate(0, 0, upperBound->getBlue(), upperBound->getValue()/float(256) * maxRange, pos);
				shaderData[pos][0] = redV;
				shaderData[pos][1] = greenV;
				shaderData[pos][2] = blueV;
				shaderData[pos][3] = alphaMap[int(normPos * 999)];
				//shaderData[pos][0] = upperBound->getRed();
				//shaderData[pos][1] = upperBound->getGreen();
				//shaderData[pos][2] = upperBound->getBlue();
				pos++;
			}
			itr->advance();
			
			if(itr->end())
			{
				while(pos < maxRange)
				{
					normPos = float(pos) / float(maxRange);
					redV = interpolate(upperBound->getRed(), upperBound->getValue()/float(256) * maxRange, 0, maxRange, pos);
					greenV = interpolate(upperBound->getGreen(), upperBound->getValue()/float(256) * maxRange, 0, maxRange, pos);
					blueV = interpolate(upperBound->getBlue(), upperBound->getValue()/float(256) * maxRange, 0, maxRange, pos);
					shaderData[pos][0] = redV;
					shaderData[pos][1] = greenV;
					shaderData[pos][2] = blueV;
					shaderData[pos][3] = alphaMap[int(normPos * 999)];
					//shaderData[pos][0] = upperBound->getRed();
					//shaderData[pos][1] = upperBound->getGreen();
					//shaderData[pos][2] = upperBound->getBlue();
					pos++;
				}
			}
			else
			{
				itr->reset();
			}
			//do for rest until end!
			
			while(!itr->end())
			{
				lowerBound = itr->getCursor();
				itr->advance();
				if(itr->end())//last values are lower bound!
				{
					while(pos < maxRange)//if were not at upperbound yet
					{
						normPos = float(pos) / float(maxRange);
						redV = interpolate(lowerBound->getRed(), lowerBound->getValue()/float(256) * maxRange, 0, maxRange, pos);
						greenV = interpolate(lowerBound->getGreen(), lowerBound->getValue()/float(256) * maxRange, 0, maxRange, pos);
						blueV = interpolate(lowerBound->getBlue(), lowerBound->getValue()/float(256) * maxRange, 0, maxRange, pos);
						shaderData[pos][0] = redV;
						shaderData[pos][1] = greenV;
						shaderData[pos][2] = blueV;
						shaderData[pos][3] = alphaMap[int(normPos * 999)];
						//shaderData[pos][0] = lowerBound->getRed();
						//shaderData[pos][1] = lowerBound->getGreen();
						//shaderData[pos][2] = lowerBound->getBlue();
						pos++;
					}
				}
				else
				{
					upperBound = itr->getCursor();
					while(pos <= upperBound->getValue() && pos < maxRange)
					{
						normPos = float(pos) / float(maxRange);
						redV = interpolate(lowerBound->getRed(), lowerBound->getValue()/float(256) * maxRange, upperBound->getRed(), upperBound->getValue()/float(256) * maxRange, pos);
						greenV = interpolate(lowerBound->getGreen(), lowerBound->getValue()/float(256) * maxRange, upperBound->getGreen(), upperBound->getValue()/float(256) * maxRange, pos);
						blueV = interpolate(lowerBound->getBlue(), lowerBound->getValue()/float(256) * maxRange, upperBound->getBlue(), upperBound->getValue()/float(256) * maxRange, pos);
						shaderData[pos][0] = redV;
						shaderData[pos][1] = greenV;
						shaderData[pos][2] = blueV;
						shaderData[pos][3] = alphaMap[int(normPos * 999)];
						pos++;
					}
				}
			}
			// Upload
			glActiveTexture(GL_TEXTURE0);
			glTexImage1D(GL_TEXTURE_1D, 0, 4, maxRange, 0, GL_RGBA , GL_FLOAT, shaderData);
			
			textureGenerated = 1;
		}
	
	updateGL();
	
// 	return;
}

float Viewer::interpolate(float lowValue, float lowPos, float hiValue, float hiPos, int pos)
{
	float width = hiPos - lowPos;// width of line
	float leftPercent = (float(pos) - lowPos) / width; //percantage left of pos
	//float rightPercent = (hiPos - float(pos)) / width; //precentage right of pos
	
	float rightPercent = float(1) - leftPercent;
	
	return (leftPercent * hiValue) + (rightPercent * lowValue);
}

void Viewer::setMaxRange(int value)
{
	maxRange = value;
}

void Viewer::setSlicesMap(ColorMap* smapin)
{
	smap = smapin;
	connect(smap, SIGNAL(changed()), this, SLOT(updateGL()));
}

void Viewer::setGlyphSlicesMap(ColorMap* smapin)
{
	gmap = smapin;
	connect(gmap, SIGNAL(changed()), this, SLOT(updateGL()));
}

void Viewer::setLicSlicesMap(ColorMap* smapin)
{
	lmap = smapin;
	connect(lmap, SIGNAL(changed()), this, SLOT(updateGL()));
}

void Viewer::setColorMap(ColorMap* smapin)
{
	cmap = smapin;
	connect(cmap, SIGNAL(changed()), this, SLOT(updateGL()));
	connect(cmap, SIGNAL(changed()), this, SLOT(regenerateTransfer()));
}

void Viewer::setIsoMap(ColorMap* imapin)
{
	imap = imapin;
	connect(imap, SIGNAL(changed()), this, SLOT(updateGL()));
}

void Viewer::mousePressEvent(QMouseEvent *event)
{
	//setMouseTracking(TRUE);
	//grabMouse();
	int invert_y = (this->height() - event->y());
	//int invert_x = (this->width() - event->x());
	texball->arcball_start(event->x(), invert_y);
	mainball->arcball_start(event->x(), invert_y);
	lastPos = event -> pos();
	if (event->button() == Qt::LeftButton)
	{
		zoom = 0;
		pan = 0;
	}
	else if (event -> button() == Qt::RightButton)
	{
		zoom = 1;
		pan = 0;
	}
	else
	{
		pan = 1;
		zoom = 0;
		//setMouseTracking(FALSE);
		//releaseMouse();
	}
	return;
}

void Viewer::mouseMoveEvent(QMouseEvent *event)
{
	float xchange, ychange;
	int invert_y = (this->height() - event->y());
	//int invert_x = (this->width() - event->x());
	//qDebug() << event->x();
	//xchange = event -> x() - lastPos.x();
	xchange = lastPos.x() - event -> x();
	ychange = lastPos.y() - event -> y();
	

	lastPos.setX(event -> x());
	lastPos.setY(event -> y());

	if(zoom == 1)
	{
		zoomAmount += ychange;
	}
	else if(pan == 1)
	{
		xOff -= xchange;
		yOff += ychange;
	}
	else
	{
		texball->arcball_move(event->x(), invert_y);
		mainball->arcball_move(event->x(), invert_y);
		xrot += xchange;
		yrot += ychange;
	}

	updateGL();
	
	return;
}

void Viewer::generateTex1D()
{
	makeCurrent();
	//glEnable(GL_TEXTURE_1D);
	transferFunctionSet = 1;
	glGenTextures (1, &shaderTexture[0]);
	glBindTexture (GL_TEXTURE_1D, shaderTexture[0]);
	glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	float shaderData[10][3];
	//make 1d texture  of floats and linearly interpolate between cmap points and paint the texture
	
	for(int i = 0; i < 10; i++)
	{
		shaderData[i][0] = 0.9;
		shaderData[i][1] = 0.5;
		shaderData[i][2] = 0.5;
	}
	
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 10, 0, GL_RGB , GL_FLOAT, shaderData);
	textureGenerated = 1;
	
	return;
}

void Viewer::initializeGL()
{
	glClearColor(0.3, 0.3, 0.3, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	
	//generateTex1D();
	
	reloadShaders();
	
	return;
}

void Viewer::reloadShaders()
{
	makeCurrent();
	QFile fileF("colorMapShader.frag");
	fileF.open(QIODevice::ReadOnly);
	QByteArray arrF = fileF.readAll();
	const char* dataF = arrF.data();
	const int sizeF = arrF.size();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	QFile fileV("colorMapShader.vert");
	fileV.open(QIODevice::ReadOnly);
	QByteArray arrV = fileV.readAll();
	const char* dataV = arrV.data();
	const int sizeV = arrV.size();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	makeCurrent();
	glShaderSource(fragmentShader, 1, &dataF, &sizeF);
	glCompileShader(fragmentShader);
	GLint compiledF;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledF);

	//qDebug()<< compiledF;
	if(compiledF != 1)
	{
		qDebug()<< "colorMapShader.frag compiled incorrectly!";
	}
	
	glShaderSource(vertexShader, 1, &dataV, &sizeV);
	glCompileShader(vertexShader);
	GLint compiledV;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledV);

	//qDebug()<< compiledV;
	makeCurrent();
	program = glCreateProgram();
	
	//glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	makeCurrent();
	glLinkProgram(program);
	glUseProgram(program);
	
	GLint texture;
	GLint colorTexture;
	texture = glGetUniformLocation(program, "my_tex");
	colorTexture = glGetUniformLocation(program, "my_coltex");
	
	//qDebug() << texture << " " << colorTexture;
	glUniform1i(texture, 1);
	glUniform1i(colorTexture, 0);
	
	QFile fileFIso("isoSurfaceShader.frag");
	fileFIso.open(QIODevice::ReadOnly);
	QByteArray arrFIso = fileFIso.readAll();
	const char* dataFIso = arrFIso.data();
	const int sizeFIso = arrFIso.size();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	makeCurrent();
	glShaderSource(fragmentShader, 1, &dataFIso, &sizeFIso);
	glCompileShader(fragmentShader);
	GLint compiledFIso;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledFIso);
	
	if(compiledFIso != 1)
	{
		qDebug() << "isoSurfaceShader.frag compiled incorrectly!";
	}
	
	QFile fileVIso("isoSurfaceShader.vert");
	fileVIso.open(QIODevice::ReadOnly);
	QByteArray arrVIso = fileVIso.readAll();
	const char* dataVIso = arrVIso.data();
	const int sizeVIso = arrVIso.size();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	makeCurrent();
	glShaderSource(vertexShader, 1, &dataVIso, &sizeVIso);
	glCompileShader(vertexShader);
	GLint compiledVIso;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledVIso);
	
	if(compiledVIso != 1)
	{
		qDebug() << "isoSurfaceShader.vert compiled incorrectly!";
	}
	
	program2 = glCreateProgram();
	glAttachShader(program2, vertexShader);
	glAttachShader(program2, fragmentShader);
	glLinkProgram(program2);
	glUseProgram(program2);
	
	
	texture = glGetUniformLocation(program2, "my_tex");
	colorTexture = glGetUniformLocation(program2, "my_coltex");
	glUniform1i(texture, 1);
	glUniform1i(colorTexture, 0);
	
	GLint dimensions;
	dimensions = glGetUniformLocation(program2, "dimensions");
	glUniform3f(dimensions, sizex, sizey, sizez);
	
	GLint isoValue;
	GLint maxSizeShader;
	isoValue = glGetUniformLocation(program2, "isoValue");
	maxSizeShader = glGetUniformLocation(program2, "maxSize");
	glUniform1f(isoValue, 0.0);
	glUniform1f(maxSizeShader, size);
	
	glUseProgram(program);
	
	QFile fileFVol("volumeSurfaceShader.frag");
	fileFVol.open(QIODevice::ReadOnly);
	QByteArray arrFVol = fileFVol.readAll();
	const char* dataFVol = arrFVol.data();
	const int sizeFVol = arrFVol.size();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	makeCurrent();
	glShaderSource(fragmentShader, 1, &dataFVol, &sizeFVol);
	glCompileShader(fragmentShader);
	GLint compiledFVol;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledFVol);
	
	if(compiledFVol != 1)
	{
		qDebug() << "volumeSurfaceShader.frag compiled incorrectly!";
	}
	
	QFile fileVVol("volumeSurfaceShader.vert");
	fileVVol.open(QIODevice::ReadOnly);
	QByteArray arrVVol = fileVVol.readAll();
	const char* dataVVol = arrVVol.data();
	const int sizeVVol = arrVVol.size();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	makeCurrent();
	glShaderSource(vertexShader, 1, &dataVVol, &sizeVVol);
	glCompileShader(vertexShader);
	GLint compiledVVol;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledVVol);
	
	if(compiledVVol != 1)
	{
		qDebug() << "volumeSurfaceShader.vert compiled incorrectly!";
	}
	
	program3 = glCreateProgram();
	glAttachShader(program3, vertexShader);
	glAttachShader(program3, fragmentShader);
	glLinkProgram(program3);
	glUseProgram(program3);
	
	
	texture = glGetUniformLocation(program3, "my_tex");
	colorTexture = glGetUniformLocation(program3, "my_coltex");
	glUniform1i(texture, 1);
	glUniform1i(colorTexture, 0);
	
	GLint dimensions2;
	dimensions2 = glGetUniformLocation(program3, "dimensions");
	glUniform3f(dimensions2, sizex, sizey, sizez);
	
	GLint isoValue2;
	GLint maxSizeShader2;
	isoValue2 = glGetUniformLocation(program3, "isoValue");
	maxSizeShader2 = glGetUniformLocation(program3, "maxSize");
	glUniform1f(isoValue2, 0.0);
	glUniform1f(maxSizeShader2, size);
	
	glUseProgram(program);
	
	
	//////return
	
	QFile fileFVew("viewShader.frag");
	fileFVew.open(QIODevice::ReadOnly);
	QByteArray arrFVew = fileFVew.readAll();
	const char* dataFVew = arrFVew.data();
	const int sizeFVew = arrFVew.size();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	makeCurrent();
	glShaderSource(fragmentShader, 1, &dataFVew, &sizeFVew);
	glCompileShader(fragmentShader);
	GLint compiledFVew;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledFVew);
	
	if(compiledFVew != 1)
	{
		qDebug() << "viewShader.frag compiled incorrectly!";
	}
	
	QFile fileVVew("viewShader.vert");
	fileVVew.open(QIODevice::ReadOnly);
	QByteArray arrVVew = fileVVew.readAll();
	const char* dataVVew = arrVVew.data();
	const int sizeVVew = arrVVew.size();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	makeCurrent();
	glShaderSource(vertexShader, 1, &dataVVew, &sizeVVew);
	glCompileShader(vertexShader);
	GLint compiledVVew;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledVVew);
	
	if(compiledVVew != 1)
	{
		qDebug() << "viewShader.vert compiled incorrectly!";
	}
	
	program4 = glCreateProgram();
	glAttachShader(program4, vertexShader);
	glAttachShader(program4, fragmentShader);
	glLinkProgram(program4);
	glUseProgram(program4);
	
	
	texture = glGetUniformLocation(program4, "my_tex");
	colorTexture = glGetUniformLocation(program4, "my_coltex");
	glUniform1i(texture, 1);
	glUniform1i(colorTexture, 0);
	
	dimensions2 = glGetUniformLocation(program4, "dimensions");
	glUniform3f(dimensions2, sizex, sizey, sizez);
	
	maxSizeShader2 = glGetUniformLocation(program4, "maxSize");
	glUniform1f(maxSizeShader2, size);
	
	///lic
	
	QFile fileFLic("licShader.frag");
	fileFLic.open(QIODevice::ReadOnly);
	QByteArray arrFLic = fileFLic.readAll();
	const char* dataFLic = arrFLic.data();
	const int sizeFLic = arrFLic.size();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	makeCurrent();
	glShaderSource(fragmentShader, 1, &dataFLic, &sizeFLic);
	glCompileShader(fragmentShader);
	GLint compiledFLic;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledFLic);
	
	if(compiledFLic != 1)
	{
		qDebug() << "licShader.frag compiled incorrectly!";
	}
	
	QFile fileVLic("licShader.vert");
	fileVLic.open(QIODevice::ReadOnly);
	QByteArray arrVLic = fileVLic.readAll();
	const char* dataVLic = arrVLic.data();
	const int sizeVLic = arrVLic.size();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	makeCurrent();
	glShaderSource(vertexShader, 1, &dataVLic, &sizeVLic);
	glCompileShader(vertexShader);
	GLint compiledVLic;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledVLic);
	
	if(compiledVLic != 1)
	{
		qDebug() << "licShader.vert compiled incorrectly!";
	}
	
	program5 = glCreateProgram();
	glAttachShader(program5, vertexShader);
	glAttachShader(program5, fragmentShader);
	glLinkProgram(program5);
	glUseProgram(program5);
	
	
	texture = glGetUniformLocation(program5, "my_tex");
	colorTexture = glGetUniformLocation(program5, "my_coltex");
	glUniform1i(texture, 1);
	glUniform1i(colorTexture, 0);
	
	//dimensions2 = glGetUniformLocation(program5, "dimensions");
	//glUniform3f(dimensions2, sizex, sizey, sizez);
	
	//maxSizeShader2 = glGetUniformLocation(program5, "maxSize");
	//glUniform1f(maxSizeShader2, size);
	
	GLint noiseTexSample;
	GLint vecTexSample;
	noiseTexSample = glGetUniformLocation(program5, "my_noisetex");
	vecTexSample = glGetUniformLocation(program5, "my_vectex");
	
	glUniform1i(noiseTexSample, 3);
	glUniform1i(vecTexSample, 2);
	
	
	glUseProgram(program);
	
	
	///GEOMETRY SHADER?
	QFile fileGGly("glyphShader.geom");
	fileGGly.open(QIODevice::ReadOnly);
	QByteArray arrGGly = fileGGly.readAll();
	const char* dataGGly = arrGGly.data();
	const int sizeGGly = arrGGly.size();
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	makeCurrent();
	glShaderSource(geometryShader, 1, &dataGGly, &sizeGGly);
	glCompileShader(geometryShader);
	GLint compiledGGly;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &compiledGGly);
	
	if(compiledGGly != 1)
	{
		qDebug() << "glyphShader.geom compiled incorrectly!";
	}
	
	QFile fileFGly("glyphShader.frag");
	fileFGly.open(QIODevice::ReadOnly);
	QByteArray arrFGly = fileFGly.readAll();
	const char* dataFGly = arrFGly.data();
	const int sizeFGly = arrFGly.size();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	makeCurrent();
	glShaderSource(fragmentShader, 1, &dataFGly, &sizeFGly);
	glCompileShader(fragmentShader);
	GLint compiledFGly;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledFGly);
	
	if(compiledFGly != 1)
	{
		qDebug() << "glyphShader.frag compiled incorrectly!";
	}
	
	QFile fileVGly("glyphShader.vert");
	fileVGly.open(QIODevice::ReadOnly);
	QByteArray arrVGly = fileVGly.readAll();
	const char* dataVGly = arrVGly.data();
	const int sizeVGly = arrVGly.size();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	makeCurrent();
	glShaderSource(vertexShader, 1, &dataVGly, &sizeVGly);
	glCompileShader(vertexShader);
	GLint compiledVGly;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledVGly);
	
	if(compiledVGly != 1)
	{
		qDebug() << "glyphShader.vert compiled incorrectly!";
	}
	
	program6 = glCreateProgram();
	glAttachShader(program6, fragmentShader);
	glAttachShader(program6, vertexShader);
	glAttachShader(program6, geometryShader);
	glProgramParameteriEXT(program6,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
	glProgramParameteriEXT(program6,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINES);
	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
	glProgramParameteriEXT(program6,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

	glLinkProgram(program6);
	glUseProgram(program6);
	
	
	texture = glGetUniformLocation(program6, "my_tex");
	colorTexture = glGetUniformLocation(program6, "my_coltex");
	glUniform1i(texture, 1);
	glUniform1i(colorTexture, 0);
	
	//dimensions2 = glGetUniformLocation(program5, "dimensions");
	//glUniform3f(dimensions2, sizex, sizey, sizez);
	
	//maxSizeShader2 = glGetUniformLocation(program5, "maxSize");
	//glUniform1f(maxSizeShader2, size);
	
// 	noiseTexSample = glGetUniformLocation(program6, "my_noisetex");
// 	vecTexSample = glGetUniformLocation(program6, "my_vectex");
// 	
// 	glUniform1i(noiseTexSample, 3);
// 	glUniform1i(vecTexSample, 2);
// 	
	
	glUseProgram(program);
	
	return;
}

void Viewer::resizeGL(int w, int h) // sets gl up
{
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, float(w)/float(h), 1, size * 10);
    glMatrixMode(GL_MODELVIEW);

    return;
}

void Viewer::drawSlices()
{
	//glPushMatrix();
	//glMatrixMode(GL_TEXTURE);
	//glTranslatef(-(sizex/float(2)),-(sizey/float(2)), -(sizez/float(2)));
	//glEnable(GL_TEXTURE_3D);
	drawnView = 0;
	ColorMapItr* itr;
	float value;
	MapNode* temp;
	if(smap == 0)
	{
		return;
	}
	itr = smap -> getIter();
	itr -> reset();
	while(!itr->end())
	{
		//only for z aligned slizes!
		temp = itr->getCursor();
		if(temp -> getBlue()  && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(0,0, value);
			glBegin(GL_QUADS);
				glColor3f(0, 0, 1);
				glTexCoord3d(0, 0, 0);
				glVertex3f(0, 0, value * sizez);
				glTexCoord3d(1, 0, 0);
				glVertex3f(sizex, 0, value * sizez);
				glTexCoord3d(1, 1, 0);
				glVertex3f(sizex, sizey, value * sizez);
				glTexCoord3d(0, 1, 0);
				glVertex3f(0, sizey, value * sizez);
			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}

	
	itr->reset();
	
	while(!itr->end())
	{
		//only for y aligned slizes!
		temp = itr->getCursor();
		if(temp -> getGreen() && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(.5, .5, .5);
			glRotatef(90, 1, 0, 0);
			glTranslatef(-.5, -.5, -.5);
			glTranslatef(0,0, 1-value);
			glBegin(GL_QUADS);
				glColor3f(0, 0, 1);
				glTexCoord3d(0, 0, 0);
				glVertex3f(0, value * sizey, 0);
				glTexCoord3d(1, 0, 0);
				glVertex3f(sizex, value * sizey, 0);
				glTexCoord3d(1, 1, 0);
				glVertex3f(sizex, value * sizey, sizez);
				glTexCoord3d(0, 1, 0);
				glVertex3f(0, value * sizey, sizez);
			glEnd();
			glRotatef(-90, 1, 0, 0);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}
	
	itr->reset();
	while(!itr->end())
	{
		//only for x aligned slizes!
		temp = itr->getCursor();
		if(temp -> getRed()  && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(.5, .5, .5);
			glRotatef(90, 0, 1, 0);
			glRotatef(180,0,1,0);
			glTranslatef(-.5, -.5, -.5);
			glTranslatef(0,0, 1-value);
			glBegin(GL_QUADS);
				glColor3f(0, 0, 1);
				glTexCoord3d(0, 0, 0);
				glVertex3f(value * sizex, 0, 0);
				glTexCoord3d(1, 0, 0);
				glVertex3f(value *sizex, 0, sizez);
				glTexCoord3d(1, 1, 0);
				glVertex3f(value * sizex, sizey, sizez);
				glTexCoord3d(0, 1, 0);
				glVertex3f(value * sizex, sizey, 0);
			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}
	return;
	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void Viewer::drawViewSlice() // paints gl
{
	ColorMapItr* itr;
	float value;
	MapNode* temp;
	if(smap == 0)
	{
		return;
	}
	itr = smap -> getIter();
	itr -> reset();
	
	glUseProgram(program4);
	GLint viewDistance;
	viewDistance = glGetUniformLocation(program4, "planeDistance");
	
	while(!itr->end())
	{
		//only for view aligned slizes!
		temp = itr->getCursor();
		if(!temp -> getRed() && !temp -> getBlue() && !temp -> getGreen()  && temp -> getAlpha())
		{
			if(drawnView != 1)
			{
				value = temp -> getValue();
				value = value / float(256);
				
				
				glUniform1f(viewDistance, value);
				
				//side 1 z far
				glBegin(GL_QUADS);
					glColor3f(0,0,1);
					glTexCoord3d(0,0,1);
					glVertex3f(0,0,0);
					
					glTexCoord3d(0,1,1);
					glVertex3f(0,sizex,0);
					
					glTexCoord3d(1,1,1);
					glVertex3f(sizex,sizey,0);
					
					glTexCoord3d(1,0,1);
					glVertex3f(sizex,0,0);
					
				glEnd();
				
				//side 2 z near
				glBegin(GL_QUADS);
					glColor3f(0,1,1);
					glTexCoord3d(0,0,0);
					glVertex3f(0,0,sizez);
					
					glTexCoord3d(0,1,0);
					glVertex3f(0,sizex,sizez);
					
					glTexCoord3d(1,1,0);
					glVertex3f(sizex,sizey,sizez);
					
					glTexCoord3d(1,0,0);
					glVertex3f(sizex,0,sizez);
					
				glEnd();
				
				//side 3 x left
				glBegin(GL_QUADS);
					glColor3f(1,0,0);
					glTexCoord3d(0,0,0);
					glVertex3f(0,0,0);
					
					glTexCoord3d(0,0,1);
					glVertex3f(0,0,sizez);
					
					glTexCoord3d(0,1,1);
					glVertex3f(0,sizey,sizez);
					
					glTexCoord3d(0,1,0);
					glVertex3f(0,sizey,0);
					
				glEnd();
				
				//side 4 x right
				glBegin(GL_QUADS);
					glColor3f(1,1,0);
					glTexCoord3d(1,0,0);
					glVertex3f(sizex,0,0);
					
					glTexCoord3d(1,0,1);
					glVertex3f(sizex,0,sizez);
					
					glTexCoord3d(1,1,1);
					glVertex3f(sizex,sizey,sizez);
					
					glTexCoord3d(1,1,0);
					glVertex3f(sizex,sizey,0);
					
				glEnd();
				
				//side 5 y bottom
				glBegin(GL_QUADS);
					glColor3f(1,1,1);
					glTexCoord3d(0,0,0);
					glVertex3f(0,0,0);
					
					glTexCoord3d(0,0,1);
					glVertex3f(0,0,sizez);
					
					glTexCoord3d(1,0,1);
					glVertex3f(sizex,0,sizez);
					
					glTexCoord3d(1,0,0);
					glVertex3f(sizex,0,0);
					
				glEnd();
				
				glBegin(GL_QUADS);
					glColor3f(.5,.5,.5);
					glTexCoord3d(0,1,0);
					glVertex3f(0,sizey,0);
					
					glTexCoord3d(0,1,1);
					glVertex3f(0,sizey,sizez);
					
					glTexCoord3d(1,1,1);
					glVertex3f(sizex,sizey,sizez);
					
					glTexCoord3d(1,1,0);
					glVertex3f(sizex,sizey,0);
					
				glEnd();
				
				/*glMatrixMode(GL_TEXTURE);
				glPushMatrix();
				
				//texball->arcball_setzoom(1, vec(0,0,zoomAmount), vec(0,1,0));
				
				glTranslatef(.5,.5,.5);
				//glRotatef(180, 1, 0, 0);
				//glRotatef(180, 0, 1, 0);
				//glRotatef(180, 0, 0, 1);
				glMultMatrixf(texball->arcball_rotate());
				//glRotatef(-180, 0, 0, 1);
				//glRotatef(-180, 0, 1, 0);
				//glRotatef(-180, 1, 0, 0);
				
				//qDebug()<< arcball_inv_rotate();
				glTranslatef(-0.5, -0.5, -0.5);
				//glRotatef(yrot, 1, 0, 0);
				//glRotatef(-xrot, 0, 1, 0);*/
				
				/*glBegin(GL_QUADS);
					glColor3f(0, 1, 1);
					
					glTexCoord3d(-.5, -.5, 1-((((zoomAmount-size)-temp -> getValue())/sizez)+.5));
					glVertex3f(-sizex, -sizey, (zoomAmount-size)-temp -> getValue());
					
					glTexCoord3d(-.5, 1.5, 1-((((zoomAmount-size)-temp -> getValue())/sizez)+.5));
					glVertex3f(-sizex, sizey, (zoomAmount-size)-temp -> getValue());
					
					glTexCoord3d(1.5, 1.5, 1-((((zoomAmount-size)-temp -> getValue())/sizez)+.5));
					glVertex3f(sizex,sizey, (zoomAmount-size)-temp -> getValue());
					
					glTexCoord3d(1.5, -.5, 1-((((zoomAmount-size)-temp -> getValue())/sizez)+.5));
					glVertex3f(sizex , -sizey, (zoomAmount-size)-temp -> getValue());
				glEnd();*/
				
				//glPopMatrix();
				//glMatrixMode(GL_MODELVIEW);
				
				drawnView = 1;
			}
		}
		itr->advance();
	}
	
}

void Viewer::drawLicSurfaces()
{
	//glPushMatrix();
	//glMatrixMode(GL_TEXTURE);
	//glTranslatef(-(sizex/float(2)),-(sizey/float(2)), -(sizez/float(2)));
	//glEnable(GL_TEXTURE_3D);
	drawnView = 0;
	ColorMapItr* itr;
	float value;
	MapNode* temp;
	if(lmap == 0)
	{
		return;
	}
	glUseProgram(program5);
	itr = lmap -> getIter();
	itr -> reset();
	while(!itr->end())
	{
		//only for z aligned slizes!
		temp = itr->getCursor();
		if(temp -> getBlue()  && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(0,0, value);
			glBegin(GL_QUADS);
				glColor3f(0, 0, 1);
				glTexCoord3d(0, 0, 0);
				glVertex3f(0, 0, value * sizez);
				glTexCoord3d(1, 0, 0);
				glVertex3f(sizex, 0, value * sizez);
				glTexCoord3d(1, 1, 0);
				glVertex3f(sizex, sizey, value * sizez);
				glTexCoord3d(0, 1, 0);
				glVertex3f(0, sizey, value * sizez);
			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}

	
	itr->reset();
	
	while(!itr->end())
	{
		//only for y aligned slizes!
		temp = itr->getCursor();
		if(temp -> getGreen() && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(.5, .5, .5);
			glRotatef(90, 1, 0, 0);
			glTranslatef(-.5, -.5, -.5);
			glTranslatef(0,0, 1-value);
			glBegin(GL_QUADS);
				glColor3f(0, 0, 1);
				glTexCoord3d(0, 0, 0);
				glVertex3f(0, value * sizey, 0);
				glTexCoord3d(1, 0, 0);
				glVertex3f(sizex, value * sizey, 0);
				glTexCoord3d(1, 1, 0);
				glVertex3f(sizex, value * sizey, sizez);
				glTexCoord3d(0, 1, 0);
				glVertex3f(0, value * sizey, sizez);
			glEnd();
			glRotatef(-90, 1, 0, 0);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}
	
	itr->reset();
	while(!itr->end())
	{
		//only for x aligned slizes!
		temp = itr->getCursor();
		if(temp -> getRed()  && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(.5, .5, .5);
			glRotatef(90, 0, 1, 0);
			glRotatef(180,0,1,0);
			glTranslatef(-.5, -.5, -.5);
			glTranslatef(0,0, 1-value);
			glBegin(GL_QUADS);
				glColor3f(0, 0, 1);
				glTexCoord3d(0, 0, 0);
				glVertex3f(value * sizex, 0, 0);
				glTexCoord3d(1, 0, 0);
				glVertex3f(value *sizex, 0, sizez);
				glTexCoord3d(1, 1, 0);
				glVertex3f(value * sizex, sizey, sizez);
				glTexCoord3d(0, 1, 0);
				glVertex3f(value * sizex, sizey, 0);
			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}
	return;
	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void Viewer::drawGlyphSurfaces()
{
	//glPushMatrix();
	//glMatrixMode(GL_TEXTURE);
	//glTranslatef(-(sizex/float(2)),-(sizey/float(2)), -(sizez/float(2)));
	//glEnable(GL_TEXTURE_3D);
	drawnView = 0;
	ColorMapItr* itr;
	float value;
	MapNode* temp;
	if(gmap == 0)
	{
		return;
	}
	glUseProgram(0);
	itr = gmap -> getIter();
	itr -> reset();
	while(!itr->end())
	{
		//only for z aligned slizes!
		temp = itr->getCursor();
		if(temp -> getBlue()  && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(0,0, value);
			//qDebug() << "got a glyph z slice" << endl;
			drawGlyphZ(value * sizez);
// 			glBegin(GL_LINES);
// 				glColor3f(0, 0, 1);
// // 				glTexCoord3d(0, 0, 0);
// 				glVertex3f(0, 0, value * sizez);
// // 				glTexCoord3d(1, 0, 0);
// // 				glVertex3f(sizex, 0, value * sizez);
// // 				glTexCoord3d(1, 1, 0);
// // 				glVertex3f(sizex, sizey, value * sizez);
// // 				glTexCoord3d(0, 1, 0);
// 				glVertex3f(0, sizey, value * sizez);
// 			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}

	
	itr->reset();
	
	while(!itr->end())
	{
		//only for y aligned slizes!
		temp = itr->getCursor();
		if(temp -> getGreen() && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(.5, .5, .5);
			glRotatef(90, 1, 0, 0);
			glTranslatef(-.5, -.5, -.5);
			glTranslatef(0,0, 1-value);
			drawGlyphY(value * sizey);
// 			glBegin(GL_LINES);
// 				glColor3f(0, 0, 1);
// // 				glTexCoord3d(0, 0, 0);
// 				glVertex3f(0, value * sizey, 0);
// // 				glTexCoord3d(1, 0, 0);
// // 				glVertex3f(sizex, value * sizey, 0);
// // 				glTexCoord3d(1, 1, 0);
// // 				glVertex3f(sizex, value * sizey, sizez);
// // 				glTexCoord3d(0, 1, 0);
// 				glVertex3f(0, value * sizey, sizez);
// 			glEnd();
			glRotatef(-90, 1, 0, 0);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}
	
	itr->reset();
	while(!itr->end())
	{
		//only for x aligned slizes!
		temp = itr->getCursor();
		if(temp -> getRed()  && temp -> getAlpha())
		{
			value = (float(temp->getValue()))/float(256);
			
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glTranslatef(.5, .5, .5);
			glRotatef(90, 0, 1, 0);
			glRotatef(180,0,1,0);
			glTranslatef(-.5, -.5, -.5);
			glTranslatef(0,0, 1-value);
			drawGlyphX(value * sizex);
// 			glBegin(GL_LINES);
// 				glColor3f(0, 0, 1);
// // // 				glTexCoord3d(0, 0, 0);
// 				glVertex3f(value * sizex, 0, 0);
// // 				glTexCoord3d(1, 0, 0);
// // 				glVertex3f(value *sizex, 0, sizez);
// // 				glTexCoord3d(1, 1, 0);
// // 				glVertex3f(value * sizex, sizey, sizez);
// // 				glTexCoord3d(0, 1, 0);
// 				glVertex3f(value * sizex, sizey, 0);
// 			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		itr->advance();
	}
	return;
	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void Viewer::drawGlyphX(float xIn)
{
	float zInc = sizez/11.0;
	float yInc = sizey/11.0;
	float zValue = 0;
	float yValue = 0;
	
	for(int i = 0; i < 10; i++)
	{
		zValue += zInc;
		yValue = 0;
		for(int j = 0; j < 10; j++)
		{
			yValue += yInc;
			//draw glyph at xIn, yValue, zValue
			glBegin(GL_LINES);
				glColor3f(0.8, 0.8, 0.8);
				glVertex3f(xIn, yValue, zValue);
				triTerpolate(xIn, yValue, zValue);
				glVertex3f(xIn + (currentVecX * size), yValue + (currentVecY * size), zValue + (currentVecZ * size));
				glVertex3f(xIn + (currentVecX * size), yValue + (currentVecY * size), zValue + (currentVecZ * size));
				glVertex3f(xIn + (currentVecX * size * 0.8), (yValue + yInc/6.0) + (currentVecY * size * 0.8), (zValue + zInc/6.0) + (currentVecZ * size * 0.8));
			glEnd();
		}
	}
}

void Viewer::drawGlyphY(float yIn)
{
	float zInc = sizez/11.0;
	float xInc = sizex/11.0;
	float zValue = 0;
	float xValue = 0;
	
	for(int i = 0; i < 10; i++)
	{
		zValue += zInc;
		xValue = 0;
		for(int j = 0; j < 10; j++)
		{
			xValue += xInc;
			//draw glyph at xValue, yIn, zValue
			glBegin(GL_LINES);
				glColor3f(0.8, 0.8, 0.8);
				glVertex3f(xValue, yIn, zValue);
				triTerpolate(xValue, yIn, zValue);
				glVertex3f(xValue + (currentVecX * size), yIn + (currentVecY * size), zValue + (currentVecZ * size));
				glVertex3f(xValue + (currentVecX * size), yIn + (currentVecY * size), zValue + (currentVecZ * size));
				glVertex3f((xValue + xInc/6.0) + (currentVecX * size * 0.8), yIn + (currentVecY * size * 0.8), (zValue + zInc/6.0) + (currentVecZ * size * 0.8));
			glEnd();
		}
	}
}

void Viewer::drawGlyphZ(float zIn)
{
	float xInc = sizex/11.0;
	float yInc = sizey/11.0;
	float xValue = 0;
	float yValue = 0;
	//qDebug() << "drawing Z glyphs at Z:" << zIn << endl;
	for(int i = 0; i < 10; i++)
	{
		xValue += xInc;
		yValue = 0;
		for(int j = 0; j < 10; j++)
		{
			yValue += yInc;
			//draw glyph at xValue, yValue, zIn
			glBegin(GL_LINES);
				glColor3f(0.8, 0.8, 0.8);
				glVertex3f(xValue, yValue, zIn);
				triTerpolate(xValue, yValue, zIn);
				glVertex3f(xValue + (currentVecX * size), yValue + (currentVecY * size), zIn + (currentVecZ * size));
				//draw head of arrow
				glVertex3f(xValue + (currentVecX * size), yValue + (currentVecY * size), zIn + (currentVecZ * size));
				glVertex3f((xValue + xInc/6.0) + (currentVecX * size * 0.8), (yValue + yInc/6.0) + (currentVecY * size * 0.8), zIn + (currentVecZ * size * 0.8));
				//glVertex3f(0,0,0);
			glEnd();
			//qDebug() << "drawing arrow start at: " << xValue << " " << yValue << " " << zIn;
			//qDebug() << "drawing arrow end at: " << xValue + currentVecX << " " << yValue + currentVecY << " " <<  zIn + currentVecZ;
		}
	}
}

void Viewer::drawIsoSurfaces()
{
	//glPushMatrix();
	//glMatrixMode(GL_TEXTURE);
	//glTranslatef(-(sizex/float(2)),-(sizey/float(2)), -(sizez/float(2)));
	//glEnable(GL_TEXTURE_3D);
	drawnView = 0;
	ColorMapItr* itr;
	float value;
	MapNode* temp;
	if(imap == 0)
	{
		return;
	}
	itr = imap -> getIter();
	itr -> reset();
	glUseProgram(program2);
	GLint isoValue;
	
	GLint detailLevel;
	detailLevel = glGetUniformLocation(program2, "detail");
	glUniform1f(detailLevel, isoDetail);
	
	while(!itr->end())
	{
		temp = itr->getCursor();
		value = temp->getValue()/256.0;
		
		
		isoValue = glGetUniformLocation(program2, "isoValue");
		glUniform1f(isoValue, value);
		
		
		//side 1 z far
		glBegin(GL_QUADS);
			glColor3f(0,0,1);
			glTexCoord3d(0,0,1);
			glVertex3f(0,0,0);
			
			glTexCoord3d(0,1,1);
			glVertex3f(0,sizex,0);
			
			glTexCoord3d(1,1,1);
			glVertex3f(sizex,sizey,0);
			
			glTexCoord3d(1,0,1);
			glVertex3f(sizex,0,0);
			
		glEnd();
		
		//side 2 z near
		glBegin(GL_QUADS);
			glColor3f(0,1,1);
			glTexCoord3d(0,0,0);
			glVertex3f(0,0,sizez);
			
			glTexCoord3d(0,1,0);
			glVertex3f(0,sizex,sizez);
			
			glTexCoord3d(1,1,0);
			glVertex3f(sizex,sizey,sizez);
			
			glTexCoord3d(1,0,0);
			glVertex3f(sizex,0,sizez);
			
		glEnd();
		
		//side 3 x left
		glBegin(GL_QUADS);
			glColor3f(1,0,0);
			glTexCoord3d(0,0,0);
			glVertex3f(0,0,0);
			
			glTexCoord3d(0,0,1);
			glVertex3f(0,0,sizez);
			
			glTexCoord3d(0,1,1);
			glVertex3f(0,sizey,sizez);
			
			glTexCoord3d(0,1,0);
			glVertex3f(0,sizey,0);
			
		glEnd();
		
		//side 4 x right
		glBegin(GL_QUADS);
			glColor3f(1,1,0);
			glTexCoord3d(1,0,0);
			glVertex3f(sizex,0,0);
			
			glTexCoord3d(1,0,1);
			glVertex3f(sizex,0,sizez);
			
			glTexCoord3d(1,1,1);
			glVertex3f(sizex,sizey,sizez);
			
			glTexCoord3d(1,1,0);
			glVertex3f(sizex,sizey,0);
			
		glEnd();
		
		//side 5 y bottom
		glBegin(GL_QUADS);
			glColor3f(1,1,1);
			glTexCoord3d(0,0,0);
			glVertex3f(0,0,0);
			
			glTexCoord3d(0,0,1);
			glVertex3f(0,0,sizez);
			
			glTexCoord3d(1,0,1);
			glVertex3f(sizex,0,sizez);
			
			glTexCoord3d(1,0,0);
			glVertex3f(sizex,0,0);
			
		glEnd();
		
		glBegin(GL_QUADS);
			glColor3f(.5,.5,.5);
			glTexCoord3d(0,1,0);
			glVertex3f(0,sizey,0);
			
			glTexCoord3d(0,1,1);
			glVertex3f(0,sizey,sizez);
			
			glTexCoord3d(1,1,1);
			glVertex3f(sizex,sizey,sizez);
			
			glTexCoord3d(1,1,0);
			glVertex3f(sizex,sizey,0);
			
		glEnd();
		itr->advance();
	}
	return;
	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void Viewer::drawVolSurfaces()
{
	//glPushMatrix();
	//glMatrixMode(GL_TEXTURE);
	//glTranslatef(-(sizex/float(2)),-(sizey/float(2)), -(sizez/float(2)));
	//glEnable(GL_TEXTURE_3D);
	drawnView = 0;
	ColorMapItr* itr;
	float value;
	MapNode* temp;
	if(imap == 0)
	{
		return;
	}
	itr = imap -> getIter();
	itr -> reset();
	glUseProgram(program3);
	GLint alphaLevel;
	GLint cutaway;
	cutaway = glGetUniformLocation(program3, "viewPlane");
	glUniform1i(cutaway, cuts);
	alphaLevel = glGetUniformLocation(program3, "alphaCorrect");
	glUniform1f(alphaLevel, alphaDetail);
	GLint detailLevel;
	detailLevel = glGetUniformLocation(program3, "detail");
	glUniform1f(detailLevel, isoDetail);
	//qDebug() << alphaDetail << endl;
	GLint combine;
	combine = glGetUniformLocation(program3, "drawIso");
	glUniform1f(combine, shouldCombine);
	
	while(!itr->end())
	{
		temp = itr->getCursor();
		value = temp->getValue()/256.0;
		GLint isoValue;
		isoValue = glGetUniformLocation(program3, "isoValue");
		glUniform1f(isoValue, value);
		
		//side 1 z far
		glBegin(GL_QUADS);
			glColor3f(0,0,1);
			glTexCoord3d(0,0,1);
			glVertex3f(0,0,0);
			
			glTexCoord3d(0,1,1);
			glVertex3f(0,sizex,0);
			
			glTexCoord3d(1,1,1);
			glVertex3f(sizex,sizey,0);
			
			glTexCoord3d(1,0,1);
			glVertex3f(sizex,0,0);
			
		glEnd();
		
		//side 2 z near
		glBegin(GL_QUADS);
			glColor3f(0,1,1);
			glTexCoord3d(0,0,0);
			glVertex3f(0,0,sizez);
			
			glTexCoord3d(0,1,0);
			glVertex3f(0,sizex,sizez);
			
			glTexCoord3d(1,1,0);
			glVertex3f(sizex,sizey,sizez);
			
			glTexCoord3d(1,0,0);
			glVertex3f(sizex,0,sizez);
			
		glEnd();
		
		//side 3 x left
		glBegin(GL_QUADS);
			glColor3f(1,0,0);
			glTexCoord3d(0,0,0);
			glVertex3f(0,0,0);
			
			glTexCoord3d(0,0,1);
			glVertex3f(0,0,sizez);
			
			glTexCoord3d(0,1,1);
			glVertex3f(0,sizey,sizez);
			
			glTexCoord3d(0,1,0);
			glVertex3f(0,sizey,0);
			
		glEnd();
		
		//side 4 x right
		glBegin(GL_QUADS);
			glColor3f(1,1,0);
			glTexCoord3d(1,0,0);
			glVertex3f(sizex,0,0);
			
			glTexCoord3d(1,0,1);
			glVertex3f(sizex,0,sizez);
			
			glTexCoord3d(1,1,1);
			glVertex3f(sizex,sizey,sizez);
			
			glTexCoord3d(1,1,0);
			glVertex3f(sizex,sizey,0);
			
		glEnd();
		
		//side 5 y bottom
		glBegin(GL_QUADS);
			glColor3f(1,1,1);
			glTexCoord3d(0,0,0);
			glVertex3f(0,0,0);
			
			glTexCoord3d(0,0,1);
			glVertex3f(0,0,sizez);
			
			glTexCoord3d(1,0,1);
			glVertex3f(sizex,0,sizez);
			
			glTexCoord3d(1,0,0);
			glVertex3f(sizex,0,0);
			
		glEnd();
		
		glBegin(GL_QUADS);
			glColor3f(.5,.5,.5);
			glTexCoord3d(0,1,0);
			glVertex3f(0,sizey,0);
			
			glTexCoord3d(0,1,1);
			glVertex3f(0,sizey,sizez);
			
			glTexCoord3d(1,1,1);
			glVertex3f(sizex,sizey,sizez);
			
			glTexCoord3d(1,1,0);
			glVertex3f(sizex,sizey,0);
			
		glEnd();
		itr->advance();
	}
	return;
	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void Viewer::paintGL() // paints gl
{
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, zoomAmount, 0, 0, 0, 0, 1, 0);
	texball->arcball_setzoom(1, vec(0,0,zoomAmount), vec(0,1,0));
	mainball->arcball_setzoom(1, vec(0,0,zoomAmount), vec(0,1,0));
	glTranslatef(xOff, yOff, 0);
	
	glPushMatrix();
	glMultMatrixf(mainball->arcball_rotate());
	
	glTranslatef(-(sizex/float(2)),-(sizey/float(2)), -(sizez/float(2)));
	
	const double clip_plane1[4] = {0.0,1.0,0.0, 1.0};
	glClipPlane(GL_CLIP_PLANE0, clip_plane1);
	glEnable(GL_CLIP_PLANE0);
	const double clip_plane2[4] = {0.0,-1.0,0.0, sizey+1};
	glClipPlane(GL_CLIP_PLANE1, clip_plane2);
	glEnable(GL_CLIP_PLANE1);
	
	const double clip_plane3[4] = {1.0,0.0,0.0, 1.0};
	glClipPlane(GL_CLIP_PLANE2, clip_plane3);
	glEnable(GL_CLIP_PLANE2);
	
	const double clip_plane4[4] = {-1.0,0.0,0.0, sizex+1};
	glClipPlane(GL_CLIP_PLANE3, clip_plane4);
	glEnable(GL_CLIP_PLANE3);
	
	const double clip_plane5[4] = {0.0,0.0,1.0, 1.0};
	glClipPlane(GL_CLIP_PLANE4, clip_plane5);
	glEnable(GL_CLIP_PLANE4);
	
	const double clip_plane6[4] = {0.0,0.0,-1.0, sizez+1};
	glClipPlane(GL_CLIP_PLANE5, clip_plane6);
	glEnable(GL_CLIP_PLANE5);
	
	drawBox();
	
	if(drawSlicesToggle == 1)
	{
		glUseProgram(program);
		drawSlices();
		drawViewSlice();
		drawLicSurfaces();
		drawGlyphSurfaces();
	}
	
	if(drawIsos == 1 && drawVols == 1)
	{
		shouldCombine = 1;
		glUseProgram(program3);
		drawVolSurfaces();
	}
	else
	{
		shouldCombine = 0;
		if(drawIsos == 1)
		{
			glUseProgram(program2);
			drawIsoSurfaces();
		}
		
		if(drawVols == 1)
		{
			glUseProgram(program3);
			drawVolSurfaces();
		}
	}
	
	glPopMatrix();
	//glTranslatef(-(sizex),-(sizey), -(sizez));
	//glTranslatef(-(sizex/float(2)),-(sizey/float(2)), -(sizez/float(2)));
	
	//glEnable(GL_TEXTURE_3D);
	//arcball_rotate();
	
	//glTranslatef(0, 0, zoomAmount - size/float(2));
	
	glUseProgram(program);
	
	
	
	return;
}

void Viewer::drawBox()
{
	glUseProgram(0);
	
	glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex3f(0,0,0);
		glVertex3f(0,sizey,0);
		
		glColor3f(1, 0, 0);
		glVertex3f(0,0,0);
		glVertex3f(sizex,0,0);
		
		glColor3f(0, 0, 1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,sizez);
		
		glColor3f(1, 1, 1);
		glVertex3f(sizex,sizey,sizez);
		glVertex3f(sizex,0,sizez);
		
		glVertex3f(sizex,sizey,sizez);
		glVertex3f(sizex,sizey,0);
		
		glVertex3f(sizex,sizey,sizez);
		glVertex3f(0,sizey,sizez);
		
		glVertex3f(0,sizey,0);
		glVertex3f(sizex,sizey,0);
		
		glVertex3f(0,sizey,0);
		glVertex3f(0,sizey,sizez);
		
		glVertex3f(sizex,0,sizez);
		glVertex3f(0,0,sizez);
		
		glVertex3f(sizex,0,sizez);
		glVertex3f(sizex,0,0);
		
		glVertex3f(0,0,sizez);
		glVertex3f(0,sizey,sizez);
		
		glVertex3f(sizex,0,0);
		glVertex3f(sizex,sizey,0);
		
		
	glEnd();
}

QSize Viewer::minimumSizeHint() const
{
     return QSize(50, 50);
}

QSize Viewer::sizeHint() const
{
     return QSize(600, 600);
}

void Viewer::readFile() // reads a file
{
	makeCurrent();
	//emit(killOtherGl());
	
	xrot = 0;
	yrot = 0;
	ColorMapItr* itr;
	MapNode* temp;
	
	itr = smap -> getIter();
	itr->reset();
	
	while(!itr->end())
	{
		temp = itr->getCursor();
		itr->advance();
		temp -> die();
	}
	
	QFileDialog filediag(this, tr("Open File"));
	filediag.setNameFilter("All Data Files (*.hdr)");
	
	QString filename = filediag.getOpenFileName(this, tr("Open File"), "./","All Data Files (*.hdr)");
	//QString filename = QFileDialog::getOpenFileName(this, tr("Open File"));
	
	
	if(filename != 0 && (filename.contains(".dat") || filename.contains("hdr")))
	{
		
		QString line; //current line and file name
		float maxValue;
		float minValue;
		int isVector = 0;
		int xDim, yDim, zDim;
		QStringList coordList; // list of floats
		int typeChoice = 0;
		quint8 currentNumberByte;
		qint16 currentNumberShort;
		qint32 currentNumberInt;
		//float currentNumberFloat;
		double currentNumberDouble;
		float currentNumber;
		float X;
		float Y;
		float Z;
		/*quint8 currentNumberByte;
		qint16 currentNumberShort;
		qint32 currentNumberInt;
		float currentNumberFloat;
		double currentNumberDouble;
		float currentNumber;*/
		QFile* theSource;// file pointer so i can handle X number of files
		theSource = 0; // set file pointer to null
		filename.chop(4);
		
		filename += ".hdr";
		theSource = new QFile(filename);
		theSource -> open(QIODevice::ReadOnly | QIODevice::Text);// | QIODevice::Text);// opens file
		QTextStream stream( theSource );
		line = stream.readLine();
		coordList = line.split(" ");
		xDim = coordList.at(0).toInt();
		yDim = coordList.at(1).toInt();
		zDim = coordList.at(2).toInt();
		size = xDim;
		
		line = stream.readLine();
		
		if(line.contains("BYTE"))
		{
			typeChoice = 0;
			//we want to use uchars
		}
		else if(line.contains("FLOAT"))
		{
			typeChoice = 1;
			//we want to use floats
		}
		else if(line.contains("SHORT"))
		{
			typeChoice = 2;
			//we want to use shorts
		}
		else if(line.contains("INT"))
		{
			typeChoice = 3;
			//we want to use ints
		}
		else if(line.contains("DOUBLE"))
		{
			typeChoice = 4;
			//we want to use doubles
		}
		
		line = stream.readLine();
		
		if(line.contains("VECTOR"))
		{
			isVector = 1;
		}
		theSource -> close();
		
		delete theSource;
		filename.chop(4);
		if(isVector == 1)
		{
			
			convertedData = (float*)malloc((xDim * yDim * zDim) * sizeof(float));///
			convertedVectorData = (float*)malloc((xDim * yDim * zDim * 3) * sizeof(float));
			filename += ".vec";
			theSource = new QFile(filename);
			theSource -> open(QIODevice::ReadOnly);// | QIODevice::Text);// opens file
			QDataStream in(theSource);
			//char* s = (char*)malloc(1*sizeof(float));
			
// 			for(int i = 0; i < (xDim*yDim*zDim * 3); i++)
// 			{
// 				if(typeChoice == 0)
// 				{
// 					in >> currentNumberByte;
// 					currentNumber = float(currentNumberByte);
// 				}
// 				else if(typeChoice == 1)
// 				{
// 					in.readRawData(s, sizeof(float));
// 					currentNumber = float(*s);
// 				}
// 				else if(typeChoice == 2)
// 				{
// 					in >> currentNumberShort;
// 					currentNumber = float(currentNumberShort);
// 				}
// 				else if(typeChoice == 3)
// 				{
// 					in >> currentNumberInt;
// 					currentNumber = float(currentNumberInt);
// 				}
// 				else
// 				{
// 					in >> currentNumberDouble;
// 					currentNumber = float(currentNumberDouble);
// 				}
// 				
// 				convertedVectorData[i] = currentNumber;
// 			}
			FILE *myFile;
			myFile = fopen( filename.toStdString().c_str(), "r");
			if(myFile == NULL) qDebug() << "FUUUCK";
			fread( convertedVectorData, sizeof(float), xDim*yDim*zDim*3, myFile);
			for(int i = 0; i < (xDim*yDim*zDim); i++)
			{
				X = convertedVectorData[(3*i) + 0];
				Y = convertedVectorData[(3*i) + 1];
				Z = convertedVectorData[(3*i) + 2];
				X *= 1000;
				Y *= 1000;
				Z *= 1000;
// 				qDebug() << X << Y << Z;
				currentNumber = pow((pow(X, 2)+pow(Y, 2)+pow(Z, 2)), 0.5);
// 				qDebug() << currentNumber;
				convertedData[i] = currentNumber;
// 				break;
				if(i == 0)//first run we store both into min and max of both
				{
					maxValue = currentNumber;
					minValue = currentNumber;
				}
				else
				{
					if(currentNumber > maxValue)
					{
						maxValue = currentNumber;
					}
					if(currentNumber < minValue)
					{
						minValue = currentNumber;
					}
				}
			}
			
			makeCurrent();
			//glEnable(GL_TEXTURE_3D);
			glActiveTexture(GL_TEXTURE2);
			glGenTextures(1, &texname2);
			glBindTexture(GL_TEXTURE_3D, texname2);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
			makeCurrent();
			glActiveTexture(GL_TEXTURE2);
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F_ARB, xDim, yDim, zDim, 0, GL_RGB, GL_FLOAT, convertedVectorData);
			
			vectorMap = convertedVectorData;
			
			///generate noise texture!;
			createNoiseTexture(xDim, yDim, zDim);
			
		}
		else
		{
			convertedData = (float*)malloc((xDim * yDim * zDim) * sizeof(float));
			filename += ".dat";
			theSource = new QFile(filename);
			theSource -> open(QIODevice::ReadOnly);// | QIODevice::Text);// opens file
			QDataStream in(theSource);
			char* s = (char*)malloc(1*sizeof(float));
			///load and convert
			
			for(int i = 0; i < (xDim*yDim*zDim) && !in.atEnd(); i++)
			{
				if(typeChoice == 0)
				{
					in >> currentNumberByte;
					currentNumber = float(currentNumberByte);
				}
				else if(typeChoice == 1)
				{
					//in >> currentNumberFloat;
					in.readRawData(s, sizeof(float));
					currentNumber = float(*s);
					//currentNumber = float(currentNumberFloat);
				}
				else if(typeChoice == 2)
				{
					in >> currentNumberShort;
					currentNumber = float(currentNumberShort);
				}
				else if(typeChoice == 3)
				{
					in >> currentNumberInt;
					currentNumber = float(currentNumberInt);
				}
				else
				{
					in >> currentNumberDouble;
					currentNumber = float(currentNumberDouble);
				}
				
				convertedData[i] = currentNumber;
	// 			if(currentNumber <= 0)
	// 			{
	// 				qDebug() << currentNumber << endl;
	// 			}
				if(i == 0)//first run we store both into min and max of both
				{
					maxValue = currentNumber;
					minValue = currentNumber;
				}
				else
				{
					if(currentNumber > maxValue)
					{
						maxValue = currentNumber;
					}
					if(currentNumber < minValue)
					{
						minValue = currentNumber;
					}
				}
			}
		}
		
		
		
		
		
		///normalize
		//qDebug() << maxValue << endl;
		//qDebug() << minValue << endl;
		if(minValue < 0)
		{
			minValue = fabs(minValue);
			maxValue = fabs(maxValue);
			maxValue = minValue + maxValue;
		}
		else
		{
			minValue = 0;
		}
		for(int i = 0; i < (xDim*yDim*zDim); i++)
		{
			
			
			convertedData[i] = ((convertedData[i]) + minValue)/float(maxValue);
		}
		makeCurrent();
		glActiveTexture(GL_TEXTURE1);
		//glEnable(GL_TEXTURE_3D);
		if(fileSelected == 0)
		{
			makeCurrent();
			//glEnable(GL_TEXTURE_3D);
			glGenTextures(1, &texname);
			glBindTexture(GL_TEXTURE_3D, texname);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		}
		makeCurrent();
		glActiveTexture(GL_TEXTURE1);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, xDim, yDim, zDim, 0, GL_LUMINANCE, GL_FLOAT, convertedData);
		fileSelected = 1;
		theSource -> close();
		//qDebug() << maxValue << endl;
		emit(newFileLoaded(convertedData, xDim, yDim, zDim, maxValue));
		
		delete theSource;
		
		sizex = xDim;
		sizey = yDim;
		sizez = zDim;
		
		size = xDim;
		if(yDim > xDim)
		{
			size = yDim;
		}
		if(zDim > yDim)
		{
			size = zDim;
		}
		
		maxRange = maxValue;
		//free(convertedData);
		//qDebug() << "got here yo" << maxRange << endl;
	}
	
	zoomAmount = size * 2.5;
	
	reloadShaders();
	
	updateGL();
}

void Viewer::createNoiseTexture(int x, int y, int z)
{
	float* noiseData = (float*)malloc((x * y * z) * sizeof(float));///
	for(int i = 0; i < (x*y*z); i++)
	{
		noiseData[i] = (float) rand()/RAND_MAX;
	}
	makeCurrent();
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &texname3);
	glBindTexture(GL_TEXTURE_3D, texname3);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	makeCurrent();
	glActiveTexture(GL_TEXTURE3);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, x, y, z, 0, GL_LUMINANCE, GL_FLOAT, noiseData);
}

void Viewer::triTerpolate(float x, float y, float z)
{
	float deltaX;
	float deltaY;
	float deltaZ;
	
	deltaX = x - floor(x);
	deltaY = y - floor(y);
	deltaZ = z - floor(z);
	
	float iOneX;
	float iOneY;
	float iOneZ;
	float iTwoX;
	float iTwoY;
	float iTwoZ;
	float jOneX;
	float jOneY;
	float jOneZ;
	float jTwoX;
	float jTwoY;
	float jTwoZ;
	float wOneX;
	float wOneY;
	float wOneZ;
	float wTwoX;
	float wTwoY;
	float wTwoZ;
	iOneX = (vecX(floor(x), floor(y), floor(z)) * (1.0 - deltaZ)) + (vecX(floor(x), floor(y), ceil(z)) * deltaZ);	
	iOneY = (vecY(floor(x), floor(y), floor(z)) * (1.0 - deltaZ)) + (vecY(floor(x), floor(y), ceil(z)) * deltaZ);	
	iOneZ = (vecZ(floor(x), floor(y), floor(z)) * (1.0 - deltaZ)) + (vecZ(floor(x), floor(y), ceil(z)) * deltaZ);	
	
	iTwoX = (vecX(floor(x), ceil(y), floor(z)) * (1.0 - deltaZ)) + (vecX(floor(x), ceil(y), ceil(z)) * deltaZ);	
	iTwoY = (vecY(floor(x), ceil(y), floor(z)) * (1.0 - deltaZ)) + (vecY(floor(x), ceil(y), ceil(z)) * deltaZ);	
	iTwoZ = (vecZ(floor(x), ceil(y), floor(z)) * (1.0 - deltaZ)) + (vecZ(floor(x), ceil(y), ceil(z)) * deltaZ);	
	
	jOneX = (vecX(ceil(x), floor(y), floor(z)) * (1.0 - deltaZ)) + (vecX(ceil(x), floor(y), ceil(z)) * deltaZ);	
	jOneY = (vecY(ceil(x), floor(y), floor(z)) * (1.0 - deltaZ)) + (vecY(ceil(x), floor(y), ceil(z)) * deltaZ);	
	jOneZ = (vecZ(ceil(x), floor(y), floor(z)) * (1.0 - deltaZ)) + (vecZ(ceil(x), floor(y), ceil(z)) * deltaZ);	
	
	jTwoX = (vecX(ceil(x), ceil(y), floor(z)) * (1.0 - deltaZ)) + (vecX(ceil(x), ceil(y), ceil(z)) * deltaZ);	
	jTwoY = (vecY(ceil(x), ceil(y), floor(z)) * (1.0 - deltaZ)) + (vecY(ceil(x), ceil(y), ceil(z)) * deltaZ);	
	jTwoZ = (vecZ(ceil(x), ceil(y), floor(z)) * (1.0 - deltaZ)) + (vecZ(ceil(x), ceil(y), ceil(z)) * deltaZ);
	
	wOneX = (iOneX * (1. - deltaY)) + (iTwoX * deltaY);
	wOneY = (iOneY * (1. - deltaY)) + (iTwoY * deltaY);
	wOneZ = (iOneZ * (1. - deltaY)) + (iTwoZ * deltaY);
	
	wTwoX = (jOneX * (1. - deltaY)) + (jTwoX * deltaY);
	wTwoY = (jOneY * (1. - deltaY)) + (jTwoY * deltaY);
	wTwoZ = (jOneZ * (1. - deltaY)) + (jTwoZ * deltaY);
	
	currentVecX = (wOneX * (1. - deltaX)) + (wTwoX * deltaX);
	currentVecY = (wOneY * (1. - deltaX)) + (wTwoY * deltaX);
	currentVecZ = (wOneZ * (1. - deltaX)) + (wTwoZ * deltaX);
	
	return;
}

float Viewer::vecX(int x, int y, int z)
{
	int yDimensionLink = sizex * 3;
	int zDimensionLink = sizex * sizey * 3;
	int location = (z * zDimensionLink) + (y * yDimensionLink) + (x * 3);
	return vectorMap[location];
}

float Viewer::vecY(int x, int y, int z)
{
	int yDimensionLink = sizex * 3;
	int zDimensionLink = sizex * sizey * 3;
	int location = (z * zDimensionLink) + (y * yDimensionLink) + (x * 3);
	return vectorMap[location + 1];
}

float Viewer::vecZ(int x, int y, int z)
{
	int yDimensionLink = sizex * 3;
	int zDimensionLink = sizex * sizey * 3;
	int location = (z * zDimensionLink) + (y * yDimensionLink) + (x * 3);
	return vectorMap[location + 2];
}
