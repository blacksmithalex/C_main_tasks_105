#include <stdio.h>

#define INVALID_ARGUMENT 1
#define CANT_OPEN_INPUT_FILE 2
#define INVALID_INPUT 3
#define CANT_OPEN_OUTPUT_FILE 4
#define EMPTY_SEQUENCE 5

struct segment {
    FILE *input;
    int max_sum;
    int max_count;
    int current_sum;
    int current_count;
    int previous;
};

int process_element(struct segment *state);

int process_element(struct segment *state)
{
    int element;
    int result = fscanf(state->input, "%d", &element);
    if (result == 1) {
        if (state->current_count == 0 || element == state->previous) {
            state->current_sum += element;
            state->current_count += 1;
        } else {
            if (state->current_count > 0 &&
                (state->current_sum > state->max_sum ||
                 state->max_count == 0)) {
                state->max_sum = state->current_sum;
                state->max_count = state->current_count;
            }
            state->current_sum = element;
            state->current_count = 1;
        }
        state->previous = element;
    }
    return result;
}

int main(int argc, char **argv)
{
    struct segment main_state;
    FILE *output;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <output-file> <input-files...>\n", argv[0]);
        return INVALID_ARGUMENT;
    }

    output = fopen(argv[1], "w");
    if (!output) {
        fprintf(stderr, "Unable to open output file: %s\n", argv[1]);
        return CANT_OPEN_OUTPUT_FILE;
    }

    main_state.max_sum = 0;
    main_state.max_count = 0;
    main_state.current_sum = 0;
    main_state.current_count = 0;
    main_state.previous = 0;

    for (int i = 2; i < argc; i++) {
        int result;
        main_state.input = fopen(argv[i], "r");
        if (!main_state.input) {
            fprintf(stderr, "Unable to open input file: %s\n", argv[i]);
            fclose(output);
            return CANT_OPEN_INPUT_FILE;
        }

        while (1) {
            result = process_element(&main_state);
            if (result != 1) {
                break;
            }
        }

        if (result != EOF) {
            fprintf(stderr, "Invalid data in file: %s\n", argv[i]);
            fclose(main_state.input);
            fclose(output);
            return INVALID_INPUT;
        }

        fclose(main_state.input);
    }

    if (main_state.current_count > 0 &&
        (main_state.current_sum > main_state.max_sum ||
         main_state.max_count == 0)) {
        main_state.max_sum = main_state.current_sum;
        main_state.max_count = main_state.current_count;
    }

    if (main_state.max_count > 0) {
        fprintf(output, "%d\n", main_state.max_count);
    } else {
        fclose(output);
        return EMPTY_SEQUENCE;
    }

    fclose(output);
    return 0;
}
