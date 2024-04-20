#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <stdexcept>
class LinearProbing
{
private:
    // create two hash tables for storing country names and their corresponding life expectancy value
    std::vector<std::string> country_names;
    std::vector<std::string> life_expectancy;
    int size = 193;

public:
    LinearProbing()
    {
        // populate both tables with dummy data
        for (int i = 0; i < size; i++)
        {
            country_names.push_back(",");
            life_expectancy.push_back(",");
        }
    }
    int hash_function(std::string country_name)
    {
        int index = 0;
        for (char c : country_name)
        {
            char lower_case = tolower(c);
            index += lower_case;
        }
        return index % size;
    }

    int collision_resolution(int index)
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
            int index = hash_function(country);

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
                int new_index = collision_resolution(index);
                string_lower(country);
                country_names[new_index] = country;
                life_expectancy[new_index] = expectancy;
            }
        }
    }
    std::string find(std::string country_name)
    {
        int index = hash_function(country_name);
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
        for (int i = 0; i < size; i++)
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

class QuadraticProbing
{
private:
    static const int table_size = 389; // Size of the hash table
    std::string country_names[table_size];
    std::string life_expectancy[table_size];

public:
    QuadraticProbing()
    {
        // Initialize arrays with dummy data
        for (int i = 0; i < table_size; i++)
        {
            country_names[i] = ",";
            life_expectancy[i] = ",";
        }
    }

    int hash_function(const std::string &country_name)
    {
        int index = 0;
        for (char c : country_name)
        {
            char lower_case = tolower(c);
            index += lower_case;
        }
        return index % table_size;
    }

    int collision_resolution(int index, int attempt)
    {
        // Quadratic probing: increment attempt by its square
        return (index + attempt * attempt) % table_size;
    }

    void hash_country()
    {
        std::ifstream file("Clean File.csv");
        std::string line;
        while (getline(file, line))
        {
            std::stringstream ss(line);
            std::string token, country, expectancy;
            int column = 0;
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
            int index = hash_function(country);
            int attempt = 0;

            // Collision resolution
            while (country_names[index] != ",")
            {
                attempt++;
                index = collision_resolution(index, attempt);
            }

            // Store data in hash table
            string_lower(country);
            country_names[index] = country;
            life_expectancy[index] = expectancy;
        }
    }

    std::string find(const std::string &country_name)
    {
        int index = hash_function(country_name);
        int attempt = 0;

        // Search for the country
        while (country_names[index] != country_name && country_names[index] != ",")
        {
            attempt++;
            index = collision_resolution(index, attempt);
        }

        if (country_names[index] == country_name)
        {
            return life_expectancy[index];
        }
        else
        {
            return "0"; // Not found
        }
    }

    void print()
    {
        for (int i = 0; i < table_size; i++)
        {
            std::cout << country_names[i] << '\n';
        }
    }

    void string_lower(std::string &country)
    {
        for (char &c : country)
        {
            c = std::tolower(c);
        }
    }
};

class CuckooHash {
private:
    int size; // size of each hash table
    std::vector<std::pair<std::string,std::string>> table1;
    std::vector<std::pair<std::string,std::string>> table2;
    std::vector<std::pair<std::string,std::string>> entries;

public:
    CuckooHash(int tablesize)
    {
        size = tablesize;
        // populate both tables with dummy data
        for (int i = 0; i < size; i++)
        {
            table1.push_back(std::make_pair("",""));
            table2.push_back(std::make_pair("",""));
        }
    }


    size_t hash1(const std::string& key) const {
        // Simple hash function based on the sum of ASCII values of characters in the key
        size_t hashValue = 0;
        for (char c : key) {
            hashValue += c;
        }
        return hashValue;
    }
    // DJBX33A Hash: This is a simple and fast hash function that can be used for strings.
    size_t hash2(const std::string& key) const {
        size_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash;
    }

    void insert(const std::string& key, const std::string& value) {
        size_t index1 = hash1(key)%size;
        size_t index2 = hash2(key)%size;
        if (table1[index1].first=="") {
            table1[index1] = std::make_pair(key, value);
        } else if (table2[index2].first=="") {
            table2[index2] = std::make_pair(key, value);
        } else {
            // If both slots are occupied, perform cuckoo hashing
            std::pair<std::string,std::string> temp = std::make_pair(key, value);
            for (int i = 0; i < size; ++i) {
                // Swap temp with the entry at hash1
                std::swap(temp, table1[index1]);
                // Recalculate hash value for temp
                index1 = hash1(temp.first) % size;
                // Check if slot at hash1 is available
                if (table1[index1].first=="") {
                    table1[index1] = temp;
                    break;
                }
                // Swap temp with the entry at hash2
                std::swap(temp, table2[index2]);
                // Recalculate hash value for temp
                index2 = hash2(temp.first) % size;
                // Check if slot at hash2 is available
                if (table2[index2].first=="") {
                    table2[index2] = temp;
                    break;
                }
            }
        }
    }

    void hash_country() {
        std::ifstream file("Clean File.csv");
        std::string line;
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string token, country, expectancy;
            int column = 0;
            while (getline(ss, token, ',')) {
                if (column == 0) {
                    country = token;
                } else if (column == 3) {
                    expectancy = token;
                }
                column++;
            }
            insert(country, expectancy);
        }
    }

    std::string search(const std::string& key) const {
        size_t index1 = hash1(key)%size;
        size_t index2 = hash2(key)%size;
        if (table1[index1].first == key) {
            return table1[index1].second;

        } else if (table2[index2].first == key) {
            return table2[index2].second;
        }

        return "-";
    }

    void string_lower(std::string &country)
    {
        for (char &c : country)
        {
            c = std::tolower(c);
        }
    }

    void print()
    {
        for (int i = 0; i < size; i++)
        {
            std::cout << table1[i].first << table1[i].second << '\n';
        }
        std::cout << "TABLE2" << '\n';
        for (int i = 0; i < size; i++)
        {
            std::cout << table2[i].first << table2[i].second << '\n';
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

    auto start = std::chrono::steady_clock::now();
    lp.string_lower(country);
    std::string return_value = lp.find(country);
    auto end = std::chrono::steady_clock::now();
    auto duration_linear = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    if (return_value == "0")
    {
        std::cout << "Invalid Country Name" << '\n';
    }
    else
    {
        std::cout << "Life Expectancy Value (Linear Probing): " << return_value << '\n';
    }
    std::cout << "Linear Probing time: " << duration_linear.count() << " nanoseconds" << std::endl;

    QuadraticProbing qp;
    qp.hash_country();
    start = std::chrono::steady_clock::now();
    qp.string_lower(country);
    return_value = qp.find(country);
    end = std::chrono::steady_clock::now();
    auto duration_quadratic = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    if (return_value == "0")
    {
        std::cout << "Invalid Country Name" << '\n';
    }
    else
    {
        std::cout << "Life Expectancy Value (Quadratic Probing): " << return_value << '\n';
    }
    std::cout << "Quadratic Probing time: " << duration_quadratic.count() << " nanoseconds" << std::endl;

    CuckooHash ch(193);
    ch.hash_country();
    start = std::chrono::steady_clock::now();
    std::string life_expectancy;
    life_expectancy = ch.search(country);
    end = std::chrono::steady_clock::now();
    auto duration_cuckoo = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    if (return_value!= "-")
    {
        std::cout << "Life Expectancy Value (Cuckoo Hashing): " << return_value << '\n';
    }

    std::cout << "Cuckoo Hashing time: " << duration_cuckoo.count() << " nanoseconds" << std::endl;
}
