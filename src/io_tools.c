#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>

bool is_directory(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0)
    {
        return false; // Unable to read file stats, assume not a directory
    }
    return S_ISDIR(path_stat.st_mode);
}


int file_exists(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0); // is it a file?
}

