/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/**************************      @SWC:        options.h              ****************************/
/**************************      @author:     Abdelrahman Sabry      ****************************/
/**************************      @date:       11 Sept                ****************************/
/**************************      @version:    1                      ****************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/


#ifndef _OPTIONS_H_
#define _OPTIONS_H_
#define _OPTIONS_H_

#define LONG_FORMAT_OPTION_l                0
#define SHOW_HIDDEN_OPTION_a                1
#define SORT_BY_TIME_OPTION_t               2
#define ACCESS_TIME_OPTION_u                3
#define CHANGE_TIME_OPTION_c                4
#define SHOW_INODE_OPTION_i                 5
#define DISABLE_EVERYTING_OPTION_f          6
#define SHOW_DIRECTORY_ITSELF_OPTION_d      7
#define SHOW_1_FILE_IN_LINE_OPTION_1        8

#define MAX_PATH_LENGTH                     2048    
#define MAX_FILES                           1000          

#ifndef S_ISVTX
#define S_ISVTX                             01000
#endif


/**
 * @brief Perform basic `ls` functionality to display files in a directory.
 * 
 * This function lists files in the specified directory with optional formatting and colors
 * based on file types, and supports various options such as displaying the directory itself,
 * showing inodes, and handling directory entries individually.
 *
 * @param file_names Array of file names in the directory.
 * @param file_count Number of files in the directory.
 * @param dir The directory path.
 */
void Basic_ls( char *file_names[],int file_count, char *dir );


/**
 * @brief Perform `ls` functionality with long format option.
 * 
 * This function lists files in the specified directory using long format (like `ls -l`), 
 * showing additional details such as permissions, owner, group, size, and time. 
 * It also supports options for sorting and showing inodes.
 *
 * @param file_names Array of file names in the directory.
 * @param file_count Number of files in the directory.
 * @param dir The directory path.
 */
void LongFormat_ls(char *file_names[], int file_count, char *dir);

/**
 * @brief Main function to list the contents of a directory.
 * 
 * This function opens the specified directory and lists its contents, supporting options such 
 * as displaying hidden files, long format, and sorting by various criteria.
 *
 * @param dir The directory path.
 */
void do_ls(char *dir);

#endif