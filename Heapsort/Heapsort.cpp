// Doğa Güneş Karadağ
// 150200104
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
using namespace std::chrono;
using namespace std;

using namespace std;

struct populations
{
    string city;
    int population;
};

vector<populations> read_file(const string &filename) // reading csv files
{
    vector<populations> data;
    ifstream file(filename);
    string line, city;
    int population;
    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, city, ';');
        ss >> population;
        data.push_back({city, population});
        // getting city and population
    }
    return data;
}

void write_file(const vector<populations> &P, const string &filename)
{
    ofstream file(filename);
    for (const auto &data : P)
    {
        file << data.city << ";" << data.population << "\n";
    }
}

void swap_elements(vector<populations> &P, int first, int second)
{
    populations temp = P[first];
    P[first] = P[second];
    P[second] = temp;
}

void max_heapify(vector<populations> &arr, int i, int size)
{
    // O(logn) time complexity
    int left_node = 2 * i + 1;
    int right_node = 2 * i + 2;
    int largest = i;

    if (left_node < size && arr[left_node].population > arr[largest].population)
    {
        largest = left_node;
    }

    if (right_node < size && arr[right_node].population > arr[largest].population)
    {
        largest = right_node;
    }

    if (largest != i)
    {

        swap_elements(arr, i, largest); // swapping arr[i] and arr[largest]

        max_heapify(arr, largest, size); // recursively heapifying the sub-tree
    }
}

void build_max_heap(vector<populations> &arr)
{
    // O(n) time complexity
    int heap_size = arr.size();

    // Starting from the last non-leaf node
    for (int i = heap_size / 2 - 1; i >= 0; --i)
    {
        max_heapify(arr, i, heap_size);
    }
}

void heapsort(vector<populations> &arr) // sorts in ascending order
{                                       // Function to perform heapsort
    int heap_size = arr.size();

    build_max_heap(arr); // building a max-heap from the input array

    /*Remove the largest element (located at the root) and ensure that the remaining elements in the
     heap maintain the property of being a max-heap by adjusting their positions accordingly*/

    for (int i = heap_size - 1; i > 0; --i)
    {
        swap_elements(arr, 0, i);
        max_heapify(arr, 0, i);
    }
}

