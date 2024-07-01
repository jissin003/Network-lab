#include<stdio.h>

int findminimum(int x,int y){
    if(x>y){
        return y;
    }
    else{
        return x;
    }

}

int main(){
int capacity,outgoing,drop=0,filled=0,sec,i,packet[50],minimum;
    printf("Enter the  Bucket capacity: ");
    scanf("%d",&capacity);
    printf("Outgoing rate: ");
    scanf("%d",&outgoing);
    printf("Enter the number of seconds you want to stimulate: ");
    scanf("%d",&sec);
    for(i=0;i<sec;i++){
        printf("Enter the incoming packet at second %d:  ",i+1);
        scanf("%d",&packet[i]);
    }
    printf("Sec|in|out|before|after|drop\n");
    
    for(i=0;i<sec;i++){
        filled +=packet[i];
        if(filled>capacity){
            drop=filled-capacity;
            filled=capacity;
        }
        minimum=findminimum(filled,outgoing);
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",i+1,packet[i],minimum,filled,filled-minimum,drop);
        filled=filled-minimum;
        drop=0;

    }

    //after the stimulation the draining of buckets
    //ivide bucket drain akiya mati so atilott onum add akenda
    //filled negative verilla becaouse of finding minimum function

    for(i=sec;filled!=0;i++){
        if(filled>capacity){
            drop=filled-capacity;
            filled=capacity;
        }
        minimum=findminimum(filled,outgoing);
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",i+1,0,minimum,filled,filled-minimum,drop);
        filled=filled-minimum;
        drop=0;
    }
return 0;

}