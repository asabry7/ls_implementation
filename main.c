/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/**************************      @SWC:        main.c                 ****************************/
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

#include "utils.h"
#include "options.h"


/**************************            GLOBAL VARIABLES           *******************************/


extern char *optarg;
extern int optind, opterr, optopt;

/* Array to carry the state of options */
extern int OptionsFlags[9];


/**************************              MAIN FUNCTION            *******************************/

int main(int argc, char *argv[])
{
    int opt;

	if (argc == 1) 
    {
		printf("Directory listing of pwd:\n");
		do_ls(".");
	} 
    
    else 
    {
        /* Parse options */
        while ((opt = getopt(argc, argv, ":latucifd1")) != -1) 
        {

            switch (opt) {
                case 'l':   OptionsFlags[LONG_FORMAT_OPTION_l] = 1;                break;
                case 'a':   OptionsFlags[SHOW_HIDDEN_OPTION_a] = 1;                break;
                case 't':   OptionsFlags[SORT_BY_TIME_OPTION_t] = 1;               break;
                case 'u':   OptionsFlags[ACCESS_TIME_OPTION_u] = 1;                break;
                case 'c':   OptionsFlags[CHANGE_TIME_OPTION_c] = 1;                break;
                case 'i':   OptionsFlags[SHOW_INODE_OPTION_i] = 1;                 break;
                case 'f':   OptionsFlags[DISABLE_EVERYTING_OPTION_f] = 1;          break;
                case 'd':   OptionsFlags[SHOW_DIRECTORY_ITSELF_OPTION_d] = 1;      break;
                case '1':   OptionsFlags[SHOW_1_FILE_IN_LINE_OPTION_1] = 1;        break;
            
            default:    printf("Unexpected case in switch()");  return -1;
		    }
        }

        /* If no directory is passed => list the current worling directory's entries */
        if (optind == argc) 
        {
            printf("Directory listing of pwd:\n");
            do_ls(".");
        } 

        else
        {
            int i = 0;
            /* Loop on the passed directories */
            while (++i < argc) 
            {
                /* If argv[i] is an option => ignore it */
                if( argv[i][0] == '-' )
                {
                    continue;
                }

                printf("Directory listing of %s:\n", argv[i]);
                do_ls(argv[i]);
                printf("\n");
            }
        }


	}

	return 0;
}

