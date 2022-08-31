/**
 * @file MergeSort.cpp
 * @author Matthew Hays
 * @brief Converts a file input (new line seperated values) to an 
 * integer array and calls merge() to sort the values using a three-way merge sort. 
 * 
 * Usage: ./a.out <input_file_path> <output_file_path>
 * or
 * Usage: Usage: ./a.out <input_file_path>
 * 
 * Output will default to root directory sorted.txt if no output_file_path is supplied
 * @version 0.1
 * @date Created: 2022-05-09
 * @date Due: 2022-05-13
 * @date Last Modified: 2022-05-13
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Perform a 3 way merge.
void merge(int givenArray[], int startLeft, int startMid,
           int startRight, int end, int duplicateArray[]);

// Recursively divide an array by three until of size one.
void mergeSort(int duplicateArray[], int start, int end, int givenArray[]);

/**
 * @brief Converts a file input (new line seperated values) to an integer array and calls merge()
 * to sort the values using a three-way merge sort.
 * 
 * @param argc int - The length of the CLI input params.
 * @param argv char *[]- An array containing all CLI input params.
 * @return int - A 0 success code - non-zero indicates failure.
 */
int main(int argc, char *argv[])
{
    ifstream inputFile;
    ofstream outputFile;
    string fileLine, stringArray;
    char *inputFileName, *outputFileName;

    // Check for required inputs
    if (argc >= 2)
    {
        inputFileName = argv[1];
        if (argc == 3)
            outputFileName = argv[2];
    }
    else
    {
        cout << "Usage: <./a.out> <input_file> <output_file>\n";
        return 1;
    }

    // Error handling on open input file.
    try
    {
        // Open file
        inputFile.open(inputFileName);
        // Read the file inputs to a CSV formatted string.
        int loop = 0;
        if (inputFile.is_open())
        {
            // Loop O(n), where n is # of inputs in file.
            while (getline(inputFile, fileLine))
            {
                if (loop == 0)
                    stringArray += fileLine;

                else
                    stringArray += "," + fileLine;

                loop++;
            }
        }
    }
    catch (ifstream::failure &e)
    {
        cout << "Exception opening file. " << &e << "\n";
    }

    // Close the inputFile
    inputFile.close();

    // Temp structures to help convert our string to an array
    stringstream stringStream(stringArray);
    vector<string> tempVector;

    // Extract values from the CSV string and load the integer to the temp vector
    // Loop O(n), where n is # of inputs in file
    while (stringStream.good())
    {
        string arrayValue;
        getline(stringStream, arrayValue, ',');
        // Remove new lines, blank spaces, etc.
        if (arrayValue != "")
            tempVector.push_back(arrayValue);
    }

    // Convert from temporary vector to an array
    // Loop O(n), where n is # of inputs in file
    int tempVectorSize = tempVector.size(),
        givenArray[tempVectorSize];
    cout << tempVectorSize << "\n";
    for (int i = 0; i < tempVectorSize; i++)
        givenArray[i] = stoi(tempVector[i]);

    // Ensure the correct size is passed to our mergeSort function
    int arraySize = sizeof(givenArray) / sizeof(*givenArray);

    // Print an "initial" output
    // Loop O(n), where n is # of inputs in file
    cout << "Before 3 way merge sort: \n";
    for (int i = 0; i < arraySize; i++)
        cout << givenArray[i] << " ";
    cout << "\n";

    // Create a duplicate array to help us with sorting
    int duplicateArray[arraySize];

    // Copy the given array to the duplicate array
    // Loop O(n), where n is # of inputs in file
    for (int i = 0; i < arraySize; i++)
        duplicateArray[i] = givenArray[i];

    // sort function
    mergeSort(duplicateArray, 0, arraySize, givenArray);

    // Copy the temporary array to the original array
    // Loop O(n), where n is # of inputs in file
    for (int i = 0; i < arraySize; i++)
        givenArray[i] = duplicateArray[i];

    // Print our "after sort" output and write to an output file in new line seperated format
    // Loop O(n), where n is # of inputs in file
    cout << "\nAfter 3 way merge sort: \n";
    try
    {
        outputFile.open(outputFileName);
        if (outputFile.is_open())
        {
            for (int i = 0; i < arraySize; i++)
            {
                cout << givenArray[i] << " ";
                outputFile << givenArray[i] << "\n";
            }
            cout << "\n";
        }
        else
        {
            cout << "No output file specified. Defaulting to \"sorted.txt\"\n";
            outputFile.open("sorted.txt");
            if (outputFile.is_open())
            {
                for (int i = 0; i < arraySize; i++)
                {
                    cout << givenArray[i] << " ";
                    outputFile << givenArray[i] << "\n";
                }
                cout << "\n";
            }
            else
            {
                cout << "Error opening/writing to output file.\n";
                return 1;
            }
        }
    }
    catch (ofstream::failure &e)
    {
        cout << "Error accessing/writing to output file. " << &e << "\n";
        return 1;
    }

    outputFile.close();

    return 0;
}

