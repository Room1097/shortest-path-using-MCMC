#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_ITER 10000 // Number of iterations
#define ALPHA 0.95 // Cooling rate

typedef struct {
    int x;
    int y;
} City;


double distance(City city1, City city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}


City* read_cities(char* filename, int* num_cities) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for reading.\n");
        exit(1);
    }

    fscanf(fp, "%d", num_cities);
    City* cities = (City*)malloc(*num_cities * sizeof(City));
    if (cities == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < *num_cities; i++) {
        fscanf(fp, "%d %d", &cities[i].x, &cities[i].y);
    }

    fclose(fp);
    return cities;
}


double tour_distance(int* tour, City* cities, int num_cities) {
    double total_distance = 0.0;
    for (int i = 0; i < num_cities; i++) {
        total_distance += distance(cities[tour[i]], cities[tour[(i + 1) % num_cities]]);
    }
    return total_distance;
}


void write_tour_to_file(int* tour, int num_cities) {
    FILE* fp = fopen("tour.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for writing.\n");
        exit(1);
    }

    for (int i = 0; i < num_cities; i++) {
        fprintf(fp, "%d\n", tour[i]);
    }

    fclose(fp);
}


void mcmc(City* cities, int num_cities) {
    srand(time(NULL));
    int* tour = (int*)malloc(num_cities * sizeof(int));
    if (tour == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

  
    for (int i = 0; i < num_cities; i++) {
        tour[i] = i;
    }
    for (int i = 0; i < num_cities; i++) {
        int j = rand() % num_cities;
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
    }

    double current_distance = tour_distance(tour, cities, num_cities);
    double best_distance = current_distance;

    for (int iter = 0; iter < N_ITER; iter++) {
        int i = rand() % num_cities;
        int j = rand() % num_cities;
        if (i == j) continue;

      
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;

        double new_distance = tour_distance(tour, cities, num_cities);
        double acceptance_prob = exp((current_distance - new_distance) / ALPHA);

        if (acceptance_prob > (double)rand() / RAND_MAX) {
            current_distance = new_distance;
            if (current_distance < best_distance) {
                best_distance = current_distance;
            }
        } else {
         
            temp = tour[i];
            tour[i] = tour[j];
            tour[j] = temp;
        }
    }

    printf("Best distance found: %f\n", best_distance);
    write_tour_to_file(tour, num_cities);
    free(tour);
}

int main() {
    char* filename = "map.txt";
    int num_cities;
    City* cities = read_cities(filename, &num_cities);

    mcmc(cities, num_cities);

    free(cities);
    return 0;
}
