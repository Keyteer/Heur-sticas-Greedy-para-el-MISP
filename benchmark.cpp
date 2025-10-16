#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <sys/stat.h>
#include "greedy.h"
#include "randGreedy.h"
#include "NeighList.h"
#include "loader.h"
#include "progressBar.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <graph_file_or_directory>\n", argv[0]);
        return 1;
    }

    char *path = argv[1];
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
        // single file case
        // remove fileName from path and store it in fileNames[0]
        fileNames = (char **)malloc(1 * sizeof(char *));
        char *base = strstr(path, "/erdos");
        base++[0] = '\0'; // null-terminate path and increment pointer to skip '/'
        fileNames[0] = (char *)malloc(strlen(base) + 1 * sizeof(char));
        strcpy(fileNames[0], base);

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
            sprintf(densStr, "p0c0.%d_", i + 1);
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

        // greedy MISP test
        auto start = std::chrono::high_resolution_clock::now();
        int misp_size = greedy(nl->n, nl, misp);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> elapsed = end - start;
        // recording results
        grTimes[density] += elapsed.count();
        grResults[density] += misp_size;


        // random greedy MISP test
        start = std::chrono::high_resolution_clock::now();
        misp_size = randGreedy(nl->n, nl);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        // recording results
        rgrTimes[density] += elapsed.count();
        rgrResults[density] += misp_size;
        
        // +1 to amount of tests for this density
        tests[density] += 1;

        delete[] misp;
        delete nl;

        // progress bar for long operations (should be overwritten by csv header)
        print_progress_bar(i + 1, fileCount, 20);
    }
    

    // print csv header
    printf("Density,Greedy_Time(μs),Greedy_MISP_Size,Random_Greedy_Time(μs),Random_Greedy_MISP_Size\n");

    for (int i = 0; i < 9; i++) {
        if (tests[i] > 0) {
            // divide sums by number of tests to get averages
            grTimes[i] /= tests[i];
            rgrTimes[i] /= tests[i];
            grResults[i] /= tests[i];
            rgrResults[i] /= tests[i];

            // print results for this density
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
