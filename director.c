#ifndef Header_Include
	#define Header_Include
	#include "header.h"
#endif

int director_serial = 0;

/*****************************************************************************************************************************************/
int CreateDirectorList(AdminDirector * AD) {

	AD->head = (NextDirector *)malloc(sizeof(NextDirector));
	AD->tail = (NextDirector *)malloc(sizeof(NextDirector));

	if (AD->head == NULL || AD->tail == NULL)
		return FALSE;

	AD->head->next_director_node = AD->tail;
	AD->tail->next_director_node = AD->tail;
	AD->size = 0;

	AD->head->director_name = NULL;
	AD->head->best_movie_name = NULL;
	AD->head->birth = NULL;

	return TRUE;
}
/*****************************************************************************************************************************************/
int ReadDirectorlog(AdminDirector * AD) { // this function read movie_log.txt
	FILE * fptr_director_log;
	fptr_director_log = fopen("direc.txt", "rt");

	char * tag = (char *)malloc(100);
	char ch;
	while ((ch = fgetc(fptr_director_log)) != EOF){
		fseek(fptr_director_log, -1, SEEK_CUR);
		fscanf(fptr_director_log, "%[^:]:", tag);
		if (strcmp(tag, "add") == 0)
			FileAddDirectorList(AD, fptr_director_log); // add

		else if (strcmp(tag, "update") == 0)
			FileUpdateDirectorList(AD, fptr_director_log); // update

		else if (strcmp(tag, "delete") == 0) // delect
			FileDeleteDirectorList(AD, fptr_director_log);
	}
	free(tag);
	fclose(fptr_director_log);
	return TRUE;
}
/*****************************************************************************************************************************************/

int FileAddDirectorList(AdminDirector * AD, FILE * fptr_director_log){
	NextDirector * newp = (NextDirector *)malloc(sizeof(NextDirector));
	NextDirector * btp = AD->head;
	NextNode * save_node;
	char * tmp = (char *)malloc(100);
	char ch;
	int i = 0;

	fscanf(fptr_director_log, "%d:", &(newp->serial_number)); // read serial_number
	if(director_serial < newp->serial_number)
		director_serial = newp->serial_number;

	fscanf(fptr_director_log, "%[^:]:", tmp); // read director_name
	tmp = ColonCheckInFILE(tmp, "??;", ":");
	newp->director_name = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->director_name, tmp);

	fscanf(fptr_director_log, "%c:", &(newp->sex)); // read sex

	fscanf(fptr_director_log, "%[^:]:", tmp); // read birth
	tmp = ColonCheckInFILE(tmp, "??;", ":");
	newp->birth = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->birth, tmp);

	newp->best_movie_name = (NextNode *)malloc(sizeof(NextNode));
	save_node = newp->best_movie_name;

	while(1){
		ch = fgetc(fptr_director_log);
		
		if(ch == ',' || ch == '\n' || ch == EOF){
			*(tmp+i) = '\0';
			if(*tmp == ' ')
				tmp = tmp + 1;
			newp->best_movie_name->name = (char *)malloc(sizeof(strlen(tmp) + 1));
			strcpy(newp->best_movie_name->name, tmp);
			newp->best_movie_name->my_node = NULL;

			if(ch == ','){
				i = 0;
				newp->best_movie_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				newp->best_movie_name = newp->best_movie_name->next_node;
			}
			else if (ch == '\n' || ch == EOF){
				newp->best_movie_name->next_node = NULL;
				break;
			}
		}
		else{
			*(tmp + i) = ch;
			++i;
		}
	}
	newp->best_movie_name = save_node;
	free(tmp);
	while(1){
		if(btp->next_director_node == AD->tail)
			break;
		btp = btp->next_director_node;
	}
	
	btp->next_director_node = newp;
	newp->next_director_node = AD->tail;
	++AD->size;
	return TRUE;
}
/*****************************************************************************************************************************************/

