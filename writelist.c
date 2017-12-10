#ifndef Header_Include
	#define Header_Include
	#include "header.h"
#endif
/*************************************************************************************************************************************************/
char * MakeFilenameMovieList(void){
	struct tm * t;
	time_t timer;
	char * ret = (char *)malloc(50);

	timer = time(NULL);
	t = localtime(&timer);

	sprintf(ret,"movie_list.%d%02d%02d%02d%02d", t->tm_year+1900 , t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
	return ret;
}

int WriteMovieList(AdminMovie * AM){
	FILE * fptr_movie_list;
	NextMovie * ptr_movie_node = AM->head->next_movie_node;
	NextNode * ptr_actor_name = ptr_movie_node->actor_name;
	int mode = F_OK;
	char * pathname = "./movie_list.txt";
	char * list_title = MakeFilenameMovieList();

	if(access(pathname, mode) == 0){ // yes, there is movie_list.txt
		rename("movie_list.txt", list_title);
		fptr_movie_list = fopen("movie_list.txt", "wt");
	}
	else{ // no, there is no movie_list.txt
		fptr_movie_list = fopen("movie_list.txt", "wt");
	}
	free(list_title);

	while(1){
		if(ptr_movie_node != AM->tail){
			fprintf(fptr_movie_list, "%d:%s:%s:%s:%d:%d:", ptr_movie_node->serial_number, ptr_movie_node->movie_name, ptr_movie_node->genre, ptr_movie_node->director_name->name, ptr_movie_node->year, ptr_movie_node->time);
			
			while(1){
				if(ptr_actor_name->next_node == NULL){
					fprintf(fptr_movie_list, "%s\n", ptr_actor_name->name);
					break;
				}
				fprintf(fptr_movie_list, "%s,", ptr_actor_name->name);
				ptr_actor_name = ptr_actor_name->next_node;
			}
			ptr_movie_node = ptr_movie_node->next_movie_node;
			ptr_actor_name = ptr_movie_node->actor_name;
		}
		else
			break;
	}
		fclose(fptr_movie_list);
		return TRUE;
}
/*************************************************************************************************************************************************/
char * MakeFilenameDirectorList(void){
	struct tm * t;
	time_t timer;
	char * ret = (char *)malloc(50);

	timer = time(NULL);
	t = localtime(&timer);

	sprintf(ret,"director_list.%d%02d%02d%02d%02d", t->tm_year+1900 , t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
	return ret;
}

int WriteDirectorList(AdminDirector * AD){
	FILE * fptr_director_list;
	NextDirector * ptr_director_node = AD->head->next_director_node;
	NextNode * ptr_best_movie_name = ptr_director_node->best_movie_name;
	char * list_title = MakeFilenameDirectorList();
	int mode = F_OK;
	char * pathname = "./director_list.txt";
	
	if(access(pathname, mode) == 0){ // yes, there is movie_list.txt
		rename("director_list.txt", list_title);
		fptr_director_list = fopen("director_list.txt", "wt");
	}
	else{ // no, there is no movie_list.txt
		fptr_director_list = fopen("director_list.txt", "wt");
	}
	free(list_title);

	while(1){
		if(ptr_director_node != AD->tail){
			fprintf(fptr_director_list, "%d:%s:%c:%s:", ptr_director_node->serial_number, ptr_director_node->director_name, ptr_director_node->sex, ptr_director_node->birth);
			
			while(1){
				if(ptr_best_movie_name->next_node == NULL){
					fprintf(fptr_director_list, "%s\n", ptr_best_movie_name->name);
					break;
				}
				fprintf(fptr_director_list, "%s,", ptr_best_movie_name->name);
				ptr_best_movie_name = ptr_best_movie_name->next_node;
			}
			ptr_director_node = ptr_director_node->next_director_node;
			ptr_best_movie_name = ptr_director_node->best_movie_name;
		}
		else
			break;
	}
		fclose(fptr_director_list);
		return TRUE;
}
/*************************************************************************************************************************************************/
char * MakeFilenameActorList(void){
	struct tm * t;
	time_t timer;
	char * ret = (char *)malloc(50);

	timer = time(NULL);
	t = localtime(&timer);

	sprintf(ret,"actor_list.%d%02d%02d%02d%02d", t->tm_year+1900 , t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
	return ret;
}

int WriteActorList(AdminActor * AA){
	FILE * fptr_actor_list;
	NextActor * ptr_actor_node = AA->head->next_actor_node;
	NextNode * ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
	char * list_title = MakeFilenameActorList();
	char * pathname = "./actor_list.txt";
	int mode = F_OK;
	if(access(pathname, mode) == 0){ // yes, there is movie_list.txt
		rename("actor_list.txt", list_title);
		fptr_actor_list = fopen("actor_list.txt", "wt");
	}
	else{ // no, there is no movie_list.txt
		fptr_actor_list = fopen("actor_list.txt", "wt");
	}
	free(list_title);

	while(1){
		if(ptr_actor_node != AA->tail){
			fprintf(fptr_actor_list, "%d:%s:%c:%s:", ptr_actor_node->serial_number, ptr_actor_node->actor_name, ptr_actor_node->sex, ptr_actor_node->birth);
			
			while(1){
				if(ptr_appear_movie_name->next_node == NULL){
					fprintf(fptr_actor_list, "%s\n", ptr_appear_movie_name->name);
					break;
				}
				fprintf(fptr_actor_list, "%s,", ptr_appear_movie_name->name);
				ptr_appear_movie_name = ptr_appear_movie_name->next_node;
			}
			ptr_actor_node = ptr_actor_node->next_actor_node;
			ptr_appear_movie_name = ptr_actor_node->appear_movie_name;
		}
		else
			break;
	}
		fclose(fptr_actor_list);
		return TRUE;
}
/*************************************************************************************************************************************************/
char * ColonCheckInFILE (char *s, const char *olds, const char *news) {
  char *result, *sr;
  size_t i, count = 0;
  size_t oldlen = strlen(olds); if (oldlen < 1) return s;
  size_t newlen = strlen(news);

  if (newlen != oldlen) {
    for (i = 0; *(s + i) != '\0';) {
      if (memcmp(&(*(s+i)), olds, oldlen) == 0) count++, i += oldlen;
      else i++;
    }
  } else i = strlen(s);


  result = (char *) malloc(i + 1 + count * (newlen - oldlen));
  if (result == NULL) return NULL;


  sr = result;
  while (*s) {
    if (memcmp(s, olds, oldlen) == 0) {
      memcpy(sr, news, newlen);
      sr += newlen;
      s  += oldlen;
    } else *sr++ = *s++;
  }
  *sr = '\0';

  return result;
}
/*************************************************************************************************************************************************/
