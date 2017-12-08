#ifndef Header_Include
	#define Header_Include
	#include "header.h"
#endif

int main(){
	AdminMovie * AM = (AdminMovie *)malloc(sizeof(AdminMovie));
	AdminActor * AA = (AdminActor *)malloc(sizeof(AdminActor));
	AdminDirector * AD = (AdminDirector *)malloc(sizeof(AdminDirector));
	void (*fptr)(int) = ControlC;
	
	
	return 0;
}

void ControlC(int sig){
	char ch;
	printf("Get Interrupt Signal\n");
	printf("Do you want to exit myMovie program? (Y/N) ");
	ch = getchar();
	if(ch == 'Y' || ch == 'y')
		exit(0);
	else if(ch == 'N' || ch == 'n')
		printf("(movie) ");
}
