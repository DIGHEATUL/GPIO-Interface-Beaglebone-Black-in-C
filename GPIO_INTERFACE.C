#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define GPIO_PATH "/sys/class/gpio"
#define P9_12 "60"   // Change this to the GPIO pin you want to control


int is_gpio_exported(const char *gpio_pin) {
    char path[255];
    sprintf(path, GPIO_PATH "/gpio%s", gpio_pin);

    // Check if the directory for the GPIO pin exists
    return access(path, F_OK) != -1;
}


// Function to export a GPIO pin if not already exported
void export_gpio(const char *gpio_pin) {
    if (!is_gpio_exported(gpio_pin)) {
        FILE *export_file = fopen(GPIO_PATH "/export", "w");
        if (export_file == NULL) {
            perror("Error exporting GPIO");
            exit(EXIT_FAILURE);
        }

        fprintf(export_file, "%s", gpio_pin);
        fclose(export_file);
    }
}

// Function to set the direction of a GPIO pin
void set_gpio_direction(const char *gpio_pin, const char *direction) {
    char path[255];
    sprintf(path, GPIO_PATH "/gpio%s/direction", gpio_pin);

    FILE *direction_file = fopen(path, "w");
    if (direction_file == NULL) {
        perror("Error setting GPIO direction");
        exit(EXIT_FAILURE);
    }

    fprintf(direction_file, "%s", direction);
    fclose(direction_file);
}

// Function to set a GPIO pin high
void set_gpio_high(const char *gpio_pin) {
    char path[255];
    sprintf(path, GPIO_PATH "/gpio%s/value", gpio_pin);

    FILE *value_file = fopen(path, "w");
    if (value_file == NULL) {
        perror("Error setting GPIO high");
        exit(EXIT_FAILURE);
    }

    fprintf(value_file, "1");
    fclose(value_file);
}

// Function to set a GPIO pin low
void set_gpio_low(const char *gpio_pin) {
    char path[255];
    sprintf(path, GPIO_PATH "/gpio%s/value", gpio_pin);

    FILE *value_file = fopen(path, "w");
    if (value_file == NULL) {
        perror("Error setting GPIO low");
        exit(EXIT_FAILURE);
    }

    fprintf(value_file, "0");
    fclose(value_file);
}

// Function to unexport a GPIO pin
void unexport_gpio(const char *gpio_pin) {
    FILE *unexport_file = fopen(GPIO_PATH "/unexport", "w");
    if (unexport_file == NULL) {
        perror("Error unexporting GPIO");
        exit(EXIT_FAILURE);
    }

    fprintf(unexport_file, "%s", gpio_pin);
    fclose(unexport_file);
}

// Function to read the value of a GPIO pin
int read_gpio_value(const char *gpio_pin) {
    char path[255];
    sprintf(path, GPIO_PATH "/gpio%s/value", gpio_pin);

    FILE *value_file = fopen(path, "r");
    if (value_file == NULL) {
        perror("Error reading GPIO value");
        exit(EXIT_FAILURE);
    }

    int value;
    fscanf(value_file, "%d", &value);
    fclose(value_file);

    return value;
}


int main() {

    // Export the GPIO pin
    export_gpio(P9_12);

    // Set the GPIO direction to out
    set_gpio_direction(P9_12, "out");

 
    while (1) {
 
        // Set GPIO low
        set_gpio_high(P9_12);
        usleep(500000); // Sleep for 100ms (adjust as needed)
        
        printf("GPIO Value high\n");
        usleep(1000000);
        
        set_gpio_low(P9_12);
        usleep(500000); // Sleep for 100ms (adjust as needed)
        
        printf("GPIO Value low\n");
        usleep(1000000);
     
    }

// Unexport the GPIO pin
    unexport_gpio(P9_12);
    
    return 0;
}


