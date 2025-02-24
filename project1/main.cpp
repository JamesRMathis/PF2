#include <iostream>

// Count from 0 to n-1
void count(int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << i << " ah-ah-ah!" << std::endl;
    }
}

// Entry point
int main(int argc, char** argv)
{
    // Print the arguments
    if (argc > 1)
    {
        std::cout << "The arguments passed in were:" << std::endl;
        for (int i = 1; i < argc; i++)
        {
            std::cout << "arg " << i << " = " << argv[i] << std::endl;
        }
    }

    // Ask the user to enter a name
    std::cout << "Hello, what is your name?" << std::endl;
    std::cout << "> ";
    std::string name;
    getline(std::cin, name);

    // Ask the user to enter a number
    std::cout << "And what is your favorite number?" << std::endl;
    std::cout << "> ";
    std::string num;
    getline(std::cin, num);
    int n = stoi(num);

    // Greet the user
    std::cout << "Ok, " << name << ", I will count to " << n << " (with zero-indexed values):" << std::endl;

    // Count to the favorite number
    count(n);

    // Bid farewell
    std::cout << "Thanks for stopping by. Have a nice day!" << std::endl;
}