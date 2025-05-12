#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// User structure for linked list
typedef struct User {
    char username[50];
    char password[50];
    char role[10]; // "admin" or "user"
    struct User *next;
} User;

// tour guider 



#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_LOCATION_LENGTH 100
#define MAX_LANGUAGES 10

typedef struct Guider {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    char gender[10];
    char phone[15];
    char email[MAX_EMAIL_LENGTH];
    char languages[MAX_LANGUAGES][MAX_NAME_LENGTH];
    int language_count;
    float per_day_charge;
    char location[MAX_LOCATION_LENGTH];
    float rating;
    int rating_count;
    struct Guider *next;
} Guider;

typedef struct Booking {
    int booking_id;
    int guider_id;
    char customer_name[MAX_NAME_LENGTH];
    char booking_date[15];
    int days;
    struct Booking *next;
} Booking;

Guider *guides = NULL;
Booking *bookings = NULL;

void addGuider() {
    Guider *g = (Guider *)malloc(sizeof(Guider));
    if (!g) {
        printf("Memory allocation failed!\n");
        return;
    }

    g->id = rand() % 1000; // Random ID for simplicity
    printf("Enter guider name: ");
    getchar();
    fgets(g->name, MAX_NAME_LENGTH, stdin);
    g->name[strcspn(g->name, "\n")] = '\0';

    printf("Enter age: ");
    scanf("%d", &g->age);
    getchar();

    printf("Enter gender: ");
    fgets(g->gender, 10, stdin);
    g->gender[strcspn(g->gender, "\n")] = '\0';

    printf("Enter phone: ");
    fgets(g->phone, 15, stdin);
    g->phone[strcspn(g->phone, "\n")] = '\0';

    printf("Enter email: ");
    fgets(g->email, MAX_EMAIL_LENGTH, stdin);
    g->email[strcspn(g->email, "\n")] = '\0';

    printf("Enter number of languages: ");
    scanf("%d", &g->language_count);
    getchar();

    for (int i = 0; i < g->language_count; i++) {
        printf("Enter language %d: ", i + 1);
        fgets(g->languages[i], MAX_NAME_LENGTH, stdin);
        g->languages[i][strcspn(g->languages[i], "\n")] = '\0';
    }

    printf("Enter per day charge: ");
    scanf("%f", &g->per_day_charge);
    getchar();

    printf("Enter location: ");
    fgets(g->location, MAX_LOCATION_LENGTH, stdin);
    g->location[strcspn(g->location, "\n")] = '\0';

    g->rating = 0;
    g->rating_count = 0;
    g->next = guides;
    guides = g;

    printf("Guider added successfully!\n");
}

void deleteGuider() {
    int id;
    printf("Enter guider ID to delete: ");
    scanf("%d", &id);

    Guider *current = guides;
    Guider *previous = NULL;

    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Guider with ID %d not found!\n", id);
        return;
    }

    if (previous == NULL) {
        guides = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    printf("Guider deleted successfully!\n");
}

void viewGuiders() {
    Guider *g = guides;
    if (g == NULL) {
        printf("No guides available!\n");
        return;
    }

    printf("\n--- Guide List ---\n");
    while (g != NULL) {
        printf("ID: %d\n", g->id);
        printf("Name: %s\n", g->name);
        printf("Age: %d\n", g->age);
        printf("Gender: %s\n", g->gender);
        printf("Phone: %s\n", g->phone);
        printf("Email: %s\n", g->email);
        printf("Languages: ");
        for (int i = 0; i < g->language_count; i++) {
            printf("%s ", g->languages[i]);
        }
        printf("\nCharge: %.2f\n", g->per_day_charge);
        printf("Location: %s\n", g->location);
        printf("Rating: %.2f (based on %d ratings)\n", g->rating, g->rating_count);
        g = g->next;
    }
}

void addBooking(int guider_id) {
    Booking *b = (Booking *)malloc(sizeof(Booking));
    if (!b) {
        printf("Memory allocation failed!\n");
        return;
    }

    b->guider_id = guider_id;
    printf("Enter customer name: ");
    getchar();
    fgets(b->customer_name, MAX_NAME_LENGTH, stdin);
    b->customer_name[strcspn(b->customer_name, "\n")] = '\0';

    printf("Enter booking date (YYYY-MM-DD): ");
    fgets(b->booking_date, 15, stdin);
    b->booking_date[strcspn(b->booking_date, "\n")] = '\0';

    printf("Enter number of days: ");
    scanf("%d", &b->days);
    getchar();

    b->next = bookings;
    bookings = b;

    printf("Booking added successfully!\n");
}

