//#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
//using namespace std;

#define SENTINEL 1000000000

int count=0;
int current_y;
int current_best_score=0;
int current_best_y;
int current_best_low;
int current_best_high;

#ifndef Boolean         /* Boolean が定義されていなかったら */
#define Boolean int
#endif

#ifndef TRUE            /* TRUE が定義されていなかったら */
#define TRUE 1
#endif

#ifndef FALSE           /* FALSE が定義されていなかったら */
#define FALSE 0
#endif

#define score_range 10000
#define Element_num 1000
#define filename "testdata_1000v.txt"
#define x_length 500
#define y_length 500


//インターバルの構造体
struct Interval
{
    int low, high, score, y;
};

//インターバルサーチ木のノードを表す構造体
struct ITNode
{
    int low;
    int high;
    int score;
    struct ITNode *left;
    struct ITNode *right;
};

static struct ITNode *root = NULL;
struct ITNode *original = NULL;
struct Interval ints2[Element_num*2+2];


struct ITNode *newNode(int low, int high, int score)
{
    struct ITNode *temp;
    temp = malloc(sizeof(struct ITNode));
    if(!temp){
      printf("エラー：メモリの割り当てに失敗しました。");
      exit(-1);
    }
    temp->low = low;
    temp->high = high;
    temp->score = score;
    temp->left = temp->right = NULL;
    if(temp->score > current_best_score){
      current_best_score = temp->score;
      current_best_y = current_y;
      current_best_low = temp->low;
      current_best_high = temp->high;
    }
    return temp;
};

//木に新しいノードを挿入
//返り値は常に一番上のルートノード
void insert(struct ITNode **node, int low, int high, int score){
    if(low <= 0){
      low = 0;
    }
    if(high >= score_range){
      high = score_range;
    }

    //木が空の場合
    if ((*node) == NULL){
      (*node) = newNode(low, high, score);
      return;
    }


    //ルートノードに格納されているインターバルの小さい値を取得
    int low_score = (*node)->low;
    int high_score = (*node)->high;

    struct ITNode *origin;
    origin = (struct ITNode *)malloc(sizeof(struct ITNode));
    origin = *node;

    //完全に範囲とインターバルがかぶっていない場合 -i-i-[-----]---
    if(high <= low_score){//左側に飛び出す
      insert(&(*node)->left, low, high, score);
      *node = origin;
      return;

    }else if(low >= high_score){//右側に飛び出す
      insert(&(*node)->right, low, high, score);
      *node = origin;
      return;

    //完全にインターバルが重なった時 ---[i-----i]---
    }else if(low == low_score && high == high_score){
      (*node)->score += score;
      if((*node)->score > current_best_score){
        current_best_score = (*node)->score;
        current_best_y = current_y;
        current_best_low = low;
        current_best_high = high;
      }
      return;

    //左側のインターバルが重なった時 ---[i---i--]---
    }else if(low == low_score && high < high_score){
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->low = low_score;
        temp1->high = high;
        temp1->score = (*node)->score + score;
        temp1->left = (*node)->left;
        temp1->right = NULL;
        if(temp1->score > current_best_score){
          current_best_score = temp1->score;
          current_best_y = current_y;
          current_best_low = temp1->low;
          current_best_high = temp1->high;
        }

        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->low = high;
        temp2->high = high_score;
        temp2->score = (*node)->score;
        temp2->left = temp1;
        temp2->right = (*node)->right;

        *node = temp2;

        return;

    //右側のインターバルが重なった時 ---[--i---i]---
    }else if(low > low_score && high == high_score){
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->low = low;
        temp1->high = high_score;
        temp1->score = (*node)->score + score;
        temp1->left = NULL;
        temp1->right = (*node)->right;
        if(temp1->score > current_best_score){
          current_best_score = temp1->score;
          current_best_y = current_y;
          current_best_low = temp1->low;
          current_best_high = temp1->high;
        }

        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->low = low_score;
        temp2->high = low;
        temp2->score = (*node)->score;
        temp2->left = (*node)->left;
        temp2->right = temp1;

        *node = temp2;

        return;

    //右側のインターバルが重なり,左が外に出た時 --i-[-----i]---
    }else if(low < low_score && high == high_score){
        (*node)->score += score;
        if((*node)->score > current_best_score){
          current_best_score = (*node)->score;
          current_best_y = current_y;
          current_best_low = low_score;
          current_best_high = high_score;
        }

        insert(&(*node)->left, low, low_score, score);

        (*node) = origin;
        return;

    //左側のインターバルが重なり,右が外に出た時 ---[i-----]-i--
    }else if(low == low_score && high > high_score){
        (*node)->score += score;
        if((*node)->score > current_best_score){
          current_best_score = (*node)->score;
          current_best_y = current_y;
          current_best_low = low_score;
          current_best_high = high_score;
        }

        insert(&(*node)->right, high_score, high, score);

        (*node) = origin;
        return;

    //完全に範囲に含まれている場合 ---[-i---i-]---
    }else if(low > low_score && high < high_score){
        struct ITNode *temp_l;
        temp_l = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_l){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_l->low = low_score;
        temp_l->high = low;
        temp_l->score = (*node)->score;
        temp_l->left = (*node)->left;
        temp_l->right = NULL;

        struct ITNode *temp_r;
        temp_r = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_r){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_r->low = high;
        temp_r->high = high_score;
        temp_r->score = (*node)->score;
        temp_r->left = NULL;
        temp_r->right = (*node)->right;

        struct ITNode *temp_c;
        temp_c = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_c){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_c->low = low;
        temp_c->high = high;
        temp_c->score = (*node)->score + score;
        temp_c->left = temp_l;
        temp_c->right = temp_r;
        if(temp_c->score > current_best_score){
          current_best_score = temp_c->score;
          current_best_y = current_y;
          current_best_low = temp_c->low;
          current_best_high = temp_c->high;
        }

        *node = temp_c;

        return;

    //左が範囲から出ている場合 --i-[--i---]---
    }else if(low < low_score && high < high_score){
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->low = high;
        temp2->high = high_score;
        temp2->score = (*node)->score;
        temp2->left = NULL;
        temp2->right = (*node)->right;

        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->low = low_score;
        temp1->high = high;
        temp1->score = (*node)->score + score;
        temp1->left = (*node)->left;
        temp1->right = temp2;
        if(temp1->score > current_best_score){
          current_best_score = temp1->score;
          current_best_y = current_y;
          current_best_low = temp1->low;
          current_best_high = temp1->high;
        }

        insert(&temp1->left, low, low_score, score);

        *node = temp1;

        return;

    //完全に範囲を含んでいる場合 --i-[-----]-i--
    }else if(low < low_score && high > high_score){

        (*node)->score += score;
        if((*node)->score > current_best_score){
          current_best_score = (*node)->score;
          current_best_y = current_y;
          current_best_low = low_score;
          current_best_high = high_score;
        }

        insert(&(*node)->left, low, low_score, score);

        *node = origin;

        insert(&(*node)->right, high_score, high, score);

        *node = origin;

        return;

    //右が範囲から出ている場合 ---[--i---]-i--
    }else if(low > low_score && high > high_score){
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->low = low;
        temp1->high = high_score;
        temp1->score = (*node)->score + score;
        temp1->left = NULL;
        temp1->right = (*node)->right;
        if(temp1->score > current_best_score){
          current_best_score = temp1->score;
          current_best_y = current_y;
          current_best_low = temp1->low;
          current_best_high = temp1->high;
        }

        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->low = low_score;
        temp2->high = low;
        temp2->score = (*node)->score;
        temp2->left = (*node)->left;
        temp2->right = temp1;

        insert(&temp1->right, high_score, high, score);
        *node = temp2;

        return;

    }else{
        printf("error in insert\n");
        printf("i:[%d,%d]root[%d,%d]\n",low,high,low_score,high_score);
    }


    return;

}

