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
	i2p = i1in.clone();
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
					Vec3b pix = i1in.at<Vec3b>(Point(j2, i2));
					Vec3b pix2 = i2p.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7));
					pix2 = pix;
					if (((j2 + currentjsearch - 7) <= 320) && ((i2 + currentisearch - 7) <= 240))
						i2p.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7)) = pix2;
				}
			}
		}
	}

	//show image

	imshow("reference", i1in);
	imshow("target", i2in);
	imshow("i2p by sequntial search", i2p);
	float stimelast = GetTickCount();
	stimelast = stimelast -stime;
	printf("execute time:%fms\n", stimelast);

	//2d logarithm search
	Mat i2p2;
	i2p2 = i1in.clone();
	float dtime = GetTickCount();
	for (int i = 0; i < ((imgrows / 16) - 1); i++)
	{
		
		for (int j = 0; j < ((imgcols / 16) - 1); j++)
		{
			
			int searchcount = 0;
			int searchcenterx;
			int searchcentery; int dvalue;
			int isearch, jsearch;
			while (searchcount < 3)
			{
				searchcount = searchcount + 1;
				smallestdifference = 256 * 16 * 16;
				if (searchcount == 1)
				{
					searchcenterx = 16 * j; searchcentery = 16 * i; dvalue = 4;
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
						jsearch = searchcenterx +16 + dvalue;
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
						jsearch = searchcenterx + 16 + dvalue;
						isearch = searchcentery;
					}
					else if (run == 6)
					{
						jsearch = searchcenterx - dvalue;
						isearch = searchcentery + 16 + dvalue;
					}
					else if (run == 7)
					{
						jsearch = searchcenterx;
						isearch = searchcentery + 16 + dvalue;
					}
					else if (run == 8)
					{
						jsearch = searchcenterx + 16 + dvalue;
						isearch = searchcentery + 16 + dvalue;
					}
					
					difference = 0;
					for (int i2 = 16 * i; i2 <= (16 * (i + 1)); i2++)
					{
						for (int j2 = 16 * j; j2 <= (16 * (j + 1)); j2++)
						{
							if (((isearch < 0) || (jsearch < 0)) || ((isearch > imgrows) || (jsearch > imgcols)))
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
			//printf("%d %d\n", searchcenterx, searchcentery);

			for (int i2 = 16 * i; i2 < (16 * (i + 1)); i2++)
			{
				for (int j2 = 16 * j; j2 < (16 * (j + 1)); j2++)
				{
					Vec3b pix = i1in.at<Vec3b>(Point(j2, i2));
					Vec3b pix2 = i2p2.at<Vec3b>(Point(searchcenterx, searchcentery));
					pix2 = pix;
					if(((searchcenterx + j2 - 16 * j)< imgcols) && ((searchcentery + i2 - 16 * i) < imgrows))
					i2p2.at<Vec3b>(Point(searchcenterx+j2-16*j, searchcentery+i2-16*i)) = pix2;
					if (!(((searchcenterx + j2 - 16 * j) <= 320) || ((searchcentery + i2 - 16 * i) <= 240)))
						printf("error");
				}
			}
		}
	}

	float dtimelast = GetTickCount();
	dtimelast = dtimelast - dtime;
	dtimelast = dtimelast;
	printf("dtimelast:%f ms\n", dtimelast);

	imshow("i2p by 2D logarithm", i2p2);

	//count SNR

	Mat i2pimg; 
	Mat i2p2img;

	cvtColor(i2p, i2pimg, CV_BGR2GRAY);
	cvtColor(i2p2, i2p2img, CV_BGR2GRAY);

	float SNR1 = 0;
	float SNR2 = 0;

	float fsquare1 = 0;
	float fdifference1 = 0;

	float fsquare2 = 0;
	float fdifference2 = 0;

	for (int j = 0; j < 320; j++)
	{
		for (int i = 0; i < 240; i++)
		{
			float temp3 = i2img.at<uchar>(j, i);
			float temp1 = i2pimg.at<uchar>(j, i);
			//float temp2 = i2p2img.at<uchar>(j, i);
			
			fsquare1 += temp1 * temp1;
			//fsquare2 += temp2 * temp2;
			fdifference1 += (temp1 - temp3)*(temp1 - temp3);
			//fdifference2 += (temp2 - temp3)*(temp2 - temp3);
		}
	}
	SNR1 = fdifference1 / fsquare1;
	//SNR2 = fdifference2 / fsquare2;

	printf("sequntial search SNR:%f\n", SNR1);
	//printf("2D logarithm search SNR:%f\n", SNR2);

	i1in.release();
	i2in.release();
	i1img.release();
	i2img.release();
	i2p.release();
	i2p2.release();
	i2pimg.release();
	i2p2img.release();

	cvWaitKey();
}

