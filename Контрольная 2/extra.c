int is_arithmetic_progression(const int *array, int size)
{
    if (size <= 1)
        return 1; 
    int diff = array[1] - array[0];
    for (int i = 2; i < size; i++) {
        if (array[i] - array[i - 1] != diff) {
            return 0;
        }
    }
    return 1;
}