#ifndef Header_Include6
	#define Header_Include
	#include "header.h"
#endif


/**********************************************************************************************************************************************************/

#ifndef Header_Include6
	#define Header_Include
	#include "header.h"
#endif

void CommandInput(AdminMovie * AM, AdminDirector * AD, AdminActor * AA){
	char * command = (char *)malloc(sizeof(char) * 40);
	char * tmp = (char *)malloc(sizeof(char) * 10);
	char * search_string = (char *)malloc(sizeof(char) * 100);
	char ch;

	int i = 0;
	
	printf("(movie) ");
	scanf("%s", command);
	while(1){
	 	if(strcmp(command, "add") == 0){
			ch = getchar();
			switch(ch){
				case 'm' :
						  AddMovie(AM);
						  LinkMovieActor(AM, AA);
						  LinkMovieDirector(AM, AD);
						  break;
				case 'd' : 
						  AddDirector(AD);
						  LinkDirectorMovie(AD, AM);
						  break;
				case 'a' : 
						  AddActor(AA);
						  LinkActorMovie(AA, AM);
						  break;
				case ' ' : 
						  continue;
				default : printf("Input error\n");
						  break;
			}
		}
	/*
		else if(strcmp(command, "update") == 0){
			ch = getchar();
			switch(ch){
				case 'm' :
						  UpdateMovie(AM);
						  LinkMovieDirector(AM, AD); // need test
						  LinkActorMovie(AA, AM); // need test
						  break;
				case 'd' :
						  UpdateDirector(AD);
						  LinkDirectorMovie(AD, AM); // need test
						  break;
				case 'a' :
						  UpdateActor(AA);
						  LinkActorMovie(AA, AM); // need test
						  break;
				case ' ' :
						  continue;
				default : 
						  printf("Input error\n");
					      break;
			}
		}
	*/
		if(strcmp(command, "delete") == 0){
			getchar();
			ch = getchar();
			switch(ch){
				case 'm' :
						  DeleteMovie(AM);
						  LinkDirectorMovie(AD, AM);
						  LinkActorMovie(AA, AM);
						  break;
				case 'd' :
						  DeleteDirector(AD);
						  LinkMovieDirector(AM, AD);
						  break;
				case 'a' :
						  DeleteActor(AA); 
						  LinkMovieActor(AM, AA);
						  break;
				default :
						  continue;
			}
		}
		else if(strcmp(command, "print") == 0){
			getchar();
			ch = getchar();
			switch(ch){
				case 'm' :
					  PrintMovie(AM);
					  break;
				case 'd' :
					  PrintDirector(AD);
					  break;
				case 'a' :
					  PrintActor(AA);
					  break;
				
				default :
					  continue;
			}
		}
		
		else if(strcmp(command, "search") == 0){
			getchar(); // ' '
			ch = getchar();
			if(ch == '-'){
				scanf("%s", tmp); // option
				getchar(); // ' '
				scanf("%[^\n]", search_string);
				for(int i=0; i<strlen(tmp); ++i){
					if(*(tmp + i) == 'm')
						SearchMovie(AM, search_string);
					if(*(tmp + i) == 'd')
					SearchDirector(AD, search_string);
					else if(*(tmp + i) == 'a')
						SearchActor(AA, search_string);
				}
			}
			else{
				scanf("%[^\n]", search_string+1);
				*(search_string) = ch;			
				SearchMovie(AM, search_string);
				SearchDirector(AD, search_string);
				SearchActor(AA, search_string);
			}
			printf("@@Done\n");
		}

		else if(strcmp(command, "sort") == 0){
			getchar();
			ch = getchar();
			switch(ch){
				case 'm' :
						  SortMovie(AM);
						  break;
				case 'd' :
						  SortDirector(AD);
						  break;
				case 'a' :
						  SortActor(AA);
						  break;
				case ' ' :
						  continue;
				default : 
						  printf("Input error\n");
					      break;
				}
		}

		else if(strcmp(command, "save") == 0){
			getchar();
			ch = getchar(); // m d a

			switch(ch){
				case 'm' :
						  SaveMovie(AM); // AM, option, file_name
						  break;

				case 'd' :
						  SaveDirector(AD);
						  break;

				case 'a' :
						  SaveActor(AA);
						  break;

				case ' ' :
						  continue;

				default : 
						  printf("Input error\n");
					      break;
				}
		}
		else if(strcmp(command, "end") == 0){
			getchar();
			printf("Do you save modified informain to file? (Y / N) ");
			ch = getchar();

			if(ch == 'Y' || ch == 'y'){
				WriteMovieList(AM);
				WriteDirectorList(AD);
				WriteActorList(AA);
				free(command);
				free(tmp);
				free(search_string);
				putchar('\n');
				exit(0);
			}
	
			else if (ch == 'N' || ch == 'n'){
				free(command);
				free(tmp);
				free(search_string);
				putchar('\n');
				exit(0);
			}
		}
		fflush(stdin);
		break;	
	}
	putchar('\n');
	free(command);
	free(tmp);
	free(search_string);
}

/**********************************************************************************************************************************************************/

