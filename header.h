/********************************************************************************************************************************************/
/* This program is made by team 3 (20170287 - Park Yeong June, 20170297 - Sin Dong Woo, 20170315 - Lee Seo Yeong, 20170317 - Lee Woo Sin)
 * This program name is orange.
 * This program is Movie admin program.
 * We write Function Name with Upper character and lower character.
 * We wrtie variable_num name with lower character and underbar. */
/********************************************************************************************************************************************/
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

int CreateMovieList(AdminMovie * AM); // Create Linkedlist of Movie
int ReadMovielog(AdminMovie * AM, AdminDirector * AD); // Read movie_log file
int FileDeleteMovieList(AdminMovie * AM, FILE * fptr_movie_log); // Process movie_log tag : delete
int FileUpdateMovieList(AdminMovie * AM, FILE * fptr_movie_log); // Process movie_log tag : update
int FileAddMovieList(AdminMovie * AM, FILE * fptr_movie_log); // Process movie_log tag : add
int PrintMovie(AdminMovie * AM); // Movie print
int DeleteMovie(AdminMovie * AM); // Movie delete
int WriteMovieList(AdminMovie * AM); // Write listfile of movie
int SearchMyMovie(NextNode * search_movie_name, AdminMovie * AM); // Search movie for link
int SaveMovie(AdminMovie * AM); // Movie save

char * MakeFilenameMovieList(void); // Make string of modern time
NextMovie * SearchMovieSerial(AdminMovie * AM, int serial_num); // Search serial number in movie

// Movie function

int CreateActorList(AdminActor * AA); // Create Linkedlist of Actor
int ReadActorlog(AdminActor * AA); // Read actor_log file
int FileDeleteActorList(AdminActor * AA, FILE * fptr_actor_log); // Process actor_log tag : delete
int FileUpdateActorList(AdminActor * AA, FILE * fptr_actor_log); // Process actor_log tag : update
int FileAddActorList(AdminActor * AA, FILE * fptr_actor_log); // Process actor_log tag : add
int PrintActor(AdminActor * AA); // Actor print
int DeleteActor(AdminActor * AA); // Actor delete
int WriteActorList(AdminActor * AA); // Write listfile of actor
int SearchMyActor(NextNode * search_actor_name, AdminActor * AA); // Search actor for link
int SaveActor(AdminActor * AA); // Actor save

char * MakeFilenameActorList(void); // Make string of modern time
NextActor * SearchActorSerial(AdminActor * AA, int serial_num); // Search serial number in actor

// Actor function

int CreateDirectorList(AdminDirector * AD); // Create Linkedlist of director
int ReadDirectorlog(AdminDirector * AD); // Read director_log file
int FileDeleteDirectorList(AdminDirector * AD, FILE * fptr_director_log); // Process director_log tag : delete
int FileUpdateDirectorList(AdminDirector * AD, FILE * fptr_director_log); // Process director_log tag : update
int FileAddDirectorList(AdminDirector * AD, FILE * fptr_director_log); // Process director_log tag : add
int PrintDirector(AdminDirector * AD); // Director print
int DeleteDirector(AdminDirector * AD); // Director delete
int WriteDirectorList(AdminDirector * AD); // Write listfile of director
int SearchMyDirector(NextNode * search_director_name, AdminDirector * AD); // Search director for link
int SaveDirector(AdminDirector * AD); // Director save

char * MakeFilenameDirectorList(void); // Make string of modern time
NextDirector * SearchDirectorSerial(AdminDirector * AD, int serial_num); // Search serial number in director

// Director function

void StartProgram(AdminMovie * AM, AdminDirector * AD, AdminActor * AA); // make linkedlist and read logfile and write list
void CommandInput(AdminMovie * AM, AdminDirector * AD, AdminActor * AA); // input command

int LinkMovieActor(AdminMovie * AM, AdminActor * AA); // if there is same something, link movie-actor
int LinkMovieDirector(AdminMovie * AM, AdminDirector * AD); // if there is same something, link movie-director
int LinkDirectorMovie(AdminDirector * AD, AdminMovie * AM); // if there is same something, link director-movie
int LinkActorMovie(AdminActor * AA, AdminMovie * AM); // if there is same something, link actor-movie
void Link(AdminActor * AA, AdminDirector * AD, AdminMovie * AM); // include all link

