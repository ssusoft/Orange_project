#ifndef Header_Include
	#define Header_Include
	#include "header.h"
#endif

int main(){
	AdminMovie * AM = (AdminMovie *)malloc(sizeof(AdminMovie));
	AdminActor * AA = (AdminActor *)malloc(sizeof(AdminActor));
	AdminDirector * AD = (AdminDirector *)malloc(sizeof(AdminDirector));
	void (*fptr)(int) = ControlC;
	
	
	return 0;#ifndef Header_Include
	#define Header_Include
	#include "header.h"
#endif

int main(){
	AdminMovie * AM = (AdminMovie *)malloc(sizeof(AdminMovie));
	AdminActor * AA = (AdminActor *)malloc(sizeof(AdminActor));
	AdminDirector * AD = (AdminDirector *)malloc(sizeof(AdminDirector));
	void (*fptr)(int) = ControlC;
	StartProgram(AM, AD, AA);
	signal(SIGINT, fptr);
	while(1)
		CommandInput(AM, AD, AA);
	
	return 0;
}
	
	
***********************************************************************************************************

	
