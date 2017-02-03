
#include<cv.h>
#include<highgui.h>
#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<iostream>
#include<sstream>
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
CvHistogram* histo[1000][3];
double comparations[1000], _rank[10];
int k[10];

void jogaPraBaixo(int indice){
    for(int i = 9; i>indice; i--){
        k[i] = k[i-1];
        _rank[i] = _rank[i-1];
    }
}
void rankeamento(){
    k[0] = k[1] = k[2] = k[3] = k[4] = k[5] = k[6] = k[7] = k[8] = k[9] = -1;
    for(int i = 0; i<1000; i++){
        for(int j = 0; j<10; j++){
            if(i==k[0] || i==k[1] || i==k[2] || i==k[3] || i==k[4] || i==k[5] || i==k[6] || i==k[7] || i==k[8] || i==k[9]){
                continue ;
            }
            if(comparations[i]>=_rank[j]){
                    jogaPraBaixo(j);
                    k[j]=i;
                    _rank[j]=comparations[i];
                    break;

            }
        }
    }
}

IplImage * Histograma(IplImage* src, int index) {

    IplImage* r_plane = cvCreateImage( cvGetSize(src), 8, 1 );
        IplImage* g_plane = cvCreateImage( cvGetSize(src), 8, 1 );
        IplImage* b_plane = cvCreateImage( cvGetSize(src), 8, 1 );
        int hist_size[] = {255};
        /* hue varies from 0 (~0 deg red) to 180 (~360 deg red again) */
        float range[] = {0, 255};
        float min_value, max_value;

        float* ranges[] = {range};
        int scale = 10;
        IplImage* hist_img =
            cvCreateImage( cvSize(255,300), 8, 3 );
        CvHistogram* hist;
        int r, g, b;

        //cvCvtColor( src, hsv, CV_BGR2HSV );
        //cvCvtColor( src, hsv, CV_HSV2BGR );
        cvSplit(src, r_plane, g_plane, b_plane, 0 );
        cvZero( hist_img );

        histo[index][0] = cvCreateHist( 1, hist_size, CV_HIST_ARRAY, ranges, 1 );
        cvCalcHist( &r_plane, histo[index][0], 0, 0 );
        cvGetMinMaxHistValue( histo[index][0], &min_value, &max_value);
        for( r = 0; r < 255; r++ )
        {
            int value = cvQueryHistValue_1D( histo[index][0], r);
            int normalized = cvRound(value*50/max_value);
            cvLine(hist_img,cvPoint(r,50), cvPoint(r,50-normalized), CV_RGB(255,0,0));
        }

        histo[index][1] = cvCreateHist( 1, hist_size, CV_HIST_ARRAY, ranges, 1 );
        cvCalcHist( &g_plane, histo[index][1], 0, 0 );
        cvGetMinMaxHistValue( histo[index][1], &min_value, &max_value);
        for( g = 0; g < 255; g++ )
        {
            int value = cvQueryHistValue_1D( histo[index][1], g);
            int normalized = cvRound(value*50/max_value);
            cvLine(hist_img,cvPoint(g,100), cvPoint(g,100-normalized), CV_RGB(0,255,0));
        }

        histo[index][2] = cvCreateHist( 1, hist_size, CV_HIST_ARRAY, ranges, 1 );
        cvCalcHist( &b_plane, histo[index][2], 0, 0 );
        cvGetMinMaxHistValue( histo[index][2], &min_value, &max_value);
        for( b = 0; b < 255; b++ )
        {
            int value = cvQueryHistValue_1D( histo[index][2], b);
            int normalized = cvRound(value*50/max_value);
            cvLine(hist_img,cvPoint(b,150), cvPoint(b,150-normalized), CV_RGB(0,0,255));
        }

    return hist_img;
}
/*
int somaArray(int hist[][3][256], int canal, int quadrante){
    int aux=0;
    for(int i=0;i<255;i++){
        aux = aux+ hist[quadrante][canal][i];
    }
    return aux;
}

void zeraHistograma(int hist[][3][256],int equalizado[][3][256]){

        for(int k=0;k<4;k++){
            for(int j=0;j<3;j++){
               for(int i=0;i<255;i++){
                    equalizado[k][j][i]=0;
                    hist[k][j][i]=0;
               }
            }
        }
}

void equalizaHistograma(int hist[][3][256],int escalas,int equalizado[][3][256]){
        float prj[4][3][256];
        float sk[4][3][256];
        int g = escalas-1;
        int soma;
        for(int k=0;k<4;k++){
            for(int j=0;j<3;j++){
                soma=somaArray(hist,j,k);
               for(int i=0;i<255;i++){
                    prj[k][j][i]=(float) hist[k][j][i]/soma;
                    if(i>0) sk[k][j][i]=prj[k][j][i]+sk[k][j][i-1];
                    else sk[k][j][i]=prj[k][j][i];
                    equalizado[k][j][(int)round((sk[k][j][i]*(float)g))]+=hist[k][j][i];
               }
            }
        }
}
*/

