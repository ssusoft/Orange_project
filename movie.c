#ifndef Header_Include
	#define Header_Include
	#include "header.h"
#endif

int movie_serial = 0;

/**************************************************************************************************************************************************************************/
int CreateMovieList(AdminMovie * AM) {

	AM->head = (NextMovie *)malloc(sizeof(NextMovie));
	AM->tail = (NextMovie *)malloc(sizeof(NextMovie));

	if (AM->head == NULL || AM->tail == NULL)
		return FALSE;

	AM->head->next_movie_node = AM->tail;
	AM->head->actor_name = NULL;
	AM->head->director_name = NULL;
	AM->head->genre = NULL;
	AM->head->movie_name = NULL;
	
	AM->tail->next_movie_node = AM->tail;
	AM->tail->actor_name = NULL;
	AM->tail->director_name = NULL;
	AM->tail->genre = NULL;
	AM->tail->movie_name = NULL;

	AM->size = 0;
	
	return TRUE;
}
/**************************************************************************************************************************************************************************/

int ReadMovielog(AdminMovie * AM, AdminDirector * AD) { // this function read movie_log.txt
	FILE * fptr_movie_log;
	fptr_movie_log = fopen("movie_log", "rt");

	char * tag = (char *)malloc(100);
	char ch;
	while ((ch = fgetc(fptr_movie_log)) != EOF){
		fseek(fptr_movie_log, -1, SEEK_CUR);
		fscanf(fptr_movie_log, "%[^:]:", tag);

		if (strcmp(tag, "add") == 0)
			FileAddMovieList(AM, fptr_movie_log); // add

		else if (strcmp(tag, "update") == 0)
			FileUpdateMovieList(AM, fptr_movie_log); // update

		else if (strcmp(tag, "delete") == 0) // delect
			FileDeleteMovieList(AM, fptr_movie_log);
	}
	free(tag);
	fclose(fptr_movie_log);
	return TRUE;
}
/**************************************************************************************************************************************************************************/
int FileAddMovieList(AdminMovie * AM, FILE * fptr_movie_log){
	NextMovie * newp = (NextMovie *)malloc(sizeof(NextMovie));
	NextMovie * btp = AM->head;
	NextNode * save_node;
	char * tmp = (char *)malloc(100);
	char ch;
	int i = 0;

	fscanf(fptr_movie_log, "%d:", &(newp->serial_number)); // read serial_number
	
	if(movie_serial < newp->serial_number)
		movie_serial = newp->serial_number;

	fscanf(fptr_movie_log, "%[^:]:", tmp); // read movie_name
	tmp = ColonCheckInFILE(tmp,"??;",":");
	newp->movie_name = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->movie_name, tmp);

	fscanf(fptr_movie_log, "%[^:]:", tmp); // read genre
	tmp = ColonCheckInFILE(tmp,"??;",":");
	newp->genre = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->genre, tmp);

	fscanf(fptr_movie_log, "%[^:]:", tmp); // read director
	tmp = ColonCheckInFILE(tmp,"??;",":");
	newp->director_name = (NextNode *)malloc(sizeof(NextNode));
	newp->director_name->my_node = NULL;
	newp->director_name->name = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->director_name->name, tmp);

	fscanf(fptr_movie_log, "%d:", &(newp->year)); // read year
	fscanf(fptr_movie_log, "%d:", &(newp->time)); // read time

	newp->actor_name = (NextNode *)malloc(sizeof(NextNode));
	save_node = newp->actor_name;

	while(1){
		ch = fgetc(fptr_movie_log);

		if(ch == ',' || ch == '\n' || ch == EOF){
			*(tmp+i) = '\0';

			if(*tmp == ' ')
				tmp = tmp + 1;

			newp->actor_name->name = (char *)malloc(sizeof(strlen(tmp) + 1));
			strcpy(newp->actor_name->name, tmp);
			newp->actor_name->my_node = NULL;

			if(ch == ','){
				i = 0;
				newp->actor_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				newp->actor_name = newp->actor_name->next_node;

			}
			else if (ch == '\n' || ch == EOF){
				newp->actor_name->next_node = NULL;
				break;
			}
		}
		else{
			*(tmp + i) = ch;
			++i;
		}
	}
	newp->actor_name = save_node;
	free(tmp);
	while(1){
		if(btp->next_movie_node == AM->tail)
			break;
		btp = btp->next_movie_node;
	}
	
	btp->next_movie_node = newp;
	newp->next_movie_node = AM->tail;
	++AM->size;
	return TRUE;
}
/**************************************************************************************************************************************************************************/
int FileUpdateMovieList(AdminMovie * AM, FILE * fptr_movie_log){
	int serial_num;
	NextMovie * match_movie_node;
	NextNode * destroy_node;
	NextNode * save_node;
	NextNode * cur_node;

	int i = 0;
	char ch, sub;
	char * tmp = (char *)malloc(100);
	fscanf(fptr_movie_log, "%d:", &serial_num);
	match_movie_node = SearchMovieSerial(AM, serial_num);

	while(1){
		ch = fgetc(fptr_movie_log);
		if(ch == ':')
			++i;
		else if(ch == '=')
			continue;
		else if(ch == '\n' || ch == EOF)
			break;

		else{
			fseek(fptr_movie_log, -1, SEEK_CUR);
			if(i == 0){
				free(match_movie_node->movie_name);
				fscanf(fptr_movie_log, "%[^:]", tmp);
				tmp = ColonCheckInFILE(tmp,"??;",":");
				match_movie_node->movie_name = (char *)malloc(strlen(tmp) + 1);
				strcpy(match_movie_node->movie_name, tmp);
			}
			else if (i == 1){
				free(match_movie_node->genre);
				fscanf(fptr_movie_log, "%[^:]", tmp);
				tmp = ColonCheckInFILE(tmp,"??;",":");
				match_movie_node->genre = (char *)malloc(strlen(tmp) + 1);
				strcpy(match_movie_node->genre, tmp);
			}
			else if (i == 2){
				free(match_movie_node->director_name->name);
				free(match_movie_node->director_name);
				fscanf(fptr_movie_log, "%[^:]", tmp);
				tmp = ColonCheckInFILE(tmp,"??;",":");
				match_movie_node->director_name = (NextNode *)malloc(sizeof(NextNode));
				match_movie_node->director_name->name = (char *)malloc(strlen(tmp) + 1);
				match_movie_node->director_name->my_node = NULL;
				strcpy(match_movie_node->director_name->name, tmp);
			}
			else if (i == 3)
				fscanf(fptr_movie_log, "%d", &(match_movie_node->year));
			else if (i == 4)
				fscanf(fptr_movie_log, "%d", &(match_movie_node->time));
			else if (i == 5){
				if(match_movie_node->actor_name != NULL)
					destroy_node = match_movie_node->actor_name;

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
				// clear destroy

				match_movie_node->actor_name = (NextNode *)malloc(sizeof(NextNode));
				match_movie_node->actor_name->my_node = NULL;
				save_node = match_movie_node->actor_name;

				while(1){
					fscanf(fptr_movie_log, "%[^,\n\r]", tmp);

					if(*tmp == ' ')
						tmp = tmp + 1;

					match_movie_node->actor_name->name = (char *)malloc(strlen(tmp)+1);
					strcpy(match_movie_node->actor_name->name, tmp);
					sub = fgetc(fptr_movie_log);

					if(sub == ','){
						match_movie_node->actor_name->next_node = (NextNode *)malloc(sizeof(NextNode));
						match_movie_node->actor_name->my_node = NULL;
						match_movie_node->actor_name = match_movie_node->actor_name->next_node;
					}

					else if (sub == '\n' || sub == EOF){
						match_movie_node->actor_name->next_node = NULL;
						break;
					}
				}
				match_movie_node->actor_name = save_node;
				break;
			}

		}
	}
	free(tmp);
	return TRUE;
}
/**************************************************************************************************************************************************************************/

