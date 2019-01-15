//#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
//using namespace std;

#ifndef Boolean         /* Boolean が定義されていなかったら */
#define Boolean int
#endif

#ifndef TRUE            /* TRUE が定義されていなかったら */
#define TRUE 1
#endif

#ifndef FALSE           /* FALSE が定義されていなかったら */
#define FALSE 0
#endif

#define score_range 50

//インターバルの構造体
struct Interval
{
    int low, high, score;
};

//インターバルサーチ木のノードを表す構造体
struct ITNode
{
    struct Interval *ik;
    int score;
    struct ITNode *left;
    struct ITNode *right;
};

static struct ITNode *root = NULL;
static struct ITNode *node = NULL;

//木に新しいノードを挿入
//返り値は常に一番上のルートノード
void insert( struct Interval i)
{

      printf("d[[%d,%d]]\n",node->ik->low,node->ik->high);

    return;

}


int main()
{
    struct Interval ints[] = {{15, 20, 1}, {10, 30, 1}, {17, 19, 1},
        {5, 20, 1}, {12, 15, 1}, {30, 40, 1}
    };
    int n = sizeof(ints)/sizeof(ints[0]);

    if (node == NULL){
      node = malloc(sizeof(struct ITNode));
      if(!node){
        printf("エラー：メモリの割り当てに失敗しました。");
        exit(-1);
      }
      node->ik = &ints[2];
      node->score = ints[2].score;
      node->left = NULL;
      node->right = NULL;
    }

    for (int i = 0; i < n; i++){
      if(node != NULL){
        printf("root[[%d,%d]]&&ints[[%d,%d]]\n",node->ik->low,node->ik->high,ints[i].low,ints[i].high);
      }
        insert( ints[i]);
    }

    printf("----------------------------------------------------\n");
    //inorder(root);

    return 0;
}
