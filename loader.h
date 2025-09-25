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
// fileNames [out]: pointer to an array of strings to store file names (allocated inside the function)
// returns the number of files read, or -1 on error
int getAllFiles(const char *dirPath, char **&fileNames) {
    int count = 0;
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // if regular file
            count++;
        }
    }

    fileNames = (char **)malloc(count * sizeof(char *));

    dir = opendir(dirPath); // reset directory stream to beginning
    for(int i = 0; (entry = readdir(dir)) != NULL;) {
        if (entry->d_type == DT_REG) { // if regular file
            fileNames[i] = (char *)malloc((strlen(entry->d_name) + 1) * sizeof(char));
            sprintf(fileNames[i], "%s", entry->d_name);
            i++;
        }
    }

    return count;
}