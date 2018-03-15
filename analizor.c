#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include<stdarg.h>
#define SAFEALLOC(var,Type)if((var=(Type*)malloc(sizeof(Type)))==NULL)err("not enough memory");
char *pch;
int line=1;
enum{ID,CT_INT,CT_REAL,CT_CHAR,CT_STRING,COMMA,SEMICOLON,
    LPAR,RPAR,LBRACKET,RBRACKET,LACC,RACC,ADD,SUB,MUL,DOT,AND,OR,ASSIGN,EQUAL,NOT,NOTEQ,LESS,LESSEQ,GREATER,GREATEREQ,DIV,BREAK,CHAR,DOUBLE,ELSE,FOR,IF,INT,REUTRN,STRUCT,VOID,WHILE,SWITCH,RETURN,END
};

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
Token *tokens;

void err(const char *fmt,...)
{
    va_list va;
    va_start(va,fmt);
    fprintf(stderr,"error:");
    vfprintf(stderr,fmt,va);
    fputc('\n',stderr);
    va_end(va);
    exit(-1);
}

void tkerr(const Token *tk,const char *fmt,...)
{
    va_list va;
    va_start(va,fmt);
    fprintf(stderr,"error in line %d:",tk->line);
    vfprintf(stderr,fmt,va);
    fputc('\n',stderr);
    va_end(va);
    exit(-1);
}



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

char *createString(char *s1,char *s2)
{
    char *s=(char*)malloc((s2-s1+1)*sizeof(char));
    strncpy(s,s1,s2-s1);
    s[s2-s1+1]='\0';
    return s;
}

