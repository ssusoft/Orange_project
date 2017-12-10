#ifndef Header_Include
#define Header_Include
#include "header.h"
#endif

int actor_serial = 0;

/*****************************************************************************************************************************************/

int CreateActorList(AdminActor * AA) {

	AA->head = (NextActor *)malloc(sizeof(NextActor));
	AA->tail = (NextActor *)malloc(sizeof(NextActor));

	if (AA->head == NULL || AA->tail == NULL)
		return FALSE;

	AA->head->next_actor_node = AA->tail;
	AA->tail->next_actor_node = AA->tail;
	AA->size = 0;

	AA->head->actor_name = NULL;
	AA->head->birth = NULL;
	AA->head->appear_movie_name = NULL;

	return TRUE;
}
/*****************************************************************************************************************************************/
int ReadActorlog(AdminActor * AA) { // this function read movie_log.txt
	FILE * fptr_actor_log;
	fptr_actor_log = fopen("actor_log", "rt");

	char * tag = (char *)malloc(100);
	while (fgetc(fptr_actor_log) != EOF){
		fseek(fptr_actor_log, -1, SEEK_CUR);
		fscanf(fptr_actor_log, "%[^:]:", tag);
		if (strcmp(tag, "add") == 0)
			FileAddActorList(AA, fptr_actor_log); // add

		else if (strcmp(tag, "update") == 0)
			FileUpdateActorList(AA, fptr_actor_log); // update

		else if (strcmp(tag, "delete") == 0) // delete
			FileDeleteActorList(AA, fptr_actor_log);
	}
	free(tag);
	fclose(fptr_actor_log);
	return TRUE;
}
/*****************************************************************************************************************************************/

int FileAddActorList(AdminActor * AA, FILE * fptr_actor_log){
	NextActor * newp = (NextActor *)malloc(sizeof(NextActor));
	NextActor * btp = AA->head;
	NextNode * save_node;
	char * tmp = (char *)malloc(100);
	char ch;
	int i = 0;

	fscanf(fptr_actor_log, "%d:", &(newp->serial_number)); // read serial_number

	if(actor_serial < newp->serial_number)
		actor_serial = newp->serial_number;

	fscanf(fptr_actor_log, "%[^:]:", tmp); // read actor_name
	tmp = ColonCheckInFILE(tmp, "??;", ":");
	newp->actor_name = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->actor_name, tmp);

	fscanf(fptr_actor_log, "%c:", &(newp->sex)); // read sex

	fscanf(fptr_actor_log, "%[^:]:", tmp); // read birth
	tmp = ColonCheckInFILE(tmp, "??;", ":");
	newp->birth = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->birth, tmp);

	newp->appear_movie_name = (NextNode *)malloc(sizeof(NextNode));
	save_node = newp->appear_movie_name;

	while(1){
		ch = fgetc(fptr_actor_log);

		if(ch == ',' || ch == '\n' || ch == EOF){
			*(tmp+i) = '\0';
			if(*tmp == ' ')
				tmp = tmp + 1;

			tmp = ColonCheckInFILE(tmp, "??;", ":");
			newp->appear_movie_name->name = (char *)malloc(sizeof(strlen(tmp) + 1));
			strcpy(newp->appear_movie_name->name, tmp);
			newp->appear_movie_name->my_node = NULL;

			if(ch == ','){
				i = 0;
				newp->appear_movie_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				newp->appear_movie_name = newp->appear_movie_name->next_node;
			}
			else if (ch == '\n' || ch == EOF){
				newp->appear_movie_name->next_node = NULL;
				break;
			}
		}
		else{
			*(tmp + i) = ch;
			++i;
		}
	}
	newp->appear_movie_name = save_node;
	free(tmp);
	while(1){
		if(btp->next_actor_node == AA->tail)
			break;
		btp = btp->next_actor_node;
	}

	btp->next_actor_node = newp;
	newp->next_actor_node = AA->tail;
	++AA->size;
	return TRUE;
}
/*****************************************************************************************************************************************/

int FileUpdateActorList(AdminActor * AA, FILE * fptr_actor_log){
	int serial_num;
	NextActor * match_actor_node;
	NextNode * destroy_node;
	NextNode * save_node;
	NextNode * cur_node;

	int i = 0;
	char ch, sub;
	char * tmp = (char *)malloc(100);
	fscanf(fptr_actor_log, "%d:", &serial_num);
	match_actor_node = SearchActorSerial(AA, serial_num);

	while(1){
		ch = fgetc(fptr_actor_log);
		if(ch == ':')
			++i;
		else if(ch == '=')
			continue;
		else if(ch == EOF || ch == '\n')
			break;

		else{
			fseek(fptr_actor_log, -1, SEEK_CUR);
			if(i == 0){
				free(match_actor_node->actor_name);
				fscanf(fptr_actor_log, "%[^:]", tmp);
				tmp = ColonCheckInFILE(tmp, "??;", ":");
				match_actor_node->actor_name = (char *)malloc(strlen(tmp) + 1);
				strcpy(match_actor_node->actor_name, tmp);
			}
			else if (i == 1)
				fscanf(fptr_actor_log, "%c", &(match_actor_node->sex));
			else if (i == 2){
				free(match_actor_node->birth);
				fscanf(fptr_actor_log, "%[^:]", tmp);
				tmp = ColonCheckInFILE(tmp, "??;", ":");
				match_actor_node->birth = (char *)malloc(strlen(tmp) + 1);
				strcpy(match_actor_node->birth, tmp);
			}
			else if (i == 3){
				destroy_node = match_actor_node->appear_movie_name;
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

				match_actor_node->appear_movie_name = (NextNode *)malloc(sizeof(NextNode));
				save_node = match_actor_node->appear_movie_name;

				while(1){
					fscanf(fptr_actor_log, "%[^,\n]", tmp);
					if(*tmp == ' ')
						tmp = tmp + 1;
					
					tmp = ColonCheckInFILE(tmp, "??;", ":");
					match_actor_node->appear_movie_name->name = (char *)malloc(strlen(tmp)+1);
					strcpy(match_actor_node->appear_movie_name->name, tmp);

					sub = fgetc(fptr_actor_log);
					if(sub == ','){
						match_actor_node->appear_movie_name->next_node = (NextNode *)malloc(sizeof(NextNode));
						match_actor_node->appear_movie_name = match_actor_node->appear_movie_name->next_node;
					}
					else if (sub == '\n' || sub == EOF){
						match_actor_node->appear_movie_name->next_node = NULL;
						break;
					}
				}
				match_actor_node->appear_movie_name = save_node;
				break;
			}
		}
	}
	free(tmp);
	return TRUE;
}
/*****************************************************************************************************************************************/

