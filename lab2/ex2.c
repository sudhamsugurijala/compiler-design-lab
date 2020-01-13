
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>

char relop[2]={'<','>'};
char arithop[5]={'+','-','*','/','%'};
char logicop[3]={'&','|','!'};
char keywords[32][12]={"auto","double","int","struct","break","else","long","switch",
	"case","enum","register","typedef","char","extern","return","union",
	"continue","for","signed","void","do","if","static","while",
	"default","goto","sizeof","volatile","const","float","short","unsigned"};

typedef struct {
	char name[10];
	char type[10];
	char value[10];
} sym_table;

int isseparator(char x)
{
	if(x==';'||x=='{'||x=='}'||x==','||x=='('||x==')') {
		return 1;
	}	
	else
		return 0;
}

int iskeyword(char word[]) {
	for(int i=0;i<32;i++)
		if(strcmp(word,keywords[i])==0)
			return 1;
	return 0;
}

int isrelop(char s) {
	for(int i=0;i<2;i++)
		if(s==relop[i])
			return 1;
	return 0;
}

int isarithop(char s) {	
	for(int i=0;i<5;i++)
		if(s==arithop[i])
			return 1;
	return 0;
}

int islogicop(char s) {	
	for(int i=0;i<3;i++)
		if(s==logicop[i])
			return 1;
	return 0;
}

void main() {

	int flag=0,size = 0;
	char file[20];
	printf("Enter file path: ");
	scanf("%s",file);
	FILE*fp=fopen(file,"r");
	char buf[200];

	sym_table *T = (sym_table *)malloc(0);

	size_t len = 0;
	char * line = NULL;
	int x=0,w=0,m;
	char word[20],w2[4],token[15];

	while ((getline(&line, &len, fp)) != -1)  {
		flag = 0;
		for(int x=0; x<strlen(line);) {
			w=0;
			word[w]=0;
			while(isalnum(line[x])>0||line[x]=='.')  	//get alnum as word
				word[w++]=line[x++];
			word[w]=0;
			if(strlen(word)>0&&iskeyword(word))	{	//kw
				strcpy(token,"KW");
				flag = 1;
				size++;
				T = (sym_table *)realloc(T,size*sizeof(sym_table));
				strcpy(T[size-1].type,word);
				printf("KW ");
			}
			else if(strlen(word)>0&&line[x]=='(') { // function call
				strcpy(token,"FC");
				printf("FC ");
				while(x<strlen(line)&&line[x++]!=')');
			}
			else if(strlen(word)>0) {
				for(m=0;m<strlen(word);m++)
					if(!isdigit(word[m])&&word[m]!='.')
						break;
				if(m==strlen(word)) {
					strcpy(token,"numconst");
					printf("numconst ");
					if(flag)
						strcpy(T[size-1].value,word);
				}
				else {
					strcpy(token,"ID"); // identifier
					if(flag)
						strcpy(T[size-1].name,word);
					printf("ID ");
				}
			}
			else if(isseparator(line[x])) {
				strcpy(token,"SP");
				printf("SP ");
				x++;
			}
			else if(isrelop(line[x])) {				//relop
				if(x+1<strlen(line)&&(isrelop(line[x+1])||line[x+1]=='=')) {
					x+=2;
					strcpy(token,"relop");
					printf("relop ");
				}
				else {
					x++;
					strcpy(token,"relop");
					printf("relop ");
				}
			}
			else if(line[x]=='/') {		// comment
				if(line[x+1]=='/') {
					strcpy(token,"comment");
					printf("comment ");
					break;	
				}
				else if(line[x+1]=='*') {
					while(strstr(line,"*/")==NULL)
						if ((getline(&line, &len, fp)) != -1);
					strcpy(token,"comment");
					printf("comment ");
					break;
				}
			}
			else if(x+1<strlen(line)&&line[x+1]=='='&&(line[x]=='='||line[x]=='!'))	{
				x+=2;							//relop	
				strcpy(token,"relop");
				printf("relop ");
			}
			else if(isarithop(line[x])) {					//arithop
				if(strcmp(token,"ID")==0||strstr(token,"const")!=NULL) {
					strcpy(token,"arithop");
					printf("arithop ");
				}
				else {
					if(line[x+1]=='-') {
						strcpy(token,"post dec");
						printf("post dec ");
						x++;
					}
					else if(line[x+1]=='+') {
						strcpy(token,"post inc");
						printf("post inc ");
						x++;
					}
				}
				x++;
			}
			else if(islogicop(line[x])) {			//logicop
				if(x+1<strlen(line)&&islogicop(line[x+1])) {	
					x+=2;
					strcpy(token,"logicop");
					printf("logicop ");
				}
				else {
					x++;
					strcpy(token,"logicop");
					printf("logicop ");
				}
			}
			else if(line[x]=='=') {				//assignment
				x++;
				strcpy(token,"assign");
				printf("assign ");
			}
			else if(line[x]=='\"') {
				while(++x<strlen(line)&&line[x]!='\"');
				strcpy(token,"string_const");
				x++;
				printf("string_const ");
			}
			else if(line[x]=='\''){
				while(++x<strlen(line)&&line[x]!='\'');
				strcpy(token,"char_const");
				x++;
				printf("char_const ");
			}
			else 
				x++;
		}
		printf("\n");
	}
	printf("\n Symbol Table: \n");
	printf("\n\tName\tType\tValue\n");
	for(x=0;x<size;x++) {
		printf("\t%s\t%s\t%s\n",T[x].name,T[x].type,T[x].value);
	}
}

