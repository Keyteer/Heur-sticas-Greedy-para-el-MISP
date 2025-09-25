#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include "NeighList.h"

// returns an neighborhood list (NeighList) from a file
NeighList *loadGraph(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return nullptr;
    }

    int n;
    fscanf(fp, "%d", &n);
    
    NeighList *nl = new NeighList(n);

    while(!feof(fp)) {
        int u, v;
        fscanf(fp, "%d %d", &u, &v);

        nl->push(u, v);
        nl->push(v, u);
    }

    fclose(fp);
    return nl;
}

// Get all file paths from a directory
// dirPath [in]: path to the directory
// maxFiles [in]: maximum number of files to read
// filePaths [out]: array to store file paths
// returns the number of files read, or -1 on error
int getAllFiles(const char *dirPath, int maxFiles, char filePaths[][256]) {
    int count = 0;
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && count < maxFiles) {
        if (entry->d_type == DT_REG) { // if regular file
            snprintf(filePaths[count], 256, "%s/%s", dirPath, entry->d_name);
            count++;
        }
    }

    closedir(dir);
    return count;
}