//Clientul ii transmite serverului un un nume de director,
//iar serverul ii retransmite clientului numarul total de bytes din toate fisierele din directorul respectiv

#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<wait.h>
#include<dirent.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>


int main(){

int pipe1[2];
int pipe2[2];
int pid;
char sir[20];

if(pipe(pipe1)<0)//descriptorii pipe-ului
{printf("eroare la creare pipe\n");
exit(1);}

if(pipe(pipe2)<0)//descriptorii pipe-ului
{printf("eroare la creare pipe\n");
exit(1);}


if((pid=fork())<0){
	printf("eroare la fork\n");
	exit(1);
}
if (pid==0){
	//SERVERUL=procesul fiu
	close(pipe1[0]);
	close(pipe2[1]);

	read(pipe2[0],sir,20);
	printf("SERVERUL\nAm primit de la client numele de director: %s\n", sir);

	char *cmd=(char*)malloc(sizeof(char)*30);
	strcpy(cmd,"du ");
	strcat(cmd,sir);
	FILE* f=popen(cmd,"r");
	int r;
	fscanf(f,"%d",&r);
	//printf("%d",r);
	write(pipe1[1],&r,sizeof(int));
	pclose(f);

	close(pipe1[1]);
	close(pipe1[0]);;
	exit(0);
}
else{	
	//CLIENTUL
	close(pipe2[0]);
	close(pipe1[1]);
	printf("CLIENTUL:\nDati un nume de director:");
	scanf("%s",sir);
	write(pipe2[1],sir,(strlen(sir)+1));
	int ra;

	read(pipe1[0],&ra,sizeof(int));
	printf("CLIENTUL:\nRaspunsul primit este: %d\n",ra);


	close(pipe2[1]);
	close(pipe1[0]);

}

return(0);

}



