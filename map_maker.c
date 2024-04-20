#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} City;

double distance(City city1, City city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

City* generate_cities(int num_cities, int map_size) {
    srand(time(NULL));
    City* cities = (City*)malloc(num_cities * sizeof(City));
    if (cities == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < num_cities; i++) {
        cities[i].x = rand() % map_size;
        cities[i].y = rand() % map_size;
    }

    return cities;
}

void write_map_to_file(City* cities, int num_cities) {
    FILE* fp = fopen("map.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for writing.\n");
        exit(1);
    }

    fprintf(fp, "%d\n", num_cities);
    for (int i = 0; i < num_cities; i++) {
        fprintf(fp, "%d %d\n", cities[i].x, cities[i].y);
    }

    fclose(fp);
}

int main() {
    int num_cities, map_size;
    printf("Enter the number of cities: ");
    scanf("%d", &num_cities);
    printf("Enter the size of the map: ");
    scanf("%d", &map_size);

    City* cities = generate_cities(num_cities, map_size);
    write_map_to_file(cities, num_cities);
    free(cities);
    return 0;
}
