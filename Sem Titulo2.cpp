#include<cv.h>
#include<highgui.h>
#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<iostream>
#include<sstream>
CvHistogram* histo;
IplImage * Histograma(IplImage* Image) {

    IplImage* imgHistogram = 0;

    //max and min value of the histogram
    float max_value = 0, min_value = 0;

   //value and normalized value
    float value;
    int normalized;

    //size of the histogram -1D histogram
    int bins = 256;
    int hsize[] = {bins};
    //ranges - grayscale 0 to 256
    float xranges[] = { 0, 256 };
    float* ranges[] = { xranges };

   //planes to obtain the histogram, in this case just one
    IplImage* planes[] = { Image };

   //get the histogram and some info about it
    histo = cvCreateHist( 1, hsize, CV_HIST_ARRAY, ranges,1);
    cvCalcHist( planes, histo, 0, NULL);
    cvGetMinMaxHistValue( histo, &min_value, &max_value);
    printf("min: %f, max: %f\n", min_value, max_value);

    //create an 8 bits single channel image to hold the histogram
    //paint it white
    imgHistogram = cvCreateImage(cvSize(bins, 50),8,1);
    cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(256,50), CV_RGB(255,255,255),-1);

    //draw the histogram :P
    for(int i=0; i < bins; i++){
            value = cvQueryHistValue_1D( histo, i);
            normalized = cvRound(value*50/max_value);
            cvLine(imgHistogram,cvPoint(i,50), cvPoint(i,50-normalized), CV_RGB(0,0,0));
    }

    return imgHistogram;
}

float perc[4][3][10];
int hist[4][3][256];
char path[1000], buf[100], cwd[1024], cwd_arquivo[1024], cwd_imagens[1024];
IplImage *dst;
CvScalar v;

main(){
int img;

for (img = 0; img < 10; img++) {
        itoa(img, buf, 100);
        strcat(buf, ".jpg");
        getcwd(cwd, 1024);
        strcpy(cwd_imagens, cwd);
        strcat(cwd_imagens, "\\image.orig\\");
        strcat(cwd_imagens, buf);
        sprintf(path, cwd_imagens, img);
        dst = cvLoadImage(path, CV_LOAD_IMAGE_COLOR);
        IplImage* imagemGrayscale = cvCreateImage(cvGetSize(dst), dst->depth, 1);
        IplImage* imagemHistogramaEqualizada = cvCloneImage(imagemGrayscale);
        cvZero(imagemHistogramaEqualizada);
        for (int i = 0; i < dst->height; i++)
             for (int j = 0; j < dst->width; j++) {
                    v = cvGet2D(dst, i, j);
                    if (i <= dst->height / 2 && j <= dst->width / 2) { //1 quadrante
                        hist[0][0][(int) v.val[0]]++; //R
                        hist[0][1][(int) v.val[1]]++; //G
                        hist[0][2][(int) v.val[2]]++; //B

                    } else if (i > dst->height / 2 && j <= dst->width / 2) { //2 quadrante
                        hist[1][0][(int) v.val[0]]++; //R
                        hist[1][1][(int) v.val[1]]++; //G
                        hist[1][2][(int) v.val[2]]++; //B
                    } else if (i <= dst->height / 2 && j > dst->width / 2) { //3 quadrante
                        hist[2][0][(int) v.val[0]]++; //R
                        hist[2][1][(int) v.val[1]]++; //G
                        hist[2][2][(int) v.val[2]]++; //B
                    } else { //4 quadrante
                        hist[3][0][(int) v.val[0]]++; //R
                        hist[3][1][(int) v.val[1]]++; //G
                        hist[3][2][(int) v.val[2]]++; //B
                   }
         } // for

        // Normalizar histograma

        FILE *arquivo;
        strcpy(cwd_arquivo, cwd);
        strcat(cwd_arquivo, "\\saidaNormalizada.txt");
        arquivo = fopen(cwd_arquivo, "w");

        if (arquivo == NULL) exit (-1);
        for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 256; k++) {
                            fprintf(arquivo, "%d   ,", hist[i][j][k]);
                        } // k
                } // j
                fprintf(arquivo, "%s\n", path);
        } // i

        fclose(arquivo);

     cvNamedWindow("Imagem");
     cvShowImage("Imagem", dst);

    // Converte imagem original colorida para n�veis de cinza
	cvCvtColor(dst, imagemGrayscale, CV_RGB2GRAY);

	 cvEqualizeHist(imagemGrayscale, imagemHistogramaEqualizada);
	cvNamedWindow("Imagem Equalizada");
	cvShowImage("Imagem Equalizada", imagemHistogramaEqualizada);

	IplImage * Imh2 = Histograma(imagemHistogramaEqualizada);
   // Mostra Histograma
    cvNamedWindow("Histograma");
    cvShowImage("Histograma", Imh2);
	 cvWaitKey(10000);

    cvReleaseImage(&imagemGrayscale);
	cvReleaseImage(&imagemHistogramaEqualizada);
	 }

}
