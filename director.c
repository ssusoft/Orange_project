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
nt DeleteDirector(AdminDirector * AD){
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
