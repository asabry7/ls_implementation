/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/**************************      @SWC:        options.c              ****************************/
/**************************      @author:     Abdelrahman Sabry      ****************************/
/**************************      @date:       11 Sept                ****************************/
/**************************      @version:    1                      ****************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

/******************************            INCLUDES           ***********************************/

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "utils.h"
#include "options.h"

/**************************            GLOBAL VARIABLES           *******************************/
extern int errno;
int OptionsFlags[9] = {0};


/**********************            FUNCTIONS IMPLEMENTATION            ***************************/

void Basic_ls( char *file_names[],int file_count, char *dir )
{
    struct stat buf;

    /* if -d option is used => print directory name only */
    if( OptionsFlags[ SHOW_DIRECTORY_ITSELF_OPTION_d ] )
    {
        /* Get file status */
        if (lstat(dir, &buf) < 0) {
            perror("Error in lstat");
            return; 
        }

        /* if -f option is used => print without color */
        if( OptionsFlags[ DISABLE_EVERYTING_OPTION_f ] )
        {
            printf("%s", dir);
        }

        else
        {
            PrintEntry( dir , buf , NULL);
        }        
    }

    else
    {
        /* Loop over the entries in the directory */
        for (int i = 0; i < file_count; i++) {

            /* Construct the full path to the file */ 
            char path[MAX_PATH_LENGTH];
            snprintf(path, sizeof(path), "%s/%s", dir, file_names[i]);

            /* Get file status */
            if (lstat(path, &buf) < 0) {
                perror("Error in lstat");
                continue; 
            }

            /* If -i option is used => print the inode number at the beginning */
            if( OptionsFlags[ SHOW_INODE_OPTION_i ] )
            {
                printf("%ld  ", buf.st_ino);
            }

            /* If -f option is used => print without color */
            if( OptionsFlags[ DISABLE_EVERYTING_OPTION_f ] )
            {
                printf("%s", file_names[i]);
            }

            else
            {
                PrintEntry( file_names[i] , buf  , path);
            }
            
            /* if -1 option is used => separate using \n */
            if( OptionsFlags[ SHOW_1_FILE_IN_LINE_OPTION_1 ] )
            {
                printf("\n");
            }

            /* Separate with tab */
            else
            {
                printf("  "); 
            }

            free(file_names[i]);
        }

    }


}


void LongFormat_ls(char *file_names[], int file_count, char *dir)
{
    struct stat buf;

    /* if -d option is used => print directory name only */
    if( OptionsFlags[ SHOW_DIRECTORY_ITSELF_OPTION_d ] )
    {
        if (lstat(dir, &buf) < 0) {
            perror("Error in lstat");
            return; 
        }

        PrintEntry_LongFormat( buf, dir, NULL );
        printf("\n");
    }

    else
    {
        /* Loop over the entries in the directory */
        for (int i = 0; i < file_count; i++) {

            /* Construct the full path to the file */
            char path[MAX_PATH_LENGTH];
            snprintf(path, sizeof(path), "%s/%s", dir, file_names[i]);

            /* Get file status */
            if (lstat(path, &buf) < 0) {
                perror("Error in lstat");
                continue; 
            }

            /* Print the entry in long format */
            PrintEntry_LongFormat( buf , file_names[i] ,  path );

            /* Separate by new line */
            printf("\n");
            
            free(file_names[i]);
        }
    }

}


void do_ls(char *dir) {
    struct dirent *entry;
    DIR *dp = opendir(dir);

    /* Array of pointers to hold file names */
    char *file_names[MAX_FILES]; 
    int file_count = 0;

    if (dp == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    /* if -f option is used: */
    if( OptionsFlags[ DISABLE_EVERYTING_OPTION_f ] )
    {
        /* Enable hidden files */
        OptionsFlags[ SHOW_HIDDEN_OPTION_a ] = 1;

        /* Disable long format option */
        OptionsFlags[ LONG_FORMAT_OPTION_l ] = 0;
    }    

    /* Loop over the entries in the directory and store them in the array */
    while ((entry = readdir(dp)) != NULL) {

        /* if -a option is not used => skip hidden files */
        if ( !OptionsFlags[SHOW_HIDDEN_OPTION_a] && (entry->d_name[0] == '.') ) {
            continue; // Skip hidden files
        }

        /* Allocate memory for the file name and copy it */
        file_names[file_count] = malloc(strlen(entry->d_name) + 1);

        if (file_names[file_count] == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }

        strcpy(file_names[file_count], entry->d_name);
        file_count++;

        if (file_count >= MAX_FILES) {
            fprintf(stderr, "Too many files in directory\n");
            break;
        }
    }

    closedir(dp); 

    /* Sort Entries */

    /* if -t option is used => sort by modification time */
    if( OptionsFlags[SORT_BY_TIME_OPTION_t] == 1 )
    {
        qsort(file_names, file_count, sizeof(char *), CompareFileModTime);
    }

    /* if -lut are used or -u only is used => sort by access time */
    else if( ( OptionsFlags[ ACCESS_TIME_OPTION_u ] && OptionsFlags[ SORT_BY_TIME_OPTION_t ] && OptionsFlags[ LONG_FORMAT_OPTION_l ] )
                || ( OptionsFlags[ ACCESS_TIME_OPTION_u ] && !OptionsFlags[ LONG_FORMAT_OPTION_l ] ) )
    {
        /* ls -ltu or ls -u => sort by access time */
        qsort(file_names, file_count, sizeof(char *), CompareFileAccessTime);
    }

    else if( ( OptionsFlags[ CHANGE_TIME_OPTION_c ] && OptionsFlags[ SORT_BY_TIME_OPTION_t ] && OptionsFlags[ LONG_FORMAT_OPTION_l ] )
                || ( OptionsFlags[ CHANGE_TIME_OPTION_c ] && !OptionsFlags[ LONG_FORMAT_OPTION_l ] ) )
    {
        /* ls -ltc or ls -c => sort by access time */
        qsort(file_names, file_count, sizeof(char *), CompareFileChangeTime);
    }

    else if( OptionsFlags[ DISABLE_EVERYTING_OPTION_f ] )
    {
        /* Do not sort */
    }

    else
    {
        /* Sort by name */
        qsort(file_names, file_count, sizeof(char *), CompareFileName);
    }


    /* If -l option is used => print in long format */
    if( OptionsFlags[LONG_FORMAT_OPTION_l] == 1 )
    {
        LongFormat_ls(file_names, file_count, dir);
    }

    /* print file names only */
    else
    {
        Basic_ls(file_names, file_count, dir);
        printf("\n");
    }    
}