/**
 * @brief Perform a 3 Way Division of a given array. O(log3(n))
 * 
 * @param duplicateArray - A copy of the given array
 * @param start - The initial index
 * @param end - The ending index
 * @param givenArray - The original given array
 */
void mergeSort(int duplicateArray[], int start,
               int end, int givenArray[])
{
    // Base case: Array length = 0 or 1
    if (end - start <= 1)
        return;

    // Calculate array starting index - (left === 0)
    int startMid = start + ((end - start) / 3);
    int startRight = start + 2 * ((end - start) / 3) + 1;

    // Sorting 3 arrays recursively
    // Recursive calls, O(log3(n)) where n is the # of inputs in file
    mergeSort(givenArray, start, startMid, duplicateArray);
    mergeSort(givenArray, startMid, startRight, duplicateArray);
    mergeSort(givenArray, startRight, end, duplicateArray);

    // Perform a 3 way merge
    // Each call results in O(n), where n is # of inputs in file
    merge(givenArray, start, startMid, startRight, end, duplicateArray);
}

/**
 * @brief Performs a 3 way merge. 
 * 
 * @param givenArray - The initial user supplied array
 * @param startLeft - The first index of the left array
 * @param startMid - The first index of the middile array
 * @param startRight - The first index of the right array
 * @param end - The last index
 * @param duplicateArray - A copy of the initial array
 */
void merge(int givenArray[], int startLeft, int startMid,
           int startRight, int end, int duplicateArray[])
{
    // Set inital index values
    int leftIdx = startLeft,
        midIdx = startMid,
        rightIdx = startRight,
        duplicateIdx = startLeft;

    // Push results into the temporary array (smallest value first)
    while (leftIdx < startMid && midIdx < startRight && rightIdx < end)
    {
        if (givenArray[leftIdx] < givenArray[midIdx])
        {
            if (givenArray[leftIdx] < givenArray[rightIdx])
                duplicateArray[duplicateIdx++] = givenArray[leftIdx++];

            else
                duplicateArray[duplicateIdx++] = givenArray[rightIdx++];
        }

        else
        {
            if (givenArray[midIdx] < givenArray[rightIdx])
                duplicateArray[duplicateIdx++] = givenArray[midIdx++];

            else
                duplicateArray[duplicateIdx++] = givenArray[rightIdx++];
        }
    }

    // Sort the remaining values in the first and second array.
    while ((leftIdx < startMid) && (midIdx < startRight))
    {
        if (givenArray[leftIdx] < givenArray[midIdx])
            duplicateArray[duplicateIdx++] = givenArray[leftIdx++];

        else
            duplicateArray[duplicateIdx++] = givenArray[midIdx++];
    }

    // Sort the remaining values in the second and third array.
    while ((midIdx < startRight) && (rightIdx < end))
    {
        if (givenArray[midIdx] < givenArray[rightIdx])
            duplicateArray[duplicateIdx++] = givenArray[midIdx++];

        else
            duplicateArray[duplicateIdx++] = givenArray[rightIdx++];
    }

    // Sort the remaining values in the first and third array.
    while ((leftIdx < startMid) && (rightIdx < end))
    {
        if (givenArray[leftIdx] < givenArray[rightIdx])
            duplicateArray[duplicateIdx++] = givenArray[leftIdx++];

        else
            duplicateArray[duplicateIdx++] = givenArray[rightIdx++];
    }

    // Copy remaining values
    while (leftIdx < startMid)
        duplicateArray[duplicateIdx++] = givenArray[leftIdx++];
    while (midIdx < startRight)
        duplicateArray[duplicateIdx++] = givenArray[midIdx++];
    while (rightIdx < end)
        duplicateArray[duplicateIdx++] = givenArray[rightIdx++];
}