int FileUpdateDirectorList(AdminDirector * AD, FILE * fptr_director_log){
	int serial_num;
	NextDirector * match_director_node;
	NextNode * destroy_node;
	NextNode * save_node;
	NextNode * cur_node;

	int i = 0;
	char ch, sub;
	char * tmp = (char *)malloc(100);
	fscanf(fptr_director_log, "%d:", &serial_num);
	match_director_node = SearchDirectorSerial(AD, serial_num);

	while(1){
		ch = fgetc(fptr_director_log);
		if(ch == ':')
			++i;
		else if(ch == '=')
			continue;
		else if(ch == EOF || ch == '\n')
			break;

		else{
			fseek(fptr_director_log, -1, SEEK_CUR);
			if(i == 0){
				free(match_director_node->director_name);
				fscanf(fptr_director_log, "%s", tmp);
				tmp = ColonCheckInFILE(tmp, "??;", ":");
				match_director_node->director_name = (char *)malloc(strlen(tmp) + 1);
				strcpy(match_director_node->director_name, tmp);
			}
			else if (i == 1)
				fscanf(fptr_director_log, "%c", &(match_director_node->sex));
			else if (i == 2){
				free(match_director_node->birth);
				fscanf(fptr_director_log, "%s", tmp);
				tmp = ColonCheckInFILE(tmp, "??;", ":");
				match_director_node->birth = (char *)malloc(strlen(tmp) + 1);
				strcpy(match_director_node->birth, tmp);
			}
			else if (i == 3){
				destroy_node = match_director_node->best_movie_name;
				while(1){

					if(destroy_node -> next_node!= NULL)
						cur_node = destroy_node->next_node;
					else
						break;
					
					free(destroy_node->name);
					free(destroy_node);
					destroy_node = cur_node;
				}
				free(destroy_node->name);
				free(destroy_node);
				// clear destroy
				
				match_director_node->best_movie_name = (NextNode *)malloc(sizeof(NextNode));
				save_node = match_director_node->best_movie_name;

				while(1){
					fscanf(fptr_director_log, "%[^,\n]", tmp);
					if(*tmp == ' ')
						tmp = tmp + 1;
					match_director_node->best_movie_name->name = (char *)malloc(strlen(tmp)+1);
					strcpy(match_director_node->best_movie_name->name, tmp);
					
					sub = fgetc(fptr_director_log);
					if(sub == ','){
						match_director_node->best_movie_name->next_node = (NextNode *)malloc(sizeof(NextNode));
						match_director_node->best_movie_name = match_director_node->best_movie_name->next_node;
					}
					else if (sub == '\n' || sub == EOF){
						match_director_node->best_movie_name->next_node = NULL;
						break;
					}
				}
				match_director_node->best_movie_name = save_node;
				break;
			}
		}
	}
	free(tmp);
	return TRUE;
}
/*****************************************************************************************************************************************/

