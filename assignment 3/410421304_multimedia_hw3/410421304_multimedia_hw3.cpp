// 410421304_multimedia_hw3.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include "Windows.h"



using namespace cv;
using namespace std;


int main()
{
	Mat i1in = imread("i1.pgm");
	Mat i2in = imread("i2.pgm");

	Mat i1img,i2img;

	cvtColor(i1in, i1img, CV_BGR2GRAY);
	cvtColor(i2in, i2img, CV_BGR2GRAY);

	int imgcols = i1img.cols;
	int imgrows = i1img.rows;
	
	float difference = 256 * 16 * 16;
	float smallestdifference = 256 * 16 * 16;

	float tempd = 0;

	int currentisearch;
	int currentjsearch;

	int z = 0;
	float temp;

	int t1=1;
	int t2=2;


	//sequntial search

	Mat i2p;
	i2p = i2in.clone();
	//i2p.create(imgrows,imgcols, CV_8UC3);

	//timer
	float stime;
	stime = GetTickCount();

	//search window:31*31, macroblock:16*16 => 31-16 = 15, 15/2 = 7
	for (int i = 0; i < imgrows/16; i++)
	{
		for (int j = 0; j < imgcols/16; j++)
		{
			smallestdifference = 256 * 16 * 16;
			for (int isearch = 0; isearch < 31; isearch++)
			{
				for (int jsearch = 0; jsearch < 31; jsearch++)
				{
					difference = 0;
					for (int i2 = 16 * i; i2 <= (16 * (i + 1)); i2++)
					{
						for (int j2 = 16 * j; j2 <= (16 * (j + 1)); j2++)
						{
							if ((((isearch - 7) < 0) || ((jsearch - 7) < 0)) || (((isearch - 7) > imgcols) || ((jsearch - 7) > imgcols)))
								tempd = i2img.at<uchar>(j2, i2);
							else
								tempd = i2img.at<uchar>(j2, i2) - i1img.at<uchar>((j2 + jsearch - 7), (i2 + isearch - 7));
							if (tempd < 0)
								tempd = tempd*(-1);
							difference += tempd;
						}
					}
					if (smallestdifference > difference)
					{
						smallestdifference = difference;
						currentisearch = isearch;
						currentjsearch = jsearch;
					}
				}
			}

			for (int i2 = 16 * i; i2 < (16 * (i + 1)); i2++)
			{
				for (int j2 = 16 * j; j2 < (16 * (j + 1)); j2++)
				{
					/*
					Vec3b pix = i1in.at<Vec3b>(Point(j2, i2)); //i1in wrong
					Vec3b pix2 = i2in.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7)); //2 isearch
					Vec3b pix3 = i2p.at<Vec3b>(Point(j2, i2));
					pix3[0] = (pix2[0] + pix[0]) / 2;
					pix3[1] = (pix2[1] + pix[1]) / 2;
					pix3[2] = (pix2[2] + pix[2]) / 2;
					i2p.at<Vec3b>(Point(j2, i2)) = pix3;
					*/
					
					
					if (((j2 + currentjsearch - 7)<=320)&& ((i2 + currentisearch - 7)<=240))
					{
						Vec3b pix = i2in.at<Vec3b>(Point(j2, i2));
						Vec3b pix2 = i2in.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7));
						Vec3b pix3 = i2p.at<Vec3b>(Point(j2 + (currentjsearch - 7) / 2, i2 + (currentisearch - 7) / 2));
						//printf("%d %d\n", j2 + (currentjsearch - 7) / 2, i2 + (currentisearch - 7) / 2);
						pix3[0] = (pix2[0] + pix[0]) / 2;
						pix3[1] = (pix2[1] + pix[1]) / 2;
						pix3[2] = (pix2[2] + pix[2]) / 2;
						t1 = j2 + (currentjsearch - 7) / 2;
						t2 = i2 + (currentisearch - 7) / 2;
						if ((t1 < imgcols) && (t2 < imgrows))
							i2p.at<Vec3b>(Point(t1, t2)) = pix3;
					}
					
					
					/*
					Vec3b pix = i1in.at<Vec3b>(Point(j2, i2));
					Vec3b pix2 = i2in.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7));
					Vec3b pix3 = i2p.at<Vec3b>(Point(j2, i2));
					//pix3[0] = (pix2[0] + pix[0]) / 2;
					//pix3[1] = (pix2[1] + pix[1]) / 2;
					//pix3[2] = (pix2[2] + pix[2]) / 2;
					//i2p.at<Vec3b>(Point(j2, i2)) = pix3;
					if(((j2 + currentjsearch - 7)<imgcols)&&((i2 + currentisearch - 7)<imgrows))
					i2p.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7)) = i2in.at<Vec3b>(Point(j2, i2));
					*/
				}
			}
		}
	}

	//show image

	imshow("reference", i1in);
	imshow("target", i2in);
	for (int i = 0; i < imgrows; i++)
	{
		for (int j = 0; j < imgcols; j++)
		{
			/*
			Vec3b pix = i2in.at<Vec3b>(Point(j, i));
			Vec3b pix2 = i2p.at<Vec3b>(Point(j, i));
			pix2 = pix;
			i2p.at<Vec3b>(Point(j, i)) = pix;
			*/
			/*
			Vec3b pix = i2img.at<Vec3b>(Point(j, i));
			Vec3b pix2 = i2p.at<Vec3b>(Point(j, i));
			pix2 = pix;
			i2p.at<Vec3b>(Point(j, i)) = pix;
			*/
		}
	}
	imshow("i2p by sequntial search", i2p);
	float stimelast = GetTickCount();
	stimelast = stimelast -stime;
	printf("execute time:%fsec\n", stimelast/1000);

	//2d logarithm search
	Mat i2p2;
	i2p2 = i2in.clone();
	for (int i = 0; i < imgrows / 16; i++)
	{
		for (int j = 0; j < imgcols / 16; j++)
		{
			int searchcount = 0;
			int searchcenterx;
			int searchcentery; int dvalue;
			int isearch, jsearch;
			while (searchcount++ < 3)
			{
				smallestdifference = 256 * 16 * 16;
				if (searchcount == 1)
				{
					searchcenterx = 16 * i; searchcentery = 16 * j; dvalue = 4;
				}
				for (int run = 0; run < 9; run++)
				{
					if(run == 0)
					{
						jsearch = searchcenterx - dvalue;
						isearch = searchcentery - dvalue;
					}
					else if (run == 1)
					{
						jsearch = searchcenterx;
						isearch = searchcentery - dvalue;
					}
					else if (run == 2)
					{
						jsearch = searchcenterx + dvalue;
						isearch = searchcentery - dvalue;
					}
					else if (run == 3)
					{
						jsearch = searchcenterx - dvalue;
						isearch = searchcentery;
					}
					else if (run == 4)
					{
						jsearch = searchcenterx;
						isearch = searchcentery;
					}
					else if (run == 5)
					{
						jsearch = searchcenterx + dvalue;
						isearch = searchcentery;
					}
					else if (run == 6)
					{
						jsearch = searchcenterx - dvalue;
						isearch = searchcentery + dvalue;
					}
					else if (run == 7)
					{
						jsearch = searchcenterx;
						isearch = searchcentery + dvalue;
					}
					else if (run == 8)
					{
						jsearch = searchcenterx + dvalue;
						isearch = searchcentery + dvalue;
					}
					
					difference = 0;
					for (int i2 = 16 * i; i2 <= (16 * (i + 1)); i2++)
					{
						for (int j2 = 16 * j; j2 <= (16 * (j + 1)); j2++)
						{
							if (((isearch < 0) || (jsearch < 0)) || ((isearch > imgcols) || (jsearch > imgcols)))
								tempd = i2img.at<uchar>(j2, i2);
							else
								tempd = i2img.at<uchar>(j2, i2) - i1img.at<uchar>((j2 + jsearch), (i2 + isearch));
							if (tempd < 0)
								tempd = tempd*(-1);
							difference += tempd;
						}
					}
					if (smallestdifference > difference)
					{
						smallestdifference = difference;
						currentisearch = isearch;
						currentjsearch = jsearch;
					}
				}
				searchcenterx = currentjsearch;
				searchcentery = currentisearch;
				dvalue = dvalue / 2;
			}
			for (int i2 = 16 * i; i2 < (16 * (i + 1)); i2++)
			{
				for (int j2 = 16 * j; j2 < (16 * (j + 1)); j2++)
				{
					if (((j2 + currentjsearch - 7) <= 320) && ((i2 + currentisearch - 7) <= 240))
					{
						Vec3b pix = i2in.at<Vec3b>(Point(j2, i2));
						Vec3b pix2 = i2in.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7));
						int jd2 = (currentisearch - 7) / 2; int id2 = (currentisearch - 7) / 2;
						Vec3b pix3 = i2p2.at<Vec3b>(Point(j2 + jd2, i2 + id2));
						pix3[0] = (pix2[0] + pix[0]) / 2;
						pix3[1] = (pix2[1] + pix[1]) / 2;
						pix3[2] = (pix2[2] + pix[2]) / 2;
						t1 = j2 + (currentjsearch - 7) / 2;
						t2 = i2 + (currentisearch - 7) / 2;
						//printf("%d %d\n", t1, t2);
						if ((t1 < imgcols) && (t2 < imgrows))
							i2p2.at<Vec3b>(Point(t1, t2)) = pix3;
					}
				}
			}
		}
	}
	imshow("i2p by 2D logarithm", i2p2);


	
	cvWaitKey();
}