int SortMovie(AdminMovie * AM); // sort movie
int SortDirector(AdminDirector * AD); // sort director
int SortActor(AdminActor * AA); // sort actor
int CompareTimeYear(const void * first, const void * second); // compare number
int CompareString(const void * first, const void * second); // compare string
void ControlC(int); // if we get SIGINT, process about that

int SearchActor(AdminActor * AA, char * string); // search actor
int SearchActorQM(AdminActor * AA, char * substr); // process question mark when searching in actor
int SearchActorQMSM(AdminActor * AA, char * substr); // process star mark and question mark when searching in actor
void SearchStringActor(AdminActor * AA, char * substr); // search string in actor
void MatchActorPrint(NextActor * ptr_actor_node); // if there is same string, print.

int SearchDirector(AdminDirector * AD, char * string); // search director
int SearchDirectorQM(AdminDirector * AD, char * substr); // process question mark when searching in director
int SearchDirectorQMSM(AdminDirector * AD, char * substr); // proecess questiong mark and star mark when searching in director
void SearchStringDirector(AdminDirector * AD, char * substr); // search string in actor
void MatchDirectorPrint(NextDirector * ptr_director_node); // if there is same string, print.

int SearchMovie(AdminMovie * AM, char * string); // search movie
int SearchMovieQM(AdminMovie * AM, char * substr); // process question mark when searching in movie
int SearchMovieQMSM(AdminMovie * AM, char * substr); // process question mark and star mark when searching in movie
void SearchStringMovie(AdminMovie * AM, char * substr); // search string in movie
void MatchMoviePrint(NextMovie * ptr_movie_node); // if there is same string, print

char * ColonCheckInFILE(char *s, const char *olds, const char *news); // http://mwultong.blogspot.com/2006/10/c-string-replace-all.html // change string

int ColonCheckInMovie(FILE *, char *); // if there is : in string, write ??; in movie_log
int ColonCheckInDirector(FILE *, char *); // if there is : in string, write ??; in director_log
int ColonCheckInActor(FILE *, char *);  // if there is : in string, write ??; in actor_log
int ColonCheckInAppearmovie(FILE *, char *); // same with ColonCheckInActor except ','
int ColonCheckInBestmovie(FILE *, char *); // same with ColonCheckInDirector except ','
int ColonCheckInActorname(FILE *, char *); // same with ColonCheckInMovie except ','

int AddMovie(AdminMovie * AM); // Add movie
int AddDirector(AdminDirector * AD); // Add director
int AddActor(AdminActor * AA); // Add actor

NextActor * ThereisSameActor(AdminActor * AA, char * string); // search same actor name in actor
NextDirector * ThereisSameDirector(AdminDirector * AD, char * string); // search same director name in director
NextMovie * ThereisSameMovie(AdminMovie * AM, char * string); // search same movie title in movie

int UpdateActor(AdminActor * AA); // update actor
int UpdateActorName(FILE * fptr_actor_log, AdminActor * AA, int serial_num); // update actor's name
int UpdateActorSex(FILE * fptr_actor_log, NextActor * ptr_actor_node); // update actor's sex
int UpdateActorBirth(FILE * fptr_actor_log, NextActor * ptr_actor_node); // update actor's birth
int UpdateActorAppearmovie(FILE * fptr_actor_log, NextActor * ptr_actor_node); // update actor's appear movie

int UpdateDirector(AdminDirector * AD); // update director
int UpdateDirectorName(FILE * fptr_director_log, AdminDirector * AD, int serial_num); // update director's name
int UpdateDirectorSex(FILE * fptr_director_log, NextDirector * ptr_director_node); // update director's sex
int UpdateDirectorBirth(FILE * fptr_director_log, NextDirector * ptr_director_node); // update director's birth
int UpdateDirectorBestmovie(FILE * fptr_director_log, NextDirector * ptr_director_node); // update director's best movie

int UpdateMovie(AdminMovie * AM); // update movie
int UpdateMovieName(FILE * fptr_movie_log, AdminMovie * AM, int serial_num); // update movie's title
int UpdateMovieGenre(FILE * fptr_movie_log, NextMovie * ptr_movie_node;); // update movie's genre
int UpdateMovieDirector(FILE * fptr_movie_log, NextMovie * ptr_movie_node;); // update movie's director
int UpdateMovieYear(FILE * fptr_movie_log, NextMovie * ptr_movie_node;); // update movie's year
int UpdateMovieTime(FILE * fptr_movie_log, NextMovie * ptr_movie_node;); // update movie's time
int UpdateMovieActorname(FILE * fptr_movie_log, NextMovie * ptr_movie_node); // update movie's actor