int FileDeleteActorList(AdminActor * AA, FILE * fptr_actor_log){

	int serial_num;
	NextNode * destroy_node;
	NextNode * cur_node;
	NextActor * match_actor_node;
	NextActor * btp = AA->head->next_actor_node;
	char ch;

	fscanf(fptr_actor_log, "%d:", &serial_num);
	match_actor_node = SearchActorSerial(AA, serial_num);

	while(1){
		if(btp->next_actor_node == match_actor_node){
			btp->next_actor_node = match_actor_node->next_actor_node;
			break;
		}

		else if(btp->next_actor_node == AA->tail){
			return FALSE;
		}

		else{
			btp = btp->next_actor_node;
		}
	}

	destroy_node = match_actor_node->appear_movie_name;
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
	free(match_actor_node);
	while(1){
		ch = fgetc(fptr_actor_log);
		if(ch == '\n' || ch == EOF)
			break;
	}
	--AA->size;
	return TRUE;
}
/*****************************************************************************************************************************************/
NextActor * SearchActorSerial(AdminActor * AA, int serial_num){
	NextActor * match_actor_node = AA->head->next_actor_node;
	while(1){
		if(match_actor_node->serial_number == serial_num){
			return match_actor_node;
		}
		else if(match_actor_node->next_actor_node == AA->tail){
			return NULL;
		}
		else
			match_actor_node = match_actor_node->next_actor_node;
	}
}
/*****************************************************************************************************************************************/
int SearchMyMovie(NextNode * search_movie_name, AdminMovie * AM){ // same with SearchMyMovie in Actor
	NextMovie * ptr_movie_node = AM->head->next_movie_node;
	while(1){
		if(ptr_movie_node == AM->tail){
			search_movie_name->my_node = NULL;
			break;
		}

		else if(strcmp(ptr_movie_node->movie_name, search_movie_name->name) == 0){ // same
			search_movie_name->my_node = ptr_movie_node;
			return TRUE;
		}

		else
			ptr_movie_node = ptr_movie_node->next_movie_node;
	}
	return FALSE;
}
/*****************************************************************************************************************************************/
int SaveActor(AdminActor * AA){
	FILE * fptr_actor_list = fopen("actor_list.save","wt");
	NextActor * ptr_actor_node = AA->head->next_actor_node;
	NextNode * ptr_appear_movie_name = ptr_actor_node->appear_movie_name;

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
			if(ptr_actor_node != AA->tail){
				fprintf(fptr_actor_list, "Name : %s\n", ptr_actor_node->actor_name); 
				fprintf(fptr_actor_list, "Sex : %c\n", ptr_actor_node->sex);
				fprintf(fptr_actor_list, "Birth : %s\n", ptr_actor_node->birth);
				fprintf(fptr_actor_list, "Appear Movie : ");

				while(1){
					if(ptr_appear_movie_name->next_node == NULL){
						fprintf(fptr_actor_list, "%s\n", ptr_appear_movie_name->name);
						break;
					}
					fprintf(fptr_actor_list, "%s, ", ptr_appear_movie_name->name);
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
				}
				ptr_actor_node = ptr_actor_node->next_actor_node;
				ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
				fprintf(fptr_actor_list, "\n");
			}
			else
				break;
		}
	}

	else{ // yes option
		while(1){
			if(ptr_actor_node == AA->tail)
				break;

			while(1){
				ch = *(option + option_check);
				if(ch == 'n')
					fprintf(fptr_actor_list, "Name : %s\n", ptr_actor_node->actor_name);
				else if(ch == 's')
					fprintf(fptr_actor_list, "Sex : %c\n", ptr_actor_node->sex);
				else if(ch == 'b')
					fprintf(fptr_actor_list, "Birth : %s\n", ptr_actor_node->birth);
				else if(ch == 'm'){
					ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
					fprintf(fptr_actor_list, "Appear Movie : ");

					while(ptr_appear_movie_name->next_node != NULL){
						fprintf(fptr_actor_list, "%s, ", ptr_appear_movie_name->name);
						ptr_appear_movie_name = ptr_appear_movie_name->next_node;
					}
					fprintf(fptr_actor_list, "%s\n", ptr_appear_movie_name->name);
				}
				else if(ch == '\0'){
					option_check = 0;
					break;
				}
				++option_check;
			}
			ptr_actor_node = ptr_actor_node->next_actor_node;
			fprintf(fptr_actor_list, "\n");
		}
	}
	fclose(fptr_actor_list);
	free(option);
	if(check == 1 || check == 2){
		rename("actor_list.save", file_name);
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
int SortActor(AdminActor * AA){
	FILE * fptr_actor_list;

	NextActor ** dptr_actor = (NextActor **)malloc(sizeof(NextActor *) * AA->size);
	NextActor * ptr_actor_node = AA->head->next_actor_node;
	NextNode * ptr_appear_movie_name = ptr_actor_node->appear_movie_name;

	char ** sort_string_arr = (char **)malloc(sizeof(char *) * AA->size);
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
			option = 'z';
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
			ptr_actor_node = AA->head->next_actor_node;
			while(1){
				if(ptr_actor_node == AA->tail)
					break;
				else if(ptr_actor_node->sex == 'F' || ptr_actor_node->sex == 'f'){
					printf("Name : %s\n", ptr_actor_node->actor_name);
					printf("Sex : %c\n", ptr_actor_node->sex);
					printf("Birth : %s\n", ptr_actor_node->birth);
					printf("Best Movie : ");
					ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
					while(1){
						if(ptr_appear_movie_name->next_node == NULL){
							printf("%s\n", ptr_appear_movie_name->name);
							break;
						}
						printf("%s, ", ptr_appear_movie_name->name);
						ptr_appear_movie_name = ptr_appear_movie_name->next_node;
					}
					putchar('\n');
				}
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}
			ptr_actor_node = AA->head;
			while(1){
				if(ptr_actor_node == AA->tail)
					break;
				else if(ptr_actor_node->sex == 'M' || ptr_actor_node->sex == 'm'){
					printf("Name : %s\n", ptr_actor_node->actor_name);
					printf("Sex : %c\n", ptr_actor_node->sex);
					printf("Birth : %s\n", ptr_actor_node->birth);
					printf("Appear Movie : ");
					ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
					while(1){
						if(ptr_appear_movie_name->next_node == NULL){
							printf("%s\n", ptr_appear_movie_name->name);
							break;
						}
						printf("%s, ", ptr_appear_movie_name->name);
						ptr_appear_movie_name = ptr_appear_movie_name->next_node;
					}
				}
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}
			putchar('\n');
		}
		else if(option == 'b'){
			for(int i = 0; i < AA->size ; ++i){
				*(sort_string_arr + i) = ptr_actor_node->birth;
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}

			qsort((void *)sort_string_arr, AA->size, sizeof(*sort_string_arr), CompareString);

			for(int i = 0; i < AA->size; ++i){
				ptr_actor_node = AA->head->next_actor_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_actor_node->birth) == 0){
						*(dptr_actor + i) = ptr_actor_node;
						break;
					}
					else
						ptr_actor_node = ptr_actor_node->next_actor_node;
				}
			}

			for(int i = 0; i < AA->size; ++i){
				printf("Name : %s\n", (*(dptr_actor + i))->actor_name);
				printf("Sex : %c\n", (*(dptr_actor + i))->sex);
				printf("Birth : %s\n", (*(dptr_actor + i))->birth);
				printf("Appear Movie : ");
				ptr_appear_movie_name = (*(dptr_actor + i))->appear_movie_name;
				while(1){
					if(ptr_appear_movie_name->next_node == NULL){
						printf("%s\n", ptr_appear_movie_name->name);
						break;
					}
					printf("%s, ", ptr_appear_movie_name->name);
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
				}
			}
			putchar('\n');
		}
		else if(option == 'm'){
			for(int i = 0; i < AA->size ; ++i){
				*(sort_string_arr + i) = ptr_actor_node->appear_movie_name->name;
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}

			qsort((void *)sort_string_arr, AA->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AA->size; ++i){
				ptr_actor_node = AA->head->next_actor_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_actor_node->appear_movie_name->name) == 0){
						*(dptr_actor + i) = ptr_actor_node;
						break;
					}
					else
						ptr_actor_node = ptr_actor_node->next_actor_node;
				}
			}

			for(int i = 0; i < AA->size; ++i){
				printf("Name : %s\n", (*(dptr_actor + i))->actor_name);
				printf("Sex : %c\n", (*(dptr_actor + i))->sex);
				printf("Birth : %s\n", (*(dptr_actor + i))->birth);
				printf("Appear Movie : ");
				ptr_appear_movie_name = (*(dptr_actor + i))->appear_movie_name;
				while(1){
					if(ptr_appear_movie_name->next_node == NULL){
						printf("%s\n", ptr_appear_movie_name->name);
						break;
					}
					printf("%s, ", ptr_appear_movie_name->name);
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
				}
			}
			putchar('\n');
		}
		else if(option == 'z' || option == 'n'){
			for(int i = 0; i < AA->size ; ++i){
				*(sort_string_arr + i) = ptr_actor_node->actor_name;
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}

			qsort((void *)sort_string_arr, AA->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AA->size; ++i){
				ptr_actor_node = AA->head->next_actor_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_actor_node->actor_name) == 0){
						*(dptr_actor + i) = ptr_actor_node;
						break;
					}
					else
						ptr_actor_node = ptr_actor_node->next_actor_node;
				}
			}

			for(int i = 0; i < AA->size; ++i){
				printf("Name : %s\n", (*(dptr_actor + i))->actor_name);
				printf("Sex : %c\n", (*(dptr_actor + i))->sex);
				printf("Birth : %s\n", (*(dptr_actor + i))->birth);
				printf("Appear Movie : ");
				ptr_appear_movie_name = (*(dptr_actor + i))->appear_movie_name;
				while(1){
					if(ptr_appear_movie_name->next_node == NULL){
						printf("%s\n", ptr_appear_movie_name->name);
						break;
					}
					printf("%s, ", ptr_appear_movie_name->name);
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
				}
			}
			putchar('\n');
		}
		else{
			printf("Option Error\n");
			return TRUE;
		}
	}
	else if(check == 1 || check == 2){ // yes file_name
		fptr_actor_list = fopen(file_name ,"wt");

		if(option == 's'){
			ptr_actor_node = AA->head->next_actor_node;
			while(1){
				if(ptr_actor_node == AA->tail)
					break;
				else if(ptr_actor_node->sex == 'F' || ptr_actor_node->sex == 'f'){
					fprintf(fptr_actor_list, "Name : %s\n", ptr_actor_node->actor_name);
					fprintf(fptr_actor_list, "Sex : %c\n", ptr_actor_node->sex);
					fprintf(fptr_actor_list, "Birth : %s\n", ptr_actor_node->birth);
					fprintf(fptr_actor_list, "Appear Movie : ");
					ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
					while(1){
						if(ptr_appear_movie_name->next_node == NULL){
							fprintf(fptr_actor_list, "%s\n", ptr_appear_movie_name->name);
							break;
						}
						fprintf(fptr_actor_list, "%s, ", ptr_appear_movie_name->name);
						ptr_appear_movie_name = ptr_appear_movie_name->next_node;
					}
				}
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}
			ptr_actor_node = AA->head;
			while(1){
				if(ptr_actor_node == AA->tail)
					break;
				else if(ptr_actor_node->sex == 'M' || ptr_actor_node->sex == 'm'){
					fprintf(fptr_actor_list, "Name : %s\n", ptr_actor_node->actor_name);
					fprintf(fptr_actor_list, "Sex : %c\n", ptr_actor_node->sex);
					fprintf(fptr_actor_list, "Birth : %s\n", ptr_actor_node->birth);
					fprintf(fptr_actor_list, "Appear Movie : ");
					ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
					while(1){
						if(ptr_appear_movie_name->next_node == NULL){
							fprintf(fptr_actor_list, "%s\n\n", ptr_appear_movie_name->name);
							break;
						}
						fprintf(fptr_actor_list, "%s, ", ptr_appear_movie_name->name);
						ptr_appear_movie_name = ptr_appear_movie_name->next_node;
					}
				}
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}
			putchar('\n');
		}

		else if(option == 'b'){
			for(int i = 0; i < AA->size ; ++i){
				*(sort_string_arr + i) = ptr_actor_node->birth;
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}

			qsort((void *)sort_string_arr, AA->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AA->size; ++i){
				ptr_actor_node = AA->head->next_actor_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_actor_node->birth) == 0){
						*(dptr_actor + i) = ptr_actor_node;
						break;
					}
					else
						ptr_actor_node = ptr_actor_node->next_actor_node;
				}
			}

			for(int i = 0; i < AA->size; ++i){
				fprintf(fptr_actor_list, "Name : %s\n", (*(dptr_actor + i))->actor_name);
				fprintf(fptr_actor_list, "Sex : %c\n", (*(dptr_actor + i))->sex);
				fprintf(fptr_actor_list, "Birth : %s\n", (*(dptr_actor + i))->birth);
				fprintf(fptr_actor_list, "Appear Movie : ");
				ptr_appear_movie_name = (*(dptr_actor + i))->appear_movie_name;
				while(1){
					if(ptr_appear_movie_name->next_node == NULL){
						fprintf(fptr_actor_list, "%s\n\n", ptr_appear_movie_name->name);
						break;
					}
					fprintf(fptr_actor_list, "%s, ", ptr_appear_movie_name->name);
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
				}
			}
			putchar('\n');
		}
		//
		else if(option == 'm'){
			for(int i = 0; i < AA->size ; ++i){
				*(sort_string_arr + i) = ptr_actor_node->appear_movie_name->name;
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}

			qsort((void *)sort_string_arr, AA->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AA->size; ++i){
				ptr_actor_node = AA->head->next_actor_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_actor_node->appear_movie_name->name) == 0){
						*(dptr_actor + i) = ptr_actor_node;
						break;
					}
					else
						ptr_actor_node = ptr_actor_node->next_actor_node;
				}
			}

			for(int i = 0; i < AA->size; ++i){
				fprintf(fptr_actor_list, "Name : %s\n", (*(dptr_actor + i))->actor_name);
				fprintf(fptr_actor_list, "Sex : %c\n", (*(dptr_actor + i))->sex);
				fprintf(fptr_actor_list, "Birth : %s\n", (*(dptr_actor + i))->birth);
				fprintf(fptr_actor_list, "Appear Movie : ");
				ptr_appear_movie_name = (*(dptr_actor + i))->appear_movie_name;
				while(1){
					if(ptr_appear_movie_name->next_node == NULL){
						fprintf(fptr_actor_list, "%s\n\n", ptr_appear_movie_name->name);
						break;
					}
					fprintf(fptr_actor_list, "%s, ", ptr_appear_movie_name->name);
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
				}
			}
			putchar('\n');
		}

		else if (option == 'z' || option == 'n'){
			for(int i = 0; i < AA->size ; ++i){
				*(sort_string_arr + i) = ptr_actor_node->actor_name;
				ptr_actor_node = ptr_actor_node->next_actor_node;
			}	

			qsort((void *)sort_string_arr, AA->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AA->size; ++i){
				ptr_actor_node = AA->head->next_actor_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_actor_node->actor_name) == 0){
						*(dptr_actor + i) = ptr_actor_node;
						break;
					}
					else
						ptr_actor_node = ptr_actor_node->next_actor_node;
				}
			}

			for(int i = 0; i < AA->size; ++i){
				fprintf(fptr_actor_list, "Name : %s\n", (*(dptr_actor + i))->actor_name);
				fprintf(fptr_actor_list, "Sex : %c\n", (*(dptr_actor + i))->sex);
				fprintf(fptr_actor_list, "Birth : %s\n", (*(dptr_actor + i))->birth);
				fprintf(fptr_actor_list, "Appear Movie : ");
				ptr_appear_movie_name = (*(dptr_actor + i))->appear_movie_name;
				while(1){
					if(ptr_appear_movie_name->next_node == NULL){
						fprintf(fptr_actor_list, "%s\n\n", ptr_appear_movie_name->name);
						break;
					}
					fprintf(fptr_actor_list, "%s, ", ptr_appear_movie_name->name);
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
				}
			}
			putchar('\n');
		}
		else{
			printf("Option Error\n");
			return FALSE;
		}
	}
	free(sort_string_arr);
	free(tmp);
	free(file_name);
	free(dptr_actor);
	printf("@@Done\n");
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchActor(AdminActor * AA, char * string){
	NextActor * ptr_actor_node = AA->head;
	NextNode * ptr_appear_movie_name;

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
			SearchStringActor(AA, substr);
		}

		else if(*(string + len - 1) == '*'){ // last *
			*(substr + len - 1) = '\0';
			SearchStringActor(AA, substr);
		}
	}

	else if (checksm == 0 && checkqm != 0){	// ?
		SearchActorQM(AA, substr);
	}

	else if (checksm != 0 && checkqm != 0){ // ? and *
		SearchActorQMSM(AA, substr);
	}

	else if (checksm == 0 && checkqm == 0){ // no
		SearchStringActor(AA, substr);
	}

	free(serial_string);
	free(substr);
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchActorQMSM(AdminActor * AA, char * substr){
	NextActor * ptr_actor_node = AA->head;
	NextNode * ptr_appear_movie_name;
	char * procstr = (char *)malloc(strlen(substr) + 1);
	strcpy(procstr, substr);

	if(*procstr == '*' && *(procstr + strlen(procstr) - 1) == '*'){
		*(procstr + strlen(procstr) - 1) = '\0';
		procstr = procstr + 1;
		SearchActorQM(AA, procstr);
	}
	else if(*procstr == '*'){
		procstr = procstr + 1;
		SearchActorQM(AA, procstr);
	}
	else if(*(procstr + strlen(procstr) - 1) == '*'){
		*(procstr + strlen(procstr) - 1) = '\0';
		SearchActorQM(AA, procstr);
	}
	free(procstr);
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchActorQM(AdminActor * AA, char * substr){
	NextActor * ptr_actor_node = AA->head;
	NextNode * ptr_appear_movie_name;
	char * cmpstr = (char *)malloc(100);
	char * serial_string = (char *)malloc(10);
	int check;

	while(1){ // check actor_name
		ptr_actor_node = ptr_actor_node->next_actor_node;

		if(ptr_actor_node == AA->tail)
			break;
		else
			strcpy(cmpstr, ptr_actor_node->actor_name);

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
				MatchActorPrint(ptr_actor_node);
				break;
			}

			else
				cmpstr = cmpstr + 1;

			if(*(cmpstr + strlen(substr) - 1) == '\0')
				break;
		}
	}

	ptr_actor_node = AA->head;

	while(1){ // check serial
		ptr_actor_node = ptr_actor_node->next_actor_node;

		if(ptr_actor_node == AA->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_actor_node->serial_number);
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
				MatchActorPrint(ptr_actor_node);
				break;
			}

			else
				serial_string = serial_string + 1;

			if(*(serial_string + strlen(substr) - 1) == '\0')
				break;
		}
	}

	ptr_actor_node = AA->head;

	while(1){ // check birth
		ptr_actor_node = ptr_actor_node->next_actor_node;

		if(ptr_actor_node == AA->tail)
			break;
		else
			strcpy(cmpstr, ptr_actor_node->birth);

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
				MatchActorPrint(ptr_actor_node);
				break;
			}
			cmpstr = cmpstr + 1;

			if(*(cmpstr + strlen(substr) - 1) == '\0')
				break;
		}
	}
	ptr_actor_node = AA->head;

	while(1){ // sex
		ptr_actor_node = ptr_actor_node->next_actor_node;

		if(strlen(substr) != 1 || ptr_actor_node == AA->tail)
			break;

		else{
			*cmpstr = ptr_actor_node->sex;
			*(cmpstr + 1) = '\0';
		}

		if(*cmpstr == *substr)
			MatchActorPrint(ptr_actor_node);
		else if(*substr == '?')
			MatchActorPrint(ptr_actor_node);
	}
	ptr_actor_node = AA->head;

	while(1){ // check appear_movie_name
		ptr_actor_node = ptr_actor_node->next_actor_node;
		ptr_appear_movie_name = ptr_actor_node->appear_movie_name;

		if(ptr_actor_node == AA->tail)
			break;

		else{
			while(1){

				if(ptr_appear_movie_name->next_node == NULL){
					strcpy(cmpstr, ptr_appear_movie_name->name);	
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
							MatchActorPrint(ptr_actor_node);
							break;
						}
						cmpstr = cmpstr + 1;

						if(*(cmpstr + strlen(substr) - 1) == '\0')
							break; // while 3을 탈출
					} // while 3
					break; // while 2를 탈출
				}
				else{
					strcpy(cmpstr, ptr_appear_movie_name->name);	
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
							MatchActorPrint(ptr_actor_node);
							break;
						}
						cmpstr = cmpstr + 1;

						if(*(cmpstr + strlen(substr) - 1) == '\0')
							break; // while 3을 탈출
					} // while 3
				}
				ptr_appear_movie_name = ptr_appear_movie_name->next_node;	
			} // else에서 while 2
		}
	} // while 1

	cmpstr = NULL;
	free(cmpstr);
	return TRUE;
}
/*****************************************************************************************************************************************/
void SearchStringActor(AdminActor * AA, char * substr){
	NextActor * ptr_actor_node = AA->head;
	NextNode * ptr_appear_movie_name;
	char * serial_string = (char *)malloc(10);

	while(1){
		ptr_actor_node = ptr_actor_node->next_actor_node;

		if(ptr_actor_node == AA->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_actor_node->serial_number);
			ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
		}

		if(strstr(serial_string, substr)) // serial
			MatchActorPrint(ptr_actor_node);

		else if(strstr(ptr_actor_node->actor_name, substr)) // name
			MatchActorPrint(ptr_actor_node);

		else if(ptr_actor_node->sex == *substr && strlen(substr) == 1) // sex
			MatchActorPrint(ptr_actor_node);

		else if(strstr(ptr_actor_node->birth, substr)) // birth
			MatchActorPrint(ptr_actor_node);

		else {
			while(1){
				if(ptr_appear_movie_name->next_node == NULL){
					if(strstr(ptr_appear_movie_name->name, substr))
						MatchActorPrint(ptr_actor_node);
					break;
				}
				else if(strstr(ptr_appear_movie_name->name, substr)){
					MatchActorPrint(ptr_actor_node);
					break;
				}
				else
					ptr_appear_movie_name = ptr_appear_movie_name->next_node;
			}
		}
	}
	free(serial_string);   
}
/*****************************************************************************************************************************************/
void MatchActorPrint(NextActor * ptr_actor_node){
	NextNode * ptr_appear_movie_name = ptr_actor_node->appear_movie_name;

	printf("%d:%s:%c:%s:", ptr_actor_node->serial_number, ptr_actor_node->actor_name, ptr_actor_node->sex, ptr_actor_node->birth);
	while(1){
		if(ptr_appear_movie_name->next_node == NULL){
			printf("%s\n", ptr_appear_movie_name->name);
			putchar('\n');
			break;
		}
		printf("%s, ", ptr_appear_movie_name->name);
		ptr_appear_movie_name = ptr_appear_movie_name->next_node;
	}
}
/*****************************************************************************************************************************************/
int DeleteActor(AdminActor * AA){
	FILE * fptr_actor_log = fopen("actor_log", "at");
	int serial_num;
	NextActor * match_actor_node;
	NextActor * bptr_actor_node = AA->head;
	NextNode * destroy_node;
	NextNode * curp;

	scanf("%d", &serial_num);
	match_actor_node = SearchActorSerial(AA, serial_num);

	if(match_actor_node == NULL){
		printf("No such record\n");
		return FALSE;	
	}

	while(bptr_actor_node->next_actor_node != match_actor_node)
		bptr_actor_node = bptr_actor_node->next_actor_node;

	bptr_actor_node->next_actor_node = match_actor_node->next_actor_node;

	free(match_actor_node->actor_name);
	free(match_actor_node->birth);
	destroy_node = match_actor_node->appear_movie_name;
	free(match_actor_node);

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
	fprintf(fptr_actor_log, "delete:%d::::\n", serial_num);
	fclose(fptr_actor_log);
	--AA->size;
	printf("@@Done\n");
	return TRUE;
}
/*****************************************************************************************************************************************/
int PrintActor(AdminActor * AA){
	NextActor * ptr_actor_node;
	NextNode * ptr_appear_movie_name; 
	NextMovie * ptr_movie_node;

	int serial_num;

	scanf("%d", &serial_num);
	ptr_actor_node = SearchActorSerial(AA, serial_num);

	if(ptr_actor_node == NULL){
		printf("No such record\n");
		return FALSE;
	}

	ptr_movie_node = (NextMovie *)ptr_actor_node->appear_movie_name->my_node;
	ptr_appear_movie_name = ptr_actor_node->appear_movie_name;

	printf("%d, %s, %c, %s\n", ptr_actor_node->serial_number, ptr_actor_node->actor_name, ptr_actor_node->sex ,ptr_actor_node->birth);

	while(1){ // if name null ?
		printf("\t%s", ptr_appear_movie_name->name);
		if(ptr_movie_node != NULL){
			printf(", %d, %d\n", ptr_movie_node->year, ptr_movie_node->time);
		}
		else
			putchar('\n');
		if(ptr_appear_movie_name->next_node == NULL)
			break;
		ptr_appear_movie_name = ptr_appear_movie_name->next_node;
		ptr_movie_node = (NextMovie *)ptr_appear_movie_name->my_node;
	}
	printf("@@Done\n");
	return TRUE;
}
/**********************************************************************************************************************************************************/
int AddActor(AdminActor * AA){
	FILE * fptr_actor_log = fopen("actor_log", "at");
	NextActor * newp = (NextActor *)malloc(sizeof(NextActor));
	NextActor * btp = AA->head;
	NextNode * ptmp = newp->appear_movie_name = (NextNode *)malloc(sizeof(NextNode));
	NextActor * match_actor_node;
	NextNode * ptr_appear_movie_name;
	char * tmp = (char *)malloc(100);
	ptmp->my_node = NULL;
	char yn;
	char ch = 0;
	int i = 0;

	getchar();
	newp->serial_number = ++actor_serial;

	printf(" Actor name > ");
	scanf("%[^\n]", tmp); getchar();
	newp->actor_name = (char *)malloc(strlen(tmp)+1);

	if((match_actor_node = ThereisSameActor(AA,tmp)) != NULL){
		ptr_appear_movie_name = match_actor_node->appear_movie_name;
		printf("@@You have the same record in actor list.\n");
		printf("%d:%s:%c:%s:",match_actor_node->serial_number, match_actor_node->actor_name, match_actor_node->sex, match_actor_node->birth);
		while(1){
			if(ptr_appear_movie_name->next_node == NULL){
				printf("%s\n", ptr_appear_movie_name->name);
				break;
			}
			printf("%s, ", ptr_appear_movie_name->name);
			ptr_appear_movie_name = ptr_appear_movie_name->next_node;
		}
		printf("Do you want to add any way? (Y / N)");
		yn = getchar(); getchar();
		if(yn == 'N' || yn == 'n')
			return TRUE;
	}

	fprintf(fptr_actor_log, "add:%d:", newp->serial_number);
	strcpy(newp->actor_name, tmp);
	ColonCheckInActor(fptr_actor_log, newp->actor_name);

	printf(" Sex > ");
	scanf("%c", &(newp->sex)); getchar();
	fprintf(fptr_actor_log, "%c:", newp->sex);

	printf(" Birth > ");
	scanf("%[^\n]", tmp); getchar();
	newp->birth = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->birth, tmp);
	ColonCheckInActor(fptr_actor_log, newp->birth);

	printf(" Appear Movie > ");

	while (1) { // processing appear_movie
		ch = getchar();

		if (ch == ',' || ch == '\n') {

			*(tmp + i) = '\0';

			if(*tmp == ' ')
				tmp = tmp + 1;

			newp->appear_movie_name->name = (char *)malloc(strlen(tmp) + 1);
			strcpy(newp->appear_movie_name->name, tmp);
			ColonCheckInAppearmovie(fptr_actor_log, newp->appear_movie_name->name);

			if(ch == ','){
				fputc(',', fptr_actor_log);
				newp->appear_movie_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				newp->appear_movie_name->my_node = NULL;
				newp->appear_movie_name = newp->appear_movie_name->next_node;
				i = 0;
			}

			else if(ch == '\n'){
				fputc('\n', fptr_actor_log);
				newp->appear_movie_name->next_node = NULL;
				break;
			}
		}
		else {
			*(tmp + i) = ch;
			++i;
		}
	}

	newp->appear_movie_name = ptmp;
	while(btp->next_actor_node != AA->tail)
		btp = btp->next_actor_node;

	newp->next_actor_node = AA->tail;
	btp->next_actor_node = newp;
	++AA->size;
	free(tmp);
	fclose(fptr_actor_log);
	printf("@@Done\n");
	return TRUE;
}
/*****************************************************************************************************************************************/
int ColonCheckInActor(FILE * fptr_actor_log, char * string){ // change : to ??;
	int i = 0;
	char ch = 'x';
	char * address;

	if((address = strchr(string, ':')) != NULL){
		while(1){
			ch = *(string + i);
			if(ch == ':')
				break;
			else
				fputc(ch, fptr_actor_log);
			++i;
		}
		fprintf(fptr_actor_log,"??;%s:", address + 1);
	}
	else
		fprintf(fptr_actor_log, "%s:", string);

	return TRUE;
}
/*****************************************************************************************************************************************/
int ColonCheckInAppearmovie(FILE * fptr_actor_log, char * string){ // change : to ??;
	int i = 0;
	char ch = 'x';
	char * address;

	if((address = strchr(string, ':')) != NULL){
		while(1){
			ch = *(string + i);
			if(ch == ':')
				break;
			else
				fputc(ch, fptr_actor_log);
			++i;
		}
		fprintf(fptr_actor_log,"??;%s", address + 1);
	}
	else
		fprintf(fptr_actor_log, "%s", string);
}
/*****************************************************************************************************************************************/
NextActor * ThereisSameActor(AdminActor * AA, char * string){
	NextActor * ptr_actor_node = AA->head;

	while(1){
		ptr_actor_node = ptr_actor_node->next_actor_node;
		if(ptr_actor_node == AA->tail)
			break;
		if(strcmp(string, ptr_actor_node->actor_name)==0)
			return ptr_actor_node;
	}
	return NULL;
}
/*****************************************************************************************************************************************/
int UpdateActor(AdminActor * AA){

	FILE * fptr_actor_log = fopen("actor_log", "at");
	NextActor * ptr_actor_node;
	char * option = (char *)malloc(10);
	char * sub = (char *)malloc(10);
	char tmp;
	int serial_num = 0;

	if(scanf("%d", &serial_num)){ // no option
		getchar();
		ptr_actor_node = SearchActorSerial(AA, serial_num);
		if(UpdateActorName(fptr_actor_log, AA, serial_num) == FALSE)
			return FALSE;

		UpdateActorSex(fptr_actor_log, ptr_actor_node);
		UpdateActorBirth(fptr_actor_log, ptr_actor_node);
		UpdateActorAppearmovie(fptr_actor_log, ptr_actor_node);
	}

	else{
		scanf("%s", sub);
		getchar();
		scanf("%d", &serial_num);
		while(getchar() != '\n') ; 
		ptr_actor_node = SearchActorSerial(AA, serial_num);

		for(int i = 0; i < strlen(sub); ++i){
			if(*(sub + i) == 'n')
				*(option) = 'n';
			else if(*(sub + i) == 's')
				*(option + 1) = 's';
			else if(*(sub + i) == 'b')
				*(option + 2) = 'b';
			else if(*(sub + i) == 'm')
				*(option + 3) = 'm';		
		}

		if(*(option) != 'n')
			fprintf(fptr_actor_log, "update:%d:", serial_num);

		for(int i = 0; i < 4 ;++i){
			if(*(option + i) == 'n'){
				if(UpdateActorName(fptr_actor_log, AA, serial_num) == FALSE){
					return FALSE;
				}
			}

			else if(*(option + i) == 's'){
				if(UpdateActorSex(fptr_actor_log, ptr_actor_node) == FALSE){
					return FALSE;
				}
			}

			else if(*(option + i) == 'b'){
				if(UpdateActorBirth(fptr_actor_log, ptr_actor_node) == FALSE){
					return FALSE;
				}
			}

			else if(*(option + i) == 'm'){
				if(UpdateActorAppearmovie(fptr_actor_log, ptr_actor_node) == FALSE){
					return FALSE;
				}
			}
			else{
				fprintf(fptr_actor_log, "=:");
			}
		}
	}
	fclose(fptr_actor_log);
	return TRUE;
}
/*****************************************************************************************************************************************/
int UpdateActorName(FILE * fptr_actor_log, AdminActor * AA, int serial_num){
	NextActor * ptr_actor_node = SearchActorSerial(AA, serial_num);
	NextActor * match_actor_node;
	NextNode * ptr_appear_movie_name;
	char * tmp = (char *)malloc(100);
	char ch;
	if(ptr_actor_node == NULL){
		printf("There is no match serial\n");
		return FALSE;
	}

	printf("Actor name : ");
	scanf("%[^\n]", tmp); getchar();

	if((match_actor_node = ThereisSameActor(AA, tmp)) == NULL){
		free(ptr_actor_node->actor_name);
		ptr_actor_node->actor_name = (char *)malloc(strlen(tmp) + 1);
		strcpy(ptr_actor_node->actor_name, tmp);
		fprintf(fptr_actor_log, "update:%d:", serial_num);
		ColonCheckInActor(fptr_actor_log, ptr_actor_node->actor_name); // fprintf
	}

	else{
		ptr_appear_movie_name = match_actor_node->appear_movie_name;
		printf("@@You have the same record in actor list.\n");
		printf("Actor name : %s\n", match_actor_node->actor_name);
		printf("Sex : %c\n", match_actor_node->sex);
		printf("Birth : %s\n", match_actor_node->birth);
		printf("Appear movie name : ");
		
		while(1){
			if(ptr_appear_movie_name->next_node == NULL){
				printf("%s\n", ptr_appear_movie_name->name);
				break;
			}
			printf("%s, ", ptr_appear_movie_name->name);
			ptr_appear_movie_name = ptr_appear_movie_name->next_node;
		}

		printf("Do you want to change the record? (Y / N)");
		ch = getchar(); getchar();

		if(ch == 'Y' || ch == 'y'){
			ptr_actor_node = ThereisSameActor(AA, tmp);
			free(ptr_actor_node->actor_name);
			ptr_actor_node->actor_name = (char *)malloc(strlen(tmp) + 1);
			strcpy(ptr_actor_node->actor_name, tmp);
			fprintf(fptr_actor_log, "update:%d:", serial_num);
			ColonCheckInActor(fptr_actor_log, ptr_actor_node->actor_name); // fprintf
		}
		else{
			return FALSE;
		}
	}
}
/*****************************************************************************************************************************************/
int UpdateActorAppearmovie(FILE * fptr_actor_log, NextActor * ptr_actor_node){
	NextNode * destroy_node;
	NextNode * cur_node;
	NextNode * save_node;
	char * tmp = (char *)malloc(100);
	char ch;
	int cnt = 0;
	if(ptr_actor_node == NULL){
		printf("There is no match serial\n");
		return FALSE;
	}
	destroy_node = ptr_actor_node->appear_movie_name;
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
	// destroy
	ptr_actor_node->appear_movie_name = (NextNode *)malloc(sizeof(NextNode));
	save_node = ptr_actor_node->appear_movie_name;
	printf("Appear Movie > ");
	
	while (1) {
		ch = getchar();

		if (ch == ',' || ch == '\n') {
			*(tmp + cnt) = '\0';

			if(*tmp == ' ')
				tmp = tmp + 1;

			ptr_actor_node->appear_movie_name->name = (char *)malloc(strlen(tmp) + 1);
			strcpy(ptr_actor_node->appear_movie_name->name, tmp);
			ColonCheckInAppearmovie(fptr_actor_log, ptr_actor_node->appear_movie_name->name);

			if(ch == ','){
				fputc(',', fptr_actor_log);
				ptr_actor_node->appear_movie_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				ptr_actor_node->appear_movie_name->my_node = NULL;
				ptr_actor_node->appear_movie_name = ptr_actor_node->appear_movie_name->next_node;
				cnt = 0;
			}

			else if(ch == '\n'){
				fputc('\n', fptr_actor_log);
				ptr_actor_node->appear_movie_name->my_node = NULL;
				ptr_actor_node->appear_movie_name->next_node = NULL;
				break;
			}
		}

		else {
			*(tmp + cnt) = ch;
			++cnt;
		}
	}
	ptr_actor_node->appear_movie_name = save_node;
	return TRUE;
}

/*****************************************************************************************************************************************/
int UpdateActorBirth(FILE * fptr_actor_log, NextActor * ptr_actor_node){
	char * tmp = (char *)malloc(100);
	char ch;

	printf("Birth : ");
	scanf("%[^\n]", tmp); getchar();

	free(ptr_actor_node->birth);
	ptr_actor_node->birth = (char *)malloc(strlen(tmp) + 1);
	strcpy(ptr_actor_node->birth, tmp);
	ColonCheckInActor(fptr_actor_log, ptr_actor_node->birth); // fprintf
	return TRUE;
}

/*****************************************************************************************************************************************/
int UpdateActorSex(FILE * fptr_actor_log, NextActor * ptr_actor_node){
	if(ptr_actor_node == NULL){
		printf("There is no match serial\n");
		return FALSE;
	}

	printf("Sex : ");
	scanf("%c", &(ptr_actor_node->sex)); getchar();
	fprintf(fptr_actor_log, "%c:", ptr_actor_node->sex);
	return TRUE;
}
