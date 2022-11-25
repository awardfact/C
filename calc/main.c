#include <stdio.h>


struct expre{
   int number;
   int isEx;
   int isUse;
};



//void atoi(char *exp ,int *intArr , int *exArr , int *itop , int *exTop);
void atoi(char *exp ,struct expre *expArr, int *exTop);
int atoi2(int i);
void inStack(struct expre stack[], int *top ,  struct expre exArr[] , int extop);
int pope(struct expre *stack, int *top , int *n , int *e);
void pushe(struct expre *stack , int *top , int n , int e);
int rank(int ex);

int popi(int *stack, int *top );
void pushi(int *stack, int *top , int e);




void calc(struct expre *stack, int *top);


// push2, pop2는 수식전용 스택 
void pushi(int *stack, int *top , int e){

    int i = *top;
    i++;
    *(stack + i) = e;
    *top = i;
   //*(stack + ++*top) = e;
}


int popi(int *stack, int *top){
   if(*top < 0){
      return -1;
   }
   int i = *top;
   int result = *(stack + i);
   i--;
   *top = i;
   return result;
   //return *(stack + *top--);
}


// 스택 팝 함수 스택에 데이터 없으면 -1 리턴 아니면 그 데이터 리턴
int pope(struct expre *stack, int *top , int *n , int *e){
   if(*top < 0){
      return -1;
   }
   int i = *top;
   *n = (stack + i)->number;
   *e = (stack + i)->isEx;
   i--;
   *top = i;
   return 1;   
}

//스택 푸시 함수 e값을 스택에 넣는다 
void pushe(struct expre *stack , int *top , int n , int e){
   int i = *top;
   i++;
   (stack + i)->number =n;
   (stack + i)->isEx = e;
   *top = i;
   return;
}

void atoi(char *exp ,struct expre *expArr, int *exTop){
   



   int i, top = 0;
   int isInt = 0; 
   int intTmp = 0;
   int it = 0;
   int et = 0;



   for(i = 0; *(exp + i) != '\0'; i++){

      //공백제거 
      if(*(exp + i) == 32){
         continue;
      }

      // 문자가 숫자인 경우 숫자를 묶어서 넣음 
      if(*(exp + i) >= '0' &&  *(exp + i)  <= '9'){
         if(isInt){
            intTmp = intTmp * 10 + atoi2(*(exp + i));
            continue;
         }else if(*(exp + i) != '0'){
            isInt = 1;
            intTmp = atoi2(*(exp + i));
            continue;
         }else{
            isInt = 0;
            intTmp = 0;
            continue;
         }
      //문자가 숫자가 아닌경우 전 문자가 숫자인 경우 숫자를 배열에 넣고 문자를 배열에 넣음 
      }else{



         if(isInt == 1){
            (expArr + et)->number = intTmp;
            (expArr + et)->isEx = 0;
            isInt = 0;
            intTmp = 0;
            et += 1;
            
         }


         (expArr + et)->number =  *(exp + i); 
         (expArr + et)->isEx = 1;
         et  += 1;
      }
      
      


   }


   //마지막 문자가 숫자였던 경우 배열에 숫자 넣음 
   if(isInt && intTmp){
      (expArr + et)->number = intTmp;
      (expArr + et)->isEx = 0;
      et += 1;
   }  


   *exTop = et;

   return;
}

/*

void atoi(char *exp ,int *intArr , int *exArr , int *itop , int *exTop){



   



   int i, top = 0;
   int isInt = 0; 
   int intTmp = 0;
   int it = 0;
   int et = 0;



   for(i = 0; *(exp + i) != '\0'; i++){

      //공백제거 
      if(*(exp + i) == 32){
         continue;
      }

      // 문자가 숫자인 경우 숫자를 묶어서 넣음 
      if(*(exp + i) >= '0' &&  *(exp + i)  <= '9'){
         if(isInt){
            intTmp = intTmp * 10 + atoi2(*(exp + i));
            continue;
         }else if(*(exp + i) != '0'){
            isInt = 1;
            intTmp = atoi2(*(exp + i));
            continue;
         }else{
            isInt = 0;
            intTmp = 0;
            continue;
         }
      //문자가 숫자가 아닌경우 전 문자가 숫자인 경우 숫자를 배열에 넣고 문자를 배열에 넣음 
      }else{



         if(isInt == 1){
            *(intArr + it) = intTmp;
            isInt = 0;
            intTmp = 0;
            it += 1;
            
         }


         *(exArr + et) =  *(exp + i); 
         et  += 1;
      }
      
      


   }


   //마지막 문자가 숫자였던 경우 배열에 숫자 넣음 
   if(isInt && intTmp){
      *(intArr + it) = intTmp;
      it += 1;
   }  


   *itop = it;
   *exTop = et;

   return;
}
*/

int atoi2(int i){
   
   return i - 48;
}


int rank(int ex){

   if(ex == -1){
      return -1;
   }
   if(ex == 40 || ex == 41){
      return 4;
   }else if(ex == 42 || ex == 47 || ex == 37){
      return 3;
   }else if(ex == 43 || ex == 45){
      return 2;
   }


}