float perc[4][3][10];
int hist[4][3][256],equalizado[4][3][256];
//CvHistogram *histograma;
char path[1000], buf[100], cwd[1024], cwd_arquivo[1024], cwd_imagens[1024];
IplImage *dst, *imgS;
CvScalar v;

main(){
int escolha;
printf("Escolha um numero de imagem para comparar com as outras: ");
scanf("%d", &escolha);
int img;
//int hsize[] = {256};
sprintf(buf, "%d", escolha);
strcat(buf, ".jpg");
getcwd(cwd, 1024);
strcpy(cwd_imagens, cwd);
strcat(cwd_imagens, "\\image.orig\\");
strcat(cwd_imagens, buf);
sprintf(path, cwd_imagens, escolha);
imgS = cvLoadImage(path, CV_LOAD_IMAGE_COLOR);
IplImage * imgSel = Histograma(imgS, escolha);

//histograma = cvCreateHist(3,hsize,CV_HIST_ARRAY);
for (img = 0; img < 1000; img++) {
        if(img == escolha){img++;}
        //cvClearHist(histograma);
        sprintf(buf, "%d", img);
        strcat(buf, ".jpg");
        strcpy(cwd_imagens, cwd);
        strcat(cwd_imagens, "\\image.orig\\");
        strcat(cwd_imagens, buf);
        sprintf(path, cwd_imagens, img);
        dst = cvLoadImage(path, CV_LOAD_IMAGE_COLOR);

        //cvCalcHist(&dst,histograma);

        //cvZero(imagemHistogramaEqualizada);
        /*
        zeraHistograma(hist, equalizado);
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

        equalizaHistograma(hist,120,equalizado);*/

        // Normalizar histograma
        /*
        strcpy(cwd_arquivo, cwd);
        strcat(cwd_arquivo, "\\saidaNormalizada.txt");

        FILE *arquivo;
        if(img == 0){
            arquivo = fopen(cwd_arquivo, "w");
        }else{
            arquivo = fopen(cwd_arquivo, "a");
        }

        if (arquivo == NULL) exit (-1);
        fprintf(arquivo, "%s\n", path);
        for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 120; k++) {
                            fprintf(arquivo,"  %d  |", equalizado[i][j][k]);
                        } // k
                        fprintf(arquivo, "\n");
                } // j
            fprintf(arquivo, "\n\n");
        } // i
        fprintf(arquivo, "\n");
        fclose(arquivo);*/

     cvShowImage("Imagem", dst);

    // Converte imagem original colorida para níveis de cinza


	IplImage * Imh2 = Histograma(dst, img);
   // Mostra Histograma
    cvShowImage("Histograma", Imh2);
    comparations[img] = cvCompareHist(histo[img][0], histo[escolha][0],0) + cvCompareHist(histo[img][1], histo[escolha][1],0) + cvCompareHist(histo[img][2], histo[escolha][2],0);
    printf("Imagem %d: %f\n",img, comparations[img]);
    //cvWaitKey(5000);

  //  cvReleaseImage(&imagemGrayscale);
//	cvReleaseImage(&imagemHistogramaEqualizada);
	 }
	 cvDestroyAllWindows();
    rankeamento();
    for(int l = 9; l>=0; l--){
        sprintf(buf, "%d", k[l]);
        strcat(buf, ".jpg");
        strcpy(cwd_imagens, cwd);
        strcat(cwd_imagens, "\\image.orig\\");
        strcat(cwd_imagens, buf);
        sprintf(path, cwd_imagens, k[l]);
        dst = cvLoadImage(path, CV_LOAD_IMAGE_COLOR);

        cvShowImage("Imagem selecionada", imgS);
        cvShowImage("Imagem próxima", dst);
        cvShowImage("Histograma selecionado", imgSel);
        cvShowImage("Histograma próximo", Histograma(dst, k[l]));
        printf("Imagem numero %d: %d\n",l+1, k[l]);
        printf("Proximidade: %f de 3\n",comparations[k[l]]);
        cvWaitKey(1000000);
    }

}
