#ifndef MODE_CALCULATOR_H
#define MODE_CALCULATOR_H

#include <algorithm> // Include the algorithm library for sorting

// Function to calculate the mode of a sorted array
int calculateMode(int array[], int size) {
    // Sort the array
    std::sort(array, array + size);

    // Now proceed with the mode calculation
    int modeCt = 0;
    int modeV = -1;

    int ct = 0;
    int v = -1;

    for(int i = 0; i < size; i++) {
        if(array[i] != v) {
            v = array[i];
            ct = 0;
        }

        ct ++;

        if(ct > modeCt) {
            modeCt = ct;
            modeV = v;
        }
    }
    Serial.println("Array values:");
    for (int i = 0; i < size; i++) {
        Serial.println(array[i]);
    }
    return modeV; // Return the mode
}

#endif
