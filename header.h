#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // file is there?
#include <signal.h> // signal

#define TRUE 1
#define FALSE 0

typedef struct _node {
	char * name;
	void * my_node; 
	struct _node * next_node;
}NextNode;
//
typedef struct _movie {
	int serial_number;
	char * movie_name;
	char * genre;
	struct _node * director_name;
	int year;
	int time;
	struct _node * actor_name;
	struct _movie * next_movie_node;
}NextMovie;

typedef struct admin_movie{
	struct _movie * head;
	struct _movie * tail;
	int size;
}AdminMovie;
// Movie

typedef struct _actor {
	int serial_number;
	char * actor_name;
	char sex;
	char * birth;
	struct _node * appear_movie_name;
	struct _actor * next_actor_node;
}NextActor;

typedef struct admin_actor{
	struct _actor * head;
	struct _actor * tail;
	int size;
}AdminActor;
// Actor

typedef struct _director {
	int serial_number;
	char * director_name;
	char sex;
	char * birth;
	struct _node * best_movie_name;
	struct _director * next_director_node;
}NextDirector;

typedef struct admin_director{
	struct _director * head;
	struct _director * tail;
	int size;
}AdminDirector;
// director

int CreateMovieList(AdminMovie * AM);
int ReadMovielog(AdminMovie * AM, AdminDirector * AD);
int FileDeleteMovieList(AdminMovie * AM, FILE * fptr_movie_log);
int FileUpdateMovieList(AdminMovie * AM, FILE * fptr_movie_log);
int FileAddMovieList(AdminMovie * AM, FILE * fptr_movie_log);
int PrintMovie(AdminMovie * AM);
int DeleteMovie(AdminMovie * AM);
int WriteMovieList(AdminMovie * AM);
int SearchMyMovie(NextNode * search_movie_name, AdminMovie * AM); // link actor - movie, director - movie
int SaveMovie(AdminMovie * AM);

char * MakeFilenameMovieList(void);
NextMovie * SearchMovieSerial(AdminMovie * AM, int serial_num);

// Movie function

int CreateActorList(AdminActor * AA);
int ReadActorlog(AdminActor * AA);
int FileDeleteActorList(AdminActor * AA, FILE * fptr_actor_log);
int FileUpdateActorList(AdminActor * AA, FILE * fptr_actor_log);
int FileAddActorList(AdminActor * AA, FILE * fptr_actor_log);
int PrintActor(AdminActor * AA);
int DeleteActor(AdminActor * AA);
int WriteActorList(AdminActor * AA);
int SearchMyActor(NextNode * search_actor_name, AdminActor * AA); // link movie - actor
int SaveActor(AdminActor * AA);

char * MakeFilenameActorList(void);
NextActor * SearchActorSerial(AdminActor * AA, int serial_num);

// Actor function

int CreateDirectorList(AdminDirector * AD);
int ReadDirectorlog(AdminDirector * AD);
int FileDeleteDirectorList(AdminDirector * AD, FILE * fptr_director_log);
int FileUpdateDirectorList(AdminDirector * AD, FILE * fptr_director_log);
int FileAddDirectorList(AdminDirector * AD, FILE * fptr_director_log);
int PrintDirector(AdminDirector * AD);
int DeleteDirector(AdminDirector * AD);
int WriteDirectorList(AdminDirector * AD);
int SearchMyDirector(NextNode * search_director_name, AdminDirector * AD); // link movie = director
int SaveDirector(AdminDirector * AD);

char * MakeFilenameDirectorList(void);
NextDirector * SearchDirectorSerial(AdminDirector * AD, int serial_num);

// Director function

void StartProgram(AdminMovie * AM, AdminDirector * AD, AdminActor * AA);
void CommandInput(AdminMovie * AM, AdminDirector * AD, AdminActor * AA);

int LinkMovieActor(AdminMovie * AM, AdminActor * AA);
int LinkMovieDirector(AdminMovie * AM, AdminDirector * AD);
int LinkDirectorMovie(AdminDirector * AD, AdminMovie * AM);
int LinkActorMovie(AdminActor * AA, AdminMovie * AM);

int SortMovie(AdminMovie * AM);
int SortDirector(AdminDirector * AD);
int SortActor(AdminActor * AA);
int CompareTimeYear(const void * first, const void * second);
int CompareString(const void * first, const void * second);
void ControlC(int);

int SearchActor(AdminActor * AA, char * string);
int SearchActorQM(AdminActor * AA, char * substr);
int SearchActorQMSM(AdminActor * AA, char * substr);
void SearchStringActor(AdminActor * AA, char * substr);
void MatchActorPrint(NextActor * ptr_actor_node);

int SearchDirector(AdminDirector * AD, char * string);
int SearchDirectorQM(AdminDirector * AD, char * substr);
int SearchDirectorQMSM(AdminDirector * AD, char * substr);
void SearchStringDirector(AdminDirector * AD, char * substr);
void MatchDirectorPrint(NextDirector * ptr_director_node);

int SearchMovie(AdminMovie * AM, char * string);
int SearchMovieQM(AdminMovie * AM, char * substr);
int SearchMovieQMSM(AdminMovie * AM, char * substr);
void SearchStringMovie(AdminMovie * AM, char * substr);
void MatchMoviePrint(NextMovie * ptr_movie_node);

char * ColonCheckInFILE(char *s, const char *olds, const char *news);
int ColonCheckInMovie(FILE *, char *);
int ColonCheckInDirector(FILE *, char *);
int ColonCheckInActor(FILE *, char *);
int AddMovie(AdminMovie * AM);
int AddDirector(AdminDirector * AD);
int AddActor(AdminActor * AA);

