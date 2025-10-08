#ifndef EXAMPLE_H
#define EXAMPLE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Add two integers
     */
    int add(int a, int b);

    /**
     * Multiply two integers
     */
    int multiply(int a, int b);

    /**
     * Print a greeting message
     */
    void print_hello(const char *name);

#ifdef __cplusplus
}
#endif

#endif // EXAMPLE_H
