// A first C++ program
// Standard libraries that we include.
#include <iostream>
#include <string>

// Specially named function that is the entry point into our program.
int main() {
  // ========================== for - loops ====================
  // Allocate an array on the stack.
  // This array cannot be referenced outside the main function.
  int myArray[10];

  // A loop that pre-increments 'i' by one each iteration of the
  // loop.
  for (int i = 0; i < 10; ++i) {
    // If we are modifying values , then use a regular for-loop
    myArray[i] = i;
    // cout is also part of the standard library, and prints
    // text to standard output (i.e. typically our terminal window)
    std::cout << i << " ";
  }

  // Here we create a string type. It is part of the
  // standard library namespace, which is why it begins with
  // std::
  std::string s = "hello";

  // Here is a different kind of loop.
  for (char c : s) {
    // Range-based for loops are useful if we are not modifying any values.
    // Ranged-based for loops were introduced in C++11 and are a new popular
    // feature in many other programming languages, and now C++
    std::cout << c;
  }

  // More information can be found here:
  // http://en.cppreference.com/w/cpp/language/range-for
}