/************************************************************************

Copyright (C) 2012 Eric Heitz (er.heitz@gmail.com). All rights reserved.

This file is part of Qtfe (Qt Transfer Function Editor).

Qtfe is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation, either version 3 of 
the License, or (at your option) any later version.

Qtfe is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Qtfe.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#include <QWidget>
#include <QPainter>
#include <QImage>

#include <Qtfe.h>

#include <math.h>

class QImageTweak : public QWidget
{
	Q_OBJECT

public:

	QImageTweak(Qtfe * editor) : editor(editor)
	{
		resize(256,256);
		QObject::connect(editor, SIGNAL(functionChanged()), this, SLOT(colorMapChanged()));
		editor->addCanals(3);
		editor->addOutputs(1);
		editor->bindCanaltoOutputR(0,0);
		editor->bindCanaltoOutputG(1,0);
		editor->bindCanaltoOutputB(2,0);		
	}

protected slots:
	void colorMapChanged(){repaint();}

protected:
	Qtfe* editor;

	virtual void paintEvent(QPaintEvent* event)
	{
		QWidget::paintEvent(event);

		QPainter painter(this);
		QImage image(size(), QImage::Format_RGB32);

		for(int j=0 ; j < height() ; ++j)
		for(int i=0 ; i < width() ; ++i)		
		{
			qreal x = i / (qreal)width();
			qreal y = j / (qreal)height();
			qreal v = 0.5 + 
				0.1*sin(-90.0*x + 50.0*y + 30.0) + 
				0.1*sin(14.0*x - 25.0*y + 10.0) + 
				0.1*sin(-2.1*x - 4.7*y + 70.5) + 
				0.1*sin(6.2*x + 70.3*y + 50.0) + 
				0.1*sin(1.4*x - 64.4*y + 90.5);

			qreal r = editor->evalf(0,v);
			qreal g = editor->evalf(1,v);
			qreal b = editor->evalf(2,v);

			unsigned int R = r * 255;
			unsigned int G = g * 255;
			unsigned int B = b * 255;
			unsigned int color = (R<<16) + (G<<8) + B;
			image.setPixel(i,j,color);
		}

		painter.drawImage(0,0,image);
	}
};