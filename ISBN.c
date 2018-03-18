/*
ISBNを与え、グループ記号と出版社記号を解読して表示するプログラムを作成せよ。
ISBN10, ISBN13のいずれかに対応すること、
グループ記号は1桁のもの(日本など)が特定できること、
出版社記号は日本の主要出版社10社程度が特定できること、チェックディジットが適正でなければ無効コードと判定できること、を最低条件とする。
ISBN10/ISBN13両対応、2桁以上のグループ記号の特定、海外出版社の特定、などを実装すれば加点する。
例: 4797327928 → Japan, Softbank
*/

/*
<グループ(４桁目から)>                                       <出版社(５桁目から)>
0および1	英語                                                 ・02 朝日新聞社
2	フランス語（フランスは979-10も使用）                           ・04 角川書店
3	ドイツ語                                                    ・06 講談社
4	日本                                                       ・08 集英社
5	ロシア・旧ソビエト連邦（ロシア以外の国では他の記号も使用）          ・09 小学館
80	チェコ・スロバキア                                         ・15 早川書房
81	インド（93も使用）                                         ・314 紀伊國屋
82	ノルウェー                                                ・334 光文社
83	ポーランド                                                ・385 三省堂
84	スペイン                                                  ・569 PHP研究所
85	ブラジル                                                  ・575 双葉社
86	セルビア（共有）                                           ・620 毎日新聞社
87	デンマーク                                                ・621 丸善
88	イタリア語                                                ・8002 宝島社
89	大韓民国（979-11も使用）
90	オランダ語（94も使用）
91	スウェーデン
92	国際NGO・欧州連合
93	インド（81も使用）                     ＊上記のみ対応しています。そのほか日本の書籍なら下記の本を特定できます。
94	オランダ語（90も使用）                   ISBN10,13両方とも対応済み。ISBN13からISBN10への変換が可能。
*/
/*
・『君の膵臓を食べたい』(小説版)
ISBN10:4575519944
ISBN13:9784575519945

・『打ち上げ花火下から見るか？横から見るか？』
ISBN10:4575519944
ISBN13:9784041054888

・『僕は明日、昨日の君とデートする』
ISBN10:4800226104
ISBN13:9784800226105
恋愛小説がメインになってしまった。。。笑
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ISBN10 10
#define ISBN13 13

int k = 0; //関数GroupとJPNPublishでISBN10,13両方対応可能にするための変数。

char number[100];

char country[][50] = {"英語圏", "フランス語圏", "ドイツ語圏", "日本語圏", "ロシア語圏", "中国語圏",
                  "チェコ・スロバキア", "インド", "ノルウェー", "ポーランド", "スペイン", "ブラジル",
                   "セルビア", "デンマーク", "イタリア語", "大韓民国", "オランダ語", "スウェーデン", "国際NGO・欧州連合"};

char publish[][20] = {"朝日新聞", "角川書店", "講談社", "集英社", "小学館", "早川書房", "紀伊国屋",
                  "光文社", "三省堂", "PHP研究所", "双葉社", "毎日新聞社" ,"丸善", "宝島社"};

void Option1(); //1の時
void Option2(); //2の時

int CheckISBN10(char a[]); //ISBN10のチェックデジット
int CheckISBN13(char b[]); //ISBN13のチェックデジット

//以降二つの関数はISBN13を基準に作成したもの。(kで10に対応させた)
void Group(char c[]); //グループ記号の解読
void JPNPublish(char d[]); //出版社記号の解読

//以降の関数は特定の本について。
void kimisui();
void utiage();
void bokuasu();

int main(){
  char option;

  while(1){
    printf("\nコマンドを入力して下さい。\n1:ISBN13からISBN10への変換した時のチェックデジットの確認.\n2:ISBN10,13のどちらかを入力して判定.\n\n");
    scanf("%s", &option);

    if(option == '1'){ //1:ISBN13からISBN10への変換した時のチェックデジットの確認.
      Option1();
      break; //連続で調べたい時にonにするとい良い。
    }
    else if(option == '2'){ //2:ISBN10,13のどちらかを入力して判定.
      Option2();
      break; //連続で調べたい時にonにするとい良い。
    }
    else printf("有効なコマンドを入力して下さい。\n");
  }
  return 0;
}

void Option1(){
  char letter[20] = {};
  while(1){
    int flag = 0;
    printf("ISBN(13桁)を入力してください：");
    scanf("%s", number);

    for(int i = 0; number[i] != '\0'; i++){
      if(number[i] >= '0' && number[i] <= '9') flag++; //数字なら加算していく
      // printf("%c\n", number[i]);
    }
    if(flag == ISBN13){ //13桁の時
      if(CheckISBN13(number) == number[ISBN13-1]){
        k = 0;
        break;
      }
      else printf("検知不可能なISBN、または存在しないISBNです。再入力をお願いします。\n");
    }
    else printf("有効な桁数でありません。再入力をお願いします。\n");
  }
  for(int i = 0; i < ISBN10-1; i++){
    letter[i] = number[i+3];
    // printf("%c\n", letter[i]);
  }
  char last = CheckISBN10(letter);
  strcat(letter, &last);
  printf("変換の結果：");
  for(int j = 0; j < ISBN10; j++)
    printf("%c", letter[j]);
    printf("\n");
}

void Option2(){
  while(1){ //10や13桁でない時やチェックデジットが違うと再入力させる。
    int flag = 0; //正しい桁数で入力されてるか確認する。
    printf("ISBN(10桁か13桁)を入力してください：");
    scanf("%s", number);

    for(int i = 0; number[i] != '\0'; i++){
      if(number[i] >= '0' && number[i] <= '9') flag++; //数字なら加算していく
      // printf("%c\n", number[i]);
    }

    if(flag == ISBN10){ //10桁の時

      if(CheckISBN10(number) == number[ISBN10-1]){
        k = 3;
        break;
      }
      else printf("検知不可能なISBN、または存在しないISBNです。再入力をお願いします。\n");
    }
    else if(flag == ISBN13){ //13桁の時

      if(CheckISBN13(number) == number[ISBN13-1]){
        k = 0;
        break;
      }
      else printf("検知不可能なISBN、または存在しないISBNです。再入力をお願いします。\n");
    }
    else printf("有効な桁数でありません。再入力をお願いします。\n");
  }

  //ここに解読する関数を入れる。
  Group(number);
}


int CheckISBN10(char a[]){
  int sum = 0, sum2 = 0;

  for(int i = 0; i < ISBN10-1; i++) //モジュラス11 ウェイト10-2
    sum += (a[i]-'0') * (ISBN10-i); //数値から数字への変換
  sum2 += '0' + (11 - sum % 11);
  // printf("%d\n", sum);
  if(sum2 == 11) sum2 = 0;
  return sum2;
}

int CheckISBN13(char b[]){
  int sum = 0, sum2 = 0;

  for(int i = 0; i < ISBN13-1; i++){ //モジュラス10 ウェイト3・1
    if(i % 2 == 0) sum += (b[i]-'0');//数値から数字への変換
    else sum += (b[i]-'0') * 3;
    // printf("%c\n", b[i]);
  }
  // printf("%d\n", sum);
  sum2 = '0' +(10 - sum % 10);

  return sum2;
}

void Group(char c[]){ //[3]から
  if( (c[3-k] == '0') || (c[3-k] == '1') ) printf("%sの本です。\n", country[0]);

  else if(c[3-k] == '2') printf("%sの本です。\n", country[1]);

  else if(c[3-k] == '3') printf("%sの本です。\n", country[2]);

  else if(c[3-k] == '4'){ //日本
    printf("%sの本です。\n", country[3]);
    JPNPublish(c); //出版社の関数へ。
  }
  else if(c[3-k] == '5') printf("%sの本です。\n", country[4]);

  else if(c[3-k] == '6') printf("%sの本です。\n", country[5]);
  //二桁の国
  else if(c[3-k] == '8'){
    switch(c[4-k]){
      case '0': printf("%sの本です。\n", country[6]);
                break;
      case '1': printf("%sの本です。\n", country[7]);
                break;
      case '2': printf("%sの本です。\n", country[8]);
                break;
      case '3': printf("%sの本です。\n", country[9]);
                break;
      case '4': printf("%sの本です。\n", country[10]);
                break;
      case '5': printf("%sの本です。\n", country[11]);
                break;
      case '6': printf("%sの本です。\n", country[12]);
                break;
      case '7': printf("%sの本です。\n", country[13]);
                break;
      case '8': printf("%sの本です。\n", country[14]);
                break;
      case '9': printf("%sの本です。\n", country[15]);
                break;
      default : printf("このソースでは現在取り扱っておりません。\n");
    }
  }
  else if(c[3-k] == '9'){
    switch(c[4-k]){
      case '0':
      case '4': printf("%sの本です。\n", country[16]);
                break;
      case '7': printf("%sの本です。\n", country[17]);
                break;
      case '3': printf("%sの本です。\n", country[7]);
                break;
      default : printf("このソースでは現在取り扱っておりません。\n");
                break;
    }
  }
  else printf("このソースでは現在取り扱っておりません。\n");
}

void JPNPublish(char d[]){
  //接頭が0
  if(d[4-k] == '0'){
    switch(d[5-k]){
      case '2': printf("%s出版です。\n", publish[0]);
                break;
      case '4': printf("%s出版です。\n", publish[1]);
                if( (d[6-k] == '1') && (d[7-k] == '0') && (d[8-k] == '5') && (d[9-k] == '4') && (d[10-k] == '8') && (d[11-k] == '8') )
                  utiage();
                break;
      case '6': printf("%s出版です。\n", publish[2]);
                break;
      case '8': printf("%s出版です。\n", publish[3]);
                break;
      case '9': printf("%s出版です。\n", publish[4]);
                break;
      default : printf("このソースでは現在取り扱っておりません。\n");
                break;
    }
  }
  //接頭が1
  else if( (d[4-k] == '1') && (d[5-k] == '5') ) printf("%s出版です。\n", publish[5]);

  //接頭が3
  else if(d[4-k] == '3'){
    if( (d[5-k] == '1') && (d[6-k] == '4') ) printf("%s出版です。\n", publish[6]);
    else if( (d[5-k] == '3') && (d[6-k] == '4') ) printf("%s出版です。\n", publish[7]);
    else if( (d[5-k] == '8') && (d[6-k] == '5') ) printf("%s出版です。\n", publish[8]);
    else printf("このソースでは現在取り扱っておりません。\n");
  }
  //接頭が5
  else if(d[4-k] == '5'){
    if( (d[5-k] == '6') && (d[6-k] == '9') ) printf("%s出版です。\n", publish[9]);
    else if( (d[5-k] == '7') && (d[6-k] == '5') ){
      printf("%s出版です。\n", publish[10]);
      if( (d[7-k] == '5') && (d[8-k] == '1') && (d[9-k] == '9') && (d[10-k] == '9') && (d[11-k] == '4') )
        kimisui();
    }
    else printf("このソースでは現在取り扱っておりません。\n");
  }
  //接頭が6
  else if( (d[4-k] == '6') && (d[5-k] == '2') ){
    if(d[6-k] == '0') printf("%s出版です。\n", publish[11]);
    else if(d[6-k] == '1') printf("%s出版です。\n", publish[12]);
    else printf("このソースでは現在取り扱っておりません。\n");
  }
  else if( (d[4-k] == '8') && (d[5-k] == '0') && (d[6-k] == '0') && (d[7-k] == '2') )
    printf("%s出版です。\n", publish[13]);
    if( (d[8-k] == '2') && (d[9-k] == '6') && (d[10-k] == '1') && (d[11-k] == '0') )
      bokuasu();
  else printf("このソースでは現在取り扱っておりません。\n"); //ここに収束
}

void kimisui(){
  printf("\n『君の膵臓をたべたい』・住野よる\n"
          "ある日、高校生の僕は病院で一冊の文庫本を日おる。タイトルは「共病文庫」。"
          "それはクラスメイトである山内桜良が綴った、秘密の日記だった。"
          "そこには、彼女の余命が膵臓の病気により、もう幾ばくもないと書かれていて・・・"
          "きっとこのタイトルに涙する。\n"
          "本読んだけど、俺は普通に泣いた。あと小説と実写では少し話が違った。\n\n");
}

void utiage(){
  printf("\n『打ち上げ花火、下から見るか？横から見るか？』・岩井俊二\n"
          "「打ち上げ花火は横から見たら丸いのか、平べったいのか?」"
          "夏の花火大会の日、港町で暮らす典道は幼なじみと灯台に登って花火を横から見る約束をする。"
          "その日の夕方、密かに想いを寄せる同級生のなずなから突然「かけおち」に誘われる。"
          "なずなが母親に連れ戻されて「かけおち」は失敗し、二人は離れ離れに。彼女を取り戻すため、"
          "典道はもう一度同じ日をやり直すことを願うが・・・。繰り返す1日の果てに起こる、恋の奇跡の物語。\n"
          "『君の名は』と比べられて批判が多かった作品だが、比べるジャンルが違うので比較してる連中は何もわかってない。"
          "・・・がこの作品の話はそこまで面白くなかったし、「ん？」となる人も多かっただろう。\n\n");
}

void bokuasu(){
  printf("\n『僕は明日、昨日の君とデートする』・七月隆文\n"
          "京都の美大に通うぼくが一目惚れした女の子。"
          "高嶺の花に見えた彼女に意を決して声をかけ、交際にこぎつけた。"
          "気配り上手でさびしがりやな彼女には、ぼくが想像もできなかった大きな秘密が隠されていて・・・。"
          "「あなたの未来がわかるって言ったら、どうする?」"
          "奇跡の運命で結ばれた二人を描く、甘くせつない恋愛小説。"
          "彼女の秘密を知ったとき、きっと最初から読み返したくなる。\n"
          "この話は本当に好き。いろんな視点から三回は読み返した。互いに時の流れが違ってうまくいかないとこも切なくていい。"
          "京都の宝ヶ池というところが舞台で私の学校の近くなので、とても親近感的なものがあった。\n\n");
}
