#include "stdio.h"
#include "stdlib.h"
#include <chrono>
#include <sys/stat.h>
#include "gready.h"
#include "NeighList.h"
#include "loader.h"

// compute degrees array from a loaded NeighList
static void computeDegrees(NeighList *nl, int *degrees) {
	int n = nl->n;
	for (int i = 0; i < n; ++i) {
		int deg = 0;
		struct Neighbor *curr = nl->neighborhoods[i];
		while (curr != nullptr) {
			++deg;
			curr = curr->next;
		}
		degrees[i] = deg;
	}
}

int main(int argc, char *argv[]) {
	const char *default_dir = "dataset_grafos_no_dirigidos/new_1000_dataset";
	const char *path = (argc >= 2) ? argv[1] : default_dir;

	// Determine if path is a directory
	struct stat st;
	bool path_is_dir = (stat(path, &st) == 0 && S_ISDIR(st.st_mode));

	const int MAX_FILES = 8192;
	char filePaths[MAX_FILES][256];
	int fileCount = 0;

	if (path_is_dir) {
		fileCount = getAllFiles(path, MAX_FILES, filePaths);
		if (fileCount < 0) {
			fprintf(stderr, "Failed to read directory: %s\n", path);
			return 1;
		}
	} else {
		// single file
		strncpy(filePaths[0], path, 255);
		filePaths[0][255] = '\0';
		fileCount = 1;
	}

	if (fileCount == 0) {
		fprintf(stderr, "No files found in %s\n", path);
		return 1;
	}

	for (int i = 0; i < fileCount; ++i) {
		const char *file = filePaths[i];
		NeighList *nl = loadGraph(file);
		if (nl == nullptr) {
			fprintf(stderr, "Skipping file (could not load): %s\n", file);
			continue;
		}

		int n = nl->n;
		int *degrees = new int[n];
		computeDegrees(nl, degrees);

		int *misp = new int[n];

		auto start = std::chrono::high_resolution_clock::now();
		int misp_size = gready(n, nl, degrees, misp);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;

		printf("%s: Time: %f ms, MISP size: %d\n", file, elapsed.count(), misp_size);

		delete[] misp;
		delete[] degrees;
		delete nl;
	}

	return 0;
}