int FileDeleteDirectorList(AdminDirector * AD, FILE * fptr_director_log){
	int serial_num;
	NextNode * destroy_node;
	NextNode * cur_node;
	NextDirector * match_director_node;
	NextDirector * btp = AD->head->next_director_node;
	char ch;

	fscanf(fptr_director_log, "%d:", &serial_num);
	match_director_node = SearchDirectorSerial(AD, serial_num);

	while(1){
		if(btp->next_director_node == match_director_node){
			btp->next_director_node = match_director_node->next_director_node;
			break;
		}

		else if(btp->next_director_node == AD->tail){
			printf("serial : %d\n", btp->serial_number);
			return FALSE;
		}

		else{
			btp = btp->next_director_node;
		}
	}

	destroy_node = match_director_node->best_movie_name;
	while(1){
		if(destroy_node->next_node != NULL)
			cur_node = destroy_node->next_node;
		else
			break;
		free(destroy_node->name);
		free(destroy_node);
		destroy_node = cur_node;
	}
	free(destroy_node->name);
	free(destroy_node);
	free(match_director_node);

	while(1){
		ch = fgetc(fptr_director_log);
		if(ch == '\n' || ch == EOF)
			break;
	}
	--AD->size;
	return TRUE;
}
/*****************************************************************************************************************************************/
NextDirector * SearchDirectorSerial(AdminDirector * AD, int serial_num){
	NextDirector * match_director_node = AD->head->next_director_node;
	while(1){
		if(match_director_node->serial_number == serial_num){
			return match_director_node;
		}
		else if(match_director_node->next_director_node == AD->tail){
			return NULL;
		}
		else
			match_director_node = match_director_node->next_director_node;
	}
}
/*****************************************************************************************************************************************/
int SaveDirector(AdminDirector * AD){
	FILE * fptr_director_list = fopen("director_list.save","wt");
	NextDirector * ptr_director_node = AD->head->next_director_node;
	NextNode * ptr_best_movie_name = ptr_director_node->best_movie_name;

	char * option = (char *)malloc(sizeof(char) * 10);
	char * tmp = (char *)malloc(sizeof(char) * 10);
	char * file_name = (char *)malloc(sizeof(char) * 100);
	int check;
	int option_check = 0;
	char ch;
	
	if(getchar() == '\n'){
		check = 0;
	}

	else{
		if(scanf("%s", option) == 1){ // nsbm  or -f
	
			if(strcmp(option, "-f") == 0){ // option is -f
				getchar(); // ' '
				scanf("%[^\n]", file_name);
				check = 1; // no option, yes file_name
			}

			else{ // option is tgdyra
				if(getchar() == ' '){
					scanf("%s", tmp); // -f
					getchar();
					scanf("%[^\n]", file_name);
					check = 2; // yes option, yes file_name
				}
				else
					check = 3; // yes option, no file_name
			}	
		}
	}

	if(check == 0 || check == 1){ // no option
		while(1){
			if(ptr_director_node != AD->tail){
				fprintf(fptr_director_list, "Name : %s\n", ptr_director_node->director_name); 
				fprintf(fptr_director_list, "Sex : %c\n", ptr_director_node->sex);
				fprintf(fptr_director_list, "Birth : %s\n", ptr_director_node->birth);
				fprintf(fptr_director_list, "Best Movie : ");

				while(1){
					if(ptr_best_movie_name->next_node == NULL){
						fprintf(fptr_director_list, "%s\n", ptr_best_movie_name->name);
						break;
					}
					fprintf(fptr_director_list, "%s, ", ptr_best_movie_name->name);
					ptr_best_movie_name = ptr_best_movie_name->next_node;
				}
				ptr_director_node = ptr_director_node->next_director_node;
				ptr_best_movie_name = ptr_director_node->best_movie_name;
				fprintf(fptr_director_list, "\n");
			}
			else
				break;
		}
	}

	else{ // yes option
		while(1){
			if(ptr_director_node == AD->tail)
				break;
			
			while(1){
				ch = *(option + option_check);
				if(ch == 'n')
					fprintf(fptr_director_list, "Name : %s\n", ptr_director_node->director_name);
				else if(ch == 's')
					fprintf(fptr_director_list, "Sex : %c\n", ptr_director_node->sex);
				else if(ch == 'b')
					fprintf(fptr_director_list, "Birth : %s\n", ptr_director_node->birth);
				else if(ch == 'm'){
					ptr_best_movie_name = ptr_director_node->best_movie_name;
					fprintf(fptr_director_list, "Best Movie : ");
				
					while(ptr_best_movie_name->next_node != NULL){
						fprintf(fptr_director_list, "%s, ", ptr_best_movie_name->name);
						ptr_best_movie_name = ptr_best_movie_name->next_node;
					}
					fprintf(fptr_director_list, "%s\n", ptr_best_movie_name->name);
				}
				else if(ch == '\0'){
					option_check = 0;
					break;
				}
				++option_check;
			}
			ptr_director_node = ptr_director_node->next_director_node;
			fprintf(fptr_director_list, "\n");
		}
	}
	fclose(fptr_director_list);
	free(option);
	if(check == 1 || check == 2){
		rename("director_list.save", file_name);
		free(file_name);
		free(tmp);
	}
	else{
		free(file_name);
		free(tmp);
	}
	printf("@@Done\n");
	return TRUE;
}
/*****************************************************************************************************************************************/
int SortDirector(AdminDirector * AD){
	FILE * fptr_director_list;

	NextDirector ** dptr_director = (NextDirector **)malloc(sizeof(NextDirector *) * AD->size);
	NextDirector * ptr_director_node = AD->head->next_director_node;
	NextNode * ptr_best_movie_name = ptr_director_node->best_movie_name;
	
	char ** sort_string_arr = (char **)malloc(sizeof(char *) * AD->size);
	char * tmp = (char *)malloc(sizeof(char) * 100);
	char * file_name = (char *)malloc(sizeof(char) * 100);
	char option;
	int check;
	
	if(getchar() == '\n'){
		check = 0;
		option = 'z';
	}

	else{
		option = getchar(); // t g d y r a or -f

		if(option == '-'){
			getchar(); // f
			getchar(); // ' '
			scanf("%[^\n]", file_name);
			check = 1; // no option, yes file_name
		}

		else{ // option is tgdyra
			if(getchar() == ' '){
				scanf("%s", tmp); // -f
				getchar();
				scanf("%[^\n]", file_name);
				check = 2; // yes option, yes file_name
			}
			else{
				check = 3; // yes option, no file_name
			}
		}	
	}

	if(check == 0 || check == 3){ // no file_name
		if(option == 's'){
			ptr_director_node = AD->head->next_director_node;
			while(1){
				if(ptr_director_node == AD->tail)
					break;
				else if(ptr_director_node->sex == 'M' || ptr_director_node->sex == 'm'){
					printf("Name : %s\n", ptr_director_node->director_name);
					printf("Sex : %c\n", ptr_director_node->sex);
					printf("Birth : %s\n", ptr_director_node->birth);
					printf("Best Movie : ");
					ptr_best_movie_name = ptr_director_node->best_movie_name;
					while(1){
						if(ptr_best_movie_name->next_node == NULL){
							printf("%s\n", ptr_best_movie_name->name);
							break;
						}
						printf("%s, ", ptr_best_movie_name->name);
						ptr_best_movie_name = ptr_best_movie_name->next_node;
					}
					putchar('\n');
				}
				ptr_director_node = ptr_director_node->next_director_node;
			}
			ptr_director_node = AD->head;
			while(1){
				if(ptr_director_node == AD->tail)
					break;
				else if(ptr_director_node->sex == 'F' || ptr_director_node->sex == 'f'){
					printf("Name : %s\n", ptr_director_node->director_name);
					printf("Sex : %c\n", ptr_director_node->sex);
					printf("Birth : %s\n", ptr_director_node->birth);
					printf("Best Movie : ");
					ptr_best_movie_name = ptr_director_node->best_movie_name;
					while(1){
						if(ptr_best_movie_name->next_node == NULL){
							printf("%s\n", ptr_best_movie_name->name);
							break;
						}
						printf("%s, ", ptr_best_movie_name->name);
						ptr_best_movie_name = ptr_best_movie_name->next_node;
					}
					putchar('\n');
				}
				ptr_director_node = ptr_director_node->next_director_node;
			}
		}
		else if(option == 'b'){
			for(int i = 0; i < AD->size ; ++i){
				*(sort_string_arr + i) = ptr_director_node->birth;
				ptr_director_node = ptr_director_node->next_director_node;
			}

			qsort((void *)sort_string_arr, AD->size, sizeof(*sort_string_arr), CompareString);
			
			for(int i = 0; i < AD->size; ++i){
				ptr_director_node = AD->head->next_director_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_director_node->birth) == 0){
						*(dptr_director + i) = ptr_director_node;
						break;
					}
					else
						ptr_director_node = ptr_director_node->next_director_node;
				}
			}

			for(int i = 0; i < AD->size; ++i){
				printf("Name : %s\n", (*(dptr_director + i))->director_name);
				printf("Sex : %c\n", (*(dptr_director + i))->sex);
				printf("Birth : %s\n", (*(dptr_director + i))->birth);
				printf("Best Movie : ");
				ptr_best_movie_name = (*(dptr_director + i))->best_movie_name;
				while(1){
					if(ptr_best_movie_name->next_node == NULL){
						printf("%s\n", ptr_best_movie_name->name);
						break;
					}
					printf("%s, ", ptr_best_movie_name->name);
					ptr_best_movie_name = ptr_best_movie_name->next_node;
				}
				putchar('\n');
			}
		}
		else if(option == 'm'){
			for(int i = 0; i < AD->size ; ++i){
				*(sort_string_arr + i) = ptr_director_node->best_movie_name->name;
				ptr_director_node = ptr_director_node->next_director_node;
			}

			qsort((void *)sort_string_arr, AD->size, sizeof(sort_string_arr[0]), CompareString);
			
			for(int i = 0; i < AD->size; ++i){
				ptr_director_node = AD->head->next_director_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_director_node->best_movie_name->name) == 0){
						*(dptr_director + i) = ptr_director_node;
						break;
					}
					else
						ptr_director_node = ptr_director_node->next_director_node;
				}
			}

			for(int i = 0; i < AD->size; ++i){
				printf("Name : %s\n", (*(dptr_director + i))->director_name);
				printf("Sex : %c\n", (*(dptr_director + i))->sex);
				printf("Birth : %s\n", (*(dptr_director + i))->birth);
				printf("Best Movie : ");
				ptr_best_movie_name = (*(dptr_director + i))->best_movie_name;
				while(1){
					if(ptr_best_movie_name->next_node == NULL){
						printf("%s\n", ptr_best_movie_name->name);
						break;
					}
					printf("%s, ", ptr_best_movie_name->name);
					ptr_best_movie_name = ptr_best_movie_name->next_node;
				}
				putchar('\n');
			}
		}
		else if(option == 'z' || option == 'n'){
			for(int i = 0; i < AD->size ; ++i){
				*(sort_string_arr + i) = ptr_director_node->director_name;
				ptr_director_node = ptr_director_node->next_director_node;
			}

			qsort((void *)sort_string_arr, AD->size, sizeof(sort_string_arr[0]), CompareString);
			
			for(int i = 0; i < AD->size; ++i){
				ptr_director_node = AD->head->next_director_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_director_node->director_name) == 0){
						*(dptr_director + i) = ptr_director_node;
						break;
					}
					else
						ptr_director_node = ptr_director_node->next_director_node;
				}
			}

			for(int i = 0; i < AD->size; ++i){
				printf("Name : %s\n", (*(dptr_director + i))->director_name);
				printf("Sex : %c\n", (*(dptr_director + i))->sex);
				printf("Birth : %s\n", (*(dptr_director + i))->birth);
				printf("Best Movie : ");
				ptr_best_movie_name = (*(dptr_director + i))->best_movie_name;
				while(1){
					if(ptr_best_movie_name->next_node == NULL){
						printf("%s\n", ptr_best_movie_name->name);
						break;
					}
					printf("%s, ", ptr_best_movie_name->name);
					ptr_best_movie_name = ptr_best_movie_name->next_node;
				}
				putchar('\n');
			}
		}
		else{
			printf("Option Error\n");
			return FALSE;
		}
	}
	else if(check == 1 || check == 2){ // yes file_name
		fptr_director_list = fopen(file_name ,"wt");
		
		if(option == 's'){
			ptr_director_node = AD->head->next_director_node;
			while(1){
				if(ptr_director_node == AD->tail)
					break;
				else if(ptr_director_node->sex == 'M' || ptr_director_node->sex == 'm'){
					fprintf(fptr_director_list, "Name : %s\n", ptr_director_node->director_name);
					fprintf(fptr_director_list, "Sex : %c\n", ptr_director_node->sex);
					fprintf(fptr_director_list, "Birth : %s\n", ptr_director_node->birth);
					fprintf(fptr_director_list, "Best Movie : ");
					ptr_best_movie_name = ptr_director_node->best_movie_name;
					while(1){
						if(ptr_best_movie_name->next_node == NULL){
							fprintf(fptr_director_list, "%s\n", ptr_best_movie_name->name);
							break;
						}
						fprintf(fptr_director_list, "%s, ", ptr_best_movie_name->name);
						ptr_best_movie_name = ptr_best_movie_name->next_node;
					}
					fputc('\n', fptr_director_list);
				}
				ptr_director_node = ptr_director_node->next_director_node;
			}
			ptr_director_node = AD->head;
			while(1){
				if(ptr_director_node == AD->tail)
					break;
				else if(ptr_director_node->sex == 'F' || ptr_director_node->sex == 'f'){
					fprintf(fptr_director_list, "Name : %s\n", ptr_director_node->director_name);
					fprintf(fptr_director_list, "Sex : %c\n", ptr_director_node->sex);
					fprintf(fptr_director_list, "Birth : %s\n", ptr_director_node->birth);
					fprintf(fptr_director_list, "Best Movie : ");
					ptr_best_movie_name = ptr_director_node->best_movie_name;
					while(1){
						if(ptr_best_movie_name->next_node == NULL){
							fprintf(fptr_director_list, "%s\n", ptr_best_movie_name->name);
							break;
						}
						fprintf(fptr_director_list, "%s, ", ptr_best_movie_name->name);
						ptr_best_movie_name = ptr_best_movie_name->next_node;
					}
					fputc('\n', fptr_director_list);
				}
				ptr_director_node = ptr_director_node->next_director_node;
			}
		}
		
		else if(option == 'b'){
			for(int i = 0; i < AD->size ; ++i){
				*(sort_string_arr + i) = ptr_director_node->birth;
				ptr_director_node = ptr_director_node->next_director_node;
			}

			qsort((void *)sort_string_arr, AD->size, sizeof(sort_string_arr[0]), CompareString);
			
			for(int i = 0; i < AD->size; ++i){
				ptr_director_node = AD->head->next_director_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_director_node->birth) == 0){
						*(dptr_director + i) = ptr_director_node;
						break;
					}
					else
						ptr_director_node = ptr_director_node->next_director_node;
				}
			}

			for(int i = 0; i < AD->size; ++i){
				fprintf(fptr_director_list, "Name : %s\n", (*(dptr_director + i))->director_name);
				fprintf(fptr_director_list, "Sex : %c\n", (*(dptr_director + i))->sex);
				fprintf(fptr_director_list, "Birth : %s\n", (*(dptr_director + i))->birth);
				fprintf(fptr_director_list, "Best Movie : ");
				ptr_best_movie_name = (*(dptr_director + i))->best_movie_name;
				while(1){
					if(ptr_best_movie_name->next_node == NULL){
						printf("%s\n", ptr_best_movie_name->name);
						break;
					}
					printf("%s, ", ptr_best_movie_name->name);
					ptr_best_movie_name = ptr_best_movie_name->next_node;
				}
				putchar('\n');
			}
		}
