#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#define MAX_PATH_LENGTH 4096
int main(int argc, char *argv[]) {
    // Check for the correct number of arguments
    if (argc < 2) {
        printf("Usage: filestats directory_name\n");
        return 1;
    }

    DIR* dr;
    struct dirent *en;
    struct stat en1;
    dr = opendir(argv[1]);

    if (dr) {
        char fullpath[MAX_PATH_LENGTH]; // Define a buffer for the full path

        while ((en = readdir(dr)) != NULL) {
            if (strcmp(en->d_name, ".") != 0 && strcmp(en->d_name, "..") != 0) {
                // Construct the full path by concatenating the directory path and the file name
                snprintf(fullpath, sizeof(fullpath), "%s/%s", argv[1], en->d_name);

                if (stat(fullpath, &en1) == 0) { // Check the result of stat
                    printf("File name: %s\n", en->d_name);
                    printf("inode number: %lu\n", (unsigned long)en->d_ino);
                    printf("number of links: %ld\n", (long)en1.st_nlink);
                    printf("User ID of owner: %ld\n", (long)en1.st_uid);
                    printf("Group ID of owner: %ld\n", (long)en1.st_gid);
                    printf("Size in bytes: %ld\n", (long)en1.st_size);
                    printf("Last access: %s", ctime(&en1.st_atime));
                    printf("Last modification: %s", ctime(&en1.st_mtime));
                    printf("Last status change: %s", ctime(&en1.st_ctime));
                    printf("Number of disk blocks allocated: %ld\n", (long)en1.st_blocks);
                    printf("Access mode in octal: %o\n", en1.st_mode);
                    
                    unsigned int access_mode = en1.st_mode;
                    char mode_str[] = "rwxrwxrwx"; // Default string with all permission characters
                    // Set permission characters based on access mode
                    if (!(access_mode & S_IRUSR)) mode_str[0] = '-';
                    if (!(access_mode & S_IWUSR)) mode_str[1] = '-';
                    if (!(access_mode & S_IXUSR)) mode_str[2] = '-';
                    if (!(access_mode & S_IRGRP)) mode_str[3] = '-';
                    if (!(access_mode & S_IWGRP)) mode_str[4] = '-';
                    if (!(access_mode & S_IXGRP)) mode_str[5] = '-';
                    if (!(access_mode & S_IROTH)) mode_str[6] = '-';
                    if (!(access_mode & S_IWOTH)) mode_str[7] = '-';
                    if (!(access_mode & S_IXOTH)) mode_str[8] = '-';

                    printf("Access mode flags: %s\n", mode_str);
                    printf("\n");

                }
                else {
                    perror("stat"); // Print an error message if stat fails
                }
            }
        }
        closedir(dr); // Close the directory
        return 0;
    } else {
        printf("Can't open %s\n", argv[1]);
        return 1;
    }
}
