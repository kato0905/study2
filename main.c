//#include <iostream>
#include<stdio.h>
#include<stdlib.h>
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

//struct ITNode *newNode(struct Interval i);
//struct ITNode *insert(struct ITNode *root, struct Interval i);

//インターバルの構造体
struct Interval
{
    int low, high, score;
};

//インターバルサーチ木のノードを表す構造体
struct ITNode
{
    struct Interval *i;
    int score;
    struct ITNode *left, *right;
};

//木に新しいノードを追加
struct ITNode *newNode(struct Interval i)
{
    struct ITNode *temp;
    temp = (struct ITNode *)malloc(sizeof(struct ITNode));
    if(!temp){
      printf("エラー：メモリの割り当てに失敗しました。");
      exit(-1);
    }
    temp->i = &i;
    temp->score = i.score;
    temp->left = temp->right = NULL;
printf("[[%d,%d,%d,%d]]\n",i.low,i.high,temp->i->low,temp->i->high);
    return temp;
};

//木に新しいノードを挿入
struct ITNode *insert(struct ITNode *root, struct Interval i)
{
    if(i.low <= 0){
      i.low = 0;
    }
    if(i.high >= score_range){
      i.high = score_range;
    }


    //木が空の場合
    if (root == NULL){
      root = newNode(i);
      return root;
    }

    printf("f[[%d,%d,%d,%d]]\n",i.low,i.high,root->i->low,root->i->high);

    //ルートノードに格納されているインターバルの小さい値を取得
    int low_score = root->i->low;
    int high_score = root->i->high;

    if(low_score >= i.high){
      insert(root->left, i);
    }else if(high_score <= i.low){
      insert(root->right, i);
    }

      //完全にインターバルが重なった時
      if(i.low == low_score && i.high == high_score){
        root->score += i.score;

        return root;

      //左側のインターバルが重なった時
      }else if(i.low == low_score && i.high < high_score){
        struct Interval j1 = {low_score,i.high};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = root->left;
        temp1->right = NULL;

        struct Interval j2 = {i.high,high_score};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->score = root->score;
        temp2->left = temp1;
        temp2->right = root->right;

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      //右側のインターバルが重なった時
    }else if(i.low > low_score && i.high == high_score){
        struct Interval j1 = {i.low,high_score};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = NULL;
        temp1->right = root->right;

        struct Interval j2 = {low_score,i.low};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->score = root->score;
        temp2->left = root->left;
        temp2->right = temp1;

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      //完全に範囲に含まれている場合
      }else if(i.low > low_score && i.high < high_score){
        struct Interval j1 = {low_score,i.low};
        struct ITNode *temp_l;
        temp_l = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_l){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_l->i = &j1;
        temp_l->score = root->score;
        temp_l->left = root->left;
        temp_l->right = NULL;

        struct Interval j3 = {i.high,high_score};
        struct ITNode *temp_r;
        temp_r = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_r){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_r->i = &j3;
        temp_r->score = root->score;
        temp_r->left = NULL;
        temp_r->right = root->right;

        struct Interval j2 = {i.low,i.high};
        struct ITNode *temp_c;
        temp_c = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_c){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_c->i = &j2;
        temp_c->score = root->score + i.score;
        temp_c->left = temp_l;
        temp_c->right = temp_r;

        printf("%d,%d\n",temp_c->i->low,temp_c->i->high);
        return temp_c;

      //左が範囲から出ている場合
      }else if(i.low < low_score && i.high <= high_score){
        struct Interval j1 = {low_score,i.high};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = root->left;
        temp1->right = NULL;

        struct Interval j2 = {i.high,high_score};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->left = temp1;
        temp2->right = root->right;

        i.high = low_score;
        insert(root->left, i);

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      //完全に範囲を含んでいる場合
      }else if(i.low < low_score && i.high > high_score){
        int high = i.high;
        int low = i.low;
        root->score += i.score;

        i.high = low_score;
        i.low = low;
        printf("root is [%d,%d]\n",root->left->i->low,root->left->i->high);
        insert(root->left, i);
        i.high = high;
        i.low = high_score;
        printf("root is [%d,%d]\n",root->right->i->low,root->right->i->high);
        insert(root->right, i);

        printf("%d,%d\n",root->i->low,root->i->high);
        return root;

      //右が範囲から出ている場合
      }else if(i.low >= low_score && i.high > high_score){
        struct Interval j1 = {i.low,high_score};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = NULL;
        temp1->right = root->right;

        struct Interval j2 = {low_score,i.low};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->left = root->left;
        temp2->right = temp1;

        i.low = high_score;
        insert(root->left, i);

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      }else{
        printf("error in insert\n");
        printf("i:[%d,%d]root[%d,%d]\n",i.low,i.high,low_score,high_score);
      }

    return root;

}

//二つのインターバルが重なっているかの判定
Boolean doOVerlap(struct Interval i1, struct Interval i2)
{
    if (i1.low <= i2.high && i2.low <= i1.high)
        return TRUE;
    return FALSE;
}

// The main function that searches a given interval i in a given
// Interval Tree.
struct Interval *overlapSearch(struct ITNode *root, struct Interval i)
{
    //木が空の時
    if (root == NULL) return NULL;

    //与えられたインターバルがルートと被っていた時
    if (doOVerlap(*(root->i), i))
        return root->i;

    // If left child of root is present and score of left child is
    // greater than or equal to given interval, then i may
    // overlap with an interval is left subtree
    if (root->left != NULL && root->left->score >= i.low)
        return overlapSearch(root->left, i);

    // Else interval can only overlap with right subtree
    return overlapSearch(root->right, i);
}

