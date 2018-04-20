/********************************************/
/*                  kpga.c                  */
/* ナップサック問題のGAによる求解プログラム */
/*                                          */
/********************************************/

/* ヘッダファイルのインクルード */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>//各変数の最大値、最小値が定義されている

/* 記号定数の定義 */
#define MAXVALUE 100; /* 重さと価値の最大値 */
#define N  30; /* 荷物の個数 */
#define WEIGHTLIMIT (N*MAXVALUE/4) /* 重量制限 */
#define POOLSIZE 30 /* プールサイズ */
#define LASTG 50 /* 打ち切り世代 */
#define MRATE 0.01 /* 突然変異の確率 */
#define SEED 32767 /* 乱数のシード*/
#define YES 1 /* YESに対応する整数値 */
#define NO 0 /* NOに対応する整数値 */

/* 関数プロトタイプの宣言 */
void initparcel(); /* 荷物の初期化 */
int  evalfit(int gene[]); /* 適応度の計算 */
void mating(int pool[POOLSIZE][N]
	    ,int ngpool[POOLSIZE*2][N]);/* 突然変異 */
void mutation(int pool[POOLSIZE][N]
	      ,int ngpool[POOLSIZE*2][N]);/* 交叉 */
void printp(int pool[POOLSIZE][N]); /* 結果出力 */
void initpool(int pool[int pool[POOLSIZE][N]]);/* 初期集団の生成 */
int rndn();/* n未満の乱数の生成 */
int notval(int v); /* 真理値の反転 */
int selectp(int roulette[POOLSIZE],int totalfitness);/* 親の選択 */
void crossing(int m[],int p[],int cl[],int c2[]);/* 特定の２染色体の交叉 */
void selecting(int ngpool[POOLSIZE*2][N]
	       ,pool[POOLSIZE][N]);/* 次世代の選択 */

/* 大域変数 */
int parcel[N][2]; /* 荷物 */

/*********************/
/*     main()        */
/*********************/
int main(int argc,char *argv[]){
  int pool[POOLSIZE][N]; /* 染色体プール */
  int ngpool[POOLSIZE*2][N]; /* 次世代染色体プール */
  int generation; /* 現在の世代数 */

  /* 乱数の初期化 */
  srand(SEED);
  
  /* 荷物の初期化 */
  initparcel();

  /* 打ち切り世代まで繰り返し */
  for(generation = 0; generation<LASTG;++generation){
    printf("%d世代\n",generation);
    mating(pool,ngpool);/* 交叉 */
    mutation(ngpool); /* 突然変異*/
    selecting(ngpool, pool);/* 次世代の選択*/
    printp(pool);/* 結果出力 */
  }

  return 0;
}


/******************************/
/*        initparcel()関数    */
/*        荷物の初期化        */
/******************************/
void initparcel(){
  int i=0;
  //scanfを用いて染色体情報の初期化を行う
  while((i<N) && (scanf("%d %d", &parcel[i][0], &parcel[i][1])) != EOF){
    ++i;
  }
}

/**************************/
/*      selecting()関数   */
/*      次世代の選択      */
/**************************/
void selecting(int ngpool[POOLSIZE*2][N]
	       ,int pool[POOLSIZE][N]){
  
}