int FileDeleteMovieList(AdminMovie * AM, FILE * fptr_movie_log){

	int serial_num;
	NextNode * destroy_node;
	NextNode * cur_node;
	NextMovie * match_movie_node;
	NextMovie * btp = AM->head->next_movie_node;
	char ch;

	fscanf(fptr_movie_log, "%d:", &serial_num);
	match_movie_node = SearchMovieSerial(AM, serial_num);

	while(1){
		if(btp->next_movie_node == match_movie_node){
			btp->next_movie_node = match_movie_node->next_movie_node;
			break;
		}

		else if(btp->next_movie_node == AM->tail){
			return FALSE;
		}

		else{
			btp = btp->next_movie_node;
		}
	}

	destroy_node = match_movie_node->actor_name;
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
	free(match_movie_node);

	while(1){
		ch = fgetc(fptr_movie_log);
		if(ch == '\n' || ch == EOF)
			break;
	}
	--AM->size;
	return TRUE;
}
/**************************************************************************************************************************************************************************/
NextMovie * SearchMovieSerial(AdminMovie * AM, int serial_num){
	NextMovie * match_movie_node = AM->head->next_movie_node;
	while(1){
		if(match_movie_node->serial_number == serial_num){
			return match_movie_node;
		}
		else if(match_movie_node->next_movie_node == AM->tail){
			return NULL;
		}
		else
			match_movie_node = match_movie_node->next_movie_node;
	}
}
/**************************************************************************************************************************************************************************/
int SearchMyActor(NextNode * search_actor_name, AdminActor * AA){
	NextActor * ptr_actor_node = AA->head->next_actor_node;
	while(1){
		if(ptr_actor_node == AA->tail){
			search_actor_name->my_node = NULL;
			break;
		}

		else if(strcmp(ptr_actor_node->actor_name, search_actor_name->name) == 0){
			search_actor_name->my_node = ptr_actor_node;
			return TRUE;
		}
		else
			ptr_actor_node = ptr_actor_node->next_actor_node;
	}
	return FALSE;
}
/**************************************************************************************************************************************************************************/
int SearchMyDirector(NextNode * search_director_name, AdminDirector * AD){
	NextDirector * ptr_director_node = AD->head->next_director_node;

	while(1){
		if(ptr_director_node == AD->tail){
			search_director_name->my_node = NULL;
			break;
		}

		else if(strcmp(ptr_director_node->director_name, search_director_name->name) == 0){ // same
			search_director_name->my_node = ptr_director_node;
			return TRUE;
		}

		else
			ptr_director_node = ptr_director_node->next_director_node;
	}
	return FALSE;
}
/**************************************************************************************************************************************************************************/
int SaveMovie(AdminMovie * AM){
	FILE * fptr_movie_list = fopen("movie_list.save","wt");
	NextMovie * ptr_movie_node = AM->head->next_movie_node;
	NextNode * ptr_actor_name = ptr_movie_node->actor_name;

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
		if(scanf("%s", option) == 1){ // tgdyra  or -f

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
			if(ptr_movie_node != AM->tail){
				fprintf(fptr_movie_list, "Title : %s\n", ptr_movie_node->movie_name); 
				fprintf(fptr_movie_list, "Genre : %s\n", ptr_movie_node->genre);
				fprintf(fptr_movie_list, "Director : %s\n", ptr_movie_node->director_name->name);
				fprintf(fptr_movie_list, "Year : %d\n", ptr_movie_node->year);
				fprintf(fptr_movie_list, "Time : %d\n", ptr_movie_node->time);
				fprintf(fptr_movie_list, "Actor : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						fprintf(fptr_movie_list, "%s\n", ptr_actor_name->name);
						break;
					}
					fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
				ptr_movie_node = ptr_movie_node->next_movie_node;
				ptr_actor_name = ptr_movie_node->actor_name;
				fprintf(fptr_movie_list, "\n");
			}
			else
				break;
		}
	}

	else{ // yes option
		while(1){
			if(ptr_movie_node == AM->tail)
				break;

			while(1){
				ch = *(option + option_check);
				if(ch == 't')
					fprintf(fptr_movie_list, "Title : %s\n", ptr_movie_node->movie_name);
				else if(ch == 'g')
					fprintf(fptr_movie_list, "Genre : %s\n", ptr_movie_node->genre);
				else if(ch == 'd')
					fprintf(fptr_movie_list, "Director : %s\n", ptr_movie_node->director_name->name);
				else if(ch == 'y')
					fprintf(fptr_movie_list, "Year : %d\n", ptr_movie_node->year);
				else if(ch == 'r')
					fprintf(fptr_movie_list, "Time : %s\n", ptr_movie_node->time);
				else if(ch == 'a'){
					ptr_actor_name = ptr_movie_node->actor_name;
					fprintf(fptr_movie_list, "actor : ");

					while(ptr_actor_name->next_node != NULL){
						fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
						ptr_actor_name = ptr_actor_name->next_node;
					}
					fprintf(fptr_movie_list, "%s\n", ptr_actor_name->name);
				}
				else if(ch == '\0'){
					option_check = 0;
					break;
				}
				++option_check;
			}
			ptr_movie_node = ptr_movie_node->next_movie_node;
			fprintf(fptr_movie_list, "\n");
		}
	}
	fclose(fptr_movie_list);
	free(option);
	if(check == 1 || check == 2){
		rename("movie_list.save", file_name);
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
/**************************************************************************************************************************************************************************/
int CompareString(const void * first, const void * second){
	return strcmp(*(char **)first, *(char **)second);
}
/**************************************************************************************************************************************************************************/
int CompareTimeYear(const void * first, const void * second){
	return *(int *)first - *(int *)second;
}
/**************************************************************************************************************************************************************************/
int SortMovie(AdminMovie * AM){
	FILE * fptr_movie_list;

	NextMovie ** dptr_movie = (NextMovie **)malloc(sizeof(NextMovie *) * AM->size);
	NextMovie * ptr_movie_node = AM->head->next_movie_node;
	NextNode * ptr_actor_name = ptr_movie_node->actor_name;

	char ** sort_string_arr = (char **)malloc(sizeof(char *) * AM->size);
	char * tmp = (char *)malloc(sizeof(char) * 100);
	char * file_name = (char *)malloc(sizeof(char) * 100);
	char option;
	int check;
	int * time_year = (int *)malloc(sizeof(int) * AM->size);

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

	if(check == 3 || check == 0){ // no file_name
		if(option == 'g'){
			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->genre;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(*sort_string_arr), CompareString);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_movie_node->genre) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				printf("Title : %s\n", (*(dptr_movie + i))->movie_name);
				printf("Genre : %s\n", (*(dptr_movie + i))->genre);
				printf("Director : %s\n", (*(dptr_movie + i))->director_name->name);
				printf("Year : %d\n", (*(dptr_movie + i))->year);
				printf("Time : %d\n", (*(dptr_movie + i))->time);
				printf("Acotr : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						printf("%s\n", ptr_actor_name->name);
						break;
					}
					printf("%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else if(option == 'd'){
			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->director_name->name;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_movie_node->director_name->name) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				printf("Title : %s\n", (*(dptr_movie + i))->movie_name);
				printf("Genre : %s\n", (*(dptr_movie + i))->genre);
				printf("Director : %s\n", (*(dptr_movie + i))->director_name->name);
				printf("Year : %d\n", (*(dptr_movie + i))->year);
				printf("Time : %d\n", (*(dptr_movie + i))->time);
				printf("Acotr : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						printf("%s\n", ptr_actor_name->name);
						break;
					}
					printf("%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}

		else if(option == 'y'){
			for(int i = 0; i < AM->size ; ++i){
				*(time_year + i) = ptr_movie_node->year;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}
			qsort((void *)time_year, AM->size, sizeof(time_year[0]), CompareTimeYear);
			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(*(time_year + i) == ptr_movie_node->year){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				printf("Title : %s\n", (*(dptr_movie + i))->movie_name);
				printf("Genre : %s\n", (*(dptr_movie + i))->genre);
				printf("Director : %s\n", (*(dptr_movie + i))->director_name->name);
				printf("Year : %d\n", (*(dptr_movie + i))->year);
				printf("Time : %d\n", (*(dptr_movie + i))->time);
				printf("Acotr : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						printf("%s\n", ptr_actor_name->name);
						break;
					}
					printf("%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}

		else if(option == 'r'){
			for(int i = 0; i < AM->size ; ++i){
				*(time_year + i) = ptr_movie_node->time;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)time_year, AM->size, sizeof(time_year[0]), CompareTimeYear);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(*(time_year + i) == ptr_movie_node->time){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				printf("Title : %s\n", (*(dptr_movie + i))->movie_name);
				printf("Genre : %s\n", (*(dptr_movie + i))->genre);
				printf("Director : %s\n", (*(dptr_movie + i))->director_name->name);
				printf("Year : %d\n", (*(dptr_movie + i))->year);
				printf("Time : %d\n", (*(dptr_movie + i))->time);
				printf("Acotr : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						printf("%s\n", ptr_actor_name->name);
						break;
					}
					printf("%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else if(option == 'a'){

			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->actor_name->name; // only first actor
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_movie_node->actor_name->name) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				printf("Title : %s\n", (*(dptr_movie + i))->movie_name);
				printf("Genre : %s\n", (*(dptr_movie + i))->genre);
				printf("Director : %s\n", (*(dptr_movie + i))->director_name->name);
				printf("Year : %d\n", (*(dptr_movie + i))->year);
				printf("Time : %d\n", (*(dptr_movie + i))->time);
				printf("Actor : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						printf("%s\n", ptr_actor_name->name);
						break;
					}
					printf("%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else if(option == 't' || option == 'z'){
			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->movie_name;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_movie_node->movie_name) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				printf("Title : %s\n", (*(dptr_movie + i))->movie_name);
				printf("Genre : %s\n", (*(dptr_movie + i))->genre);
				printf("Director : %s\n", (*(dptr_movie + i))->director_name->name);
				printf("Year : %d\n", (*(dptr_movie + i))->year);
				printf("Time : %d\n", (*(dptr_movie + i))->time);
				printf("Actor : ");
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				while(1){
					if(ptr_actor_name->next_node == NULL){
						printf("%s\n", ptr_actor_name->name);
						break;
					}
					printf("%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else{
			free(dptr_movie);
			free(sort_string_arr);
			free(tmp);
			free(file_name);
			free(time_year);
			printf("Option Error\n");
			return FALSE;
		}
	}
	else if(check == 1 || check == 2){ // yes file_name
		fptr_movie_list = fopen(file_name ,"wt");
		if(option == 'g'){
			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->genre;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_movie_node->genre) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				fprintf(fptr_movie_list, "Title : %s\n", (*(dptr_movie + i))->movie_name);
				fprintf(fptr_movie_list, "Genre : %s\n", (*(dptr_movie + i))->genre);
				fprintf(fptr_movie_list, "Director : %s\n", (*(dptr_movie + i))->director_name->name);
				fprintf(fptr_movie_list, "Year : %d\n", (*(dptr_movie + i))->year);
				fprintf(fptr_movie_list, "Time : %d\n", (*(dptr_movie + i))->time);
				fprintf(fptr_movie_list, "Actor : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						fprintf(fptr_movie_list, "%s\n\n", ptr_actor_name->name);
						break;
					}
					fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else if(option == 'd'){
			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->director_name->name;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_movie_node->director_name->name) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				fprintf(fptr_movie_list, "Title : %s\n", (*(dptr_movie + i))->movie_name);
				fprintf(fptr_movie_list, "Genre : %s\n", (*(dptr_movie + i))->genre);
				fprintf(fptr_movie_list, "Director : %s\n", (*(dptr_movie + i))->director_name->name);
				fprintf(fptr_movie_list, "Year : %d\n", (*(dptr_movie + i))->year);
				fprintf(fptr_movie_list, "Time : %d\n", (*(dptr_movie + i))->time);
				fprintf(fptr_movie_list, "Acotr : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						fprintf(fptr_movie_list, "%s\n\n", ptr_actor_name->name);
						break;
					}
					fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}

		else if(option == 'y'){
			for(int i = 0; i < AM->size ; ++i){
				*(time_year + i) = ptr_movie_node->year;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}
			qsort((void *)time_year, AM->size, sizeof(time_year[0]), CompareTimeYear);
			for(int i=0;i<3;++i)
				fprintf(fptr_movie_list, "%d\n", *(time_year + i));
			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(*(time_year + i) == ptr_movie_node->year){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				fprintf(fptr_movie_list, "Title : %s\n", (*(dptr_movie + i))->movie_name);
				fprintf(fptr_movie_list, "Genre : %s\n", (*(dptr_movie + i))->genre);
				fprintf(fptr_movie_list, "Director : %s\n", (*(dptr_movie + i))->director_name->name);
				fprintf(fptr_movie_list, "Year : %d\n", (*(dptr_movie + i))->year);
				fprintf(fptr_movie_list, "Time : %d\n", (*(dptr_movie + i))->time);
				fprintf(fptr_movie_list, "Acotr : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						fprintf(fptr_movie_list, "%s\n\n", ptr_actor_name->name);
						break;
					}
					fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}

		else if(option == 'r'){
			for(int i = 0; i < AM->size ; ++i){
				*(time_year + i) = ptr_movie_node->time;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)time_year, AM->size, sizeof(time_year[0]), CompareTimeYear);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(*(time_year + i) == ptr_movie_node->time){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				fprintf(fptr_movie_list, "Title : %s\n", (*(dptr_movie + i))->movie_name);
				fprintf(fptr_movie_list, "Genre : %s\n", (*(dptr_movie + i))->genre);
				fprintf(fptr_movie_list, "Director : %s\n", (*(dptr_movie + i))->director_name->name);
				fprintf(fptr_movie_list, "Year : %d\n", (*(dptr_movie + i))->year);
				fprintf(fptr_movie_list, "Time : %d\n", (*(dptr_movie + i))->time);
				fprintf(fptr_movie_list, "Acotr : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						fprintf(fptr_movie_list, "%s\n\n", ptr_actor_name->name);
						break;
					}
					fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else if(option == 'a'){
			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->actor_name->name;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(sort_string_arr[0]), CompareString);
			ptr_movie_node = AM->head->next_movie_node; // error plz
			for(int i = 0; i < AM->size; ++i){
				for(int j = 0; j < AM->size; ++j){
					if(strcmp(*(sort_string_arr + j), ptr_movie_node->actor_name->name) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
				}
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			for(int i = 0; i < AM->size; ++i){
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				fprintf(fptr_movie_list, "Title : %s\n", (*(dptr_movie + i))->movie_name);
				fprintf(fptr_movie_list, "Genre : %s\n", (*(dptr_movie + i))->genre);
				fprintf(fptr_movie_list, "Director : %s\n", (*(dptr_movie + i))->director_name->name);
				fprintf(fptr_movie_list, "Year : %d\n", (*(dptr_movie + i))->year);
				fprintf(fptr_movie_list, "Time : %d\n", (*(dptr_movie + i))->time);
				fprintf(fptr_movie_list, "Actor : ");
				while(1){
					if(ptr_actor_name->next_node == NULL){
						fprintf(fptr_movie_list, "%s\n\n", ptr_actor_name->name);
						break;
					}
					fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else if (option == 't' || option == 'z'){
			for(int i = 0; i < AM->size ; ++i){
				*(sort_string_arr + i) = ptr_movie_node->movie_name;
				ptr_movie_node = ptr_movie_node->next_movie_node;
			}

			qsort((void *)sort_string_arr, AM->size, sizeof(sort_string_arr[0]), CompareString);

			for(int i = 0; i < AM->size; ++i){
				ptr_movie_node = AM->head->next_movie_node; // error check plz
				while(1){
					if(strcmp(*(sort_string_arr + i), ptr_movie_node->movie_name) == 0){
						*(dptr_movie + i) = ptr_movie_node;
						break;
					}
					else
						ptr_movie_node = ptr_movie_node->next_movie_node;
				}
			}

			for(int i = 0; i < AM->size; ++i){
				fprintf(fptr_movie_list, "Title : %s\n", (*(dptr_movie + i))->movie_name);
				fprintf(fptr_movie_list, "Genre : %s\n", (*(dptr_movie + i))->genre);
				fprintf(fptr_movie_list, "Director : %s\n", (*(dptr_movie + i))->director_name->name);
				fprintf(fptr_movie_list, "Year : %d\n", (*(dptr_movie + i))->year);
				fprintf(fptr_movie_list, "Time : %d\n", (*(dptr_movie + i))->time);
				fprintf(fptr_movie_list, "Actor : ");
				ptr_actor_name = (*(dptr_movie + i))->actor_name;
				while(1){
					if(ptr_actor_name->next_node == NULL){
						fprintf(fptr_movie_list, "%s\n\n", ptr_actor_name->name);
						break;
					}
					fprintf(fptr_movie_list, "%s, ", ptr_actor_name->name);
					ptr_actor_name = ptr_actor_name->next_node;
				}
			}
			putchar('\n');
		}
		else{
			fclose(fptr_movie_list);
			printf("Option Error\n");
			return FALSE;
		}
		fclose(fptr_movie_list);
	}
	free(dptr_movie);
	free(sort_string_arr);
	free(tmp);
	free(file_name);
	free(time_year);
	printf("@@Done\n");
	return TRUE;
}
/**************************************************************************************************************************************************************************/
int DeleteMovie(AdminMovie * AM){
	FILE * fptr_movie_log = fopen("movie_log", "at");
	int serial_num;
	NextMovie * match_movie_node;
	NextMovie * bptr_movie_node = AM->head;
	NextNode * destroy_node;
	NextNode * curp;

	scanf("%d", &serial_num);
	match_movie_node = SearchMovieSerial(AM, serial_num);
	if(match_movie_node == NULL){
		printf("No such record\n");
		return FALSE;	
	}

	while(bptr_movie_node->next_movie_node != match_movie_node)
		bptr_movie_node = bptr_movie_node->next_movie_node;

	bptr_movie_node->next_movie_node = match_movie_node->next_movie_node;

	free(match_movie_node->movie_name);
	free(match_movie_node->genre);
	free(match_movie_node->director_name->name);
	free(match_movie_node->director_name);
	destroy_node = match_movie_node->actor_name;
	free(match_movie_node);

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
	fprintf(fptr_movie_log, "delete:%d::::::\n", serial_num);
	fclose(fptr_movie_log);
	--AM->size;
	printf("@@Done\n");
	return TRUE;
}
/**************************************************************************************************************************************************************************/
int PrintMovie(AdminMovie * AM){ // Movie Update have to repair
	NextMovie * ptr_movie_node;
	NextNode * ptr_actor_name; 
	NextActor * ptr_my_actor_node = NULL;
	NextDirector * ptr_my_director_node = NULL;
	int serial_num;
	int i = 1;

	scanf("%d", &serial_num);
	ptr_movie_node = SearchMovieSerial(AM, serial_num);

	if(ptr_movie_node == NULL){
		printf("No such record\n");
		return FALSE;
	}

	ptr_my_director_node = (NextDirector *)ptr_movie_node->director_name->my_node;
	ptr_actor_name = ptr_movie_node->actor_name;

	printf("%d, %s, %s\n", ptr_movie_node->serial_number, ptr_movie_node->movie_name ,ptr_movie_node->genre);
	printf("\t D : %s", ptr_movie_node->director_name->name);

	if(ptr_my_director_node == NULL)
		printf("(-)\n");
	else
		printf("(%s)\n", ptr_my_director_node->birth);

	while(1){
		ptr_my_actor_node = (NextActor *)ptr_actor_name->my_node;

		if(ptr_actor_name->next_node == NULL){
			printf("\tA%d : %s", i, ptr_actor_name->name);

			if(ptr_my_actor_node != NULL)
				printf("(%s)\n", ptr_my_actor_node->birth);
			else
				printf("(-)\n");

			return TRUE;
		}

		printf("\tA%d : %s", i, ptr_actor_name->name);

		if(ptr_my_actor_node != NULL)
			printf("(%s)\n", ptr_my_actor_node->birth);
		else
			printf("(-)\n");

		ptr_actor_name = ptr_actor_name->next_node;
		++i;
	}
	printf("@@Done\n");
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchMovie(AdminMovie * AM, char * string){
	NextMovie * ptr_movie_node = AM->head;
	NextNode * ptr_actor_name;

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
			SearchStringMovie(AM, substr);
		}

		else if(*(string + len - 1) == '*'){ // last *
			*(substr + len - 1) = '\0';
			SearchStringMovie(AM, substr);
		}
	}

	else if (checksm == 0 && checkqm != 0){	// ?
		SearchMovieQM(AM, substr);
	}

	else if (checksm != 0 && checkqm != 0){ // ? and *
		SearchMovieQMSM(AM, substr);
	}

	else if (checksm == 0 && checkqm == 0){ // no
		SearchStringMovie(AM, substr);
	}

	free(serial_string);
	free(substr);
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchMovieQMSM(AdminMovie * AM, char * substr){
	NextMovie * ptr_movie_node = AM->head;
	NextNode * ptr_actor_name;
	char * procstr = (char *)malloc(strlen(substr) + 1);
	strcpy(procstr, substr);

	if(*procstr == '*' && *(procstr + strlen(procstr) - 1) == '*'){
		*(procstr + strlen(procstr) - 1) = '\0';
		procstr = procstr + 1;
		SearchMovieQM(AM, procstr);
	}
	else if(*procstr == '*'){
		procstr = procstr + 1;
		SearchMovieQM(AM, procstr);
	}
	else if(*(procstr + strlen(procstr) - 1) == '*'){
		*(procstr + strlen(procstr) - 1) = '\0';
		SearchMovieQM(AM, procstr);
	}
	free(procstr);
	return TRUE;
}
/*****************************************************************************************************************************************/
int SearchMovieQM(AdminMovie * AM, char * substr){
	NextMovie * ptr_movie_node = AM->head;
	NextNode * ptr_actor_name;
	char * cmpstr = (char *)malloc(100);
	char * serial_string = (char *)malloc(10);
	int check;

	while(1){ // check movie_name
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
		else
			strcpy(cmpstr, ptr_movie_node->movie_name);

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
				MatchMoviePrint(ptr_movie_node);
				break;
			}

			else
				cmpstr = cmpstr + 1;

			if(*(cmpstr + strlen(substr) - 1) == '\0')
				break;
		}
	}
	ptr_movie_node = AM->head;

	while(1){ // check serial
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_movie_node->serial_number);
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
				MatchMoviePrint(ptr_movie_node);
				break;
			}

			else
				serial_string = serial_string + 1;

			if(*(serial_string + strlen(substr) - 1) == '\0')
				break;
		}
	}

	ptr_movie_node = AM->head;

	while(1){ // check genre
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
		else
			strcpy(cmpstr, ptr_movie_node->genre);

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
				MatchMoviePrint(ptr_movie_node);
				break;
			}
			cmpstr = cmpstr + 1;

			if(*(cmpstr + strlen(substr) - 1) == '\0')
				break;
		}
	}
	ptr_movie_node = AM->head;

	while(1){ // check director_name
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
		else
			strcpy(cmpstr, ptr_movie_node->director_name->name);

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
				MatchMoviePrint(ptr_movie_node);
				break;
			}
			cmpstr = cmpstr + 1;

			if(*(cmpstr + strlen(substr) - 1) == '\0')
				break;
		}
	}
	ptr_movie_node = AM->head;

	while(1){ // check year
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_movie_node->year);
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
				MatchMoviePrint(ptr_movie_node);
				break;
			}

			else
				serial_string = serial_string + 1;

			if(*(serial_string + strlen(substr) - 1) == '\0')
				break;
		}
	}
	ptr_movie_node = AM->head;

	while(1){ // check time
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_movie_node->time);
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
				MatchMoviePrint(ptr_movie_node);
				break;
			}

			else
				serial_string = serial_string + 1;

			if(*(serial_string + strlen(substr) - 1) == '\0')
				break;
		}
	}
	ptr_movie_node = AM->head;

	while(1){ // check actor_name
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
	
		else{
			ptr_actor_name = ptr_movie_node->actor_name;
			while(1){
				if(ptr_actor_name->next_node == NULL){
					strcpy(cmpstr, ptr_actor_name->name);	
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
							MatchMoviePrint(ptr_movie_node);
							break;
						}
						cmpstr = cmpstr + 1;

						if(*(cmpstr + strlen(substr) - 1) == '\0')
							break; // while 3을 탈출
					} // while 3
					break; // while 2를 탈출
				}
				else{
					strcpy(cmpstr, ptr_actor_name->name);	
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
							MatchMoviePrint(ptr_movie_node);
							break;
						}
						cmpstr = cmpstr + 1;

						if(*(cmpstr + strlen(substr) - 1) == '\0')
							break; // while 3을 탈출
					 } // while 3
				}
			   ptr_actor_name = ptr_actor_name->next_node;	
			} // else에서 while 2
		}
	} // while 1
	cmpstr = NULL;
	free(cmpstr);
	return TRUE;
}
/*****************************************************************************************************************************************/
void SearchStringMovie(AdminMovie * AM, char * substr){
	NextMovie * ptr_movie_node = AM->head;
	NextNode * ptr_actor_name;
	char * serial_string = (char *)malloc(10);
	char * year_string = (char *)malloc(10);
	char * time_string = (char *)malloc(10);

	while(1){
		ptr_movie_node = ptr_movie_node->next_movie_node;

		if(ptr_movie_node == AM->tail)
			break;
		else{
			sprintf(serial_string, "%d", ptr_movie_node->serial_number);
			sprintf(year_string, "%d", ptr_movie_node->year);
			sprintf(time_string, "%d", ptr_movie_node->time);
			ptr_actor_name = ptr_movie_node->actor_name;
		}

		if(strstr(serial_string, substr)) // serial
			MatchMoviePrint(ptr_movie_node);

		else if(strstr(ptr_movie_node->movie_name, substr)) // name
			MatchMoviePrint(ptr_movie_node);
		
		else if(strstr(ptr_movie_node->genre, substr)) // genre
			MatchMoviePrint(ptr_movie_node);

		else if(strstr(ptr_movie_node->director_name->name, substr)) // director_name
			MatchMoviePrint(ptr_movie_node);
		
		else if(strstr(year_string, substr)) // year
			MatchMoviePrint(ptr_movie_node);
		
		else if(strstr(serial_string, substr)) // time
			MatchMoviePrint(ptr_movie_node);

		else { // actor_name
			while(1){
				if(ptr_actor_name->next_node == NULL){
					if(strstr(ptr_actor_name->name, substr))
						MatchMoviePrint(ptr_movie_node);
					break;
				}
				else if(strstr(ptr_actor_name->name, substr)){
					MatchMoviePrint(ptr_movie_node);
					break;
				}
				else
					ptr_actor_name = ptr_actor_name->next_node;
			}
		}
	}
	free(serial_string);   
	free(year_string);   
	free(time_string);   
}
/*****************************************************************************************************************************************/
void MatchMoviePrint(NextMovie * ptr_movie_node){
	NextNode * ptr_actor_name = ptr_movie_node->actor_name;
	printf("%d:%s:%s:%s:%d:%d:", ptr_movie_node->serial_number, ptr_movie_node->movie_name, ptr_movie_node->genre, ptr_movie_node->director_name->name, ptr_movie_node->year, ptr_movie_node->time);
	while(1){
		if(ptr_actor_name->next_node == NULL){
			printf("%s\n", ptr_actor_name->name);
			putchar('\n');
			break;
		}
		printf("%s, ", ptr_actor_name->name);
		ptr_actor_name = ptr_actor_name->next_node;
	}
}
/*****************************************************************************************************************************************/
int AddMovie(AdminMovie * AM){
	FILE * fptr_movie_log = fopen("movie_log", "at");
	NextMovie * newp = (NextMovie *)malloc(sizeof(NextMovie));
	NextMovie * btp = AM->head;
	NextMovie * match_movie_node;
	NextNode * ptr_actor_name;
	NextNode * ptmp = newp->actor_name = (NextNode *)malloc(sizeof(NextNode));
	ptmp->my_node = NULL;
	char * tmp = (char *)malloc(100);
	char yn;
	char ch = 0;
	int i = 0;
	getchar();
	newp->serial_number = ++movie_serial;
	
	printf(" title > ");
	scanf("%[^\n]", tmp); getchar();

	if((match_movie_node = ThereisSameMovie(AM,tmp)) != NULL){
		ptr_actor_name = match_movie_node->actor_name;
		printf("@@You have the same record in movie list.\n");
		printf("%d:%s:%s:%s:%d:%d:",match_movie_node->serial_number, match_movie_node->movie_name, match_movie_node->genre, match_movie_node->director_name->name, match_movie_node->year, match_movie_node->time);
		while(1){
			if(ptr_actor_name->next_node == NULL){
				printf("%s\n", ptr_actor_name->name);
				break;
			}
			printf("%s, ", ptr_actor_name->name);
			ptr_actor_name = ptr_actor_name->next_node;
		}
		printf("Do you want to add any way? (Y / N)");
		yn = getchar(); getchar();
		if(yn == 'N' || yn == 'n')
			return TRUE;
	}

	fprintf(fptr_movie_log, "add:%d:", newp->serial_number);
	newp->movie_name = (char *)malloc(strlen(tmp)+1);
	strcpy(newp->movie_name, tmp);
	ColonCheckInMovie(fptr_movie_log, newp->movie_name);
	
	printf(" genre > ");
	scanf("%[^\n]", tmp); getchar();
	newp->genre = (char *)malloc(strlen(tmp)+1);
	strcpy(newp->genre, tmp);
	ColonCheckInMovie(fptr_movie_log, newp->genre);
	
	printf(" director > ");
	scanf("%[^\n]", tmp); getchar();
	newp->director_name = (NextNode *)malloc(sizeof(NextNode));
	newp->director_name->name = (char *)malloc(strlen(tmp) + 1);
	strcpy(newp->director_name->name, tmp);
	ColonCheckInMovie(fptr_movie_log, newp->director_name->name);
	
	printf(" year > ");
	scanf("%d", &(newp->year)); getchar();
	fprintf(fptr_movie_log,"%d:", newp->year);
	
	printf(" time > ");
	scanf("%d",&(newp->time)); getchar();
	fprintf(fptr_movie_log,"%d:", newp->time);
	
	printf(" actor > ");

	while (1) { // processing actor
		ch = getchar();

		if (ch == ',' || ch == '\n') {

			*(tmp + i) = '\0';

			if(*tmp == ' ')
				tmp = tmp + 1;

			newp->actor_name->name = (char *)malloc(strlen(tmp) + 1);
			strcpy(newp->actor_name->name, tmp);
			fprintf(fptr_movie_log, "%s", newp->actor_name->name); //////////////////////////////////////////////////////colon check

			if(ch == ','){
				fputc(',', fptr_movie_log);
				newp->actor_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				newp->actor_name->my_node = NULL;
				newp->actor_name = newp->actor_name->next_node;
				i = 0;
			}

			else if(ch == '\n'){
				fputc('\n', fptr_movie_log);
				newp->actor_name->next_node = NULL;
				break;
			}
		}
		else {
			*(tmp + i) = ch;
			++i;
		}

	}

	newp->actor_name = ptmp;
	while(btp->next_movie_node != AM->tail)
		btp = btp->next_movie_node;

	newp->next_movie_node = AM->tail;
	btp->next_movie_node = newp;
	++AM->size;
	free(tmp);
	fclose(fptr_movie_log);
	printf("@@Done\n");
	return TRUE;
}
/*****************************************************************************************************************************************/
int ColonCheckInMovie(FILE * fptr_movie_log, char * string){ // change : to ??;
	int i = 0;
	char ch = 'x';
	char * address;
	
	if((address = strchr(string, ':')) != NULL){
		while(1){
			ch = *(string + i);
			if(ch == ':')
				break;
			else
				fputc(ch, fptr_movie_log);
			++i;
		}
		fprintf(fptr_movie_log,"??;%s:", address + 1);
	}
	else
		fprintf(fptr_movie_log, "%s:", string);

	return TRUE;
}
/*****************************************************************************************************************************************/
int ColonCheckInActorname(FILE * fptr_movie_log, char * string){ // change : to ??;
	int i = 0;
	char ch = 'x';
	char * address;

	if((address = strchr(string, ':')) != NULL){
		while(1){
			ch = *(string + i);
			if(ch == ':')
				break;
			else
				fputc(ch, fptr_movie_log);
			++i;
		}
		fprintf(fptr_movie_log,"??;%s", address + 1);
	}
	else
		fprintf(fptr_movie_log, "%s", string);
}
/*****************************************************************************************************************************************/
NextMovie * ThereisSameMovie(AdminMovie * AM, char * string){
	NextMovie * ptr_movie_node = AM->head;

	while(1){
		ptr_movie_node = ptr_movie_node->next_movie_node;
		if(ptr_movie_node == AM->tail)
			break;
		if(strcmp(string, ptr_movie_node->movie_name)==0)
			return ptr_movie_node;
	}
	return NULL;
}
/*****************************************************************************************************************************************/
int UpdateMovie(AdminMovie * AM){

	FILE * fptr_movie_log = fopen("movie_log", "at");
	NextMovie * ptr_movie_node;
	char * option = (char *)malloc(10);
	char * sub = (char *)malloc(10);
	char tmp;
	int serial_num = 0;

	if(scanf("%d", &serial_num)){ // no option
		getchar();
		ptr_movie_node = SearchMovieSerial(AM, serial_num);
		if(UpdateMovieName(fptr_movie_log, AM, serial_num) == FALSE)
			return FALSE;

		UpdateMovieGenre(fptr_movie_log, ptr_movie_node); //////////////////////////////////////////////////////////
		UpdateMovieDirector(fptr_movie_log, ptr_movie_node);
		UpdateMovieYear(fptr_movie_log, ptr_movie_node);
		UpdateMovieTime(fptr_movie_log, ptr_movie_node);
		UpdateMovieActorname(fptr_movie_log, ptr_movie_node);
	}

	else{
		scanf("%s", sub);
		getchar();
		scanf("%d", &serial_num);
		while(getchar() != '\n') ; 
		ptr_movie_node = SearchMovieSerial(AM, serial_num);

		for(int i = 0; i < strlen(sub); ++i){
			if(*(sub + i) == 't')
				*(option) = 't';
			else if(*(sub + i) == 'g')
				*(option + 1) = 'g';
			else if(*(sub + i) == 'd')
				*(option + 2) = 'd';
			else if(*(sub + i) == 'y')
				*(option + 3) = 'y';		
			else if(*(sub + i) == 'r')
				*(option + 4) = 'r';		
			else if(*(sub + i) == 'a')
				*(option + 5) = 'a';		
		}

		if(*(option) != 't')
			fprintf(fptr_movie_log, "update:%d:", serial_num);
		
		for(int i = 0; i < 6 ;++i){
			if(*(option + i) == 't'){
				if(UpdateMovieName(fptr_movie_log, AM, serial_num) == FALSE){
					return FALSE;
				}
			}

			else if(*(option + i) == 'g'){
				if(UpdateMovieGenre(fptr_movie_log, ptr_movie_node) == FALSE){
					return FALSE;
				}
			}

			else if(*(option + i) == 'd'){
				if(UpdateMovieDirector(fptr_movie_log, ptr_movie_node) == FALSE){
					return FALSE;
				}
			}

			else if(*(option + i) == 'y'){
				if(UpdateMovieYear(fptr_movie_log, ptr_movie_node) == FALSE){
					return FALSE;
				}
			}
			else if(*(option + i) == 'r'){
				if(UpdateMovieTime(fptr_movie_log, ptr_movie_node) == FALSE){
					return FALSE;
				}
			}
			else if(*(option + i) == 'a'){
				if(UpdateMovieActorname(fptr_movie_log, ptr_movie_node) == FALSE){
					return FALSE;
				}
			}
			else{
				fprintf(fptr_movie_log, "=:");
			}
		}
	}
	fclose(fptr_movie_log);
	return TRUE;
}
/*****************************************************************************************************************************************/
int UpdateMovieName(FILE * fptr_movie_log, AdminMovie * AM, int serial_num){
	NextMovie * ptr_movie_node = SearchMovieSerial(AM, serial_num);
	NextMovie * match_movie_node;
	NextNode * ptr_actor_name;
	char * tmp = (char *)malloc(100);
	char ch;
	if(ptr_movie_node == NULL){
		printf("There is no match serial\n");
		return FALSE;
	}

	printf("title > ");
	scanf("%[^\n]", tmp); getchar();

	if((match_movie_node = ThereisSameMovie(AM, tmp)) == NULL){
		free(ptr_movie_node->movie_name);
		ptr_movie_node->movie_name = (char *)malloc(strlen(tmp) + 1);
		strcpy(ptr_movie_node->movie_name, tmp);
		fprintf(fptr_movie_log, "update:%d:", serial_num);
		ColonCheckInMovie(fptr_movie_log, ptr_movie_node->movie_name); // fprintf
	}

	else{
		ptr_actor_name = match_movie_node->actor_name;
		printf("@@You have the same record in movie list.\n");
		printf("Title : %s\n", match_movie_node->movie_name);
		printf("Genre : %s\n", match_movie_node->genre);
		printf("Director : %s\n", match_movie_node->director_name->name);
		printf("Year : %d\n", match_movie_node->year);
		printf("Time : %d\n", match_movie_node->time);
		printf("Actor name : ");
		
		while(1){
			if(ptr_actor_name->next_node == NULL){
				printf("%s\n", ptr_actor_name->name);
				break;
			}
			printf("%s, ", ptr_actor_name->name);
			ptr_actor_name = ptr_actor_name->next_node;
		}

		printf("Do you want to change the record? (Y / N)");
		ch = getchar(); getchar();

		if(ch == 'Y' || ch == 'y'){
			ptr_movie_node = ThereisSameMovie(AM, tmp);
			free(ptr_movie_node->movie_name);
			ptr_movie_node->movie_name = (char *)malloc(strlen(tmp) + 1);
			strcpy(ptr_movie_node->movie_name, tmp);
			fprintf(fptr_movie_log, "update:%d:", serial_num);
			ColonCheckInMovie(fptr_movie_log, ptr_movie_node->movie_name); // fprintf
		}
		else{
			return FALSE;
		}
	}
}
/*****************************************************************************************************************************************/
int UpdateMovieActorname(FILE * fptr_movie_log, NextMovie * ptr_movie_node){
	NextNode * destroy_node;
	NextNode * cur_node;
	NextNode * save_node;
	char * tmp = (char *)malloc(100);
	char ch;
	int cnt = 0;
	if(ptr_movie_node == NULL){
		printf("There is no match serial\n");
		return FALSE;
	}
	destroy_node = ptr_movie_node->actor_name;
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
	ptr_movie_node->actor_name = (NextNode *)malloc(sizeof(NextNode));
	save_node = ptr_movie_node->actor_name;
	printf("Actor name > ");
	
	while (1) {
		ch = getchar();

		if (ch == ',' || ch == '\n') {
			*(tmp + cnt) = '\0';

			if(*tmp == ' ')
				tmp = tmp + 1;

			ptr_movie_node->actor_name->name = (char *)malloc(strlen(tmp) + 1);
			strcpy(ptr_movie_node->actor_name->name, tmp);
			ColonCheckInActorname(fptr_movie_log, ptr_movie_node->actor_name->name);

			if(ch == ','){
				fputc(',', fptr_movie_log);
				ptr_movie_node->actor_name->next_node = (NextNode *)malloc(sizeof(NextNode));
				ptr_movie_node->actor_name->my_node = NULL;
				ptr_movie_node->actor_name = ptr_movie_node->actor_name->next_node;
				cnt = 0;
			}

			else if(ch == '\n'){
				fputc('\n', fptr_movie_log);
				ptr_movie_node->actor_name->my_node = NULL;
				ptr_movie_node->actor_name->next_node = NULL;
				break;
			}
		}

		else {
			*(tmp + cnt) = ch;
			++cnt;
		}
	}
	ptr_movie_node->actor_name = save_node;
	return TRUE;
}