void viewBookings() {
    Booking *b = bookings;
    if (b == NULL) {
        printf("No bookings found!\n");
        return;
    }

    printf("\n--- Bookings ---\n");
    while (b != NULL) {
        printf("Booking ID: %d, Guider ID: %d, Customer Name: %s, Booking Date: %s, Days: %d\n",
            b->booking_id, b->guider_id, b->customer_name, b->booking_date, b->days);
        b = b->next;
    }
}

void rateGuider(int guider_id) {
    Guider *g = guides;
    while (g != NULL && g->id != guider_id) {
        g = g->next;
    }

    if (g == NULL) {
        printf("Guider with ID %d not found!\n", guider_id);
        return;
    }

    float rating;
    printf("Enter rating for the guider (1-5): ");
    scanf("%f", &rating);

    if (rating < 1 || rating > 5) {
        printf("Invalid rating! Please enter a value between 1 and 5.\n");
        return;
    }

    g->rating = ((g->rating * g->rating_count) + rating) / (g->rating_count + 1);
    g->rating_count++;

    printf("Thank you for your rating!\n");
}

void loadData() {
    FILE *file = fopen("guiders_data.txt", "r");
    if (file == NULL) {
        printf("No existing data file found.\n");
        return;
    }

    // Load guider data from file
    while (!feof(file)) {
        Guider *g = (Guider *)malloc(sizeof(Guider));
        fscanf(file, "%d\n", &g->id);
        fgets(g->name, MAX_NAME_LENGTH, file);
        g->name[strcspn(g->name, "\n")] = '\0';
        fscanf(file, "%d\n", &g->age);
        fgets(g->gender, 10, file);
        g->gender[strcspn(g->gender, "\n")] = '\0';
        fgets(g->phone, 15, file);
        g->phone[strcspn(g->phone, "\n")] = '\0';
        fgets(g->email, MAX_EMAIL_LENGTH, file);
        g->email[strcspn(g->email, "\n")] = '\0';
        fscanf(file, "%d\n", &g->language_count);

        for (int i = 0; i < g->language_count; i++) {
            fgets(g->languages[i], MAX_NAME_LENGTH, file);
            g->languages[i][strcspn(g->languages[i], "\n")] = '\0';
        }

        fscanf(file, "%f\n", &g->per_day_charge);
        fgets(g->location, MAX_LOCATION_LENGTH, file);
        g->location[strcspn(g->location, "\n")] = '\0';

        g->rating = 0;
        g->rating_count = 0;
        g->next = guides;
        guides = g;
    }

    fclose(file);
}

void saveData() {
    FILE *file = fopen("guiders_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for saving data.\n");
        return;
    }

    Guider *g = guides;
    while (g != NULL) {
        fprintf(file, "%d\n", g->id);
        fprintf(file, "%s\n", g->name);
        fprintf(file, "%d\n", g->age);
        fprintf(file, "%s\n", g->gender);
        fprintf(file, "%s\n", g->phone);
        fprintf(file, "%s\n", g->email);
        fprintf(file, "%d\n", g->language_count);
        for (int i = 0; i < g->language_count; i++) {
            fprintf(file, "%s\n", g->languages[i]);
        }
        fprintf(file, "%.2f\n", g->per_day_charge);
        fprintf(file, "%s\n", g->location);
        fprintf(file, "%.2f\n", g->rating);
        fprintf(file, "%d\n", g->rating_count);
        g = g->next;
    }

    fclose(file);
}