//
		else if(option == 'm'){
			for(int i = 0; i < AD->size ; ++i){
				*(sort_string_arr + i) = ptr_director_node->best_movie_name->name;
				ptr_director_node = ptr_director_node->next_director_node;
			}

			qsort((void *)sort_string_arr, AD->size, sizeof(sort_string_arr[0]), CompareString);
			
			for(int i = 0; i < AD->size; ++i){
				ptr_director_node = AD->head->next_director_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_director_node->best_movie_name->name) == 0){
						*(dptr_director + i) = ptr_director_node;
						break;
					}
					else
						ptr_director_node = ptr_director_node->next_director_node;
				}
			}

			for(int i = 0; i < AD->size; ++i){
				fprintf(fptr_director_list, "Name : %s\n", (*(dptr_director + i))->director_name);
				fprintf(fptr_director_list, "Sex : %c\n", (*(dptr_director + i))->sex);
				fprintf(fptr_director_list, "Birth : %s\n", (*(dptr_director + i))->birth);
				fprintf(fptr_director_list, "Best Movie : ");
				ptr_best_movie_name = (*(dptr_director + i))->best_movie_name;
				while(1){
					if(ptr_best_movie_name->next_node == NULL){
						printf("%s\n", ptr_best_movie_name->name);
						break;
					}
					printf("%s, ", ptr_best_movie_name->name);
					ptr_best_movie_name = ptr_best_movie_name->next_node;
				}
				putchar('\n');
			}
		}

		else if (option == 'z' || option == 'n'){
			for(int i = 0; i < AD->size ; ++i){
				*(sort_string_arr + i) = ptr_director_node->director_name;
				ptr_director_node = ptr_director_node->next_director_node;
			}

			qsort((void *)sort_string_arr, AD->size, sizeof(sort_string_arr[0]), CompareString);
			
			for(int i = 0; i < AD->size; ++i){
				ptr_director_node = AD->head->next_director_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_director_node->director_name) == 0){
						*(dptr_director + i) = ptr_director_node;
						break;
					}
					else
						ptr_director_node = ptr_director_node->next_director_node;
				}
			}

			for(int i = 0; i < AD->size; ++i){
				printf("Name : %s\n", (*(dptr_director + i))->director_name);
				printf("Sex : %c\n", (*(dptr_director + i))->sex);
				printf("Birth : %s\n", (*(dptr_director + i))->birth);
				printf("Best Movie : ");
				ptr_best_movie_name = (*(dptr_director + i))->best_movie_name;
				while(1){
					if(ptr_best_movie_name->next_node == NULL){
						printf("%s\n", ptr_best_movie_name->name);
						break;
					}
					printf("%s, ", ptr_best_movie_name->name);
					ptr_best_movie_name = ptr_best_movie_name->next_node;
				}
				putchar('\n');
			}
		}
		else{
			printf("Option Error\n");
			return FALSE;
		}
	}
	free(sort_string_arr);
	free(tmp);
	free(file_name);
	free(dptr_director);
	printf("\n@@Done\n");
	return TRUE;
}
/*****************************************************************************************************************************************/
int DeleteDirector(AdminDirector * AD){
	FILE * fptr_director_log = fopen("direc.txt", "at");
	int serial_num;
	NextDirector * match_director_node;
	NextDirector * bptr_director_node = AD->head;
	NextNode * destroy_node;
	NextNode * curp;
	
	scanf("%d", &serial_num);
	match_director_node = SearchDirectorSerial(AD, serial_num);
	if(match_director_node == NULL){
		printf("No such record\n");
	   return FALSE;	
	}

	while(bptr_director_node->next_director_node != match_director_node)
		bptr_director_node = bptr_director_node->next_director_node;

	bptr_director_node->next_director_node = match_director_node->next_director_node;
	
	free(match_director_node->director_name);
	free(match_director_node->birth);
	destroy_node = match_director_node->best_movie_name;
	free(match_director_node);
	
	while(1){
		if(destroy_node->next_node == NULL){
			free(destroy_node->name);
			free(destroy_node);
			break;
		}
		curp = destroy_node->next_node;
		free(destroy_node->name);
		free(destroy_node);
		destroy_node = curp;
	}
	fprintf(fptr_director_log, "delete:%d::::", serial_num);
	fclose(fptr_director_log);
	--AD->size;
	printf("@@Done\n");
	return TRUE;
}

