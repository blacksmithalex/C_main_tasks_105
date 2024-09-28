#include <stdio.h>
#include <math.h>

int main() {
    double a, b, c;
    
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        return 2; 
    }

    if (a == 0) {
        return 2; 
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return 1; 
    }

    if (discriminant == 0) {
        double x = -b / (2 * a);
        printf("%.10g\n", x); 
    } else {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        
        printf("%.10g %.10g\n", x2, x1);
    }

    return 0; 
}