void inorder(struct ITNode *root)
{

    if (root == NULL) return;

    printf("[%d,%d,%d]\n",root->i->low,root->i->high,root->score);

    inorder(root->left);

    inorder(root->right);
}


int main()
{
    // Let us create interval tree shown in above figure
    struct Interval ints[] = {{15, 20, 1}, {10, 30, 1}, {17, 19, 1},
        {5, 20, 1}, {12, 15, 1}, {30, 40, 1}
    };
    int n = sizeof(ints)/sizeof(ints[0]);
    struct ITNode *root = NULL;
    for (int i = 0; i < n; i++){
        root = insert(root, ints[i]);
        printf("root is [%d,%d]\n",root->i->low,root->i->high);
    }
    printf("----------------------------------------------------\n");
    inorder(root);
/*
    printf("Inorder traversal of constructed Interval Tree is\n");
    inorder(root);

    struct Interval x = {6, 7};

    printf("\nSearching for interval [%d,%d]",x.low,x.high);
    struct Interval *res = overlapSearch(root, x);
    if (res == NULL)
        printf("\nNo Overlapping Interval");
    else
        printf("\nOverlaps with [%d, %d]",res->low,res->high);
*/
    return 0;
}

/*
    struct ITNode *temp1;
    temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
    if(!temp1){
      printf("エラー：メモリの割り当てに失敗しました。");
      exit(-1);
    }
    temp1->i = &i;
    temp1->score = root->score + i.score;
    temp1->left = root;
    temp1->right = NULL;
    printf("11root[%d,%d]\n",root->i->low,root->i->high);
    printf("11temp[%d,%d]\n",temp1->left->i->low,temp1->left->i->high);
    return;
*/
/*
    //ルートノードに格納されているインターバルの小さい値を取得
    int low_score = root->i->low;
    int high_score = root->i->high;

    if(low_score >= i.high){
      root = insert(root->left, i);
      return root;
    }else if(high_score <= i.low){
      root = insert(root->right, i);
      return root;
    }

      //完全にインターバルが重なった時
      if(i.low == low_score && i.high == high_score){
        root->score += i.score;

        return root;

      //左側のインターバルが重なった時
      }else if(i.low == low_score && i.high < high_score){
        struct Interval j1 = {low_score,i.high};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = root->left;
        temp1->right = NULL;

        struct Interval j2 = {i.high,high_score};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->score = root->score;
        temp2->left = temp1;
        temp2->right = root->right;

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      //右側のインターバルが重なった時
    }else if(i.low > low_score && i.high == high_score){
        struct Interval j1 = {i.low,high_score};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = NULL;
        temp1->right = root->right;

        struct Interval j2 = {low_score,i.low};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->score = root->score;
        temp2->left = root->left;
        temp2->right = temp1;

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      //完全に範囲に含まれている場合
      }else if(i.low > low_score && i.high < high_score){
        struct Interval j1 = {low_score,i.low};
        struct ITNode *temp_l;
        temp_l = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_l){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_l->i = &j1;
        temp_l->score = root->score;
        temp_l->left = root->left;
        temp_l->right = NULL;

        struct Interval j3 = {i.high,high_score};
        struct ITNode *temp_r;
        temp_r = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_r){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_r->i = &j3;
        temp_r->score = root->score;
        temp_r->left = NULL;
        temp_r->right = root->right;

        struct Interval j2 = {i.low,i.high};
        struct ITNode *temp_c;
        temp_c = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp_c){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp_c->i = &j2;
        temp_c->score = root->score + i.score;
        temp_c->left = temp_l;
        temp_c->right = temp_r;

        printf("%d,%d\n",temp_c->i->low,temp_c->i->high);
        return temp_c;

      //左が範囲から出ている場合
      }else if(i.low < low_score && i.high <= high_score){
        struct Interval j1 = {low_score,i.high};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = root->left;
        temp1->right = NULL;

        struct Interval j2 = {i.high,high_score};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->left = temp1;
        temp2->right = root->right;

        i.high = low_score;
        insert(root->left, i);

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      //完全に範囲を含んでいる場合
      }else if(i.low < low_score && i.high > high_score){
        int high = i.high;
        int low = i.low;
        root->score += i.score;

        i.high = low_score;
        i.low = low;
        printf("root is [%d,%d]\n",root->left->i->low,root->left->i->high);
        insert(root->left, i);
        i.high = high;
        i.low = high_score;
        printf("root is [%d,%d]\n",root->right->i->low,root->right->i->high);
        insert(root->right, i);

        printf("%d,%d\n",root->i->low,root->i->high);
        return root;

      //右が範囲から出ている場合
      }else if(i.low >= low_score && i.high > high_score){
        struct Interval j1 = {i.low,high_score};
        struct ITNode *temp1;
        temp1 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp1){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp1->i = &j1;
        temp1->score = root->score + i.score;
        temp1->left = NULL;
        temp1->right = root->right;

        struct Interval j2 = {low_score,i.low};
        struct ITNode *temp2;
        temp2 = (struct ITNode *)malloc(sizeof(struct ITNode));
        if(!temp2){
          printf("エラー：メモリの割り当てに失敗しました。");
          exit(-1);
        }
        temp2->i = &j2;
        temp2->left = root->left;
        temp2->right = temp1;

        i.low = high_score;
        insert(root->left, i);

        printf("%d,%d\n",temp1->i->low,temp1->i->high);
        return temp1;

      }else{
        printf("error in insert\n");
        printf("i:[%d,%d]root[%d,%d]\n",i.low,i.high,low_score,high_score);
      }
*/