void userFunctions() {
    int choice;
    while (1) {
        printf("\n--- User Menu ---\n");
        printf("1. View Guides\n");
        printf("2. Add Booking\n");
        printf("3. Rate Guider\n");
        printf("4. View Bookings\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewGuiders();
                break;
            case 2:
                {
                    int guider_id;
                    printf("Enter guider ID for booking: ");
                    scanf("%d", &guider_id);
                    addBooking(guider_id);
                }
                break;
            case 3:
                {
                    int guider_id;
                    printf("Enter guider ID to rate: ");
                    scanf("%d", &guider_id);
                    rateGuider(guider_id);
                }
                break;
            case 4:
                viewBookings();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void adminFunctions() {
    int choice;
    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Guider\n");
        printf("2. Delete Guider\n");
        printf("3. View Guides\n");
        printf("4. View Bookings\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addGuider();
                break;
            case 2:
                deleteGuider();
                break;
            case 3:
                viewGuiders();
                break;
            case 4:
                viewBookings();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}





// car hire 
#define MAX_NAME_LEN 100
#define MAX_LOCATION_LEN 50

// Car Node Structure
typedef struct Car {
    int carID;  // Unique identifier for each car
    char carType[MAX_NAME_LEN];  // Type of car (e.g., SUV, Sedan)
    char location[MAX_LOCATION_LEN];  // Location (city/country)
    float pricePerDay;  // Rental price per day
    int availability;  // 1 if available, 0 if rented
    int rentDuration;  // Rent duration in days
    struct Car* next;  // Pointer to next car in the linked list
} Car;

// Global declaration of carList
Car* carList = NULL;  // Initialize the head of the linked list to NULL

// Function to create a new car node
Car* createCar(int carID, char* carType, char* location, float pricePerDay) {
    Car* newCar = (Car*)malloc(sizeof(Car));
    newCar->carID = carID;
    strcpy(newCar->carType, carType);
    strcpy(newCar->location, location);
    newCar->pricePerDay = pricePerDay;
    newCar->availability = 1;  // Initially available
    newCar->rentDuration = 0;  // No rent duration at the time of addition
    newCar->next = NULL;
    return newCar;
}

// Function to add a car to the linked list
void addCar(int carID, char* carType, char* location, float pricePerDay) {
    Car* newCar = createCar(carID, carType, location, pricePerDay);
    if (carList == NULL) {
        carList = newCar;
    } else {
        Car* temp = carList;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newCar;
    }
}

// Function to search for cars by location and type
void searchCars(char* location, char* carType) {
    Car* temp = carList;
    int found = 0;
    printf("\nSearch results for cars in %s of type %s:\n", location, carType);
    while (temp != NULL) {
        if ((strcmp(temp->location, location) == 0) && (strcmp(temp->carType, carType) == 0)) {
            printf("Car ID: %d, Type: %s, Price per day: %.2f, Availability: %s\n",
                   temp->carID, temp->carType, temp->pricePerDay,
                   temp->availability ? "Available" : "Rented");
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) {
        printf("No cars found for the given location and type.\n");
    }
}

// Function to rent a car (mark it as rented)
void rentCar(int carID, int rentDuration) {
    Car* temp = carList;
    while (temp != NULL) {
        if (temp->carID == carID) {
            if (temp->availability == 1) {
                temp->availability = 0;  // Mark car as rented
                temp->rentDuration = rentDuration;
                printf("Car %d rented successfully for %d days.\n", carID, rentDuration);
                printf("Total Price: %.2f\n", rentDuration * temp->pricePerDay);
                return;
            } else {
                printf("Car %d is already rented.\n", carID);
                return;
            }
        }
        temp = temp->next;
    }
    printf("Car with ID %d not found.\n", carID);
}

// Function to return a rented car (mark it as available)
void returnCar(int carID) {
    Car* temp = carList;
    while (temp != NULL) {
        if (temp->carID == carID) {
            if (temp->availability == 0) {
                temp->availability = 1;  // Mark car as available
                temp->rentDuration = 0;  // Reset rent duration
                printf("Car %d returned successfully.\n", carID);
                return;
            } else {
                printf("Car %d was not rented.\n", carID);
                return;
            }
        }
        temp = temp->next;
    }
    printf("Car with ID %d not found.\n", carID);
}

// Function to display all available cars
void displayCars() {
    Car* temp = carList;
    if (temp == NULL) {
        printf("No cars available.\n");
        return;
    }
    printf("\nList of cars available for rent:\n");
    while (temp != NULL) {
        if (temp->availability == 1) {
            printf("Car ID: %d, Type: %s, Location: %s, Price per day: %.2f\n",
                   temp->carID, temp->carType, temp->location, temp->pricePerDay);
        }
        temp = temp->next;
    }
}

// Manage Cars Menu (formerly Admin Menu)
void manageCarsMenu() {
    int choice, carID, rentDuration;
    char carType[MAX_NAME_LEN], location[MAX_LOCATION_LEN];
    float pricePerDay;

    while (1) {
        printf("\nManage Cars Menu:\n");
        printf("1. Add Car\n2. Search Cars\n3. Display Cars\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter car ID: ");
                scanf("%d", &carID);
                printf("Enter car type (e.g., SUV, Sedan): ");
                scanf(" %[^\n]", carType);
                printf("Enter location (city/country): ");
                scanf(" %[^\n]", location);
                printf("Enter price per day: ");
                scanf("%f", &pricePerDay);
                addCar(carID, carType, location, pricePerDay);
                printf("Car added successfully.\n");
                break;

            case 2:
                printf("Enter location (city/country): ");
                scanf(" %[^\n]", location);
                printf("Enter car type (e.g., SUV, Sedan): ");
                scanf(" %[^\n]", carType);
                searchCars(location, carType);
                break;

            case 3:
                displayCars();
                break;

            case 4:
                return;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// Rent Cars Menu (formerly User Menu)
void rentCarsMenu() {
    int choice, carID, rentDuration;

    while (1) {
        printf("\nRent Cars Menu:\n");
        printf("1. Rent a Car\n2. Return a Car\n3. Display Available Cars\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter car ID to rent: ");
                scanf("%d", &carID);
                printf("Enter rent duration (in days): ");
                scanf("%d", &rentDuration);
                rentCar(carID, rentDuration);
                break;

            case 2:
                printf("Enter car ID to return: ");
                scanf("%d", &carID);
                returnCar(carID);
                break;

            case 3:
                displayCars();
                break;

            case 4:
                return;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}






//flight 
#define MAX_FLIGHTS 100

// Flight structure
typedef struct Flight {
    char flight_code[10];
    char from_city[50];
    char from_airport[50];
    char to_city[50];
    char to_airport[50];
    char flight_class[20];
    char type[20];
    char layover_city[50];
    int capacity_economy;
    int capacity_business;
    char airline[50];
    float price;
    struct Flight* next;
} Flight;

Flight* flight_list = NULL;

// Function to remove newline character from string
void remove_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Function to create a new flight node
Flight* create_flight(char* flight_code, char* from_city, char* from_airport,
                      char* to_city, char* to_airport, char* flight_class,
                      char* type, char* layover_city, int capacity_economy,
                      int capacity_business, char* airline, float price) {
    Flight* new_flight = (Flight*)malloc(sizeof(Flight));
    strcpy(new_flight->flight_code, flight_code);
    strcpy(new_flight->from_city, from_city);
    strcpy(new_flight->from_airport, from_airport);
    strcpy(new_flight->to_city, to_city);
    strcpy(new_flight->to_airport, to_airport);
    strcpy(new_flight->flight_class, flight_class);
    strcpy(new_flight->type, type);
    strcpy(new_flight->layover_city, layover_city);
    new_flight->capacity_economy = capacity_economy;
    new_flight->capacity_business = capacity_business;
    strcpy(new_flight->airline, airline);
    new_flight->price = price;
    new_flight->next = NULL;
    return new_flight;
}

// Combined Function for Add/Remove Flight (Admin Only)
void manage_flight() {
    int action;
    printf("\nChoose an action:\n");
    printf("1. Add Flight\n");
    printf("2. Delete Flight\n");
    printf("Enter choice: ");
    scanf("%d", &action);
    getchar();  // Clear buffer

    if (action == 1) {  // Add flight
        char flight_code[10], from_city[50], from_airport[50], to_city[50], to_airport[50],
             flight_class[20], type[20], layover_city[50], airline[50];
        int capacity_economy, capacity_business;
        float price;

        printf("\nEnter Flight Details:\n");
        printf("Flight Code: ");
        scanf("%s", flight_code);
        getchar();

        // Check if the flight code already exists in the list
        Flight* temp = flight_list;
        while (temp != NULL) {
            if (strcmp(temp->flight_code, flight_code) == 0) {
                printf("Error: Flight Code %s already exists. Please enter a unique Flight Code.\n", flight_code);
                return;
            }
            temp = temp->next;
        }

        printf("From City: ");
        fgets(from_city, sizeof(from_city), stdin);
        remove_newline(from_city);

        printf("From Airport: ");
        fgets(from_airport, sizeof(from_airport), stdin);
        remove_newline(from_airport);

        printf("To City: ");
        fgets(to_city, sizeof(to_city), stdin);
        remove_newline(to_city);

        printf("To Airport: ");
        fgets(to_airport, sizeof(to_airport), stdin);
        remove_newline(to_airport);

        printf("Flight Class (Economy/Business): ");
        scanf("%s", flight_class);
        getchar();

        printf("Flight Type (Direct/Connecting): ");
        scanf("%s", type);
        getchar();

        char layover[50] = "";
        if (strcmp(type, "Connecting") == 0) {
            printf("Enter Layover City: ");
            fgets(layover, sizeof(layover), stdin);
            remove_newline(layover);
        }

        printf("Economy Capacity: ");
        scanf("%d", &capacity_economy);
        printf("Business Capacity: ");
        scanf("%d", &capacity_business);
        printf("Airline: ");
        scanf("%s", airline);
        printf("Price: ");
        scanf("%f", &price);

        Flight* new_flight = create_flight(flight_code, from_city, from_airport,
                                            to_city, to_airport, flight_class,
                                            type, layover, capacity_economy,
                                            capacity_business, airline, price);

        if (flight_list == NULL) {
            flight_list = new_flight;
        } else {
            Flight* temp = flight_list;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_flight;
        }
        printf("\nFlight added successfully!\n");

    } else if (action == 2) {  // Delete flight
        char flight_code[10];
        printf("\nEnter the flight code to delete: ");
        scanf("%s", flight_code);

        Flight *temp = flight_list, *prev = NULL;

        // If the flight to be deleted is the first flight in the list
        if (temp != NULL && strcmp(temp->flight_code, flight_code) == 0) {
            flight_list = temp->next;  // Change head
            free(temp);
            printf("Flight with code %s deleted successfully!\n", flight_code);
            return;
        }

        // Search for the flight to be deleted
        while (temp != NULL && strcmp(temp->flight_code, flight_code) != 0) {
            prev = temp;
            temp = temp->next;
        }

        // If flight was not found
        if (temp == NULL) {
            printf("Flight with code %s not found!\n", flight_code);
            return;
        }

        // Unlink the node from the linked list
        prev->next = temp->next;
        free(temp);
        printf("Flight with code %s deleted successfully!\n", flight_code);
    } else {
        printf("Invalid choice! Try again.\n");
    }
}

// Function to search flights
void search_flights() {
    char from_city[50], to_city[50], flight_class[20], type[20], flight_code[10];
    int found = 0;

    printf("\nSearch Flights by:\n");
    printf("1. Destination (From City to To City)\n");
    printf("2. Flight Class (Economy/Business)\n");
    printf("3. Flight Type (Direct/Connecting)\n");
    printf("4. Unique Flight Code\n");
    printf("Enter choice: ");
    int choice;
    scanf("%d", &choice);
    getchar();  // Clear buffer

    Flight* temp = flight_list;
    Flight* matched_flights[MAX_FLIGHTS];
    int matched_count = 0;

    if (choice == 1) {
        printf("Enter From City: ");
        fgets(from_city, sizeof(from_city), stdin);
        remove_newline(from_city);
        printf("Enter To City: ");
        fgets(to_city, sizeof(to_city), stdin);
        remove_newline(to_city);
    } else if (choice == 2) {
        printf("Enter Flight Class (Economy/Business): ");
        fgets(flight_class, sizeof(flight_class), stdin);
        remove_newline(flight_class);
    } else if (choice == 3) {
        printf("Enter Flight Type (Direct/Connecting): ");
        fgets(type, sizeof(type), stdin);
        remove_newline(type);
    } else if (choice == 4) {
        printf("Enter Flight Code: ");
        fgets(flight_code, sizeof(flight_code), stdin);
        remove_newline(flight_code);
    }

    printf("\nSearch Results:\n");
    while (temp != NULL) {
        if ((choice == 1 && strcmp(temp->from_city, from_city) == 0 && strcmp(temp->to_city, to_city) == 0) ||
            (choice == 2 && strcmp(temp->flight_class, flight_class) == 0) ||
            (choice == 3 && strcmp(temp->type, type) == 0) ||
            (choice == 4 && strcmp(temp->flight_code, flight_code) == 0)) {

            matched_flights[matched_count++] = temp;
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No flights found matching the search criteria.\n");
        return;
    }

    for (int i = 0; i < matched_count; i++) {
        printf("Flight Code: %s, From: %s (%s) To: %s (%s), Class: %s, Type: %s, Price: %.2f, Airline: %s",
               matched_flights[i]->flight_code, matched_flights[i]->from_city, matched_flights[i]->from_airport,
               matched_flights[i]->to_city, matched_flights[i]->to_airport, matched_flights[i]->flight_class,
               matched_flights[i]->type, matched_flights[i]->price, matched_flights[i]->airline);
        if (strlen(matched_flights[i]->layover_city) > 0) {
            printf(", Layover: %s", matched_flights[i]->layover_city);
        }
        printf("\n");
    }
}


//hotel 
#define FILENAME "hotels.dat"

// Node structure for hotel data
typedef struct Node {
    char hotelName[100];
    char location[100];
    char uniqueCode[20];
    int totalRatings;
    int numberOfUsers;
    int bookings;
    int capacity;
    float averageRating;
    float price; // Price for the hotel
    struct Node* next;
} Node;

// Function to load data from file into the linked list
void loadHotelsFromFile(Node** head) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No previous data found, starting with empty list.\n");
    } else {
        char name[100], location[100], uniqueCode[20];
        int totalRatings, numberOfUsers, bookings, capacity;
        float averageRating, price;
        while (fscanf(file, " %[^\n] %[^\n] %[^\n] %d %d %d %d %f %f\n",
                       name, location, uniqueCode, &totalRatings,
                       &numberOfUsers, &bookings, &capacity, &averageRating, &price) != EOF) {
            Node* newNode = (Node*)malloc(sizeof(Node));
            strcpy(newNode->hotelName, name);
            strcpy(newNode->location, location);
            strcpy(newNode->uniqueCode, uniqueCode);
            newNode->totalRatings = totalRatings;
            newNode->numberOfUsers = numberOfUsers;
            newNode->bookings = bookings;
            newNode->capacity = capacity;
            newNode->averageRating = averageRating;
            newNode->price = price;
            newNode->next = *head;
            *head = newNode;
        }
        fclose(file);
    }
}

// Function to save data from the linked list to the file
void saveHotelsToFile(Node* head) {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file to save data.\n");
    } else {
        Node* temp = head;
        while (temp != NULL) {
            fprintf(file, "%s\n%s\n%s\n%d\n%d\n%d\n%d\n%.2f\n%.2f\n",
                    temp->hotelName, temp->location, temp->uniqueCode,
                    temp->totalRatings, temp->numberOfUsers, temp->bookings,
                    temp->capacity, temp->averageRating, temp->price);
            temp = temp->next;
        }
        fclose(file);
    }
}

// Admin function for hotel management
void adminHotelManagement(Node** head) {
    FILE* file;
    int choice, capacity;
    char name[100], location[100], uniqueCode[20];
    float price;

    while (1) {
        printf("\nAdmin Menu:\n");
        printf("1. Add Hotel\n2. Edit Hotel Capacity\n3. View All Hotels\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter hotel name: ");
                scanf(" %[^\n]", name);
                printf("Enter location: ");
                scanf(" %[^\n]", location);
                printf("Enter unique code: ");
                scanf(" %[^\n]", uniqueCode);
                printf("Enter capacity: ");
                scanf("%d", &capacity);
                printf("Enter price per night: ");
                scanf("%f", &price);
                break;

                // Add new hotel
                {
                    Node* newNode = (Node*)malloc(sizeof(Node));
                    strcpy(newNode->hotelName, name);
                    strcpy(newNode->location, location);
                    strcpy(newNode->uniqueCode, uniqueCode);
                    newNode->totalRatings = 0;
                    newNode->numberOfUsers = 0;
                    newNode->bookings = 0;
                    newNode->capacity = capacity;
                    newNode->averageRating = 0.0;
                    newNode->price = price;
                    newNode->next = *head;
                    *head = newNode;
                }
                saveHotelsToFile(*head);
                break;

            case 2:
                printf("Enter unique code to edit the capacity: ");
                scanf(" %[^\n]", uniqueCode);
                printf("Enter the new capacity: ");
                scanf("%d", &capacity);
                {
                    Node* temp = *head;
                    int updated = 0;
                    while (temp != NULL) {
                        if (strcmp(temp->uniqueCode, uniqueCode) == 0) {
                            temp->capacity = capacity;
                            printf("Capacity updated successfully for hotel '%s'. New Capacity: %d\n", temp->hotelName, temp->capacity);
                            updated = 1;
                            break;
                        }
                        temp = temp->next;
                    }
                    if (!updated) {
                        printf("Hotel with unique code '%s' not found.\n", uniqueCode);
                    } else {
                        saveHotelsToFile(*head);
                    }
                }
                break;

            case 3:
                {
                    Node* temp = *head;
                    if (temp == NULL) {
                        printf("No hotels to display.\n");
                    } else {
                        printf("\nHotel List:\n");
                        while (temp != NULL) {
                            printf("Name: %s, Location: %s, Unique Code: %s, Price: %.2f, Avg Rating: %.2f, Bookings: %d, Capacity: %d\n",
                                   temp->hotelName, temp->location, temp->uniqueCode, temp->price,
                                   temp->averageRating, temp->bookings, temp->capacity);
                            temp = temp->next;
                        }
                    }
                }
                break;

            case 4:
                printf("Exiting Admin Menu.\n");
                return;

            default:
                printf("Invalid choice, please try again.\n");
        }
    }
}

// User function for hotel management
void userHotelManagement(Node* head){
    int choice, rating;
    char name[100];

    while (1) {
        printf("\nUser Menu:\n");
        printf("1. Search Hotel\n2. Book Hotel\n3. Rate Hotel\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter hotel name, location, or unique code to search: ");
                scanf(" %[^\n]", name);
                {
                    Node* temp = head;
                    int found = 0;
                    while (temp != NULL) {
                        if (strcmp(temp->hotelName, name) == 0 || strcmp(temp->location, name) == 0 || strcmp(temp->uniqueCode, name) == 0) {
                            printf("\nHotel Found:\n");
                            printf("Name: %s\nLocation: %s\nUnique Code: %s\nPrice: %.2f\nAverage Rating: %.2f\nBookings: %d\nCapacity: %d\n",
                                   temp->hotelName, temp->location, temp->uniqueCode, temp->price, temp->averageRating, temp->bookings, temp->capacity);
                            found = 1;
                        }
                        temp = temp->next;
                    }
                    if (!found) {
                        printf("No hotel found with the keyword '%s'.\n", name);
                    }
                }
                break;

            case 2:
                printf("Enter hotel name to book: ");
                scanf(" %[^\n]", name);
                {
                    Node* temp = head;
                    int booked = 0;
                    while (temp != NULL) {
                        if (strcmp(temp->hotelName, name) == 0) {
                            if (temp->bookings < temp->capacity) {
                                temp->bookings++;
                                printf("Hotel '%s' successfully booked! Total bookings: %d\n", name, temp->bookings);
                                booked = 1;
                            } else {
                                printf("Hotel '%s' is fully booked! No more capacity available.\n", name);
                            }
                            break;
                        }
                        temp = temp->next;
                    }
                    if (booked) {
                        saveHotelsToFile(head);
                    }
                }
                break;

            case 3:
                printf("Enter hotel name to rate: ");
                scanf(" %[^\n]", name);
                printf("Enter rating (1-5): ");
                scanf("%d", &rating);
                if (rating >= 1 && rating <= 5) {
                    Node* temp = head;
                    while (temp != NULL) {
                        if (strcmp(temp->hotelName, name) == 0) {
                            temp->totalRatings += rating;
                            temp->numberOfUsers++;
                            temp->averageRating = (float)temp->totalRatings / temp->numberOfUsers;
                            printf("Rating added successfully! New Average Rating: %.2f\n", temp->averageRating);
                            saveHotelsToFile(head);
                            break;
                        }
                        temp = temp->next;
                    }
                    printf("Hotel not found.\n");
                } else {
                    printf("Invalid rating. Must be between 1 and 5.\n");
                }
                break;

            case 4:
                printf("Exiting User Menu.\n");
                return;

            default:
                printf("Invalid choice, please try again.\n");
        }
    }
}



// admin & user 

User *head = NULL; // Head of the linked list

// File to store user data
#define FILENAME "userdata.txt"

// Function prototypes
void mainmenuHeading();
void adminMenu();
void userMenu();
void registerUser(const char *role);
void loginUser(const char *role);
void loadUsersFromFile();
void saveUserToFile(User *newUser);
int usernameExists(const char *username);
void adminPanel();
void userPanel();
void viewUsers();
void getPassword(char *password);

// Main menu heading
void mainmenuHeading() {
    system("cls");
    system("color 09");
    printf("\n\n\n");
    printf("\t\t\t\t\t*****************************************\n");
    printf("\t\t\t\t\t|\t\t\t\t\t|\n");
    printf("\t\t\t\t\t|   Welcome To Our Tour Management System |\n");
    printf("\t\t\t\t\t|\t\t\t\t\t|\n");
    printf("\t\t\t\t\t*****************************************\n");
}

// Load users from file
void loadUsersFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return;

    char username[50], password[50], role[10];
    while (fscanf(file, "%s %s %s", username, password, role) != EOF) {
        User *newUser = (User *)malloc(sizeof(User));
        strcpy(newUser->username, username);
        strcpy(newUser->password, password);
        strcpy(newUser->role, role);
        newUser->next = head;
        head = newUser;
    }
    fclose(file);
}

// Save a user to file
void saveUserToFile(User *newUser) {
    FILE *file = fopen(FILENAME, "a");
    if (file) {
        fprintf(file, "%s %s %s\n", newUser->username, newUser->password, newUser->role);
        fclose(file);
    }
}

// Check if a username already exists
int usernameExists(const char *username) {
    User *current = head;
    while (current) {
        if (strcmp(current->username, username) == 0) return 1;
        current = current->next;
    }
    return 0;
}

// Register a new user with a specific role
void registerUser(const char *role) {
    system("cls");
    char username[50], password[50];

    printf("\n\n--- Registration (%s) ---\n", role);
    while (1) {
        printf("Enter a username: ");
        scanf("%s", username);
        if (usernameExists(username)) {
            printf("Username already exists. Try again.\n");
        } else {
            break;
        }
    }

    printf("Enter a password: ");
    getPassword(password);

    User *newUser = (User *)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    strcpy(newUser->role, role);
    newUser->next = head;
    head = newUser;

    saveUserToFile(newUser);
    printf("\n%s registration successful!\n", role);
}

// Login for a specific role
void loginUser(const char *role) {
    system("cls");
    char username[50], password[50];
    printf("\n\n--- %s Log In ---\n", role);
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    getPassword(password);

    User *current = head;
    while (current) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0 && strcmp(current->role, role) == 0) {
            printf("\nLogin successful! Welcome, %s (%s).\n", username, role);
            if (strcmp(role, "admin") == 0) adminPanel();
            return;
        }
        current = current->next;
    }
    printf("\nInvalid username or password for %s. Try again.\n", role);
}

// Admin panel
void adminPanel() {
    int choice;
    while (1) {
        printf("\n--- Admin Panel ---\n");
        printf("1. View Users\n");
        printf("2. Hotel\n");
        printf("3. Flight\n");
        printf("4. Tour Guide\n");
        printf("5. Car Hire\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewUsers();
                break;
            case 2:
                 adminHotelManagement(&head);
                break;
            case 3:
                 manage_flight();  // Admin adds or deletes flight
                break;
            case 4:
               adminFunctions();// tour guider 
                break;
            case 5:
                 manageCarsMenu();// car hire 
                break;
            case 6:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// User panel
void userPanel() {
    int choice;
    while (1) {
        printf("\n--- User Panel ---\n");
        printf("1. Hotel\n");
        printf("2. Flight\n");
        printf("3. Tour Guide\n");
        printf("4. Car Hire\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
             userHotelManagement(head);
                break;
            case 2:
                printf("Flight options...\n");
                break;
            case 3:
                printf("Tour Guide options...\n");
                break;
            case 4:
                printf("Car Hire options...\n");
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// View all users
void viewUsers() {
    User *current = head;
    printf("\n--- Registered Users ---\n");
    if (!current) {
        printf("No users found.\n");
        return;
    }

    while (current) {
        printf("Username: %s, Role: %s\n", current->username, current->role);
        current = current->next;
    }
}

// Mask password input
void getPassword(char *password) {
    char ch;
    int i = 0;
    while ((ch = getch()) != '\r') {
        if (ch == '\b' && i > 0) {
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

// Admin Menu
void adminMenu() {
    int choice;
    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Register as Admin\n");
        printf("2. Login as Admin\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser("admin");
                break;
            case 2:
                loginUser("admin");
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// User Menu
void userMenu() {
    int choice;
    while (1) {
        printf("\n--- User Menu ---\n");
        printf("1. Hotel\n");
        printf("2. Flight\n");
        printf("3. Tour Guide\n");
        printf("4. Car Hire\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
             userHotelManagement(head);
                break;
            case 2:
                printf("Flight options...\n");
                break;
            case 3:
                userFunctions();// tour guider
                break;
            case 4:
                     rentCarsMenu(); // car hire 
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
          
    loadUsersFromFile();
    mainmenuHeading();
    int choice;
    while (1) {
        printf("\n1. Admin Menu\n");
        printf("2. User Menu\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminMenu();
                break;
            case 2:
                userMenu();
                break;
            case 3:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
