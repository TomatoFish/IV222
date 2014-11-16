#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>
#include "errors.h"
#include <signal.h>
#include <chrono>
#include <iostream>
#define CREW_SIZE 8

using namespace std;
typedef struct work_tag {
    struct work_tag *next;
    char *path;
} work_t, *work_p;

typedef struct worker_tag {
    int index;
    pthread_t thread;
    struct crew_tag *crew;
} worker_t, *worker_p;

typedef struct crew_tag {
    int crew_size;
    worker_t crew[CREW_SIZE];
    long work_count;
    work_t *first, *last;
    pthread_mutex_t mutex;
    pthread_cond_t done;
    pthread_cond_t go;
} crew_t, *crew_p;

size_t path_max;
size_t name_max;
sigset_t mask;
pthread_t sig;
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
int QUIT = 0;



void copy_to(char *a, char *b)
{
    while(*b != '.')
    {
        *a = *b;
        *b++; *a++;
    }
}

void *read_signals(void *arg)
{
    crew_t *cr = (crew_t*)arg;
    int signo;
    for(;;){
        sigwait(&mask,&signo);
        switch(signo)
        {
        case SIGINT:

            for(int i = 0;i < CREW_SIZE;i++){
                pthread_cancel(cr->crew[i].thread);
            }
            exit(0);
            break;
        case SIGQUIT:     
            if(QUIT == 0)
                QUIT = 1;
            break;
        default:
            printf("Неизвестный сигнал");
            exit(1);
        }
    }

}

void unlock(void *arg)
{
    pthread_mutex_t *t;
    t = (pthread_mutex_t *)arg;
    pthread_mutex_unlock(t);
}

void close_file(void *arg)
{
    FILE *f;
    f = (FILE *)arg;
    fclose(f);
}

void *worker_routine (void *arg) {
    worker_p mine = (worker_t *) arg;
    crew_p crew = mine->crew;
    work_p work, new_work;
    struct stat filestat;
    struct dirent *entry; int rc;
    FILE *log;
    pthread_cleanup_push(close_file,&log);
    log = fopen("log.txt","a+");

    entry = (struct dirent*)malloc (
                sizeof (struct dirent) + name_max);
    pthread_cleanup_push(unlock, &crew->mutex);
    pthread_mutex_lock (&crew->mutex);
    while (crew->work_count == 0) {
        pthread_cond_wait (&crew->go, &crew->mutex);
    } 
    pthread_mutex_unlock (&crew->mutex);
    pthread_cleanup_pop(0);

    for (;;) {
        pthread_cleanup_push(unlock, &crew->mutex);
        pthread_mutex_lock (&crew->mutex);
        while (crew->first == NULL) {
            pthread_cond_wait (&crew->go, &crew->mutex);
        }

        work = crew->first;
        crew->first = work->next;
        if (crew->first == NULL)
            crew->last = NULL;
        pthread_mutex_unlock (&crew->mutex);
        pthread_cleanup_pop(0);
        lstat (work->path, &filestat);
        if (S_ISDIR (filestat.st_mode)) {
            DIR *directory;
            struct dirent *result;
            directory = opendir (work->path);
            if (directory == NULL) {
                printf("...");
                continue;
            }
            for (;;) {
                rc = readdir_r (directory, entry, &result);
                if (rc != 0) { fprintf(stderr, "..."); break; }
                if (result == NULL)
                    break;

                if (strcmp (entry->d_name, ".") == 0) continue;
                if (strcmp (entry->d_name, "..") == 0) continue;

                new_work = (work_p) malloc (sizeof (work_t));
                new_work->path = (char*) malloc (path_max);

                strcpy (new_work->path, work->path);
                strcat (new_work->path, "/");
                strcat (new_work->path, entry->d_name);

                new_work->next = NULL;
                pthread_cleanup_push(unlock, &crew->mutex);
                pthread_mutex_lock (&crew->mutex);

                if (crew->first == NULL) {
                    crew->first = new_work;
                    crew->last = new_work;
                } else {
                    crew->last->next = new_work;
                    crew->last = new_work;
                }

                crew->work_count++;
                pthread_cond_signal (&crew->go);
                pthread_mutex_unlock (&crew->mutex);
                pthread_cleanup_pop(0);

            }
            closedir(directory);
        } else if (S_ISREG (filestat.st_mode)) {

            if (strstr(work->path,".cpp")){

                char ch[200]="\0", buff[200]="\0", buf[200]="\0";
                FILE *fp;
                copy_to(buff,work->path);
                sprintf(ch, "g++ %s -o %s && %s",work->path,buff,buff);
                pthread_cleanup_push(close_file, &fp);
                fp = popen(ch,"r");

                pthread_cleanup_push(unlock, &mu);
                pthread_mutex_lock(&mu);
                while((fgets(buf, 80, fp)) != NULL)
                {
                    fprintf(log, "Результат \"%s\" работы потока %d\n",buf,mine->index);
                }
                pthread_mutex_unlock(&mu);
                pthread_cleanup_pop(0);
                pclose(fp);
                pthread_cleanup_pop(0);
            }


            else if(strstr(work->path,".sh"))
            {


                char ch[80],buf[80];
                sprintf(ch, "chmod +x %s && %s",work->path,work->path);
                FILE *f;
                pthread_cleanup_push(close_file,&f);
                f = popen(ch,"r");
                pthread_cleanup_push(unlock,&mu);
                pthread_mutex_lock(&mu);
                while((fgets(buf, 80, f)) != NULL)
                {
                    fprintf(log, "Результат '%s' работы потока %d\n",buf,mine->index);
                }
                pthread_mutex_unlock(&mu);
                pthread_cleanup_pop(0);
                pclose(f);
                pthread_cleanup_pop(0);

            }


        }
        free(work->path);
        free(work);
        pthread_cleanup_push(unlock,&crew->mutex);
        pthread_mutex_lock (&crew->mutex);
        crew->work_count--;
        if (crew->work_count <= 0) {
            pthread_cond_broadcast (&crew->done);
            pthread_mutex_unlock (&crew->mutex);
            break;
        }
        pthread_mutex_unlock (&crew->mutex);
        pthread_cleanup_pop(0);
    }
    fclose(log);
    pthread_cleanup_pop(0);
    free (entry);
    return NULL;
}

