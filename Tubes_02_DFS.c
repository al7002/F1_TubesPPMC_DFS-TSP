/* EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
* Modul            : Tugas Besar - Travelling Salesmen Problem (TSP)
* Hari dan Tanggal : Senin, 20 Mei 2024
* Nama (NIM)       : Aliya Marzooqa H. (18321015)
* Asisten (NIM)    : Emmanuella Pramudita Rumanti (13220031)
* Nama File        : main.c
* Deskripsi        : Program yang memecahkan masalah TSP dengan algoritma DFS (Depth First Search)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>

#define EARTH_RADIUS 6371 // in kilometers

// Structure to hold city information
struct City {
    char name[50];
    double latitude;
    double longitude;
    int visited;
};

// Function to calculate distance between two cities using Haversine formula
double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dlat = (lat2 - lat1) * M_PI / 180.0;
    double dlon = (lon2 - lon1) * M_PI / 180.0;
    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

// DFS to find the shortest path
void DFS(struct City *cities, int numCities, int currentCityIndex, int startCityIndex, int *path, int *bestPath, int *pathIndex, int *bestPathIndex, double *totalDistance, double *minDistance, double currentDistance) {
    cities[currentCityIndex].visited = 1;
    path[*pathIndex] = currentCityIndex;
    (*pathIndex)++;

    if (*pathIndex == numCities) {
        // All cities visited, return to the start city
        currentDistance += haversine(cities[currentCityIndex].latitude, cities[currentCityIndex].longitude, cities[startCityIndex].latitude, cities[startCityIndex].longitude);
        if (currentDistance < *minDistance) {
            *minDistance = currentDistance;
            *totalDistance = currentDistance;
            memcpy(bestPath, path, numCities * sizeof(int));
            bestPath[numCities] = startCityIndex;
            *bestPathIndex = numCities + 1;
        }
    } else {
        // Explore next cities
        for (int i = 0; i < numCities; i++) {
            if (!cities[i].visited) {
                double newDistance = currentDistance + haversine(cities[currentCityIndex].latitude, cities[currentCityIndex].longitude, cities[i].latitude, cities[i].longitude);
                DFS(cities, numCities, i, startCityIndex, path, bestPath, pathIndex, bestPathIndex, totalDistance, minDistance, newDistance);
            }
        }
    }

    // Backtrack
    cities[currentCityIndex].visited = 0;
    (*pathIndex)--;
}

void removeQuotes(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '\"') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

int main() {
    char fileName[100], startingCityName[50];
    printf("Enter list of cities file name: ");
    scanf("%s", fileName);
    printf("Enter starting point: ");
    scanf("%s", startingCityName);

    // Read cities from file
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int numCities = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        numCities++;
    }
    rewind(file);

    struct City *cities = malloc(numCities * sizeof(struct City));
    for (int i = 0; i < numCities; i++) {
        fgets(line, sizeof(line), file);

        char *token = strtok(line, ",");
        removeQuotes(token);
        strcpy(cities[i].name, token);

        token = strtok(NULL, ",");
        cities[i].latitude = atof(token);

        token = strtok(NULL, ",");
        cities[i].longitude = atof(token);

        cities[i].visited = 0;
    }
    fclose(file);

    // Remove quotes from startingCityName
    removeQuotes(startingCityName);

    // Find index of starting city
    int startingCityIndex = -1;
    for (int i = 0; i < numCities; i++) {
        if (strcmp(cities[i].name, startingCityName) == 0) {
            startingCityIndex = i;
            break;
        }
    }
    if (startingCityIndex == -1) {
        printf("Starting city not found in the list.\n");
        free(cities);
        return 1;
    }

    // Initialize variables for DFS
    double totalDistance = 0;
    double minDistance = DBL_MAX;
    int *path = malloc((numCities + 1) * sizeof(int));
    int *bestPath = malloc((numCities + 1) * sizeof(int));
    int pathIndex = 0;
    int bestPathIndex = 0;

    // Measure start time
    clock_t startTime = clock();

    // Perform DFS to find the shortest path
    DFS(cities, numCities, startingCityIndex, startingCityIndex, path, bestPath, &pathIndex, &bestPathIndex, &totalDistance, &minDistance, 0);

    // Measure end time
    clock_t endTime = clock();
    double timeElapsed = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Print the path
    printf("Best route found:\n");
    for (int i = 0; i < bestPathIndex; i++) {
        printf("%s", cities[bestPath[i]].name);
        if (i < bestPathIndex - 1) {
            printf(" -> ");
        }
    }
    printf("\nBest route distance: %.5lf km\n", totalDistance);
    printf("Time elapsed: %.10lf s\n", timeElapsed);

    // Free allocated memory
    free(cities);
    free(path);
    free(bestPath);

    return 0;
}
