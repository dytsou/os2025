#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

int main(int argc, char **argv){
    if (argc < 2){
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(1);
    }

    // Open directory
    DIR *dir = opendir(argv[1]);
    if (dir == NULL){
        fprintf(stderr, "Error: Cannot open directory %s\n", argv[1]);
        exit(1);
    }

    struct dirent *entry;
    struct stat file_stat;
    char filepath[1024];
    char file_type;
    char permissions[11]; // 10 chars + null terminator
    char *time_str;
    char time_str_clean[26];

    while ((entry = readdir(dir)) != NULL)
    {
        // Build full path for stat
        snprintf(filepath, sizeof(filepath), "%s/%s", argv[1], entry->d_name);

        // Use lstat to get file information (lstat doesn't follow symbolic links)
        if (lstat(filepath, &file_stat) != 0)
        {
            continue; // Skip if stat fails
        }

        // Determine file type
        if (S_ISREG(file_stat.st_mode))
        {
            file_type = '-';
        }
        else if (S_ISDIR(file_stat.st_mode))
        {
            file_type = 'd';
        }
        else if (S_ISLNK(file_stat.st_mode))
        {
            file_type = 'l';
        }
        else if (S_ISFIFO(file_stat.st_mode))
        {
            file_type = 'f';
        }
        else if (S_ISSOCK(file_stat.st_mode))
        {
            file_type = 's';
        }
        else if (S_ISBLK(file_stat.st_mode))
        {
            file_type = 'b';
        }
        else if (S_ISCHR(file_stat.st_mode))
        {
            file_type = 'c';
        }
        else
        {
            file_type = '?';
        }

        // Build permission string
        // Format: rwxrwxrwx (owner, group, other)
        permissions[0] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
        permissions[1] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
        
        // Handle owner execute and SUID
        if (file_stat.st_mode & S_ISUID)
        {
            permissions[2] = (file_stat.st_mode & S_IXUSR) ? 's' : 'S';
        }
        else
        {
            permissions[2] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
        }

        permissions[3] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
        permissions[4] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
        
        // Handle group execute and SGID
        if (file_stat.st_mode & S_ISGID)
        {
            permissions[5] = (file_stat.st_mode & S_IXGRP) ? 's' : 'S';
        }
        else
        {
            permissions[5] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
        }

        permissions[6] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
        permissions[7] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
        
        // Handle other execute and SBIT (sticky bit)
        if (file_stat.st_mode & S_ISVTX)
        {
            permissions[8] = (file_stat.st_mode & S_IXOTH) ? 't' : 'T';
        }
        else
        {
            permissions[8] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
        }
        
        permissions[9] = '\0';

        // Get modification time (ctime returns string with newline)
        time_str = ctime(&file_stat.st_mtime);
        // Remove newline from ctime output
        strncpy(time_str_clean, time_str, 24);
        time_str_clean[24] = '\0';

        // Output formatted result
        // Format: {{file type}}{{file permission}} {{UID}} {{GID}} {{size}} {{time}} {{filename}}
        printf("%c%s %4d %4d %6ld %s %s\n",
               file_type,
               permissions,
               (int)file_stat.st_uid,
               (int)file_stat.st_gid,
               (long)file_stat.st_size,
               time_str_clean,
               entry->d_name);
    }

    closedir(dir);
    return 0;
}

