/**
 * @file Karatsuba.cpp
 * @author Matthew Hays
 * @brief Implementation of the Karatsuba Multiplication Algorithm usings strings
 * to allow for incorporation of extremely large numbers too large for typical computer methods.
 *
 * Usage: $ ./a.out <file_path> | <num_1> <num_2>
 * @date 2022-05-20
 *
 */

#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>

using namespace std;

/**
 * @brief Recursively split strings until base case of one input of size 1.
 *
 * @return String representation of either: (1) basic multiplication (base case) or (2) Karatsuba multiplication.
 */
string karatsuba(string, string);

/**
 * @brief Performs addition on two strings. Assumes string representation of an integer.
 *
 * @return String representation of the result of addition.
 */
string add_strings(string, string);

/**
 * @brief Performs subtraction on two strings. Assumes string representation of an ineger.
 *
 * @return String representation of the result of subtraction.
 */
string subtract_strings(string, string);

/**
 * @brief Primary program driver.
 *
 * Time Complexity: n is the input size of the largest input string
 * Convert from input to string: O(n + m)
 * Pad Leading Zeros: O(n)
 * 3 Recursive Karatsuba calls: 3T(n/2)
 * On each recursive call--
 * 4 calls to add_strings(): 4 * O(2n) => O(8n)
 * 2 calls to subtract_Strings(): 2 * O(2n) => O(4n)
 *
 * Overall: 3T(n/2) + O(14n + m) => Theta(n^1.58)
 *
 * @param argc Number of CLI provided arguments.
 * @param argv CLI arguments. Only the first two arguments are examined.
 * @return Integer success code. Non-zero represents an error.
 */
int main(int argc, char *argv[])
{
    string num1,
        num2,
        file_path,
        file_line,
        num_array[2];

    ifstream input_stream;

    // Gather inputs based on CLI input length.
    // Can be extended to check for regular expressions (specific inputs) - not implemented.
    // Get numbers from CLI.
    if (argc == 3)
    {
        num1 = argv[1];
        num2 = argv[2];
    }
    // Get numbers from an input file. *Only takes the first two lines as numbers.
    else if (argc == 2)
    {
        file_path = argv[1];
        input_stream.open(file_path);
        int loop = 0;

        if (input_stream.is_open())
        {
            while (getline(input_stream, file_line) && loop < 2)
            {
                num_array[loop] = file_line;
                loop++;
            }

            num1 = num_array[0];
            num2 = num_array[1];
            input_stream.close();
        }
    }
    // If no acceptable input, print a usage statement and exit.
    else
    {
        cout << "Usage is: $ ./a.out (<num1> <num2>) | <input_path>\n";
        return 1;
    }

    // Pad numbers with leading zeros to make the inputs the same size
    while (num1.length() != num2.length())
    {
        if (num1.length() > num2.length())
        {
            num2.insert(0, num1.length() - num2.length(), '0');
        }
        if (num2.length() > num1.length())
        {
            num1.insert(0, num2.length() - num1.length(), '0');
        }
    }

    // Display the numbers accepted as inputs for easy copy + paste verification
    // *I used Wolframalpha to verify.
    cout << "Num 1: " + num1 + "\n";
    cout << "Num 2: " + num2 + "\n";

    // Initial Call into the algorithm - store the resultant string.
    string solution = karatsuba(num1, num2);

    // Strip the leading zeros prior to display
    while (solution.at(0) == '0')
    {
        solution = solution.substr(1);
    }

    // Display the solution
    cout << "Solution:\n" + solution + "\n\n";

    // Success!
    return 0;
}

/**
 * @brief Recursively split a string representation of an integer.
 * Convert to an integer, perform multiplication, and return a string representation of the product on the base case.
 * Base case of one input string's length smaller than 2.
 * Conversion to an integer was done to simplify the case of karatsuba("5", "16") where the result is 80.
 * String multiplication could be done here as well, but was not implemented for simplicity.
 *
 * Time complexity:
 * 3 Recursive Calls with half input size each call: 3T(n/2)
 *
 * @param num1 String representation of the first integer.
 * @param num2 String representation of the second integer.
 * @return String representation of either: (1) Result of base integer multiplcation (base case). (2) Result of karatsuba multiplication.
 */
