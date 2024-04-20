#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define M_PI 3.14
int MAX = 10000; // Number of iterations (can be changed)
float ALPHA = 0.95; // Temperature (we are taking it by default but it can be changed by the user)

typedef struct {
    double latitude;
    double longitude;
} City;

double distance(City city1, City city2) {
    double deg_to_rad = M_PI / 180.0; // Conversion factor from degrees to radians

    double lat1_rad = city1.latitude * deg_to_rad;
    double lon1_rad = city1.longitude * deg_to_rad;
    double lat2_rad = city2.latitude * deg_to_rad;
    double lon2_rad = city2.longitude * deg_to_rad;

    double dLat = lat2_rad - lat1_rad;
    double dLon = lon2_rad - lon1_rad;

    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1_rad) * cos(lat2_rad);
    double rad = 6371.0; // Earth radius in kilometers
    double c = 2 * asin(sqrt(a));

    return rad * c;
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
        fscanf(fp, "%lf %lf", &cities[i].latitude, &cities[i].longitude);
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

    for (int iter = 0; iter < MAX; iter++) {
        int i = rand() % num_cities;
        int j = rand() % num_cities;
        if (i == j) continue;

        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;

        double new_distance = tour_distance(tour, cities, num_cities);
        double deltad = current_distance - new_distance;
        double acceptance_prob = exp((deltad) / ALPHA);
        
        if (deltad > 0 || (ALPHA > 0 && acceptance_prob > (double)rand() / RAND_MAX)) {
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

    printf("Best distance found: %f\n km", best_distance);
    write_tour_to_file(tour, num_cities);
    free(tour);
}

int main() {
    char* filename = "map_earh.txt";
    int num_cities;
    printf("Input the value for temperature (it is taken as 0.95 by default to continue with it type 0.95)");
    scanf("%lf",&ALPHA);
    printf("Input the value of MAX iteration (it is taken 10000 by default to continue with it type 10000)");
    scanf("%lf",&MAX);
    City* cities = read_cities(filename, &num_cities);

    mcmc(cities, num_cities);

    free(cities);
    return 0;
}
