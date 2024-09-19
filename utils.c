/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/**************************      @SWC:        utils.c                ****************************/
/**************************      @author:     Abdelrahman Sabry      ****************************/
/**************************      @date:       11 Sept                ****************************/
/**************************      @version:    1                      ****************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

/******************************            INCLUDES           ***********************************/

#include "utils.h"
#include "options.h"

/**************************            GLOBAL VARIABLES           *******************************/

extern int OptionsFlags[9];

/**********************            FUNCTIONS IMPLEMENTATION            ***************************/

int CompareFileName(const void *p1, const void *p2)
{
    /* The actual arguments to this function are "pointers to
       pointers to char", but strcmp(3) arguments are "pointers
       to char", hence the following cast plus dereference */
    return strcasecmp(*(const char **)p1, *(const char **)p2);
}

int CompareFileModTime(const void *p1, const void *p2)
{
    struct stat buf1, buf2;

    /** Cast the arguments back to pointers to char (file names) */
    const char *file1 = *(const char **)p1;
    const char *file2 = *(const char **)p2;

    /** Get file stats for each file */
    if (lstat(file1, &buf1) < 0)
    {
        printf("stat error for file1: %s", file1);
        return 0; /** Error case, treat files as equal */
    }
    if (lstat(file2, &buf2) < 0)
    {
        perror("stat error for file2");
        return 0; /** Error case, treat files as equal */
    }

    /** Compare modification times (st_mtime) */
    if (buf1.st_mtime > buf2.st_mtime)
        return -1; /** Sort in descending order (most recent first) */
    else if (buf1.st_mtime < buf2.st_mtime)
        return 1; /** Sort in descending order */
    else
        return 0; /** Modification times are the same */
}

int CompareFileAccessTime(const void *p1, const void *p2)
{
    struct stat buf1, buf2;

    /** Cast the arguments back to pointers to char (file names) */
    const char *file1 = *(const char **)p1;
    const char *file2 = *(const char **)p2;

    /** Get file stats for each file */
    if (lstat(file1, &buf1) < 0)
    {
        perror("stat error for file1");
        return 0; /** Error case, treat files as equal */
    }
    if (lstat(file2, &buf2) < 0)
    {
        perror("stat error for file2");
        return 0; /** Error case, treat files as equal */
    }

    /** Compare access times (st_atime) */
    if (buf1.st_atime > buf2.st_atime)
        return -1; /** Sort in descending order (most recent first) */
    else if (buf1.st_atime < buf2.st_atime)
        return 1; /** Sort in descending order */
    else
        return 0; /** Access times are the same */
}

int CompareFileChangeTime(const void *p1, const void *p2)
{
    struct stat buf1, buf2;

    /** Cast the arguments back to pointers to char (file names) */
    const char *file1 = *(const char **)p1;
    const char *file2 = *(const char **)p2;

    /** Get file stats for each file */
    if (lstat(file1, &buf1) < 0)
    {
        perror("stat error for file1");
        return 0; /** Error case, treat files as equal */
    }
    if (lstat(file2, &buf2) < 0)
    {
        perror("stat error for file2");
        return 0; /** Error case, treat files as equal */
    }

    /** Compare change times (st_ctime) */
    if (buf1.st_ctime > buf2.st_ctime)
        return -1;
    else if (buf1.st_ctime < buf2.st_ctime)
        return 1;
    else
        return 0; /** Change times are the same */
}

int CheckSymbolicLinkTarget(const char *path)
{
    struct stat buf;
    if (stat(path, &buf) == -1)
    {

        if (errno == ENOENT)
        {
            return BROKEN_LINK;
        }
        else
        {
            perror("stat error");
        }
    }

    return PROPER_LINK;
}

