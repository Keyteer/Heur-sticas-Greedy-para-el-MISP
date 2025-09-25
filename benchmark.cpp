#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <sys/stat.h>
#include "gready.h"
#include "NeighList.h"
#include "loader.h"
#include "progressBar.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <graph_file_or_directory>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    if (path[strlen(path) - 1] == '/')
        ((char *)path)[strlen(path) - 1] = '\0';

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        perror("Error accessing path");
        return 1;
    }

    char **fileNames;
    int fileCount = 0;

    if (S_ISDIR(path_stat.st_mode)) {
        // directory
        fileCount = getAllFiles(path, fileNames);

        if (fileCount == 0) {
            fprintf(stderr, "No files found in %s\n", path);
            return 1;
        }
    } else {
        // single file
        fileNames = (char **)malloc(1 * sizeof(char *));
        fileNames[0] = (char *)malloc((strlen(path) + 1) * sizeof(char));
        strncpy(fileNames[0], path, strlen(path) + 1);
        fileCount = 1;
    }

    // indicator sumations for densities [0.1, 0.2, ... , 0.9]
    double grTimes[9] = {0,0,0,0,0,0,0,0,0};
    double rgrTimes[9] = {0,0,0,0,0,0,0,0,0};
    int grResults[9] = {0,0,0,0,0,0,0,0,0};
    int rgrResults[9] = {0,0,0,0,0,0,0,0,0};
    int tests[9] = {0,0,0,0,0,0,0,0,0};

    for (int i = 0; i < fileCount; i++) {
        const char *file = fileNames[i];

        int density = -1;
        // selecting only files with density [0.1, 0.2, ... , 0.9]
        for (int i = 0; i < 9; i++) {
            char densStr[10];
            sprintf(densStr, "p0c0.%d", i + 1);
            if (strstr(file, densStr)) {
                density = i;
                break;
            }
        }
        if (density < 0) 
            continue;

        // load graph
        char *fullPath = new char[strlen(path) + strlen(file) + 2];
        sprintf(fullPath, "%s/%s", path, file);       
        NeighList *nl = loadGraph(fullPath);
        if (nl == nullptr) {
            fprintf(stderr, "Skipping file (could not load): %s\n", file);
            continue;
        }

        int *misp = new int[nl->n];

        auto start = std::chrono::high_resolution_clock::now();
        int misp_size = gready(nl->n, nl, misp);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        grTimes[density] += elapsed.count();
        grResults[density] += misp_size;
        tests[density] += 1;

        delete[] misp;
        delete nl;

        print_progress_bar(i + 1, fileCount, 20);
    }
    

    printf("Desnsity,Greedy_Time(ms),Greedy_MISP_Size,Random_Greedy_Time(ms),Random_Greedy_MISP_Size\n");
    for (int i = 0; i < 9; i++) {
        if (tests[i] > 0) {
            grTimes[i] /= tests[i];
            rgrTimes[i] /= tests[i];
            grResults[i] /= tests[i];
            rgrResults[i] /= tests[i];
            printf("0.%d,%f,%d,%f,%d\n", i + 1,
                   grTimes[i] / tests[i], grResults[i],
                   rgrTimes[i] / tests[i], rgrResults[i]);
        }
    }

    for (int i = 0; i < fileCount; ++i) {
        free(fileNames[i]);
    }
    free(fileNames);

    return 0;
}
