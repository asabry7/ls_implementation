/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/**************************      @SWC:        utils.h                ****************************/
/**************************      @author:     Abdelrahman Sabry      ****************************/
/**************************      @date:       11 Sept                ****************************/
/**************************      @version:    1                      ****************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

#ifndef _UTILS_H_
#define _UTILS_H_
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

/* Text Colors */
#define green "\033[1;32m"                          // For executable files
#define red "\033[1;31m"                            // For broken symlinks or missing files
#define RED_HIGHLIGHT "\033[1;31;40m"               // Red text with black background for broken symlinks
#define WHITE_TEXT_RED_HIGHLIGHT "\033[1;37;41m"    // White text on red background
#define BLACK_TEXT_YELLOW_HIGHLIGHT "\033[1;30;43m" // Black text on yellow background

#define yellow "\033[1;33m"  // For device files (block/character devices)
#define blue "\033[1;34m"    // For directories
#define magenta "\033[1;35m" // For symlinks
#define cyan "\033[1;36m"    // For symbolic links or special files (e.g., FIFO, sockets)
#define white "\033[1;37m"   // For regular files with no special attributes
#define reset "\033[0m"      // Reset to default color
#define bold "\033[1m"       // For bold text (e.g., to highlight selected items)
#define underline "\033[4m"  // For underlined text (optional, for emphasis)

#define NAMED_PIPE yellow             // FIFO or named pipe
#define CHARACTER_SPECIAL_FILE yellow // Character special file
#define DIRECTORY blue                // Directory
#define BLOCK_SPECIAL_FILE yellow     // Block special file
#define REGULAR_FILE white            // Regular file
#define SOFT_LINK cyan                // Symbolic link (soft link)
#define SOCKET yellow                 // Socket
#define EXECUTABLE_FILE green         // Executable file (this is not a file type but rather a permission bit)

#define UNKNOWN_TYPE -1

#define BROKEN_LINK 0
#define PROPER_LINK 1

#ifndef S_ISVTX
#define S_ISVTX 01000
#endif

/**
 * @brief Compares two file names alphabetically.
 *
 * The function is used for sorting file names in lexicographical order.
 *
 * @param p1 Pointer to the first file name (pointer to a pointer to char).
 * @param p2 Pointer to the second file name (pointer to a pointer to char).
 *
 * @return Negative value if the first file name is less than the second.
 *         Zero if the file names are identical.
 *         Positive value if the first file name is greater.
 */
int CompareFileName(const void *p1, const void *p2);

/**
 * @brief Compares two files based on their modification time.
 *
 * This function retrieves the modification time of each file using `lstat`
 * and compares them to sort files in descending order (most recent first).
 *
 * @param p1 Pointer to the first file name (pointer to a pointer to char).
 * @param p2 Pointer to the second file name (pointer to a pointer to char).
 *
 * @return -1 if the first file is more recently modified.
 *         1 if the second file is more recently modified.
 *         0 if both files have the same modification time or if an error occurs.
 */
int CompareFileModTime(const void *p1, const void *p2);

/**
 * @brief Compares two files based on their access time.
 *
 * Similar to CompareFileModTime, but compares the last access time of the files.
 *
 * @param p1 Pointer to the first file name (pointer to a pointer to char).
 * @param p2 Pointer to the second file name (pointer to a pointer to char).
 *
 * @return -1 if the first file is more recently accessed.
 *         1 if the second file is more recently accessed.
 *         0 if both files have the same access time or if an error occurs.
 */
int CompareFileAccessTime(const void *p1, const void *p2);

/**
 * @brief Compares two files based on their change time.
 *
 * This function compares the last status change time of the files.
 *
 * @param p1 Pointer to the first file name (pointer to a pointer to char).
 * @param p2 Pointer to the second file name (pointer to a pointer to char).
 *
 * @return -1 if the first file has a more recent change time.
 *         1 if the second file has a more recent change time.
 *         0 if both files have the same change time or if an error occurs.
 */
int CompareFileChangeTime(const void *p1, const void *p2);

/**
 * @brief Checks whether a symbolic link is broken.
 *
 * This function checks if the symbolic link points to a valid target.
 *
 * @param path The file path of the symbolic link.
 *
 * @return BROKEN_LINK if the symbolic link points to a non-existent file.
 *         PROPER_LINK if the link is valid.
 */
int CheckSymbolicLinkTarget(const char *path);

/**
 * @brief Prints file entry details with appropriate color formatting based on file type and permissions.
 *
 * This function prints file entries with colors indicating specific file types (e.g., directories, symbolic links).
 * It also handles broken symbolic links and displays symbolic link targets in long format mode.
 *
 * @param Entry The name of the file entry.
 * @param buf A struct containing the file's metadata.
 * @param path The file path of the entry.
 * @param max_len The length of the largest file name.
 */
void PrintEntry(char *Entry, struct stat buf, char *path, int max_len);
/**
 * @brief Retrieves and formats the permissions of a file into a string.
 *
 * This function converts the file permissions (stored in the stat buffer) into a human-readable string format.
 *
 * @param str A string to hold the formatted permissions (must be at least 11 characters long).
 * @param buf A struct containing the file's metadata.
 */
void GetFilePermessions(char *str, struct stat buf);

/**
 * @brief Prints detailed file information in long format.
 *
 * This function prints the file's inode, permissions, number of hard links, owner, group, size, modification time,
 * and the file name in long format. It also displays the symbolic link target if the file is a symbolic link.
 *
 * @param buf A struct containing the file's metadata.
 * @param file_name The name of the file.
 * @param path The file path.
 */
void PrintEntry_LongFormat(struct stat buf, char *file_name, char *path);

#endif