// Sovanna Ramirez
// CS 4375.004 Machine Learning
// HW #1: Data Exploration

// I took code from Karen Mazidi's documentation for the reading in of the csv file

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// function declaration
void print_stats(vector<double> v);
double calc_sum(vector<double> v);
double calc_mean(vector<double> v);
double calc_med(vector<double> v);
vector<double> calc_range(vector<double> v);
double covar (vector<double> v1, vector<double> v2);
double cor(vector<double> v1, vector<double> v2);

int main(int argc, char** argv) {

    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    // try to open file
    cout << "Opening file Boston.csv." << endl;
    inFS.open("Boston.csv");

    if(!inFS.is_open()) {
        cout << "Could not open file Boston.csv" << endl;
        return 1;
    }

    // can now use inFS stream like cin stream
    // Boston.csv should contain two doubles

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    // echo heading
    cout << "Heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good()) {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "new length " << rm.size() << endl;

    cout << "Closing file Boston.csv" << endl;
    inFS.close(); // done, so close

    cout << "Number of records: " << numObservations << endl;

    cout << "\nStats for rm" << endl;
    print_stats(rm);

    cout << "\nStats for medv" << endl;
    print_stats(medv);

    cout << "\nCovariance = " << covar(rm, medv) << endl; // write the functions for covar
    cout << "\nCorrelation = " << cor(rm, medv) << endl; // write function for corr
    cout << "\nProgram terminated.";

    return 0;
}

void print_stats(vector<double> v) { // Call the functions described in 1-4 for rm and separately for medv
    cout << "Sum: " << calc_sum(v) << endl;
    cout << "Mean: " << calc_mean(v) << endl;
    cout << "Median: " << calc_med(v) << endl;

    vector<double> vector_range = calc_range(v);
    cout << "Range: " << "[" << vector_range.at(0) << ", " << vector_range.at(1) << "]" << endl;
}

double calc_sum(vector<double> v) { // good
    double sum = 0;
    for(int i = 0; i < v.size(); i++)
        sum = sum + v.at(i);
    return sum;
}

double calc_mean(vector<double> v) { // good
    double mean = 0;
    double sum = calc_sum(v);
    mean = sum / v.size(); // sum of the vector / size of vector
    return mean;
}

double calc_med(vector<double> v) { // good
    int n = v.size() / 2;
    nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}

vector<double> calc_range(vector<double> v) { // good
    vector<double> vector_range;

    double max = *max_element(v.begin(), v.end());
    double min = *min_element(v.begin(), v.end());

   vector_range.push_back(min);
   vector_range.push_back(max);

   return vector_range;
}

double covar(vector<double> v1, vector<double> v2) { // covariance
    // get the mean values
    double covariance = 0;
    double x_bar = calc_mean(v1);
    double y_bar = calc_mean(v2);

    for(int i = 0; i < v1.size(); i++) {
        covariance += (v1.at(i) - x_bar) * (v2.at(i) - y_bar);
    }

    return covariance / v1.size();
}

double cor(vector<double> v1, vector<double> v2) { // correlation
    double covariance = 0;
    double sum_x = 0, sum_y = 0;
    double x_bar = calc_mean(v1);
    double y_bar = calc_mean(v2);

    // correlation = covariance / (std_dev(x)) * (std_dev(y))

    for(int i = 0; i < v1.size(); i++) {
        covariance += (v1.at(i) - x_bar) * (v2.at(i) - y_bar);

        sum_x += pow((v1.at(i) - x_bar), 2);
        sum_y += pow((v2.at(i) - y_bar), 2);
    }

    double std_dev = sqrt(sum_x * sum_y);
    double correlation = covariance / std_dev;
    return correlation;
}