void max_heap_insert(vector<populations> &array, populations value)
{

    array.push_back(value); // adding new element at the end of the array

    int i = array.size() - 1; // index

    while (i > 0 && array[(i - 1) / 2].population < array[i].population) // Move the new element up the tree in order to find the right place
    {
        swap_elements(array, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

populations heap_extract_max(vector<populations> &array) // extract and return the maximum element of the max-heap degree==2
{
    if (array.empty())
    {
        cout << "Heap is empty.\n";
        populations p;
        p.population = -1;
        p.city = "empty heap";
        return p; // p represents error
    }

    populations max_element = array[0]; // maximum element is the root on the max heap
    array[0] = array.back();
    array.pop_back();

    //  the max-heap property
    max_heapify(array, 0, array.size());

    return max_element;
}

void heap_increase_key(vector<populations> &array, int index, int new_key) // increase the key of a specified element of the max-heap
{
    if (index < 0 || index >= array.size() || new_key <= array[index].population)
    {
        cout << "Invalid index or new key is smaller than ex key.\n"; // when data doesn't come the right format
        return;
    }

    array[index].population = new_key; // updating the key

    // move the updated element up the tree to maintain the max-heap property
    while (index > 0 && array[(index - 1) / 2].population < array[index].population)
    {
        swap_elements(array, index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

// returns the maximum element of the max heap
populations heap_maximum(vector<populations> &array)
{

    if (array.empty())
    {
        cout << "Heap is empty.\n";
        populations p;
        p.city = "empty";
        p.population = 0;
        return p; // p represents error
    }

    populations max_element = array[0];
    return max_element;
}

// D-ARY HEAP

void dary_max_heapify(vector<populations> &heap, int i, int d)
{
    int largest = i;
    for (int j = 1; j <= d; ++j)
    {
        int child = d * i + j;
        if (child < heap.size() && heap[child].population > heap[largest].population)
        {
            largest = child;
        }
    }

    if (largest != i)
    {
        swap_elements(heap, i, largest);
        dary_max_heapify(heap, largest, d);
    }
}

void build_dary_heap(vector<populations> &heap, int degree)
{
    for (int i = (heap.size() - 1) / degree; i >= 0; --i)
    {
        dary_max_heapify(heap, i, degree);
    }
}

int dary_calculate_height(int n, int d) // function to calculate the height of a d-ary heap
{
    // d for degree
    int height = 0;
    int nodes_at_current_level = 1;

    while (n > 0)
    {
        n -= nodes_at_current_level;
        nodes_at_current_level *= d;
        height++;
    }

    return height;
}

populations dary_extract_max(vector<populations> &arr, int degree) // extract and return the maximum element of the max-heap
{

    if (arr.empty())
    {
        cout << "Heap is empty.\n";
        populations p;
        p.population = -1;
        p.city = "empty heap";
        return p; // p represents error
    }

    populations max_element = arr[0]; // first element has the largest population since it is max-heap
    arr[0] = arr.back();
    arr.pop_back();

    dary_max_heapify(arr, 0, degree); //  the max-heap property

    return max_element;
}

void dary_insert_element(vector<populations> &heap, populations p, int degree)
{
    heap.push_back(p); // adding it to the last

    int i = heap.size() - 1; // index

    // Move the new element up the tree to maintain the max-heap property
    while (i > 0 && heap[(i - 1) / degree].population < heap[i].population)
    {
        swap_elements(heap, i, (i - 1) / degree);
        i = (i - 1) / degree;
    }
}

void dary_increase_key(vector<populations> &array, int i, int new_key, int degree) // increase the key of a specified element of the max-heap
{
    if (i < 0 || i >= array.size() || new_key <= array[i].population)
    {
        cout << "Invalid index or new key is smaller than ex key.\n";
        return;
    }

    array[i].population = new_key; // updating the key

    // move the updated element up the tree to find the right place
    while (i > 0 && array[(i - 1) / degree].population < array[i].population)
    {
        swap_elements(array, i, (i - 1) / degree);
        i = (i - 1) / degree;
    }
}

int main(int argc, char **argv)
{
    string filename = argv[1];
    string function = argv[2];
    string output_file = argv[3];
    vector<populations> data = read_file(filename); // read csv file
    auto begin = high_resolution_clock::now();      // time start
    // finding method

    if (function == "heapsort")
    {
        heapsort(data);
    }
    else if (function == "build_max_heap")
    {
        build_max_heap(data);
    }
    else if (function == "max_heapify")
    {
        int index = stoi(string(argv[4]).substr(1));
        max_heapify(data, index - 1, data.size());
    }
    else if (function == "max_heap_insert")
    {
        build_max_heap(data); // since my data is not heap
        string command = argv[4];
        if (command.substr(0, 2) == "k_")
        {
            size_t command_line = command.find('_');
            if (command_line != string::npos)
            {
                string city_name = command.substr(command_line + 1, command.rfind('_') - command_line - 1);
                int population = stoi(command.substr(command.rfind('_') + 1));
                populations p;
                p.city = city_name;
                p.population = population;
                max_heap_insert(data, p);
            }
        }
    }
    else if (function == "heap_extract_max")
    {
        build_max_heap(data); // since my data is not heap
        populations p = heap_extract_max(data);
        cout << "extract data :" << p.city << " " << p.population << endl;
    }
    else if (function == "heap_increase_key")
    {
        build_max_heap(data);
        int key;
        int i; // index
        if (argc > 5)
        {
            string command1 = argv[4];
            string command2 = argv[5];
            if (command1[0] == 'i')
            {
                i = stoi(command1.substr(1));
                key = stoi(command2.substr(1));
            }
            else if (command1[0] == 'k')
            {
                i = stoi(command2.substr(1));
                key = stoi(command1.substr(1));
            }
            heap_increase_key(data, (i - 1), key);
        }
    }
    else if (function == "heap_maximum")
    {
        build_max_heap(data);
        populations max = heap_maximum(data);
        cout << "max " << max.city << " " << max.population << endl;
    }
    else if (function == "dary_calculate_height")
    {
        if (argv[4] != nullptr)
        {
            int degree = stoi(string(argv[4]).substr(1));
            int height = dary_calculate_height(data.size(), degree);
            cout << "Height: " << height << endl;
        }
        else
        {
            cout << "error" << endl;
        }
    }
    else if (function == "dary_extract_max")
    {
        int degree = stoi(string(argv[4]).substr(1));
        build_dary_heap(data, degree); // since my data is not heap
        populations p;
        p = dary_extract_max(data, degree);
        cout << "extract data " << p.city << p.population << endl;
    }
    else if (function == "dary_insert_element")
    {
        if (argc > 5)
        {
            string command1 = argv[4];
            string command2 = argv[5];
            if (command1.substr(0, 2) == "k_")
            {
                size_t command_line = command1.find('_');
                if (command_line != string::npos)
                {
                    int degree = stoi(command2.substr(1));
                    build_dary_heap(data, degree); // since my data is not heap
                    string city_name = command1.substr(command_line + 1, command1.rfind('_') - command_line - 1);
                    int population = stoi(command1.substr(command1.rfind('_') + 1));
                    populations p; // I send population to dary insert element
                    p.city = city_name;
                    p.population = population;
                    dary_insert_element(data, p, degree);
                }
            }
            else if (command2.substr(0, 2) == "k_")
            {
                size_t command_line = command2.find('_');
                if (command_line != string::npos)
                {
                    int degree = stoi(command1.substr(1));
                    build_dary_heap(data, degree); // since my data is not heap
                    string city_name = command2.substr(command_line + 1, command2.rfind('_') - command_line - 1);
                    int population = stoi(command2.substr(command2.rfind('_') + 1));
                    populations p; // I send population to dary insert element
                    p.city = city_name;
                    p.population = population;
                    dary_insert_element(data, p, degree);
                }
            }
        }
    }
    else if (function == "dary_increase_key")
    {
        if (argc > 6)
        {
            string command1 = argv[4]; // I don't know which one is key, degree or index
            string command2 = argv[5];
            string command3 = argv[6];
            int index, key, degree;

            if (command1[0] == 'i') // when first one is index try to find other variable: k and degree
            {
                index = stoi(command1.substr(1));
                if (command2[0] == 'k')
                {
                    key = stoi(command2.substr(1));
                    degree = stoi(command3.substr(1));
                }
                else
                {
                    key = stoi(command3.substr(1));
                    degree = stoi(command2.substr(1));
                }
            }
            else if (command1[0] == 'd') // when first one is degree try to find other variable: k and index
            {
                degree = stoi(command1.substr(1));
                if (command2[0] == 'k')
                {
                    key = stoi(command2.substr(1));
                    index = stoi(command3.substr(1));
                }
                else
                {
                    key = stoi(command3.substr(1));
                    index = stoi(command2.substr(1));
                }
            }
            else if (command1[0] == 'k')
            {
                key = stoi(command1.substr(1));
                if (command2[0] == 'i')
                {
                    index = stoi(command2.substr(1));
                    degree = stoi(command3.substr(1));
                }
                else
                {
                    index = stoi(command3.substr(1));
                    degree = stoi(command2.substr(1));
                }
            }
            build_dary_heap(data, degree); // since data is not heap and I need to send heap to increase key
            dary_increase_key(data, index - 1, key, degree);
        }
    }
    else if (function == "build_dary_heap")
    {
        int degree = stoi(string(argv[4]).substr(1));
        build_dary_heap(data, degree);
    }
    else
    {
        cout << "wrong command" << endl;
    }

    auto end = high_resolution_clock::now(); // end time

    auto time = duration_cast<nanoseconds>(end - begin); // I calculate sorting time(without reading and writing)
    cout << time.count() << endl;
    write_file(data, output_file); // writing csv file

    return 0;
}