int getNextTK()
{
    int s=0,n;
    char ch;
    char* pStartch;
    Token *tk;
    for(;;)
    {
        ch=*pch;
        printf("# %d %d(%d) \n",s,ch,ch);
        switch(s)
        {
            case 0:
                if(isalpha(ch)||ch == '_'){pStartch=pch;s=1;pch++;}
                else if(ch==' '||ch=='\t'||ch=='\n'||ch=='\r'){pch++;}
                else if(isdigit(ch)&&(ch>='1'&& ch<='9')){s=3;pch++;}
                else if(isdigit(ch)&& ch=='0'){s=5; pch++;}
                else if(ch=='\''){s=16;pch++;}
                else if(ch=='\"'){s=20;pch++;}
                else if(ch==','){s=23;pch++;}
                else if(ch==';'){s=24;pch++;}
                else if(ch=='('){s=25;pch++;}
                else if(ch==')'){s=26;pch++;}
                else if(ch=='['){s=27;pch++;}
                else if(ch==']'){s=28;pch++;}
                else if(ch=='{'){s=29;pch++;}
                else if(ch=='}'){s=30;pch++;}
                else if(ch=='+'){s=31;pch++;}
                else if(ch=='-'){s=32;pch++;}
                else if(ch=='*'){s=33;pch++;}
                else if(ch=='/'){s=34;pch++;}
                else if(ch=='.'){s=35;pch++;}
                else if(ch=='&'){s=54;pch++;}
                else if(ch=='|'){s=55;pch++;}
                else if(ch=='!'){s=38;pch++;}
                else if(ch=='='){s=39;pch++;}
                else if(ch=='<'){s=44;pch++;}
                else if(ch=='>'){s=47;pch++;}
                else if(ch==0){addtk(END);
                    return END;
                }
                else printf("eroare \n");
                break;
            case 1:if(isalnum(ch)||ch=='_'){pch++;}
                   else s=2;
                    break;
            case 2:n=pch -pStartch;
                  if(!memcmp(pStartch,"if",n)&&n==2)
                  {   (IF);
                      return IF;
                  }
                  else if(!memcmp(pStartch,"for",n)&&n==3)
                  {
                    addtk(FOR);
                    return FOR;
                  }
                  else if(!memcmp(pStartch,"while",n)&&n==5)
                  {
                      addtk(WHILE);
                      return WHILE;
                  }
                  else if(!memcmp(pStartch,"switch",n)&&n==6)
                  {
                    addtk(SWITCH);
                    return SWITCH;
                  }
                   else if(!memcmp(pStartch,"break",n)&&n==5)
                  {
                    addtk(BREAK);
                    return BREAK;
                  }
                   else if(!memcmp(pStartch,"double",n)&&n==6)
                  {
                    addtk(DOUBLE);
                    return DOUBLE;
                  }
                   else if(!memcmp(pStartch,"else",n)&&n==4)
                  {
                    addtk(ELSE);
                    return ELSE;
                  }
                   else if(!memcmp(pStartch,"char",n)&&n==4)
                  {
                    addtk(CHAR);
                    return CHAR;
                  }
                  else if(!memcmp(pStartch,"int",n)&&n==3)
                  {
                    addtk(INT);
                    return INT;
                  }
                   else if(!memcmp(pStartch,"return",n)&&n==6)
                  {
                    addtk(RETURN);
                    return RETURN;
                  }
                   else if(!memcmp(pStartch,"struct",n)&&n==6)
                  {
                    addtk(STRUCT);
                    return STRUCT;
                  }
                   else if(!memcmp(pStartch,"void",n)&&n==4)
                  {
                    addtk(VOID);
                    return VOID;
                  }
                  else
                  {
                  tk=addtk(ID);
                  tk->text=createString(pStartch,pch); 
                  return ID;
                  }
            case 3:if(isdigit(ch)){pch++;}
                 else if(ch=='.'){s=10;pch++;}
                 else if(ch=='e'||ch=='E'){s=12;pch++;}
                 else s=4;
                 break;
            case 4:addtk(CT_INT);
                return CT_INT;
            case 5:if(isalpha(ch) && ch=='x'){s=7;pch++;}
                   else if(ch>='0' && ch<='7'){s=6;pch++;}
                   else if(ch=='8'||ch=='9'){s=9;pch++;}
                   break;
            case 6:if(ch>='0' && ch<='7'){pch++;}
            else if(ch=='8'||ch=='9'){s=9;pch++;}
            else if(ch=='.'){s=10;pch++;}
            else if(ch=='e'||ch=='E'){s=12;pch++;}
            else s=4;
            break;
            case 7:if(isalnum(ch)){s=8;pch++;}
                break;
            case 8: if(isalnum(ch)){pch++;}
                   else s=4;
                   break;
            case 9:if(isdigit(ch)){pch++;}
            else if(ch=='.'){s=10;pch++;}
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
            case 15:addtk(CT_REAL);
                return CT_REAL;
            case 16:if(ch=='\\'){s=17;pch++;}
            else if(ch!='\\'||ch!='\''){s=18;pch++;}
            break;
            case 17:if(ch=='a'||ch=='b'||ch=='f'||ch=='n'||
            ch=='r'||ch=='t'||ch=='v'||ch=='\''||ch=='?'||ch=='\\'||ch=='0'||ch=='\"'){s=18;pch++;}
            break;
            case 18:if(ch=='\''){s=19;pch++;}
            break;
            case 19:addtk(CT_CHAR);
            return CT_CHAR;
            case 20:if(ch=='\\'){s=21;pch++;}
            else if(ch=='\"'){s=22;pch++;}
            else {s=20;pch++;}
            break;
            case 21:if(ch=='a'||ch=='b'||ch=='f'||ch=='n'||
            ch=='r'||ch=='t'||ch=='v'||ch=='\''||ch=='?'||ch=='\\'||ch=='0'||ch=='\"'){s=20;pch++;}
            break;
            case 22:addtk(CT_STRING);
            return CT_STRING;
            case 23:addtk(COMMA);
            return COMMA;
            case 24:addtk(SEMICOLON);
            return SEMICOLON;
            case 25:addtk(LPAR);
            return LPAR;
            case 26:addtk(RPAR);
            return RPAR;
            case 27:addtk(LBRACKET);
            return LBRACKET;
            case 28:addtk(RBRACKET);
            return RBRACKET;
            case 29:addtk(LACC);
           return LACC;
           case 30:addtk(RACC);
           return RACC;
           case 31:addtk(ADD);
           return ADD;
           case 32:addtk(SUB);
           return SUB;
           case 33:addtk(MUL);
           return MUL;
           case 34:if(ch=='/'){s=51;pch++;}
           else if(ch=='*'){s=52;pch++;}
           else s=50;
           break;
           case 35:addtk(DOT);
           return DOT;
           case 36:addtk(AND);
           return AND;
           case 37:addtk(OR);
           return OR;
           case 38:if(ch=='='){s=43;pch++;}
           else
           s=42;
           break;
           case 39:if(ch=='='){s=40;pch++;}
           else s=41;
           break;
           case 40:addtk(EQUAL);
           return EQUAL;
           case 41:addtk(ASSIGN);
           return ASSIGN;
           case 42:addtk(NOT);
           return NOT;
           case 43:addtk(NOTEQ);
           return NOTEQ;
           case 44:if(ch=='='){s=46;pch++;}
           else s=45;
           break;
           case 45:addtk(LESS);
           return LESS;
           case 46:addtk(LESSEQ);
           return LESSEQ;
           case 47:if(ch=='='){s=49;pch++;}
           else s=48;
           break;
           case 48:addtk(GREATER);
           return GREATER;
           case 49:addtk(GREATEREQ);
           return GREATEREQ;
           case 50:addtk(DIV);
           return DIV;
           case 51:if(ch=='\n'||ch=='\r'||ch!='\0')
           s=0;
           else
           pch++;
           break;
           case 52:if(ch=='*'){s=53;pch++;}
           else {pch++;}
           break;
           case 53:if(ch=='*'){pch++;}
           else if(ch=='/'){s=0;pch++;}
           else {s=52;pch++;}
           break;
           default:printf("stare netratata \n");
                return -1;
        }
    }
}


int main(int argc,char **argv)
{
    char fisier[1000];
    FILE *f;
    strcpy(fisier,argv[1]);
    char inbuff[30001];
    int n;
    if((f=fopen(fisier,"r"))==0)
    {
        printf("eroare la deschidere fisier \n");
        return -1;
    }
    n = fread(inbuff,1,30000,f);
    fclose(f);
    inbuff[n] = '\0';
    pch=&inbuff[0];
    while(getNextTK()!=END)
    {}
    /*for(Token *t=tokens;t!=NULL;t=t->next)
    {
        printf("")
    }*/
    return 0;
}
