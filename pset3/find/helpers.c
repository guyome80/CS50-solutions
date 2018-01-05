/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Swap function used to swap two elements of an array.
 */
void Swap(int tab[], int a, int b);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // Search the value integer among the n values of the values[] array.
    int i;
    for (i = 0; i < n; i++)
    {
        if (values[i] == value)
        {
            return true;
        }
    }
    return false;
}


/**
 * Description of the swap function of two integers in an array with temporary int.
 */
void Swap(int tab[], int a, int b)
{
    int temp = tab[a];
    tab[a] = tab[b];
    tab[b] = temp;
    return;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // use an selection sort algorithm O(n^2).
    int min = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            if (values[j] < values[min])
            {
                //swap minimum array number
                Swap(values, min, j);
            }
        }
    }
    return;
}