string karatsuba(string num1, string num2)
{
    // If either number is 1 digit, we have reached the base case.
    // Perform basic multiplication and return the result as a string.
    // Conversion to an integer was performed for the case of k("10", "5"), etc. and simplicity.
    // Changes could implement a string based approach.
    if (num1.length() < 2 || num2.length() < 2)
    {
        return to_string(stoi(num1) * stoi(num2));
    }

    // How many digits does the largest number have?
    int max_digits = max(num1.length(), num2.length());

    // Determine max_digits (n) / 2 floored
    int half_digits = max_digits / 2;

    // To split our number, we will determine our division point from the reverse
    int last_idx_num1 = num1.length(),
        last_idx_num2 = num2.length();

    // Split the string into 4 parts; a, b, c, d
    string a = num1.substr(0, last_idx_num1 - half_digits);
    string b = num1.substr(last_idx_num1 - half_digits);
    string c = num2.substr(0, last_idx_num2 - half_digits);
    string d = num2.substr(last_idx_num2 - half_digits);

    // Recursively determine ac, bd, and ad + bc.
    // To get ad + bc:
    // ad + bc + ac + bd =>  ad + bc + ac + bd - ac - bd => ad + bc
    string ac = karatsuba(a, c);
    string bd = karatsuba(b, d);
    string ad_plus_bc = subtract_strings(
        subtract_strings(
            karatsuba(
                add_strings(a, b), add_strings(c, d)),
            ac),
        bd);

    // Calculate and return ac * 10 ^ (2 * n / 2) + (ad + bc) * 10 ^ (n / 2) + bd. This is all completed in string format.
    return add_strings(
        add_strings(
            ac.insert(ac.length(), 2 * half_digits, '0'), ad_plus_bc.insert(ad_plus_bc.length(), half_digits, '0')),
        bd);
}

/**
 * @brief Performs addition between two strings.
 *
 * Time Complexity:
 * Perform straightline addition. O(n) where n is the size of the largest input string.
 * Reverse the output string. O(n + 1) where n is the size of the largest input string.
 *
 * Overall: O(2n + 1) => O(2n)
 *
 * @param num1 String representation of the first integer.
 * @param num2 String representation of the second integer.
 * @return String representation of the result of addition.
 */
string add_strings(string num1, string num2)
{
    // We'll add in reverse and track the carry
    int idx1 = num1.length() - 1,
        idx2 = num2.length() - 1,
        carry = 0;

    string new_string;

    while (idx1 >= 0 || idx2 >= 0)
    {
        // Include the carry.
        int sum = carry;

        // If there's a number in the first number, add it to sum.
        if (idx1 >= 0)
        {
            sum += num1.at(idx1) - '0';
            idx1--;
        }
        // If there's a number in the second number, add it to sum.
        if (idx2 >= 0)
        {
            sum += num2.at(idx2) - '0';
            idx2--;
        }

        // We only want the first digit to be stored in our result: % 10
        new_string.append(to_string(sum % 10));

        // Keep track of the carry for the next iteration.
        carry = sum / 10;
    }

    // If we're out of digits in either number, but still have a carry, add it to the string.
    if (carry != 0)
    {
        new_string.append(to_string(carry));
        carry--;
    }

    // Since we built our string backward, we reverse it before output.
    reverse(new_string.begin(), new_string.end());

    // Return the result of addition.
    return new_string;
}

/**
 * @brief Perform subtraction between two strings.
 * Assumption: num1 > num2; no error handling performed. Error checking can be implmented from the start of each string,
 * ensuring that each digit is > or = the digit in the same position within the second string; returning when the > condition is met.
 *
 * Time Complexity:
 * Perform straightline subtraction. O(n) where n is the size of the largest input string.
 * Reverse the output string. O(n) where n is the size of the largest input string.
 *
 * Overall: O(2n)
 *
 * @param num1 String representation of the first integer (largest number).
 * @param num2 String representation of the second integer (smallest number).
 * @return String representation of the result of subtraction.
 */
string subtract_strings(string num1, string num2)
{
    string new_string;

    // Subtract in reverse and track the carry
    int idx1 = num1.length() - 1,
        idx2 = num2.length() - 1,
        carry = 0;

    // We build our result in reverse
    while (idx1 >= 0 && idx2 >= 0)
    {
        // Include the carry.
        int difference = carry;

        // Calculate the difference between the digits. num1 > num2
        difference = (num1.at(idx1) - '0') - (num2.at(idx2) - '0') - carry;
        idx1--;
        idx2--;

        // Reset the carry.
        carry = 0;

        // If the difference results in a negative number.
        if (difference < 0)
        {
            // Reset to the absolute value.
            difference += 10;
            // Track the carry.
            carry = 1;
        }

        // Append the first digit to our result: % 10
        new_string.append(to_string(difference % 10));
    }

    // Since num1 > num2 assumption:
    // If num1 still has digits remaining
    while (idx1 >= 0)
    {
        // Include any remaining carry.
        int difference = (num1.at(idx1) - '0') - carry;
        idx1--;

        // Reset the carry.
        carry = 0;

        // Possible if carry and num1 next digit is 0. Track the carry.
        if (difference < 0)
        {
            difference += 10;
            carry = 1;
        }

        // NOTE: Running out of carry results in an infinite loop and error.
        // However, we only want the first digit of the result.
        new_string.append(to_string(difference % 10));
    }

    // Since we built our string backward, we reverse it before output.
    reverse(new_string.begin(), new_string.end());

    // Return the results of the difference.
    return new_string;
}