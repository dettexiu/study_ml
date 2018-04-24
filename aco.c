/***************************************************/
/*                 aco.c                           */
/* 蟻コロニー最適化法(aco:Ant Colony Optimization) */
/*  acoにより最適値を学習します                    */
/***************************************************/


/* ヘッダファイルのインクルード */
#include<stdio.h>
//#include<math.h>
#include<stdlib.h>

/* 記号定数の定義 */
#define NOA 10 /* 蟻の個体数(Nunber Of Ant)*/
#define ILIMIT 50 /* 繰り返しの回数 */
#define Q 3.0 /* フェロモン更新の定数 */
#define RHO 0.8 /* 蒸発の定数 */
#define STEP 10 /* 道のりのステップ数 */
#define EPSILON 0.15 /* 行動選択のランダム性の決定 */
#define SEED 32768


#define _CRT_SECURE_NO_WARNINGS

/* 関数のプロトタイプ宣言 */
void printp(double pheromone[2][STEP]);/* 表示 */
void printmstep(int mstep[NOA][STEP]);/* 蟻の挙動の表示 */
void walk(int cost[2][STEP]
	 ,double pheromone[2][STEP]
	 ,int mstep[NOA][STEP]
	  );/* 蟻を歩かせる */
void update(int cost[2][STEP]
	 ,double pheromone[2][STEP]
	 ,int mstep[NOA][STEP]
	    );/* フェロモンの更新 */

double rand1(); /* 0〜1の実数を返す乱数関数 */
int rand01(); /* 0または1を返す乱数関数 */

/************************************************/
/*        main()関数                            */
/************************************************/
int main(){

  //---------------------------------------------
  // 経路は8個の中間点(ステップ)があり,
  // ステップとステップの間の移動には
  // 2つの分岐ルートを選択できる。
  //---------------------------------------------
  
  int cost[2][STEP]={/* 各ステップのコスト(距離)*/
      {1,1,1,1,1,1,1,1,1,1},
      {5,5,5,5,5,5,5,5,5,5}
    };

  
  double pheromone[2][STEP]={
     {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
     {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}
  }; /* 各ステップのフェロモン量 */
  //ステップの分岐のフェロモン量を格納する。全20個。

  
  int mstep[NOA][STEP]={
                        {0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0},
			{0,0,0,0.0,0,0,0,0,0,0}
                       };
  
  int i; /* 繰り返し回数の制御 */

  /* 乱数の初期化*/
  srand(SEED);

  /* 最適化の本体 */
  for(i=0;i<ILIMIT;++i){

    printf("===============================================================\n");
    /* フェロモンの状態出力 */
    printf("学習%d回目:\n",i);
    printp(pheromone);

    /* 蟻を歩かせる */
    walk(cost,pheromone,mstep);

    /* フェロモンの更新 */
    update(cost,pheromone,mstep);
    
  }

  /* フェロモンの状態出力 */
  printf("%d:\n",i);
  printp(pheromone);


  return 0;
}


/*************************************/
/*          update関数               */
/*          フェロモンの更新         */
/*************************************/
void update(int cost[2][STEP]
	 ,double pheromone[2][STEP]
	 ,int mstep[NOA][STEP]){
  
  int m; /* 蟻の個体番号 */
  int lm; /* 移動距離*/
  int i,j;
  double sum_lm;/* 歩いた合計距離 */
  
  sum_lm=0.0;
  

  /* フェロモンの蒸発 */
  for (i=0;i<2;++i){
    for (j = 0 ;j<STEP;++j){
      pheromone[i][j] *= RHO;
    }
  }

  /* 蟻による上塗り */
  for (m=0;m<NOA;++m){
    lm = 0;
    
    /* 個体mの移動距離lmの計算 */
    for (i=0;i<STEP;++i){
      lm += cost[mstep[m][i]][i];
    }

    //フェロモンの上塗り
    for(i=0;i<STEP;++i){
      pheromone[mstep[m][i]][i] +=( Q * (1.0/lm)) ;
    }
    
    sum_lm += (double)lm; 
  }
  
  /* 蟻の歩いた平均距離を出力 */
  printf("\n");
  printf("蟻が歩いた平均距離:\n");
  printf("%lf\n",sum_lm/NOA);
}


/**********************************************/
/*               walk()関数                   */
/*              蟻を歩かせる                  */
/**********************************************/
void walk(int cost[2][STEP]
	 ,double pheromone[2][STEP]
	 ,int mstep[NOA][STEP]){
  int m; /* 蟻の個体番号 */
  int s; /* 蟻の現在ステップ位置 */


  //選択経路がおかしくなる原因→abs(絶対値をとる関数)
  
  for ( m=0; m<NOA ; ++m){
    for(s=0;s<STEP;++s){

      
	
      /* ε-グリーディ法 */
      if(rand1()<EPSILON){
	  //絶対値の取得関数
	  //型がint:abs
	  //型がdoube:fabs
	/* ランダムに移動 */
	mstep[m][s] = rand01();
	/* 差がほとんどない時 */
      }

      //絶対値が1e-9以下のとき
      //math.hを読むときは -lm
      else if(pheromone[0][s] - pheromone[1][s]<1e-9 &&
	      -(pheromone[0][s] - pheromone[1][s])>(-(1e-9))  ){
	mstep[m][s] = rand01();
	}
      else{
	/* フェロモン濃度により決定 */
	if(pheromone[0][s] > pheromone[1][s]){
	  mstep[m][s]=0;
	}else{
	  mstep[m][s]=1;
	}
      }
    }
  }
  printf("\n");
  /* 蟻の挙動の出力 */
  printmstep(mstep);
  
}

/**********************************************/
/*       printmstep()関数                         */
/*      蟻の挙動の表示                      */
/**********************************************/
void printmstep(int mstep[NOA][STEP]){
  int i,j;
  printf("各蟻の選択経路\n");
  for(i=0;i<NOA;++i){
    for(j=0;j<STEP;++j){
      printf("%d",mstep[i][j]);
    }
    printf("\n");
  }
}


/**********************************************/
/*       printp()関数                         */
/*      フェロモンの表示                      */
/**********************************************/
void printp(double pheromone[2][STEP]){
  int i,j;
  printf("フェロモンの付着状態:\n");
  for(i=0;i<2;++i){
    for(j=0;j<STEP;++j){
      printf("%4.2lf\t",pheromone[i][j]);
    }
    printf("\n");
  }
}


/*********************************************/
/*        ramd1()関数                        */
/*     0〜1の実数を返す乱数関数              */
/*********************************************/
double rand1(){
  /* 乱数の計算*/
  return ((double)rand()/RAND_MAX);
}


/*********************************************/
/*            rand01()関数                   */
/*      0または1を返す乱数関数               */
/*********************************************/
int rand01(){
  int rnd;

  /* 乱数の最大値を除く */
  while((rnd = rand()) == RAND_MAX);

  /* 乱数の計算 */
  return (int)((double)rnd/RAND_MAX*2);
}