void search(struct ITNode *now){
  if (now == NULL){
    return;
  }

  search(now->left);

  printf("[%d,%d]%d\n", now->low,now->high,now->score);

  search(now->right);

}

void sort(struct ITNode **node){
  struct ITNode *copy = *node;
  //なるべく右を親に
  if((*node)->right != NULL){
      if((*node)->right->left != NULL){
        while((*node)->right->left == NULL){

        }
      }
      (*node)->right->left = (*node)->left;

      *node = (*node)->right;

  }else if((*node)->left != NULL){
      (*node)->left->right = (*node)->right;
      *node = (*node)->left;

  }else if((*node)->left == NULL && (*node)->right == NULL){
    *node = NULL;

  }else{
    printf("error in sort\n");
  }

  free(copy);
  return;
}

void delete(struct ITNode **node, int low, int high, int score){
  //NULLなら上に戻る
  if((*node) == NULL){
  //  printf("----------------------------------------------------\n");
  //  search(root);
  //  delete(&original, low, high, score);
    return;
  }
  int low_score = (*node)->low;
  int high_score = (*node)->high;
  //printf("再帰：root[[%d,%d]]からインターバル[[%d,%d]]を削除\n",(*node)->low,(*node)->high,low,high);

  //完全にインターバルの外にいる時 -i-i-[-----]---
  if(high <= low_score){
    delete(&(*node)->left, low, high, score);
  }else if(low >= high_score){
    delete(&(*node)->right, low, high, score);

  //完全にインターバルが重なった時 ---[i-----i]---
  }else if(low == low_score && high == high_score){
    (*node)->score -= score;
    if((*node)->score == 0){
    //  sort(&(*node));
    }

  //右側のインターバルが重なり,左が外に出た時 --i-[-----i]---
  }else if(low < low_score && high == high_score){
    (*node)->score -= score;
    if((*node)->score == 0){
    //  sort(&(*node));
    }
    delete(&(*node), low, low_score, score);

  //左側のインターバルが重なり,右が外に出た時 ---[i-----]-i--
  }else if(low == low_score && high > high_score){
    (*node)->score -= score;
    if((*node)->score == 0){
    //  sort(&(*node));
    }
    delete(&(*node), high_score, high, score);

  //完全に範囲を含んでいる場合 --i-[-----]-i--
  }else if(low < low_score && high > high_score){
    (*node)->score -= score;
    if((*node)->score == 0){
    //  sort(&(*node));
    }
    delete(&(*node), low, low_score, score);
    delete(&(*node), high_score, high, score);

  }else{
    printf("error in delete\n");
    printf("i:[%d,%d]root[%d,%d]\n",low,high,low_score,high_score);
  }

  return;
}