void PrintEntry(char *Entry, struct stat buf, char *path, int max_len)
{
    if (buf.st_mode & S_ISUID)
    {
        printf(WHITE_TEXT_RED_HIGHLIGHT);
    }

    else if (buf.st_mode & S_ISGID)
    {
        printf(BLACK_TEXT_YELLOW_HIGHLIGHT);
    }

    /** Check if it's an executable regular file */
    else if (S_ISREG(buf.st_mode) && (buf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
    {
        /** Executable file */
        printf(EXECUTABLE_FILE);
    }
    /** Check if it's a regular file */
    else if (S_ISREG(buf.st_mode))
    {
        /** Regular file */
        printf(REGULAR_FILE);
    }
    /** Check if it's a directory */
    else if (S_ISDIR(buf.st_mode))
    {
        /** Directory */
        printf(DIRECTORY);
    }

    /** Check if it's a character special file */
    else if (S_ISCHR(buf.st_mode))
    {
        /** Character special file (e.g., terminal devices) */
        printf(CHARACTER_SPECIAL_FILE);
    }
    /** Check if it's a block special file */
    else if (S_ISBLK(buf.st_mode))
    {
        /** Block special file (e.g., disk devices) */
        printf(BLOCK_SPECIAL_FILE);
    }
    /** Check if it's a FIFO or named pipe */
    else if (S_ISFIFO(buf.st_mode))
    {
        /** FIFO or named pipe */
        printf(NAMED_PIPE);
    }
    /** Check if it's a socket */
    else if (S_ISSOCK(buf.st_mode))
    {
        /** Socket */
        printf(SOCKET);
    }

    /** Check if it's a symbolic link */
    else if (S_ISLNK(buf.st_mode))
    {
        if (CheckSymbolicLinkTarget(path) == BROKEN_LINK)
        {
            /** Broken link => color is red */
            printf(RED_HIGHLIGHT);
        }
        else
        {
            /** Proper Symbolic link */
            printf(SOFT_LINK);
        }
    }

    /** Default case for unrecognized file type */
    else
    {
        /** Default case (unrecognized file type) */
        printf(white);
    }

    /** Print the entry name */
    if (!OptionsFlags[LONG_FORMAT_OPTION_l])
    {
        printf("%s", Entry); // Print the entry name
        printf(reset);       // Reset the color after printing the entry

        // Manually print spaces to align the output
        int entry_len = strlen(Entry); // Get the length of the entry
        for (int i = 0; i < (max_len - entry_len); i++)
        {
            printf(" "); // Print space
        }

        // Print two additional spaces
        printf("  ");
    }

    else
    {
        printf("%s", Entry);
    }

    /** Check if long format option is set and if it's a symbolic link => print the target file */
    if (OptionsFlags[LONG_FORMAT_OPTION_l] && S_ISLNK(buf.st_mode))
    {
        char link_target[MAX_PATH_LENGTH];
        ssize_t len = readlink(path, link_target, sizeof(link_target) - 1);
        if (len != -1)
        {
            /** Null-terminate the string */
            link_target[len] = '\0';
            /** Print the symbolic link target */
            printf(" -> %s", link_target);
        }
        else
        {
            /** Error reading symbolic link */
            perror("Error reading symbolic link");
        }
    }

    /** Reset color to default after printing */
    printf(reset);
}

void GetFilePermessions(char *str, struct stat buf)
{
    int mode = buf.st_mode;
    // Check if the file is executable
    if (S_ISDIR(mode))
        str[0] = 'd'; // Directory
    else if (S_ISLNK(mode))
        str[0] = 'l'; // Symbolic link
    else if (S_ISREG(mode))
        str[0] = '-'; // Regular file

    /* owner  permissions */
    str[1] = (mode & S_IRUSR) ? 'r' : '-'; // Owner read
    str[2] = (mode & S_IWUSR) ? 'w' : '-'; // Owner write
    if (mode & S_ISUID)
    {
        str[3] = (mode & S_IXUSR) ? 's' : 'S'; // Set UID and execute permissions
    }
    else
    {
        str[3] = (mode & S_IXUSR) ? 'x' : '-'; // Owner execute
    }

    /* group permissions */
    str[4] = (mode & S_IRGRP) ? 'r' : '-'; // Group read
    str[5] = (mode & S_IWGRP) ? 'w' : '-'; // Group write
    if (mode & S_ISGID)
    {
        str[6] = (mode & S_IXGRP) ? 's' : 'S'; // Set GID and execute permissions
    }
    else
    {
        str[6] = (mode & S_IXGRP) ? 'x' : '-'; // Group execute
    }

    /* Others' permissions */
    str[7] = (mode & S_IROTH) ? 'r' : '-'; // Others read
    str[8] = (mode & S_IWOTH) ? 'w' : '-'; // Others write
    if (mode & S_ISVTX)
    {
        str[9] = (mode & S_IXOTH) ? 't' : 'T'; // Sticky bit and others' execute permissions
    }
    else
    {
        str[9] = (mode & S_IXOTH) ? 'x' : '-'; // Others execute
    }

    str[10] = '\0'; // Null-terminate the string
}

void PrintEntry_LongFormat(struct stat buf, char *file_name, char *path)
{

    if (OptionsFlags[SHOW_INODE_OPTION_i])
    {
        printf("%ld  ", buf.st_ino);
    }
    char str[11];
    strcpy(str, "----------");

    GetFilePermessions(str, buf);
    // Print file permissions
    printf("%3s ", str);

    // Number of hard links (right-aligned with a width of 3)
    printf("%3ld ", buf.st_nlink);

    // Owner name (left-aligned with a width of 8)
    struct passwd *pwd = getpwuid(buf.st_uid);
    printf("%-8s ", pwd->pw_name);

    // Group name (left-aligned with a width of 8)
    struct group *grp = getgrgid(buf.st_gid);
    printf("%-2s ", grp->gr_name);

    // File size (right-aligned with a width of 8)
    printf("%8ld ", buf.st_size);

    /* Time */
    if (OptionsFlags[ACCESS_TIME_OPTION_u])
    {
        char *time_str = ctime(&buf.st_atime);
        time_str[strlen(time_str) - 1] = '\0'; // Remove the newline
        printf(" %s ", time_str);
    }

    else if (OptionsFlags[CHANGE_TIME_OPTION_c])
    {
        char *time_str = ctime(&buf.st_atime);
        time_str[strlen(time_str) - 1] = '\0'; // Remove the newline
        printf(" %s ", time_str);
    }

    else
    {
        /* Default: Modification time */
        char *time_str = ctime(&buf.st_mtime);
        time_str[strlen(time_str) - 1] = '\0'; // Remove the newline
        printf(" %s ", time_str);
    }

    // File name (left-aligned)
    PrintEntry(file_name, buf, path, 0);
}