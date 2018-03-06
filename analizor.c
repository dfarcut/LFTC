#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define SAFEALLOC(var,Type)if((var=(Type*)malloc(sizeof(Type)))==NULL)err("not enough memory);
char *pch;
enum{ID, END, CT_INT,ASSIGN, SEMICOLON...};
typedef struct _Token{
    int code;
    union{
        char *text;
        long int i;
        double r;
    };
    int line;
    struct _Token *next;
}Token;
Token *lastToken;

Token *addtk(int code)
{
    Token *tk;
    SAFEALLOC(tk,Token);
    tk->code=code;
    tk->line=line;
    tk->next==NULL;
    if(lastToken)
    {
        lastToken->next=tk;
    }
    else
        tokens=tk;
    lastToken=tk;
    return tk;
}


int getNextTK()
{
    int s=0;
    char ch;
    for(;;)
    {
        ch=*pch;
        switch(s)
        {
            case 0:
                if(isalpha(ch)||ch == '_'){s=1;pch++;}
                else if(ch==' '||ch=='\t'||ch=='\n'||ch=='\r'){pch++;}
                else if(isdigit(ch)&&(ch>='1'&& ch<='9')){s=3;pch++;}
                else if(isdigit(ch)&& ch=='0'){s=5; pch++;}
                else if(ch=='\''){s=16;pch++;}
                else printf("eroare \n");
                break;
            case 1:if(isalnum(ch)||ch=='_'){pch++;}
                   else s=2;
                    break;
            case 2:addtk(ID);
                    return ID;
            case 3:if(isdigit(ch)){pch++;}
                 else if(ch=='.'){s=10;pch++;}
                 else if(ch=='e'||ch=='E'){s=12;pch++;}
                 else s=4;
                 break;
            case 4:addtk(CT_INT)
                return CT_INT;
            case 5:if(isalpha(ch) && ch=='x'){s=7; pch++;}
                   else if(ch>='0' && ch<='7'){s=6;pch++;}
                   else if(ch=='8'||ch=='9'){s=9;pch++;}
                   break:
            case 6:if(ch>='0' && ch<='7'){pch++;}
            else if(ch=='8'||ch=='9'){s=9;pch++;}
            else if(ch=='.'){s=10'pch++;}
            else if(ch=='e'||ch=='E'){s=12;pch++;}
            else s=4;
            break;
            case 7:if(isalnum(ch)){s=8;pch++;}
                break;
            case 8: if(isalnum(ch)){pch++;}
                   else s=4;
                   break;
            case 9:if(isdigit(ch)){pch++;}
            else if(pch=='.'){s=10;pch++;}
            else if(ch=='e'||ch=='E'){s=12;pch++;}
            break;
            case 10:if(isdigit(ch)){s=11;pch++;}
            break;
            case 11:if(isdigit(ch)){s=12;pch++;}
            else s=15;
            break;
            case 12:if(ch=='-'||ch=='+'){s=13;pch++;}
            else s=13;
            break;
            case 13:if(isdigit(ch)){s=14;pch++;}
            break;
            case 14:if(isdigit(ch)){pch++;}
            else
            s=15;
            break;
            case 15:addtk(CT_REAL)
                return CT_REAL;
            default:printf("Stare netratata %d \n",s);
        }
    }
}


int main(int argc,char **argv)
{  
    char fisier;
    strcpy(fisier,argv[1]);
    char inbuff[30001];
    int n;
    if(fopen(fisier,'r')==0)
    {
        printf("eroare la deschidere fisier \n");
        return -1;
    }
    n = fread(inbuff,1,30000,fisier);
    inbuff[n] = '\0';
    pch=inbuff[0];
    while(getNextTK()!=END)
    {}
    return 0;
}
