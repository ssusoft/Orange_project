#ifndef Header_Include
	#define Header_Include
	#include "header.h"
#endif

void StartProgram(AdminMovie * AM, AdminDirector * AD, AdminActor * AA){
	printf(">> Welcome to My Movie <<\n");
	printf("File Loading.....\n");

	if(CreateMovieList(AM) == TRUE){
		ReadMovielog(AM, AD);
	}
	
	else{
		printf("CreateMovieList Error\n");
		printf("End program\n");
		exit(0);
	}

	if(CreateActorList(AA) == TRUE)
		ReadActorlog(AA);
	else{
		printf("CreateActorList Error\n");
		printf("End program\n");
		exit(0);
	}
	
	if(CreateDirectorList(AD) == TRUE)
		ReadDirectorlog(AD);

	else{
		printf("CreateDirectoreList Error\n");
		printf("End program\n");
		exit(0);
	}
	WriteMovieList(AM);
	WriteDirectorList(AD);
	WriteActorList(AA);
	Link(AA, AD, AM);
	printf("You can use add, update, delete, search, sort, save, end commands.\n");
}

void Link(AdminActor * AA, AdminDirector * AD, AdminMovie * AM){
	LinkMovieActor(AM, AA);
	LinkMovieDirector(AM, AD);
	LinkDirectorMovie(AD, AM);
	LinkActorMovie(AA, AM);
}

int LinkMovieActor(AdminMovie * AM, AdminActor * AA){
	NextNode * search_node;
	NextMovie * ptr_movie_node = AM->head->next_movie_node;
	while(1){
		if(ptr_movie_node == AM->tail)
			break;
		else{
			search_node = ptr_movie_node->actor_name;
			while(1){
				if(search_node->next_node == NULL){
					SearchMyActor(search_node, AA);
					break;
				}
				else{
					SearchMyActor(search_node, AA);
					search_node = search_node->next_node;
				}
			}
		}
		ptr_movie_node = ptr_movie_node->next_movie_node;
	}
	return TRUE;
}

int LinkMovieDirector(AdminMovie * AM, AdminDirector * AD){
	NextNode * search_node;
	NextMovie * ptr_movie_node = AM->head->next_movie_node;
	while(1){
		if(ptr_movie_node == AM->tail)
			break;
		else{
			search_node = ptr_movie_node->director_name;
			SearchMyDirector(search_node, AD);
			ptr_movie_node = ptr_movie_node->next_movie_node;
		}
	}
	return TRUE;
}
int LinkDirectorMovie(AdminDirector * AD, AdminMovie * AM){
	NextNode * search_node;
	NextDirector * ptr_director_node = AD->head->next_director_node;
	while(1){
		if(ptr_director_node == AD->tail)
			break;
		else{
			search_node = ptr_director_node->best_movie_name;
			while(1){
				if(search_node->next_node == NULL){
					SearchMyMovie(search_node, AM);
					break;
				}
				else{
					SearchMyMovie(search_node, AM);
					search_node = search_node->next_node;
				}
			}
			ptr_director_node = ptr_director_node->next_director_node;
		}
	}
}

int LinkActorMovie(AdminActor * AA, AdminMovie * AM){
	NextNode * search_node;
	NextActor * ptr_actor_node = AA->head->next_actor_node;
	while(1){
		if(ptr_actor_node == AA->tail)
			break;
		else{
			search_node = ptr_actor_node->appear_movie_name;
			while(1){
				if(search_node->next_node == NULL){
					SearchMyMovie(search_node, AM);
					break;
				}
				else{
					SearchMyMovie(search_node, AM);
					search_node = search_node->next_node;
				}
			}
		}
		ptr_actor_node = ptr_actor_node->next_actor_node;
	}

	return TRUE;
}