/*****************************************************************************************************************************************/
int SearchDirector(AdminDirector * AD, char * string){
	NextDirector * ptr_director_node = AD->head;
	NextNode * ptr_best_movie_name;

	int checksm = 0;
	int checkqm = 0;
	int len = strlen(string);
	char * serial_string = (char *)malloc(10);
	char * substr = (char *)malloc(strlen(string) + 1);
	strcpy(substr, string);

	for(int i = 0; i < len ; ++i){
		if(*(string + i) == '*')
			++checksm;

		else if(*(string + i) == '?')
			++checkqm;
	}

	if(checksm != 0 && checkqm == 0){ // *, SearchActorSM
		if(*string == '*'){ // first *
			substr = substr + 1;
			SearchStringDirector(AD, substr);
		}

		else if(*(string + len - 1) == '*'){ // last *
			*(substr + len - 1) = '\0';
			SearchStringDirector(AD, substr);
		}
	}

	else if (checksm == 0 && checkqm != 0){	// ?
		SearchDirectorQM(AD, substr);
	}
	
	else if (checksm != 0 && checkqm != 0){ // ? and *
		SearchDirectorQMSM(AD, substr);
	}
	
	else if (checksm == 0 && checkqm == 0){ // no
		SearchStringDirector(AD, substr);
	}

	free(serial_string);
	free(substr);
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchDirectorQMSM(AdminDirector * AD, char * substr){
	NextDirector * ptr_director_node = AD->head;
	NextNode * ptr_best_movie_name;
	char * procstr = (char *)malloc(strlen(substr) + 1);
	strcpy(procstr, substr);

	if(*procstr == '*' && *(procstr + strlen(procstr) - 1) == '*'){
		*(procstr + strlen(procstr) - 1) = '\0';
		procstr = procstr + 1;
		SearchDirectorQM(AD, procstr);
	}
	else if(*procstr == '*'){
		procstr = procstr + 1;
		SearchDirectorQM(AD, procstr);
	}
	else if(*(procstr + strlen(procstr) - 1) == '*'){
		*(procstr + strlen(procstr) - 1) = '\0';
		SearchDirectorQM(AD, procstr);
	}
	free(procstr);
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchDirectorQM(AdminDirector * AD, char * substr){
	NextDirector * ptr_director_node = AD->head;
	NextNode * ptr_best_movie_name;
	char * cmpstr = (char *)malloc(100);
	char * serial_string = (char *)malloc(10);
	int check;

	while(1){ // check actor_name
		ptr_director_node = ptr_director_node->next_director_node;

		if(ptr_director_node == AD->tail)
			break;
		else
			strcpy(cmpstr, ptr_director_node->director_name);
	
		while(1){	
			check = 0;

			for(int i = 0 ; i < strlen(substr) ; ++i){
				if(*(substr + i) == *(cmpstr + i))
					++check;
				else if(*(substr + i) == '?')
					++check;
				else
					break;
			}

			if(check == strlen(substr)){
				MatchDirectorPrint(ptr_director_node);
				break;
			}

			else
				cmpstr = cmpstr + 1;

			if(*(cmpstr + strlen(substr) - 1) == '\0')
				break;
		}
	}

	ptr_director_node = AD->head;
	
	while(1){ // check serial
		ptr_director_node = ptr_director_node->next_director_node;

		if(ptr_director_node == AD->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_director_node->serial_number);
		}

		while(1){
			check = 0;

			for(int i = 0 ; i < strlen(substr) ; ++i){
				if(*(substr + i) == *(serial_string + i))
					++check;
				else if(*(substr + i) == '?')
					++check;
				else
					break;
			}

			if(check == strlen(substr)){
				MatchDirectorPrint(ptr_director_node);
				break;
			}

			else
				serial_string = serial_string + 1;

			if(*(serial_string + strlen(substr) - 1) == '\0')
				break;
		}
	}

	ptr_director_node = AD->head;

	while(1){ // check birth
		ptr_director_node = ptr_director_node->next_director_node;

		if(ptr_director_node == AD->tail)
			break;
		else
			strcpy(cmpstr, ptr_director_node->birth);
	
		while(1){	
			check = 0;
			for(int i = 0 ; i < strlen(substr) ; ++i){
				if(*(substr + i) == *(cmpstr + i))
					++check;
				else if(*(substr + i) == '?')
					++check;
				else
					break;
			}
			if(check == strlen(substr)){
				MatchDirectorPrint(ptr_director_node);
				break;
			}
			cmpstr = cmpstr + 1;

			if(*(cmpstr + strlen(substr) - 1) == '\0')
				break;
		}
	}
	ptr_director_node = AD->head;

	while(1){ // sex
		ptr_director_node = ptr_director_node->next_director_node;
		
		if(strlen(substr) != 1 || ptr_director_node == AD->tail)
			break;

		else{
			*cmpstr = ptr_director_node->sex;
			*(cmpstr + 1) = '\0';
		}

		if(*cmpstr == *substr)
			MatchDirectorPrint(ptr_director_node);
		else if(*substr == '?')
			MatchDirectorPrint(ptr_director_node);
	}

	ptr_director_node = AD->head;
	
	while(1){ // check best_movie_name
		ptr_director_node = ptr_director_node->next_director_node;
		ptr_best_movie_name = ptr_director_node->best_movie_name;

		if(ptr_director_node == AD->tail)
			break;
	
		else{
			while(1){

				if(ptr_best_movie_name->next_node == NULL){
					strcpy(cmpstr, ptr_best_movie_name->name);	
					while(1){	
						check = 0;
						for(int i = 0 ; i < strlen(substr) ; ++i){
							if(*(substr + i) == *(cmpstr + i))
								++check;
							else if(*(substr + i) == '?')
								++check;
							else
								break;
						}
						if(check == strlen(substr)){
							MatchDirectorPrint(ptr_director_node);
							break;
						}
						cmpstr = cmpstr + 1;

						if(*(cmpstr + strlen(substr) - 1) == '\0')
							break; // while 3을 탈출
					} // while 3
					break; // while 2를 탈출
				}
				else{
					strcpy(cmpstr, ptr_best_movie_name->name);	
					while(1){	
						check = 0;
						for(int i = 0 ; i < strlen(substr) ; ++i){
							if(*(substr + i) == *(cmpstr + i))
								++check;
							else if(*(substr + i) == '?')
								++check;
							else
								break;
						}
						if(check == strlen(substr)){
							MatchDirectorPrint(ptr_director_node);
							break;
						}
						cmpstr = cmpstr + 1;

						if(*(cmpstr + strlen(substr) - 1) == '\0')
							break; // while 3을 탈출
					 } // while 3
				}
			   ptr_best_movie_name = ptr_best_movie_name->next_node;	
			} // else에서 while 2
		}
	} // while 1

	cmpstr = NULL;
	free(cmpstr);
	return TRUE;
}
/*****************************************************************************************************************************************/
void SearchStringDirector(AdminDirector * AD, char * substr){
	NextDirector * ptr_director_node = AD->head;
	NextNode * ptr_best_movie_name;
	char * serial_string = (char *)malloc(10);

	while(1){
		ptr_director_node = ptr_director_node->next_director_node;

		if(ptr_director_node == AD->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_director_node->serial_number);
			ptr_best_movie_name = ptr_director_node->best_movie_name;
		}

		if(strstr(serial_string, substr)) // serial
			MatchDirectorPrint(ptr_director_node);

		else if(strstr(ptr_director_node->director_name, substr)) // name
			MatchDirectorPrint(ptr_director_node);

		else if(ptr_director_node->sex == *substr && strlen(substr) == 1) // sex
			MatchDirectorPrint(ptr_director_node);

		else if(strstr(ptr_director_node->birth, substr)) // birth
			MatchDirectorPrint(ptr_director_node);

		else { // best_movie
			while(1){
				if(ptr_best_movie_name->next_node == NULL){
					if(strstr(ptr_best_movie_name->name, substr))
						MatchDirectorPrint(ptr_director_node);
					break;
				}
				else if(strstr(ptr_best_movie_name->name, substr)){
					MatchDirectorPrint(ptr_director_node);
					break;
				}
				else
					ptr_best_movie_name = ptr_best_movie_name->next_node;
			}
		}
	}
   free(serial_string);   
}
/*****************************************************************************************************************************************/
void MatchDirectorPrint(NextDirector * ptr_director_node){
	NextNode * ptr_best_movie_name = ptr_director_node->best_movie_name;

	printf("%d:%s:%c:%s:", ptr_director_node->serial_number, ptr_director_node->director_name, ptr_director_node->sex, ptr_director_node->birth);
	while(1){
		if(ptr_best_movie_name->next_node == NULL){
			printf("%s\n", ptr_best_movie_name->name);
			putchar('\n');
			break;
		}
		printf("%s, ", ptr_best_movie_name->name);
		ptr_best_movie_name = ptr_best_movie_name->next_node;
	}
}
/**********************************************************************************************************************************************************/
int PrintDirector(AdminDirector * AD){
	NextDirector * ptr_director_node;
	NextNode * ptr_best_movie_name; 
	NextMovie * ptr_movie_node;

	int serial_num;
	
	scanf("%d", &serial_num);
	ptr_director_node = SearchDirectorSerial(AD, serial_num);
	
	if(ptr_director_node == NULL){
		printf("No such record\n");
		return FALSE;
	}

	ptr_movie_node = (NextMovie *) ptr_director_node->best_movie_name->my_node;
	ptr_best_movie_name = ptr_director_node->best_movie_name;

	printf("%d, %s, %c, %s\n", ptr_director_node->serial_number, ptr_director_node->director_name, ptr_director_node->sex ,ptr_director_node->birth);
	printf("\t%s", ptr_best_movie_name->name);
	
	while(1){
		if(ptr_movie_node == NULL){
			putchar('\n');
			break;
		
		}
		printf(", %d, %d\n", ptr_movie_node->year, ptr_movie_node->time);
		if(ptr_best_movie_name->next_node == NULL)
			break;
		ptr_best_movie_name = ptr_best_movie_name->next_node;
		ptr_movie_node = (NextMovie *)ptr_best_movie_name->my_node;
	}
	printf("@@Done\n");
	return TRUE;
}
/**********************************************************************************************************************************************************/
int AddDirector(AdminDirector * AD){
	FILE * fptr_director_log = fopen("direc.txt", "at");
	NextDirector * newp = (NextDirector *)malloc(sizeof(NextDirector));
	NextDirector * btp = AD->head;
	NextNode * ptmp = newp->best_movie_name = (NextNode *)malloc(sizeof(NextNode));
	ptmp->my_node = NULL;
	char * tmp = (char *)malloc(100);
	
	char ch = 0;
	int i = 0;

	getchar();
	newp->serial_number = ++director_serial;
	
	fprintf(fptr_director_log, "add:%d:", newp->serial_number);
	printf(" Director name > ");
	scanf("%[^\n]", tmp); getchar();
	newp->director_name = (char *)malloc(strlen(tmp)+1);
	strcpy(newp->director_name, tmp);
	ColonCheckInDirector(fptr_director_log, newp->director_name);
	
	printf(" Sex > ");
	scanf("%c", &(newp->sex)); getchar();
	
	printf(" Birth > ");
	scanf("%[^\n]", tmp); getchar();
	newp->birth = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->birth, tmp);
	ColonCheckInDirector(fptr_director_log, newp->birth);
	
	printf(" Best Movie > ");

	while (1) { // processing best_movie
		ch = getchar();

		if (ch == ',' || ch == '\n') {

			*(tmp + i) = '\0';

			if(*tmp == ' ')
				tmp = tmp + 1;

			newp->best_movie_name->name = (char *)malloc(strlen(tmp) + 1);
			strcpy(newp->best_movie_name->name, tmp);
			fprintf(fptr_director_log, "%s", newp->best_movie_name->name);

			if(ch == ','){
				fputc(',', fptr_director_log);
				newp->best_movie_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				newp->best_movie_name->my_node = NULL;
				newp->best_movie_name = newp->best_movie_name->next_node;
				i = 0;
			}

			else if(ch == '\n'){
				fputc('\n', fptr_director_log);
				newp->best_movie_name->next_node = NULL;
				break;
			}
		}
		else {
			*(tmp + i) = ch;
			++i;
		}

	}

	newp->best_movie_name = ptmp;
	while(btp->next_director_node != AD->tail)
		btp = btp->next_director_node;

	newp->next_director_node = AD->tail;
	btp->next_director_node = newp;
	++AD->size;
	free(tmp);
	fclose(fptr_director_log);
	return TRUE;
}
/*****************************************************************************************************************************************/
int ColonCheckInDirector(FILE * fptr_director_log, char * string){ // change : to ??;
	int i = 0;
	char ch = 'x';
	char * address;
	
	if((address = strchr(string, ':')) != NULL){
		while(1){
			ch = *(string + i);
			if(ch == ':')
				break;
			else
				fputc(ch, fptr_director_log);
			++i;
		}
		fprintf(fptr_director_log,"??;%s:", address + 1);
	}
	else
		fprintf(fptr_director_log, "%s:", string);

	return TRUE;
}
/*****************************************************************************************************************************************/
