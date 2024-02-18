//Doğa Güneş Karadağ
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std::chrono;
using namespace std;

struct population_data
{
    string city;
    int population;
};

vector<population_data> read_file(const string &filename) // reading csv files
{
    vector<population_data> data;
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

// logs when verbose is true
void write_log(ofstream &logFile, int l, int h, int pivot, vector<population_data> &arr)
{
    if (logFile.is_open())
    {
        logFile << " Pivot: " << pivot << " Array: [";
        for (int k = l; k <= h; k++)
        {
            logFile << arr[k].population << (k < h ? ", " : "");
        }
        logFile << "]" << endl;
    }
}
void swap_elements(vector<population_data> &P, int first, int second)
{
    population_data temp = P[first];
    P[first] = P[second];
    P[second] = temp;
}

int partition(vector<population_data> &P, int l, int h, ofstream &logFile, bool v) // partition last element
{

    population_data pivot = P[h];
    // choosing the pivot as last element from the array
    int temp = 0;
    int i = (l - 1);

    for (int j = l; j <= h; j++)
    {
        // When current population value of an element is smaller than the pivot population value
        if (P[j].population < pivot.population)
        {
            i++;
            swap_elements(P, i, j);
        }
    }

    swap_elements(P, i + 1, h);
    if (v == true)
    {
        write_log(logFile, l, h, pivot.population, P);
    }

    return (i + 1); // return pivot's place
}

int partition_random(vector<population_data> &P, int l, int h, ofstream &logFile, bool v)
{
    srand(time(0));
    int x = rand() % (h - l + 1);
    int random_variable = l + x; // finding random variable between h and l
    swap_elements(P, random_variable, h);
    return partition(P, l, h, logFile, v);
}

int partition_median(vector<population_data> &P, int l, int h, ofstream &logFile, bool v)
{
    srand(time(0));
	// 3 random element
    int a = l + rand() % (h - l + 1);
    int b = l + rand() % (h - l + 1);
    int c = l + rand() % (h - l + 1);
    int temp;
	// finding median of three
    if ((P[a].population < P[b].population && P[b].population < P[c].population) || (P[c].population < P[b].population && P[b].population < P[a].population))
    {
        swap_elements(P, b, h);
    }

    else if ((P[b].population < P[a].population && P[a].population < P[c].population) || (P[c].population < P[a].population && P[a].population < P[b].population))
    {
        swap_elements(P, a, h);
    }
    else
        swap_elements(P, c, h);

    return partition(P, l, h, logFile, v);
}

void insertionSort(vector<population_data> &P, int l, int h)
{
    int i, j;
    population_data key;
    for (i = l; i < h + 1; i++)
    {
        key = P[i];
        j = i - 1;

        while (j >= 0 && P[j].population > key.population)
        {
            P[j + 1] = P[j];
            j = j - 1;
        }
        P[j + 1] = key;
    }
}

void quickSort(vector<population_data> &P, int l, int h, string type, int k, ofstream &logFile, bool v)
{
    // when low is less than high
    if (l < h)
    {
        int pivot;
        int size = h - l + 1;
        if ((k >= size) && (k != 1))
        {
            insertionSort(P, l, h);
        }
        else
        {
            if (k == 1 || type == "l")
            {
                pivot = partition(P, l, h, logFile, v);
            }
            else if (type == "m")
            {
                pivot = partition_median(P, l, h, logFile, v);
            }
            else
            {
                pivot = partition_random(P, l, h, logFile, v);
            }

            quickSort(P, l, pivot - 1, type, k, logFile, v);
            quickSort(P, pivot + 1, h, type, k, logFile, v);
        }
    }
}

void write_file(const vector<population_data> &P, const string &filename)
{
    ofstream file(filename);
    for (const auto &data : P)
    {
        file << data.city << ";" << data.population << "\n";
    }
}

int main(int argc, char **argv)
{
    
    string filename = argv[1];
    string type = argv[2];
    int k = stoi(argv[3]);
    string output_file = argv[4];
    bool verbose;
    if (argc > 4 && string(argv[4]) == "v")
    {
        verbose = true;
    }
    else
    {
        verbose = false;
    }
    
   /*
    string filename = "dummyData.csv";
    string type = "r";
    int k = 6;
    string output_file = "abc.csv";
    bool verbose = true;
    if (argc > 4 && string(argv[4]) == "v")
    {
        verbose = true;
    }
    else
    {
        verbose = false;
    }
    */
    cout << type << k << output_file << verbose;

    vector<population_data> data = read_file(filename); // read csv file
    int n = data.size();
    auto begin = high_resolution_clock::now(); // time start
    ofstream logFile;
    if (verbose == true)
    {
        logFile.open("log.txt", ios::trunc);
    }

    if (k == 1)
    {
        quickSort(data, 0, n - 1, "l", k, logFile, verbose);
    }
    else if (n > k)
    {

        quickSort(data, 0, n - 1, type, k, logFile, verbose);
    }
    else
    {
        insertionSort(data, 0, n - 1);
    }

    auto end = high_resolution_clock::now(); // end time

    auto time = duration_cast<nanoseconds>(end - begin);//I calculate sorting time(without reading and writing)


    write_file(data, output_file);

    cout << "Time taken by QuickSort with pivot strategy " << type << " and threshold " << k << ": " << time.count() << " ns.";
    

    return 0;
}