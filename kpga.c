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
#define MAXVALUE 100 /* 重さと価値の最大値 */
#define N  30 /* 荷物の個数 */
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
void mutation(int ngpool[POOLSIZE*2][N]);/* 交叉 */
void printp(int pool[POOLSIZE][N]); /* 結果出力 */
void initpool(int pool[POOLSIZE][N]);/* 初期集団の生成 */
int rndn();/* n未満の乱数の生成 */
int notval(int v); /* 真理値の反転 */
int selectp(int roulette[POOLSIZE],int totalfitness);/* 親の選択 */
void crossing(int m[],int p[],int cl[],int c2[]);/* 特定の２染色体の交叉 */
void selecting(int ngpool[POOLSIZE*2][N]
	       ,int pool[POOLSIZE][N]);/* 次世代の選択 */



/* 大域変数 */
int parcel[N][2];

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

  
  /* 初期集団の初期化 */
  initpool(pool);

  /* 打ち切り世代まで繰り返し */
  for(generation = 0; generation<LASTG;++generation){
    printf("\n-----------------------------------------------------------------------------/n");
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
/*
void initparcel(){
  int i=0;
  //scanfを用いて染色体情報の初期化を行う
  while((i<N) && (scanf("%d %d", &parcel[i][0], &parcel[i][1])) != EOF){
    ++i;
  }
}
*/
void initparcel(){
  int packageWeight[30]={65,39, 9,72,87,91,34,32,10,22 ,43,55,23,34,55,12,10, 5, 9,90 ,49,30,80,99,23,34,67,65,97,81};
  int packageValue[30]= {27,82,85,71,91,28,92,30, 1,45 ,30,21,34,89,95,14,13, 9,10,10 ,65,30,99,80,30,35,98,85,10,90};

  int i=0;
  for(i=0;i<N;i++){
    parcel[i][0]=packageWeight[i];
    parcel[i][1]=packageValue[i];
    
  }
}

/**************************/
/*      selecting()関数   */
/*      次世代の選択      */
/**************************/
void selecting(int ngpool[POOLSIZE*2][N]
	       ,int pool[POOLSIZE][N]){
  int i,j,c; /* 繰り返しの制御関数 */
  int totalfitness = 0;       /* 適応度の合計値 */
  int roulette[POOLSIZE*2];   /* 適応度を格納 */
  int ball;                   /* 玉（選択位置の数値）*/
  int acc = 0;                /* 適応度の積算値*/

  /* 選択を繰り返す */
  for(i=0;i<POOLSIZE;++i){
    /* ルーレットの作成 */
    totalfitness = 0;

    for(c=0;c<POOLSIZE*2;++c){
      roulette[c]=evalfit(ngpool[c]);
      /* 適応度の合計値を計算 */
      totalfitness += roulette[c];
    }

    /* 染色体を一つ選ぶ */
    ball=rndn(totalfitness);
    acc = 0;
    for(c=0;c<POOLSIZE*2;++c){
      acc+=roulette[c]; /* 適応度を積算 */
      if(acc > ball)break;/* 対応する遺伝子*/
    }

    /* 染色体のコピー */
    for(j=0;j<N;++j){
      pool[i][j] = ngpool[c][j];
    }
  }
}


/****************************/
/*      selectp()関数       */
/*      親の選択            */
/****************************/
int selectp(int roulette[POOLSIZE],int totalfitness){
  int i; /* 繰り返しの制御関数 */
  int ball; /* 玉（選択位置の数値） */
  int acc=0; /* 適応度の積算値 */

  ball = rndn(totalfitness);
  for(i=0;i<POOLSIZE;++i){
    acc+=roulette[i]; /* 適応度を積算 */
    if(acc > ball)break;
  }
  return i;
}


/*****************************/
/*      mating()関数         */
/*      交叉                 */
/*****************************/
void mating(int pool[POOLSIZE][N]
	    ,int ngpool[POOLSIZE*2][N]){
  int i; /* 繰り返しの制御関数 */
  int totalfitness = 0; /* 適応度の合計値 */
  int roulette[POOLSIZE]; /* 適応度の格納 */
  int mama,papa; /* 親の遺伝子の番号 */

  
  /* ルーレットの作成 */
  for(i=0;i<POOLSIZE;++i){
    roulette[i]=evalfit(pool[i]);
    /* 適応度の合計値を計算 */
    totalfitness +=roulette[i];
  }

  /* 選択と交叉を繰り返す */
  for(i=0;i<POOLSIZE;++i){
    do{
      /* 親の選択*/
      mama = selectp(roulette,totalfitness);
      papa = selectp(roulette,totalfitness);
    }while(mama == papa);

    /* 特定の２染色体の交叉 */
    crossing(pool[mama],pool[papa],ngpool[i*2],ngpool[i*2+1]);
  }
}


/*****************************/
/*       crossing()関数      */
/*   特定の２染色体の交叉    */
/*****************************/
void crossing(int m[],int p[],int c1[],int c2[]){
  int j=0; /*  繰り返しの制御関数 */
  int cp=0; /* 交叉する点 */

  /* 交叉点の決定 */
  //交叉点はランダム
  cp=rndn(N);
  
  /* 前半部分のコピー */
  for(j=0;j<cp;++j){
    c1[j]=m[j];
    c2[j]=p[j];
  }

  /* 後半部分のコピー */
  for(;j<N;++j){
    c2[j]=m[j];
    c1[j]=p[j];
  }
  
}


/******************************/
/*      evalfit()関数         */
/*      適応度の計算          */
/******************************/
int evalfit(int g[]){
  int pos;        /* 遺伝子座の指定 */
  int value = 0;  /* 評価値 */
  int weight = 0; /* 重量 */

  /* 各遺伝子を調べて重量と評価値を計算 */
  for(pos=0;pos<N;++pos){
    weight += (parcel[pos][0]*g[pos]);
    value += (parcel[pos][0]*g[pos]);
  }

  /* 致死遺伝子の処理 */
  //重さが重量制限以上なら評価値0
  if(weight >=WEIGHTLIMIT){
    value=0;
  }

  return value;
}


/***************************/
/*        printp()関数     */
/*        結果出力         */
/***************************/
void printp(int pool[POOLSIZE][N]){
  int i,j;     /* 繰り返しの制御関数 */
  int fitness=0; /* 適応度 */
  double totalfitness = 0.0; /* 適応度の合計値 */
  int elite,bestfit=0;/* エリート遺伝子の処理用変数 */

  for(i=0;i<POOLSIZE;++i){
    for(j=0;j<N;++j){
      printf("%ld",pool[i][j]);
    }

    fitness = evalfit(pool[i]);
    printf("\t%ld\n",fitness);
    if(fitness > bestfit){
      /* エリート解 */
      bestfit = fitness;
      elite = i;
    }

    totalfitness+=fitness;
  }

  /* エリート解の適応度を出力 */
  printf("%d\t%d\t",elite,bestfit);

  /* 平均の適応度を出力 */
  printf("%lf\n",totalfitness / POOLSIZE);
}


/***************************/
/*        initpool()関数   */
/*        初期集団の生成   */
/***************************/
void initpool(int pool[POOLSIZE][N]){
  int i,j;   /* 繰り返しの制御変数 */
  
  for(i=0;i<POOLSIZE;++i){
    for(j=0;j<N;++j){
      pool[i][j]=rndn(2);
    }
  }
}


/***************************/
/*     rndn()関数          */
/*     n未満の乱数の生成   */
/***************************/
int rndn(int l){
  int rndno;  /* 生成した乱数 */
  while((rndno=((double)rand()/RAND_MAX)*l)==l);
  return rndno;
}


/***************************/
/*    mutaion()関数        */
/*    突然変異             */
/***************************/
void mutation(int ngpool[POOLSIZE*2][N]){
  int i,j;/* 繰り返しの制御変数 */
  for(i=0;i<POOLSIZE*2;++i){
    for(j=0;j<N;++j){
      //MRATEの確率で突然変異を発生させる
      if((double)rndn(100)/100.0 <= MRATE){
	/* 反転の突然変異 */
	ngpool[i][j] = notval(ngpool[i][j]);
      }
    } 
  }
}


/****************************/
/*    notval()関数          */
/*    真理値の反転          */
/****************************/
int notval(int v){
  if(v==YES){
    return NO;
  }else{
    return YES;
  }  
}