int crew_create (crew_t *crew, int crew_size)
{
    int rc;
    int crew_index;

    if (crew_size > CREW_SIZE)
        return -1;

    crew->crew_size = crew_size;
    crew->work_count = 0;
    crew->first = NULL;
    crew->last = NULL;

    pthread_mutex_init (&crew->mutex, NULL);
    pthread_cond_init (&crew->done, NULL);
    pthread_cond_init (&crew->go, NULL);
    for (crew_index = 0; crew_index < CREW_SIZE; crew_index++) {
        crew->crew[crew_index].index = crew_index;
        crew->crew[crew_index].crew = crew;
        rc = pthread_create (&crew->crew[crew_index].thread,
                        NULL, worker_routine, (void*)&crew->crew[crew_index]);
        if(rc != 0)
            printf("pthread_create error");
    }
    rc = pthread_create(&sig,NULL,read_signals,crew);
    if(rc != 0)
        printf("pthread_create error");
    return 0;
}

int crew_start(crew_p crew, char *filepath) {
    work_p request;
    pthread_mutex_lock (&crew->mutex);

    while (crew->work_count > 0)
        pthread_cond_wait (&crew->done, &crew->mutex);
    if(QUIT == 1){
        return 0;
    }
    path_max = pathconf (filepath, _PC_PATH_MAX);
    if (path_max == -1) { /* ... */ }
    name_max = pathconf(filepath, _PC_NAME_MAX);
    if (name_max == -1) { /* ... */ }
    path_max++; name_max++;

    request = (work_p) malloc(sizeof(work_t));
    request->path = (char *) malloc(path_max);
    strcpy (request->path, filepath);
    request->next = NULL;

    if (crew->first == NULL) {
        crew->first = request;
        crew->last = request;
    } else {
        crew->last->next = request;
        crew->last = request;
    }
    crew->work_count++;
    pthread_cond_signal (&crew->go);


    while (crew->work_count > 0)
        pthread_cond_wait (&crew->done, &crew->mutex);

    pthread_mutex_unlock (&crew->mutex);
    return 0;
}

int main (int argc, char *argv[])
{
    int rc;
    sigset_t oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    rc = pthread_sigmask(SIG_BLOCK, &mask, &oldmask);
    if(rc != 0)
        printf("pthread_sigmask error");

    crew_t my_crew;
    argv[1]="/home/madbomber/";
    crew_create(&my_crew, CREW_SIZE);
    auto start = chrono::high_resolution_clock::now();
    crew_start(&my_crew, argv[1]);
    auto end = chrono::high_resolution_clock::now();
    cout << "Time taken: " << (end-start).count() * ((double)chrono::high_resolution_clock::period::num
                                                     / chrono::high_resolution_clock::period::den) << endl;

    return 0;
}