void merge(struct Interval ints[],int left,int mid,int right){
  int n1,n2,i,j,k;
  int *L,*R,*L_low,*L_high,*L_score,*R_low,*R_high,*R_score;
  n1=mid-left;//左の要素数
  n2=right-mid;//右の要素数

  L=(int *)malloc(sizeof(int)*(n1+1));
  L_low=(int *)malloc(sizeof(int)*(n1+1));
  L_high=(int *)malloc(sizeof(int)*(n1+1));
  L_score=(int *)malloc(sizeof(int)*(n1+1));

  R=(int *)malloc(sizeof(int)*(n2+1));
  R_low=(int *)malloc(sizeof(int)*(n1+1));
  R_high=(int *)malloc(sizeof(int)*(n1+1));
  R_score=(int *)malloc(sizeof(int)*(n1+1));

  for(i=0;i<=n1-1;i++){
    L[i]=ints[left+i].y;
    L_low[i]=ints[left+i].low;
    L_high[i]=ints[left+i].high;
    L_score[i]=ints[left+i].score;
  }
  for(j=0;j<=n2-1;j++){
    R[j]=ints[mid+j].y;
    R_low[j]=ints[mid+j].low;
    R_high[j]=ints[mid+j].high;
    R_score[j]=ints[mid+j].score;
  }
  L[n1]=SENTINEL;
  R[n2]=SENTINEL;
  i=0;
  j=0;
  for(k=left;k<=right-1;k++){
   if(L[i]<=R[j]){
      ints[k].y=L[i];
      ints[k].low=L_low[i];
      ints[k].high=L_high[i];
      ints[k].score=L_score[i];
      i++;
      count++;
    }
    else{
      ints[k].y=R[j];
      ints[k].low=R_low[j];
      ints[k].high=R_high[j];
      ints[k].score=R_score[j];
      j++;
      count++;
    }
  }
  free(L);
  free(L_low);
  free(L_high);
  free(L_score);
  free(R);
  free(R_low);
  free(R_high);
  free(R_score);

}

void mergeSort(struct Interval ints[],int left,int right){
  int i,mid;
  if(left+1<right){
    mid=(left+right)/2;
    mergeSort(ints,left,mid);
    mergeSort(ints,mid,right);
    merge(ints,left,mid,right);
  }
}

void sample(struct Interval ints[],int left,int right){
  mergeSort(ints,left,right);
  return;
}

int main()
{
    FILE *fp;
    int i = 0;
    struct Interval ints[Element_num*2+2];
    int x_location, y_location, score;

    fp = fopen(filename,"r");//テストデータ展開
    if (fp == NULL) {
      printf("[%s] is not open!\n",filename);
      return -1;
    }else{
      printf("[%s] is opened!\n",filename);
    }

    //ファイルの読み込み
    while(fscanf(fp,"%d,%d,%d", &x_location, &y_location, &score)!=EOF) {
      //イベント点を記録
      ints[i].low = x_location - x_length/2;
      ints[i].high = x_location + x_length/2;
      ints[i].score = score;
      ints[i].y = y_location - y_length/2;
      i++;
      ints[i].low = x_location - x_length/2;
      ints[i].high = x_location + x_length/2;
      ints[i].score = -score;
      ints[i].y = y_location + y_length/2;
      i++;
    }

    //mergeSort(ints,0,i);

    sample(ints,0,i);

    for (int j = 0; j < i; j++){//下から

      current_y = ints[j].y;
      if(ints[j].score > 0){
        insert(&root, ints[j].low, ints[j].high, ints[j].score);
      }else if(ints[j].score < 0){
        original = (struct ITNode *)malloc(sizeof(struct ITNode));
        original = root;
        delete(&root, ints[j].low, ints[j].high, -ints[j].score);
      }

    }

    printf("----------------------------------------------------\n");
    //search(root);
    printf("current_best is y= %d,[%d,%d],score is %d\n",current_best_y,current_best_low,current_best_high,current_best_score);
/*

int sss = 0;
for(int j = 0; j<i;j++){
  sss += ints[j].score;
  printf("%d,[%d,%d],%d\n",ints[j].y,ints[j].low,ints[j].high,ints[j].score);
}
printf("%d\n",sss);
printf("%d\n",count);*/
    return 0;
}
