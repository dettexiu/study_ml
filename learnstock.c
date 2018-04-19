
/*******************************************/
/*            learnstock                   */
/*  単純な帰納的学習の例題プログラム       */
/*          パターン学習器                 */
/* 100個の学習データを読み込んで、         */
/* 適合する10桁の2進数パターンを答えます   */
/*******************************************/

#include<stdio.h>
#include<stdlib.h>

/* 記号定数の定義 */
#define OK 1
#define NG 0
#define SETSIZE 10  /* 学習データセットの大きさ */
#define CNO 10       /* 学習データの桁数(10社分) */
#define GENMAX 10000 /* 解候補生成回数           */
#define SEED   32767 /* 乱数のシード             */

/*** 関数のプロトタイプの宣言 ***/
void readdata(int data[SETSIZE][CNO],int teacher[SETSIZE]);

void showdata(int data[SETSIZE][CNO],int teacher[SETSIZE]);

/* 学習データセットの読み込み */
int rand012();  /* 0、1または2を返す乱数関数*/
int calcscore(int data[SETSIZE][CNO],int teacher[SETSIZE]
	      ,int answer[CNO]);
/* 解候補パターンのスコアの計算 */
/********************************/

/*****************/
/*   main()関数  */
/*****************/
int main()

{
  int i,j;
  int score =0;           /* スコア(0〜SETSIZE) */
  int answer[CNO];        /* 解候補             */
  int data[SETSIZE][CNO]; /* 学習データセット   */
  int teacher[SETSIZE];   /* 教師データ         */
  int bestscore = 0;      /* スコアの最良値     */
  int bestanswer[CNO];    /* 探索途中での最良解 */

  srand(SEED); /* 乱数の初期化 */

  /* 学習データセットの読み込み */
  readdata(data,teacher);

  //配列のデータを表示(テスト用)-------
  showdata(data,teacher);
  //-----------------------------------

  /* 解候補生成と検査 */
  for (i = 0; i < GENMAX ; ++i){
    
    /* 解候補作成 */
    //学習データセットと同じ桁数の解候補データを作成
    for(j = 0 ; j<CNO ; ++j){
      answer[j] = rand012();
    }
    
    /* 検査 */
    score=calcscore(data,teacher,answer);
    
    /* 最良スコアの更新 */
    if (score > bestscore){
      for(j=0; j<CNO;++j){
        bestanswer[j]=answer[j];
      }

      bestscore = score;

      for(j=0;j<CNO;++j){
	      printf("%d",bestanswer[j]);
      }
    }
    printf(":score=%d\n",score);
  
  }

  /* 最良解の出力 */
  printf("\n最良解\n");
  for(j=0;j<CNO;++j){
    printf("%d",bestanswer[j]);
  }

  printf(":score=%d\n",bestscore);

  return 0;
}



/*********************************************/
/*            calcscore()関数                */
/*    学習データセットの読み込み             */
/*********************************************/

int calcscore(int data[SETSIZE][CNO],int teacher[SETSIZE],int answer[CNO]){
  int score = 0;/* スコア       */
  int point;    /* 一致した桁数 */
  int i,j;

  for(i=0;i<SETSIZE;++i){
    /* 一致計算*/
    point = 0;
    
    //各桁を調べ一致していたらポイント追加
    for (j=0;j<CNO;++j ){
      if(answer[j]==2){
	      ++point;/*ワイルドカード*/
      }else if(answer[j]==data[i][j]){
	      ++point;/* 一致 */
      }
    }

    if((point == CNO) && (teacher[i]==1)){
	      ++score;
      }else if((point != CNO) && (teacher[i]==0)){
	      ++score;
      }
  }
  return score;
}

/***********************************************/
/*           readdata()関数                    */
/* 学習データセットの読み込み                  */
/***********************************************/
/*
void readdata(int data[SETSIZE][CNO],int teacher[SETSIZE])
{
  int i,j;
  for(i=0;i<SETSIZE;++i){
    for(j=0;j<CNO;++j){
      scanf("%d",&data[i][j]);
    }
    scanf("%d",&teacher[i]);
  }
}
*/


void readdata(int data[SETSIZE][CNO],int teacher[SETSIZE]){
  int i,j;
  
  int ldata[SETSIZE][CNO]=
    {
      { 1,0,0,0,0,0,1,0,0,1},	
      { 0,1,0,1,0,1,1,1,0,1},	
      { 0,1,0,0,0,1,1,0,1,0},
      { 1,0,0,1,1,0,1,0,0,1},	
      { 1,0,0,1,1,0,1,1,1,1},	
      { 0,0,0,0,0,0,1,1,0,0},	
      { 1,1,1,1,0,0,1,1,0,1},
      { 0,1,1,1,0,1,1,1,0,1},
      { 0,0,1,1,0,1,1,1,0,0},	
      { 1,0,0,0,1,0,1,0,1,1},
    };

    int tdata[SETSIZE]=
    {1
    ,1
    ,0
    ,1
    ,0
    ,1
    ,0
    ,0
    ,0
     ,0};

    
  for(i=0;i<SETSIZE;++i){
    for(j=0;j<CNO;++j){
      data[i][j]=ldata[i][j];
    }
    teacher[i]=tdata[i];
  }
  
}

//配列に格納された情報を表示
void showdata(int data[SETSIZE][CNO],int teacher[SETSIZE]){
  int i,j;
  for(i=0;i<SETSIZE;++i){
    for(j=0;j<CNO;++j){

      if(j==CNO-1){
	printf("%d\t%d\n", data[i][j],teacher[i]);
      }else{
	printf("%d,", data[i][j]);
      }
    }
  }
}


/************************************************/
/*            rand012()関数                     */
/*    0,1または2を返す乱数関数                  */
/************************************************/
int rand012(){
  int rnd;

  /* 乱数の最大値を除く */
  while((rnd =rand()) == RAND_MAX);
  
  /* 乱数の計算 */
  return (double)rnd / RAND_MAX * 3;
}
