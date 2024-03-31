#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class LinearProbing
{
private:
    // create two hash tables for storing country names and their corresponding life expectancy value
    // 389 size chosen because this is the next prime number after doubling the size of dataset
    std::string country_names[389];
    std::string life_expectancy[389];
public:
    LinearProbing()
    {
        // populate both tables with dummy data
        for (int i = 0; i < 389; i++)
        {
            country_names[i] = ",";
            life_expectancy[i] = ",";
        }
    }
    int hash_function(std::string country_name, int size)
    {
        int index = 0;
        for (char c : country_name)
        {
            char lower_case = tolower(c);
            index += lower_case;
        }
        return index % size;
    }

    int collision_resolution(int index, int size)
    {
        while (country_names[index] != ",")
        {
            index = (index + 1) % size;
        }
        return index;
    }

    void hash_country()
    {
        std::ifstream file3("Clean File.csv");
        std::string line;
        // loop over file line by line
        while(getline(file3, line))
        {
            std::stringstream ss(line);
            std::string token, country, expectancy;
            int column = 0;
            // loop over file column by column
            while (getline(ss, token, ','))
            {
                if (column == 0)
                {
                    country = token;
                }
                else if (column == 3)
                {
                    expectancy = token;
                }
                column++;
            }
            int index = hash_function(country, 389);

            // check if index that a country hashes to is empty
            if (country_names[index] == ",")
            {
                string_lower(country);
                country_names[index] = country;
                life_expectancy[index] = expectancy;
            }

            // index is not empty, which means there is a collision
            else
            {
                std::cout << "collision" << '\n';
                int new_index = collision_resolution(index, 389);
                string_lower(country);
                country_names[new_index] = country;
                life_expectancy[new_index] = expectancy;
            }
        }
    }
    std::string find(std::string country_name, int size)
    {
        int index = hash_function(country_name, size);
        if (country_names[index] == country_name)
        {
            return life_expectancy[index];
        }
        else
        {
            int start = index;
            int new_index = index + 1;
            while (new_index != start)
            {
                if (country_names[new_index] == country_name)
                {
                    return life_expectancy[new_index];

                }
                new_index = (new_index + 1) % size;
            }
            return "0";
        }
    }
    void print()
    {
        for (int i = 0; i < 389; i++)
        {
            std::cout << country_names[i] << '\n';
        }
    }
    void string_lower(std::string &country)
    {
        for (char& c : country)
        {
            c = std::tolower(c);
        }
    }
};

int main()
{
    // put relevant data in a new file
    std::ifstream file("Life Expectancy Data.csv");
    std::ofstream file2 ("Clean File.csv");
    std::string current_line, next_line;

    if (getline(file, current_line))
    {
        while (getline(file, next_line))
        {
            std::stringstream current_ss(current_line);
            std::string current_country;
            getline(current_ss, current_country, ',');

            std::stringstream next_ss(next_line);
            std::string next_country;
            getline(next_ss, next_country, ',');

            if (current_country != next_country)
            {
                file2 << next_line << '\n';
            }
            current_line = next_line;
        }
    }

    LinearProbing lp;
    lp.hash_country();
    std::string country;
    std::cout << "Enter Country Name: ";
    std::cin >> country;
    lp.string_lower(country);
    std::string return_value = lp.find(country, 389);

    if (return_value == "0")
    {
        std::cout << "Invalid Country Name" << '\n';
    }
    else
    {
        std::cout << return_value;
    }
    // lp.print();
    
}