#pragma once
#include <iostream>
#include <string>
#include <dirent.h>
#include <QPushButton>
#include <QDockWidget>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QTimer>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
/*!
 ImageViewer Class Definition
 This class is used to view incoming images from the UAV
 It can move forward, backward, and just play through the images
*/
class ImageViewer : public QDockWidget
{
	Q_OBJECT
	public:
		explicit ImageViewer(QWidget *parent=0);
	private:
		QPushButton* play;
		QPushButton* next;
		QPushButton* prev;
		QLabel* image;
		DIR *dirp;
		struct dirent *inFile;
		char imagePath[60];
		char nameBuffer[1000][20];
		int imageCounter;
		int curImage;
		QTimer *timer;
		bool playFlag;
	signals:

	public slots:
		//! Next image function
		void nextImg();
		//! Previous image function
		void prevImg();
		//! Play image function
		void playImg();

};
