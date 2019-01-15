#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/sqlite3.h"
#include "mydef.h"
#define SENTINEL 1000000000
//int RANGE;//gridで区切る格子の大きさ(初期)
//int SCOPE;//x,y座標の最大値
int eventnum;
int count=0;

void merge(int A[],int left,int mid,int right);
void mergeSort(int A[],int left,int right);

int opencsv(int Event[]) {//csvファイルからテストデータ展開
  FILE *fp;
  int x_location, y_location, score;
  eventnum = 0;
  int i;

  fp = fopen(datafile,"r");//テストデータ展開
  if (fp == NULL) {
    printf("[%s] is not open!\n",datafile);
    return -1;
  }else{
    printf("[%s] is opened!\n",datafile);
  }

  //ファイルの読み込み
  while(fscanf(fp,"%d,%d,%d", &x_location, &y_location, &score)!=EOF) {
    //イベント点を記録
    Event[eventnum] = y_location - y_length/2;
    eventnum++;
    Event[eventnum] = y_location + y_length/2;
    eventnum++;
  }
  mergeSort(Event,0,eventnum);
  for(i=0;i<eventnum;i++){
    printf("%d,",Event[i]);
  }
  //終了処理
  fclose(fp);
  return 0;
}


void merge(int A[],int left,int mid,int right){
  int n1,n2,i,j,k;
  int *L,*R;
  n1=mid-left;
  n2=right-mid;
  L=(int *)malloc(sizeof(int)*(n1+1));
  R=(int *)malloc(sizeof(int)*(n2+1));
  for(i=0;i<=n1-1;i++){
    L[i]=A[left+i];
  }
  for(j=0;j<=n2-1;j++){
    R[j]=A[mid+j];
  }
  L[n1]=SENTINEL;
  R[n2]=SENTINEL;
  i=0;
  j=0;
  for(k=left;k<=right-1;k++){
   if(L[i]<=R[j]){
      A[k]=L[i];
      i++;
      count++;
    }
    else{
      A[k]=R[j];
      j++;
      count++;
    }
  }
  free(L);
  free(R);
}

void mergeSort(int A[],int left,int right){
  int i,mid;
  if(left+1<right){
    mid=(left+right)/2;
    mergeSort(A,left,mid);
    mergeSort(A,mid,right);
    merge(A,left,mid,right);
  }
}
