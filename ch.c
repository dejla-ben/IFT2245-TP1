//
//  ch.c
//  
//
//  Created by Dejla Ben ltaief on 2016-01-26.
//
//

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/wait.h>   /* pour avoir wait & co. */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h> /*pour avoir Path-max*/
#include <stdlib.h>



int main() {
    int i = 0;
    char ligne[256]; //  entrée utilisateur
    char * elems[256]; // les differents elems dans la commande
    int numelems=0;
    while(1){
        
        fprintf (stdout, "%% ");
        //initialisation
        memset ( ligne, '\0', 256 );
        
        // Lire ligne
        fgets(ligne, 256, stdin);
        
        // decoupage mot
        
        if((elems[0] = strtok(ligne," \n\t")) == NULL) continue;
        numelems = 1;
        while((elems[numelems] = strtok(NULL, " \n\t")) != NULL) numelems++;
        
        if(strcmp(elems[0],"exit") == 0){
            fprintf (stdout, "bye ");
            
        exit(0);
        }
        
        // changer repertoire pour la commande cd
        if (strcmp(elems[0],"cd") == 0) {
            
            if (elems[1] == NULL) {
                chdir(getenv("HOME"));
                
            }
            
            else{
                chdir(elems[1]);
                   
                }
            }
        
       else{
            while (elems[i] != NULL){
                
                // si | est detecté
                if (strcmp(elems[i],"|") == 0){
                    pid_t pid;
                    int pipefd[2];
                    int pipefd1[2];
                    char *command[256];
                    int i = 0;
                    int j = 0;
                    int k = 0;
                    while (elems[j] != NULL){
                        k = 0;
                        while (strcmp(elems[j],"|") != 0){
                            command[k] = elems[j];
                            j++;
                            if (elems[j] == NULL){
                                k++;
                                break;
                            }
                            k++;
                        }
                        
                        command[k] = NULL;
                        j++;
                        if (i % 2 != 0){
                            pipe(pipefd);
                        }else{
                            pipe(pipefd1);
                        }
                        
                        pid=fork();
                        
                        if(pid==-1){
                            printf("Child process could not be created\n");
                        }
                        if(pid==0){
                            
                            if (i == 0){
                                dup2(pipefd1[1], STDOUT_FILENO);
                            }
                            
                            else{
                                
                                dup2(pipefd1[0],STDIN_FILENO);
                            }
                            
                            execvp(command[0],command);
                        }
                        
                        if (i == 0){
                            
                            close(pipefd[1]);
                        }
                        
                        else {
                            
                            close(pipefd1[0]);
                        }
                        
                        waitpid(pid,NULL,0);
                        
                        i++;
                    }

                    // si '*' est detecté
                      // deuxieme fonctionnalité
                }else if(strcmp(elems[i],"*") == 0){
                    
                    
                        char cwd[1024];
                        
                        DIR* rep = NULL;
                        
                        
                        
                        struct dirent* fichierLu = NULL; /* Déclaration d'un pointeur vers la structure dirent. */
                    
                        
                        if ((rep = opendir(getcwd( cwd, sizeof(cwd) )))!=NULL)
                        {
                            while ((fichierLu = readdir(rep)) != NULL)
                            {
                                if (strcmp(fichierLu->d_name, "." ) == 0 || strcmp(fichierLu->d_name, ".." ) == 0)
                                    continue;
                                printf("Le fichier lu s'appelle '%s'\n", fichierLu->d_name);
                            }
                        }
                        
                    
                    if (closedir(rep) == -1) exit(-1);
                    
                    
                     
                    
                    }
        
               
                  // troisieme fonctionnalité
                    // si '>' est detecté
                else if ((strcmp(elems[i],"<") == 0 )&& (strcmp(elems[i+2],">") == 0)) {
                    
                    
                    
                    if ((elems[i+1] == NULL) ||(elems[i+2] == NULL) || (elems[i+3] == NULL)) {
                        printf("pas assez d'arguments");
                        
                    }
                    
                    pid_t pid;
                    
                    
                    int fileDescriptor;
                    if((pid=fork())==-1){
                        printf("Child process could not be created\n");
                    }
                    if(pid==0){
                            fileDescriptor = open(elems[i+1],O_RDONLY);
                            dup2(fileDescriptor,STDIN_FILENO);
                            close(fileDescriptor);
                            fileDescriptor = open(elems[i+3],O_WRONLY|O_CREAT,0666);
                            dup2(fileDescriptor,STDOUT_FILENO);
                            close(fileDescriptor);
                            return 1;
                        
                        
                            
                        }
                    
                    
                   
                        
                    waitpid(pid,NULL,0);
                    
                    
                   
                    
                    
                }
                // si '>' est detecte
                else if (strcmp(elems[i],">") == 0){
                    
                    if (elems[i+1] == NULL){
                        printf("pas assez d'arguments");
                        
                    }
                    pid_t pid;
                    
                    int out;
                    if((pid=fork())==-1){
                        printf("Child process could not be created");
                    }
                    if(pid==0){
                    
                        out= open(elems[i+1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
                        dup2(out , STDOUT_FILENO);
                        close(out);
                    
                    }
                    
                    
                    
                    waitpid(pid,NULL,0);
                   
                }
                
                // si '<' est detecté
                
                else if (strcmp(elems[i],"<") == 0){
                    
                    
                    if (elems[i+1] == NULL){
                        printf("pas assez d'arguments");
                        
                    }
                    
                    pid_t pid;
                    
                    int in;
                    
                    if((pid=fork())==-1){
                        printf("Child process could not be created");
                    }
                    if(pid==0){
                        
                        in = open(elems[i+1], O_RDONLY);
                        dup2(in,STDIN_FILENO);
                        close(in);
                        
                    }
                    
                     waitpid(pid,NULL,0);
                    
                }
                
                
                
            i++;
           
        
            }
            pid_t pid;
            if((pid=fork())==-1){
                
                printf("Child process could not be created\n");
                
            }
            if (pid==0)
            {
                execvp(elems[0],elems);
                
            }
            
            else {
                
                waitpid(pid,NULL,0);
            }

            
            }
      }
    
    return 0;
    
}