/*
1.단항 - +  
2. 괄호 ( )
3. 곱하기 나누기 나머지 
4. 더하기 빼기 
*/
void inStack(struct expre stack[], int *top ,  struct expre exArr[] , int extop){


   int i = 0;
   int j = 0;
   int before = 0;
   
   
   int open = 0;

   int eStack[1000];
   int etop = -1;
   
   int tmp = 0;
   int topTmp = *top;
 //  printf("숫자 : %d\n" , itop);
  // printf("문자 : %d\n" , extop);
   int erank  = 0;
   int erank2 = 0;
   printf("%d   asdasd\n", extop);
   for(i = 0; i < extop; i++){

       printf("%d \n", exArr[i].isEx);
      //수식인경우 
      if( exArr[i].isEx == 1 ){

         // 수식 스택이 비어있는경우 
         if(etop == -1){
             printf("etop : %d\n", etop);
            //수식을 푸시함 
            pushi(eStack , &etop , exArr[i].number);
            printf("etop : %d\n", etop);

         }else{

             printf("1\n");
            // 우선순위가 낮은 연산자를 만날떄까지 
            do{
               if(etop == -1){
                  break;
               }
               printf("%d  %d\n" ,  eStack[etop]  , exArr[i].number);
               erank = rank((exArr[i].number));
               erank2 = rank(eStack[etop]);

               printf("%d %d \n" , erank, erank2);
               //스택에 있는 연산자의 우선순위가 현재 연산자보다 같거나 크면 연산자를 pop한 후 출력, 현재 연산자는 스택에 push한다 
               if(erank <= erank2){
                  stack[topTmp].number = popi(eStack, &etop);
                  stack[topTmp].isEx = 1;

                  topTmp++;
               //pushi(eStack, &etop, (exArr + i)->number);
               // 현재 연산자가 우선순위가 더 크면 현재 연산자를 push한다 
               }else{
                  pushi(eStack, &etop, exArr[i].number);
                  break;
               }
            }while(erank > erank2);
         }
      }else{
      //숫자인경우 
          stack[topTmp].number = exArr[i].number;
          stack[topTmp].isEx = exArr[i].isEx;
         topTmp++;
      }
   //   push(stack , top , (exArr + i)->number, (exArr + i)->isEx);

   }


   
   do{
      tmp = popi(eStack, &etop);
      if(tmp != -1){
         stack[topTmp].number = popi(eStack, &etop);
         stack[topTmp].isEx = 1;
         topTmp++;
      }
   }while(tmp  != -1);


   /*
      while( popi(eStack, &etop)){


      
      (stack + topTmp)->number = popi(eStack, &etop);
      (stack + topTmp)->isEx = 1;
      topTmp++;
   }
   
   */



   *top = topTmp;

  // printf("==================================================\n");
   for(i = 0; i < topTmp; i++){
    //  printf("%d    %d \n" , (stack + i)->number, (stack + i)->isEx);
   }



}





void calc(struct expre *stack , int *top){

   int result = 0;
   int tmp = 0;
   int tmpN = 0;
   int tmpEx = 0;
   // 전에 수식인 경우 1
   int isEx = 0;
   //수식 
   int exp = 0;

   int beforeNum = 0;
   int beforeEx = 0;

   do{   
      tmp = pope(stack, top , &tmpN, &tmpEx);

      printf("%d  %d  %d\n", tmpN, tmpEx, tmp);
      //수식인 경우 

      if(tmp == -1){
         break;
      }
      if(tmpEx == 1){
         isEx = 1;
         
         beforeNum = tmpN;
         beforeEx = 1;
      //숫자인 경우 
      }else{
         isEx = 0;

         //이전것이 수식인 경우 
         if(beforeEx == 1){
            switch(beforeNum){
               //나머지
               case 37 :
               result %= tmpN;
               break;
               // 곱하기
               case 42 : 
               result *= tmpN;
               break;
               //더하기
               case 43 :
               result += tmpN;
               break;

               // 빼기
               case 45 :
               result -= tmpN;
               break;

               //나누기
               case 47 :
               result /= tmpN;
               break;
            }
            
         }else if(result == 0){
            result = tmpN;
         }
         beforeNum = tmpN;
         beforeEx = 0;
      }

   }while(tmp != -1);



   printf("result :  %d \n", result);


}

/*
계산기 프로그램 


*/
int main() {

   // 계산에 필요한 스택 표현식 스택의 탑 
   struct expre stack[10000];

   

   struct expre stackTmp[10000];

   //int intArr[1000]  = {0,};
   //int exArr[1000]  = {0,};
   char expression[1000];
   int top  = 0;
   int intTop = 0;
   int tmpTop  = 0;
   int i;


   //표현식을 입력받음
   scanf("%[^\n]s" , expression);


   //입력받은 문자열을 숫자로 변경하고 스택에 넣음 
   atoi(expression, stackTmp  , &tmpTop);
   inStack(stack, &top, stackTmp , tmpTop);
   //calc(stack , &top);


   
   return 0;
}