/*****************************************************************************************************************************************/
int UpdateMovieGenre(FILE * fptr_movie_log, NextMovie * ptr_movie_node){
	char * tmp = (char *)malloc(100);
	char ch;

	printf("Genre > ");
	scanf("%[^\n]", tmp); getchar();

	free(ptr_movie_node->genre);
	ptr_movie_node->genre = (char *)malloc(strlen(tmp) + 1);
	strcpy(ptr_movie_node->genre, tmp);
	ColonCheckInMovie(fptr_movie_log, ptr_movie_node->genre); // fprintf
	return TRUE;
}
/*****************************************************************************************************************************************/
int UpdateMovieDirector(FILE * fptr_movie_log, NextMovie * ptr_movie_node){
	char * tmp = (char *)malloc(100);
	char ch;

	printf("Director > ");
	scanf("%[^\n]", tmp); getchar();

	free(ptr_movie_node->director_name->name);
	free(ptr_movie_node->director_name);
	ptr_movie_node->director_name = (NextNode *)malloc(sizeof(NextNode));
	ptr_movie_node->director_name->name = (char *)malloc(strlen(tmp) + 1);
	strcpy(ptr_movie_node->director_name->name, tmp);
	ColonCheckInMovie(fptr_movie_log, ptr_movie_node->director_name->name); // fprintf
	return TRUE;
}
/*****************************************************************************************************************************************/
int UpdateMovieYear(FILE * fptr_movie_log, NextMovie * ptr_movie_node){
	if(ptr_movie_node == NULL){
		printf("There is no match serial\n");
		return FALSE;
	}

	printf("Year > ");
	scanf("%d", &(ptr_movie_node->year)); getchar();
	fprintf(fptr_movie_log, "%d:", ptr_movie_node->year);
	return TRUE;
}
/*****************************************************************************************************************************************/
int UpdateMovieTime(FILE * fptr_movie_log, NextMovie * ptr_movie_node){
	if(ptr_movie_node == NULL){
		printf("There is no match serial\n");
		return FALSE;
	}

	printf("Time > ");
	scanf("%d", &(ptr_movie_node->time)); getchar();
	fprintf(fptr_movie_log, "%d:", ptr_movie_node->time);
	return TRUE;
}
/*****************************************************************************************************************************************/
