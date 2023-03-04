// Sovanna Ramirez & Sanjana Jadhav
// CS 4375.004 Machine Learning
// HW #2: C++ Algorithms from Scratch

// I took code from Karen Mazidi's documentation for the reading in of the csv file

// Repeat the above but implement Naïve Bayes using predictors age, pclass, and sex to
// predict survival on the Titanic data. Use the first 800 observations for train, the rest for test
// c. Use the remaining data to predict values.
// d. Write functions to calculate accuracy, sensitivity, specificity.
// e. Output the test metrics and the run time for the algorithm. You can use chrono to
// measure time. Measure just the training time of the algorithm.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <math.h>

using namespace std;

// age_x is variable, calculate age likelihood using mean and var passed to this function
// 1 / sqrt(2 * pi * var_v) * exp(-((v-mean_v)^2)/(2 * var_v))
double calc_age_lh(double age_x, double age_mean, double age_var) {
    return 1.0 / sqrt(2.0 * M_PI * age_var) * exp(-(pow((age_x - age_mean),2.0)) / (2.0 * age_var));
}

double calc_raw_prob(double pclass, double sex, double age) {

}

int main() {

    ifstream inFS;
    string line;
    string pno_in, pclass_in, survived_in, sex_in, age_in;
    const int MAX_LEN = 1500;

    vector<double> pclass(MAX_LEN);
    vector<double> survived(MAX_LEN);
    vector<double> sex(MAX_LEN);
    vector<double> age(MAX_LEN);

    // try to open file
    cout << "Opening file titanic_project.csv." << endl;
    inFS.open("titanic_project.csv");

    if(!inFS.is_open()) {
        cout << "Could not open file titanic_project.csv" << endl;
        return 1;
    }

    getline(inFS, line);

    int numObservations = 0;

    while (inFS.good()) {
        getline(inFS, pno_in, ',');
        getline(inFS, pclass_in, ',');
        getline(inFS, survived_in, ',');
        getline(inFS, sex_in, ',');
        getline(inFS, age_in, '\n');

        pclass.at(numObservations) = stod(pclass_in);
        survived.at(numObservations) = stod(survived_in);
        sex.at(numObservations) = stod(sex_in);
        age.at(numObservations) = stod(age_in);

        numObservations++;
    }

    pclass.resize(numObservations);
    survived.resize(numObservations);
    sex.resize(numObservations);
    age.resize(numObservations);

    cout << "Closing file titanic_project.csv" << endl;
    inFS.close();

    // train/test split
    vector<double> train_pclass(800);
    vector<double> train_survived(800);
    vector<double> train_sex(800);
    vector<double> train_age(800);

    vector<double> test_pclass(numObservations - 800);
    vector<double> test_survived(numObservations - 800);
    vector<double> test_sex(numObservations - 800);
    vector<double> test_age(numObservations - 800);

    for(int i = 0; i < numObservations; i++) {
        if(i < 800) {
            train_pclass.at(i) = pclass.at(i);
            train_survived.at(i) = survived.at(i);
            train_sex.at(i) = sex.at(i);
            train_age.at(i) = age.at(i);
        }
        else {
            test_pclass.at(i - 800) = pclass.at(i);
            test_survived.at(i - 800) = survived.at(i);
            test_sex.at(i - 800) = sex.at(i);
            test_age.at(i - 800) = age.at(i);
        }
    }

    // start clock
    auto start = chrono::steady_clock::now();

    // calculate A-priori probabilities using training data
    double alive_count = 0; // survived = 1
    double deceased_count = 0; // survived = 0

    for(int i = 0; i < train_survived.size(); i++) { // iterate through train set of survived check 0 or 1
        if(train_survived[i] == 1){
            alive_count++;
        }
        else {
            deceased_count++;
        }
    }

    // prob = alive count / total of train_survived
    double a_priori_alive = alive_count / train_survived.size();
    double a_priori_deceased = deceased_count / train_survived.size();

    // print
    cout << "\nA-priori Probabilities: " << endl;
    cout << "1 (Alive): " << a_priori_alive << "\t\t\t\t" << "0 (Deceased): " << a_priori_deceased << endl;

    // conditional probabilities...
    // calculate likelihood: p(pclass|survived) ---------------------
    double pclass1_alive = 0;
    double pclass1_deceased = 0;

    double pclass2_alive = 0;
    double pclass2_deceased = 0;

    double pclass3_alive = 0;
    double pclass3_deceased = 0;

    // iterate through train set -> check survived 0/1 && check pclass 1/2/3
    for(int i = 0; i < train_survived.size(); i++) {
        if(train_survived[i] == 1 && train_pclass[i] == 1){ // alive && pclass = 1
            pclass1_alive++;
        }
        else if(train_survived[i] == 0 && train_pclass[i] == 1){ // deceased && pclass = 1
            pclass1_deceased++;
        }
        else if(train_survived[i] == 1 && train_pclass[i] == 2){ // alive && pclass =2
            pclass2_alive++;
        }
        else if(train_survived[i] == 0 && train_pclass[i] == 2) { // deceased && pclass = 2
            pclass2_deceased++;
        }
        else if(train_survived[i] == 1 && train_pclass[i] == 3) { // alive && pclass = 3
            pclass3_alive++;
        }
        else if(train_survived[i] == 0 && train_pclass[i] == 3) { // deceased && pclass = 3
            pclass3_deceased++;
        }
    }

    double prob_pclass1_alive = pclass1_alive / alive_count;
    double prob_pclass1_deceased = pclass1_deceased / deceased_count;

    double prob_pclass2_alive = pclass2_alive / alive_count;
    double prob_pclass2_deceased = pclass2_deceased / deceased_count;

    double prob_pclass3_alive = pclass3_alive / alive_count;
    double prob_pclass3_deceased = pclass3_deceased / deceased_count;

    cout << "\nConditional Probabilities:" << endl;
    cout << "p(pclass|survived)... " << endl;
    cout << "Pclass 1 & Alive: " << prob_pclass1_alive << "\t";
    cout << "Pclass 1 & Deceased: " << prob_pclass1_deceased << endl;

    cout << "Pclass 2 & Alive: " << prob_pclass2_alive << "\t";
    cout << "Pclass 2 & Deceased: " << prob_pclass2_deceased << endl;

    cout << "Pclass 3 & Alive: " << prob_pclass3_alive << "\t";
    cout << "Pclass 3 & Deceased: " << prob_pclass3_deceased << endl;


    // calculate likelihood: p(sex|survived) -----------------------------
    double female_alive_count = 0;
    double female_deceased_count = 0;

    double male_alive_count = 0;
    double male_deceased_count = 0;

    // iterate through train set -> check survived 0/1 && check sex 0/1 (female - 0 , male - 1)
    for(int i = 0; i < train_survived.size(); i++) {
        if(train_survived[i] == 1 && train_sex[i] == 0){ // female alive
            female_alive_count++;
        }
        else if(train_survived[i] == 0 && train_sex[i] == 0){ // female deceased
            female_deceased_count++;
        }
        else if(train_survived[i] == 1 && train_sex[i] == 1){ // male alive
            male_alive_count++;
        }
        else if(train_survived[i] == 0 && train_sex[i] == 1) { // male deceased
            male_deceased_count++;
        }
    }

    double prob_female_alive = female_alive_count / alive_count;
    double prob_female_deceased = female_deceased_count / deceased_count;

    double prob_male_alive = male_alive_count / alive_count;
    double prob_male_deceased = male_deceased_count / deceased_count;

    cout << "\np(sex|survived)... " << endl;
    cout << "Female & Alive: " << prob_female_alive << "\t";
    cout << "Female & Deceased: " << prob_female_deceased << endl;

    cout << "Male & Alive: " << prob_male_alive << "\t\t";
    cout << "Male & Deceased: " << prob_male_deceased << endl;


    // calculate likelihood: p(age|survived) ------------------------------

    // mean and variance of age
    double age_mean_alive = 0; // count of alive for mean
    double age_mean_deceased = 0; // count of deceased for mean

    double age_var_alive = 0; // count for alive for var
    double age_var_deceased = 0; // count for deceased for var

    // mean...
    for(int i = 0; i < train_survived.size(); i++) {
        if(train_survived[i] == 0) {
            age_mean_deceased += train_age[i];
        }
        else {
            age_mean_alive += train_age[i];
        }
    }

    age_mean_deceased /= deceased_count;
    age_mean_alive /= alive_count;

    // variance...
    for(int i = 0; i < train_survived.size(); i++) {
        if(train_survived[i] == 0) {
            age_var_deceased += pow((train_age[i] - age_mean_deceased), 2);
        }
        else {
            age_var_alive += pow((train_age[i] - age_mean_alive), 2);
        }
    }

    age_var_deceased /= deceased_count - 1;
    age_var_alive /= alive_count - 1;

    cout << "\np(age|survived)... " << endl;
    cout << "Mean Alive: " << age_mean_alive << "\t\t\t\t";
    cout << "Mean Deceased: " << age_mean_deceased << endl;

    cout << "Variance Alive: " << sqrt(age_var_alive) << "\t\t";
    cout << "Variance Deceased: " << sqrt(age_var_deceased) << endl;

    auto end = std::chrono::steady_clock::now();
    cout << "\nTraining Time of Naive Bayes Algorithm: " << chrono::duration_cast<chrono::seconds>(end - start).count() << " s" << endl;

    cout << "\nPredictions on Test Data: " << endl;
    cout << setw(5) << "[0]" << "\t\t" << setw(10) << "[1]" << endl;

    // sex 0/1 (female - 0 , male - 1)
    for(int i = 0; i < test_survived.size() - 1; i ++) {

        // num_s <- lh_pclass(1/2/3 + alive) * lh_sex(f-0, m-1 + alive) * apriori(alive) * calc_age_lh(age, age_mean(alive), age_var(alive))
        // num_p <- pclass(1/2/3 + dec) * sex(f-0, m-1 + dec) * apriori(dec) * calc_age_lh(age, age_mean(dec), age_var(dec))
        // denom <- num_s + num_p

        double num_s = 0;
        double num_p = 0;
        double denom = 0;

        if (test_pclass[i] == 1 && test_sex[i] == 0) { // pclass 1 && female
            num_s = prob_pclass1_alive * prob_female_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive);
            num_p = prob_pclass1_deceased * prob_female_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
            denom = prob_pclass1_alive * prob_female_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive)
                          + prob_pclass1_deceased * prob_female_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
        }
        else if(test_pclass[i] == 2 && test_sex[i] == 0) { // pclass 2 && female
            num_s = prob_pclass2_alive * prob_female_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive);
            num_p = prob_pclass2_deceased * prob_female_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
            denom = prob_pclass2_alive * prob_female_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive)
                    + prob_pclass2_deceased * prob_female_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
        }
        else if(test_pclass[i] == 3 && test_sex[i] == 0) { // pclass 3 && female
            num_s = prob_pclass3_alive * prob_female_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive);
            num_p = prob_pclass3_deceased * prob_female_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
            denom = prob_pclass3_alive * prob_female_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive)
                    + prob_pclass3_deceased * prob_female_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
        }
        else if(test_pclass[i] == 1 && test_sex[i] == 1) { // pclass 1 && male
            num_s = prob_pclass1_alive * prob_male_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive);
            num_p = prob_pclass1_deceased * prob_male_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
            denom = prob_pclass1_alive * prob_male_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive)
                    + prob_pclass1_deceased * prob_male_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
        }
        else if(test_pclass[i] == 2 && test_sex[i] == 1) { // pclass 2 && male
            num_s = prob_pclass2_alive * prob_male_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive);
            num_p = prob_pclass2_deceased * prob_male_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
            denom = prob_pclass2_alive * prob_male_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive)
            + prob_pclass2_deceased * prob_male_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
        }
        else if(test_pclass[i] == 3 && test_sex[i] == 1) { // pclass 3 && male
            num_s = prob_pclass3_alive * prob_male_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive);
            num_p = prob_pclass3_deceased * prob_male_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
            denom = prob_pclass3_alive * prob_male_alive * a_priori_alive * calc_age_lh(test_age[i], age_mean_alive, age_var_alive)
                    + prob_pclass3_deceased * prob_male_deceased * a_priori_deceased * calc_age_lh(test_age[i], age_mean_deceased, age_var_deceased);
        }

        double prob_alive_test = num_s / denom;
        double prob_deceased_test = num_p / denom;

        cout << "[" << i << "]" << prob_deceased_test << "\t\t\t" << prob_alive_test << endl;
    }

}
