#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MIN_LATITUDE -90.0 // - means south
#define MAX_LATITUDE 90.0 // + means north
#define MIN_LONGITUDE -180.0 // - means West
#define MAX_LONGITUDE 180.0 // - means East

typedef struct {
    double latitude;
    double longitude;
} City;

City* generate_cities(int num_cities) {
    srand(time(NULL));
    City* cities = (City*)malloc(num_cities * sizeof(City));
    if (cities == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < num_cities; i++) {
        cities[i].latitude = (rand() / (double)RAND_MAX) * (MAX_LATITUDE - MIN_LATITUDE) + MIN_LATITUDE;
        cities[i].longitude = (rand() / (double)RAND_MAX) * (MAX_LONGITUDE - MIN_LONGITUDE) + MIN_LONGITUDE;
    }

    return cities;
}

void write_map_to_file(City* cities, int num_cities) {
    FILE* fp = fopen("map_earth.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for writing.\n");
        exit(1);
    }

    fprintf(fp, "%d\n", num_cities);
    for (int i = 0; i < num_cities; i++) {
        fprintf(fp, "%f %f\n", cities[i].latitude, cities[i].longitude);
    }

    fclose(fp);
}

int main() {
    int num_cities;
    printf("Enter the number of cities: ");
    scanf("%d", &num_cities);

    if (num_cities <= 0) {
        fprintf(stderr, "Number of cities must be positive.\n");
        return 1;
    }

    City* cities = generate_cities(num_cities);
    write_map_to_file(cities, num_cities);
    free(cities);
    return 0;
}
