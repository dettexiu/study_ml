
/****************************************/
/*         qlearning                    */
/*   強化学習（Q学習）の例題プログラム  */
/*   迷路の探索を学習します             */
/*   使い方                             */
/****************************************/


/* ヘッダファイルのインクルード */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* 記号定数の定義 */
#define GENMAX  1000   /* 学習の繰り返し回数 */
#define NODENO    15   /* Q値のノード図 */
#define ALPHA      0.1 /* 学習係数 */
#define GAMMA      0.9 /* 割引率*/
#define EPSILON    0.3 /* 行動選択のランダム性を決定 */
#define SEED   32767   /* 乱数のシード */


/* 関数のプロトタイプ宣言 */
int rand100();      /* 0~100を返す乱数関数    */
int rand01();       /* 0または1を返す乱数関数 */
double rand1();     /* 0~1の実数を返す乱数関数*/

void printqvalue();                    /* Q値出力                    */
int selecta(int s,int qvalue[NODENO]); /* 行動選択のランダム性を決定 */
int updateq(int s,int qvalue[NODENO]); /* Q値更新                    */

//void toCSV(int qvalue[NODENO]);


/****************/
/*   main()関数 */
/****************/
int main(){
  int i;
  
  int s;      /* 状態 */
  int t;      /* 時刻 */
  int qvalue[NODENO]; /* Q値 */

  srand(SEED); /* 乱数の初期化 */
  return 0;
  
  /* Q値の初期化 -----------------------------------------------------*/
  for (i = 0;i<NODENO;++i){
    qvalue[i]=rand100();
  }
  printqvalue(qvalue);
  /*------------------------------------------------------------------*/

  //ラベル
  printf("Q1\tQ2\tQ3\tQ4\tQ5\tQ6\tQ7\tQ8\tQ9\tQ10\tQ11\tQ12\tQ13\tQ14\n");
  
  /* 学習の本体 */
  for (i=0;i<GENMAX;i++){//学習をGENMAX回行う
    s=0;                /* 行動の初期状態        */
    for (t=0;t<3;++t){  /* tを再下段まで繰り返す(階層は全部で３階) */

      /* 行動選択 */
      s=selecta(s,qvalue);

      /* Q値の更新 */
      qvalue[s]=updateq(s,qvalue);
      
    }
    /* Q値の出力 */
    printqvalue(qvalue);

    //CSV出力
    //toCSV(qvalue);
  }
  return 0;
  
}


/*****************************/
/*    update()関数           */
/*    Q値を更新する          */
/*****************************/
int updateq(int s,int qvalue[NODENO]){
  int qv;     /* 更新されるQ値           */
  int qmax;   /* 選択可能なQ値の最大値   */
  int reward; /* 報酬                    */

  if (s > 6){
    /* 最下段の場合 ---------------------------------------------*/
    if (s == 14){
      /* 報酬の付与*/
      reward=1000;
      qv = qvalue[s]+ALPHA*( reward - qvalue[s]);
     
      // }else if(s==11){
    /* 報酬の付与 */
      // reward=500;
      // qv = qvalue[s] + ALPHA*(reward - qvalue[s]);
    }else{
      /* 報酬なし */
      // 変わらなくなったら打ち止めという条件をつけるため
      qv = qvalue[s];
    }
    /*-----------------------------------------------------------*/
  }else{
    /* 最下段以外の場合 -----------------------------------------*/
    if((qvalue[2*s+1])>(qvalue[2*s+2])){
      qmax = qvalue[2*s+1];
    }else{
      qmax = qvalue[2*s+2];
    }
    qv = qvalue[s]+ALPHA*(GAMMA*qmax-qvalue[s]);
    /*-----------------------------------------------------------*/
    
  }

  return qv;
}

/**********************************/
/*     selecta()関数              */
/*    行動を選択する              */
/*--引数--------------------------*/
/* olds  :対象のノード番号        */
/* qvalue:各Q値の配列             */
/*--出力--------------------------*/
/* s     :遷移先のノード          */
/**********************************/
int selecta(int olds,int qvalue[NODENO]){
  int s;

  /* ε-qreedy法による行動選択 */
  if (rand1() < EPSILON){
    /* ランダムに行動 */
    if(rand01() == 0){
      s = 2*olds+1;
    }else{
      s=2*olds+2;
    }
  }else{
    /* Q値最大値を選択 */
    if ((qvalue[2*olds+1]) > (qvalue[2*olds+2])){
      s=2*olds+1; 
    }else{
      s=2*olds+2;
    }
  }
  return s;
}


/********************************/
/*    printqvalue()関数         */
/*    Q値を出力する             */
/*                              */
/*--備考------------------------*/
/*   出力するのは1~14の14個     */
/*   0番ノードは出力しない      */
/********************************/
void printqvalue(int qvalue[NODENO]){
  int i;
  
  /* Q値出力--------------------- */
  for(i=1;i<NODENO;++i){

    if(i == 3){
      printf("\n");
    }

    if(i == 7){
      printf("\n");
    }

    
    printf("%d\t",qvalue[i]);

  }
  printf("\n\n");
  /* ---------------------------- */
}


/* Q値をCSV出力 */
/*
void toCSV(int qvalue[NODENO]){
  int i;
  

  FILE* outputfile;
  // ファイルオープン
  outputfile=fopen("learndata.csv","rw");

  //Q値は最大５桁を想定
  char buf[5]="";
  
  char* txt="";
  
  txt="testです";
  
  //エラー時の処理
  if(outputfile == NULL){
    printf("cannot open\n");
    exit(1);
  }

  txt=
  
  //qvalue[1]〜[14]をファイルに出力
  fprintf(outputfile,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
	   ,qvalue[1],qvalue[2],qvalue[3],qvalue[4],qvalue[5],qvalue[6],qvalue[7],qvalue[8],qvalue[9],qvalue[10],qvalue[11],qvalue[12],qvalue[13],qvalue[14]);
  
  //ファイルクローズ
  fclose(outputfile);
}
*/

/********************************/
/*    rand1()関数               */
/* 0~1の実数を返す関数          */
/********************************/
double rand1(){
  /* 乱数の計算 */
  return (double)(rand()/RAND_MAX);
}

/********************************/
/*     rand01()関数             */
/*   0または1を返すランダム関数 */
/********************************/
int rand01(){
  int rnd;
  /*乱数の最大値を除く */
  while((rnd = rand())==RAND_MAX);

  /* 乱数の計算 */
  return (int)((double)rnd/RAND_MAX*2);
}

/********************************/
/*     rand100()関数            */
/*     0~100を返す関数          */
/********************************/
int rand100(){
  int rnd;
  /* 乱数の最大値を除く*/
  while((rnd = rand())==RAND_MAX);
  
  /* 乱数の計算 */
  return (int)((double)rnd/RAND_MAX*101);
}
