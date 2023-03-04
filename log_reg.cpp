//
//  Logistic Regression (C++)
//  Created by Sanjana Jadhav & Sovanna Ramirez on 2/22/23.
//

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;

// formula source: https://mathworld.wolfram.com/SigmoidFunction.html
vector<double> sigmoidFunction(vector<double> matrix)
{
    int matrixSize = matrix.size();

    vector<double> sigmoidVector(matrixSize);
    double currentVal = 0.0;
    double denominator = 0.0;
    double x = 0.0;

    for (int i = 0; i < matrix.size(); i++)
    {
        x = 0 - matrix.at(i); // -x
        denominator = 1.0 + pow(2.718, x);
        currentVal = 1.0 / denominator;   // formula of sigmoid
        sigmoidVector.at(i) = currentVal; // store in sigmoid vector
    }

    return sigmoidVector;
}

// matrix multiplcation
vector<double> matrixMultiplication(vector<vector<double>> dataMatrix, vector<double> vector1)
{

    vector<double> answer(dataMatrix.size());
    for (int i = 0; i < dataMatrix.size(); i++)
    {

        // formula/rules source: https://www.mathsisfun.com/algebra/matrix-multiplying.html
        // sample example:
        // matrix1 = { {1, 2},                           vector1: { {0},
        //             {3, 4} }                                     {1} }
        // resulting vector: { {1*0 + 2*1},       =               { {2},
        //                     {3*0 + 4*1} }                        {4} }
        answer[i] = 0;
        for (int j = 0; j < vector1.size(); j++)
        {
            answer[i] += dataMatrix[i][j] * vector1[j];
        }
    }

    return answer;
}

// calculate probability
// formula: sigmoid(data_matrix %*% weights)
vector<double> probsCalculation(vector<vector<double>> dataMatrix, vector<double> weights)
{
    vector<double> mult;

    // matrix multiplication
    mult = matrixMultiplication(dataMatrix, weights);

    // sigmoid
    vector<double> answer = sigmoidFunction(mult);

    return answer;
}

// calculate error
// formula: labels - prob_vector
vector<double> errorCalculation(vector<double> labels, vector<double> prob)
{
    vector<double> error(labels.size());
    for (int i = 0; i < labels.size(); i++)
    {
        error[i] = (labels[i] - prob[i]); // subtract
    }
    return error;
}

// transpose
// formula source: https://www.programiz.com/cpp-programming/examples/matrix-transpose
vector<vector<double>> transpose(vector<vector<double>> dataMatrix)
{
    const int rows = dataMatrix.size();
    const int cols = dataMatrix[0].size();
    vector<vector<double>> transposed;

    // resizing
    // rows = number of cols (transpose)
    transposed.resize(cols);
    for (int i = 0; i < cols; ++i)
    {
        // cols = number of rows (transpose)
        transposed[i].resize(rows);
    }

    // swapping values
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            transposed[j][i] = dataMatrix[i][j];
        }
    }

    return transposed;
}

// matrix multiplication
// used to multiply learning rate * transpose matrix --> multiplying a vector by a value
vector<double> matrixMultiplication2(vector<double> vector1)
{
    vector<double> answer(vector1.size());
    double learningRate = 0.001;
    for (int i = 0; i < vector1.size(); i++)
    {
        answer[i] = learningRate * vector1[i]; // multiply each value by learning rate
    }

    return answer;
}

// addition
// for adding the weights vector
vector<double> add(vector<double> vector1, vector<double> vector2)
{
    vector<double> answer(vector1.size());
    for (int i = 0; i < vector1.size(); i++)
    {
        answer[i] = vector1[i] + vector2[i]; // add the values and store in new vector
    }

    return answer;
}

// weights + learning_rate * t(data_matrix) %*% error
vector<double> weightsCalculation(vector<vector<double>> dataMatrix, vector<double> error, vector<double> weights)
{
    // transpose
    vector<vector<double>> transposeMat = transpose(dataMatrix);

    // t(data_matrix) %*% error
    vector<double> matMult = matrixMultiplication(transposeMat, error);

    // multiply by learning rate
    vector<double> multLR = matrixMultiplication2(matMult);

    // add weights
    vector<double> answer = add(multLR, weights);

    return answer;
}

// prediction probability
vector<double> probTest(vector<vector<double>> dataMatrixTest, vector<double> weights)
{

    // create vectors for predictions
    vector<double> predictVals = matrixMultiplication(dataMatrixTest, weights);
    vector<double> prob(predictVals.size());

    // formula: https://github.com/kjmazidi/Machine_Learning_2nd_edition/blob/master/Part_2_Linear_Models/6_1_LogReg-plasma.Rmd
    // p = exp(y) / (1 + exp(y))
    double computation = 0;
    for (int i = 0; i < prob.size(); i++)
    {
        computation = (exp(predictVals[i])) / (1 + exp(predictVals[i]));
        prob[i] = computation;
    }

    return prob;
}

int main()
{
    ifstream inFile;
    string line;
    string current_passNum, current_pClass, current_survived, current_sex, current_age;
    const int MAX_LEN = 1000;

    // create vectors to hold data
    // create train set & test set vectors
    vector<double> pClassTrainSet(MAX_LEN);
    vector<double> survivedTrainSet(MAX_LEN);
    vector<double> sexTrainSet(MAX_LEN);
    vector<double> ageTrainSet(MAX_LEN);

    vector<double> pClassTestSet(MAX_LEN);
    vector<double> survivedTestSet(MAX_LEN);
    vector<double> sexTestSet(MAX_LEN);
    vector<double> ageTestSet(MAX_LEN);

    cout << "Opening titanic_project.csv file" << endl;

    // open file
    inFile.open("titanic_project.csv");

    // print error
    if (!inFile.is_open())
    {
        cout << "Error in opening titanic_project.csv" << endl;
        return 1;
    }

    // read first line
    cout << "Reading Line 1" << endl;
    getline(inFile, line);

    // print heading
    cout << "heading: " << line << endl;

    int numObservationsTrain = 0;
    int numObservationsTest = 0;

    // read 800 entries
    while (inFile.good())
    {
        if (numObservationsTrain >= 800)
            break;

        getline(inFile, current_passNum, ','); // not a necessary column
        getline(inFile, current_pClass, ',');
        getline(inFile, current_survived, ',');
        getline(inFile, current_sex, ',');
        getline(inFile, current_age, '\n');

        // store the values in the vectors
        pClassTrainSet.at(numObservationsTrain) = stoi(current_pClass);
        survivedTrainSet.at(numObservationsTrain) = stoi(current_survived);
        sexTrainSet.at(numObservationsTrain) = stoi(current_sex);
        ageTrainSet.at(numObservationsTrain) = stoi(current_age);

        numObservationsTrain++;
    }

    // remove extra indices of the train vectors
    pClassTrainSet.resize(numObservationsTrain);
    survivedTrainSet.resize(numObservationsTrain);
    sexTrainSet.resize(numObservationsTrain);
    ageTrainSet.resize(numObservationsTrain);

    // read the rest of the entries
    while (inFile.good())
    {
        getline(inFile, current_passNum, ','); // not a necessary column
        getline(inFile, current_pClass, ',');
        getline(inFile, current_survived, ',');
        getline(inFile, current_sex, ',');
        getline(inFile, current_age, '\n');

        // store the values in the vectors
        pClassTestSet.at(numObservationsTest) = stoi(current_pClass);
        survivedTestSet.at(numObservationsTest) = stoi(current_survived);
        sexTestSet.at(numObservationsTest) = stoi(current_sex);
        ageTestSet.at(numObservationsTest) = stoi(current_age);

        numObservationsTest++;
    }

    // remove extra indices of the test vectors
    pClassTestSet.resize(numObservationsTest);
    survivedTestSet.resize(numObservationsTest);
    sexTestSet.resize(numObservationsTest);
    ageTestSet.resize(numObservationsTest);

    // print size of train/test vectors
    cout << "Number of Observations in Train Vectors: " << pClassTrainSet.size() << endl;
    cout << "Number of Observations in Test Vectors: " << pClassTestSet.size() << endl;

    cout << "Closing titanic_project.csv file" << endl;
    inFile.close();

    cout << "\nPerforming Logistic Regression:" << endl;

    // create weights vector, data matrix and label vector
    vector<double> weightsVector(2); // set up weights vector
    weightsVector.at(0) = 1;
    weightsVector.at(1) = 1;

    vector<vector<double>> dataMatrix(pClassTrainSet.size()); // data matrix
    for (int i = 0; i < pClassTrainSet.size(); i++)
    {
        // initialize and add values
        dataMatrix[i] = vector<double>(2);
        dataMatrix[i][0] = 1;
        dataMatrix[i][1] = sexTrainSet[i];
    }

    vector<double> labelsVector = survivedTrainSet; // labels of survived column

    vector<double> probVector;  // probability vector
    vector<double> errorVector; // error vector

    // gradient descent
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; i++)
    {
        probVector = probsCalculation(dataMatrix, weightsVector);
        errorVector = errorCalculation(labelsVector, probVector);
        weightsVector = weightsCalculation(dataMatrix, errorVector, weightsVector);
    }
    auto end = std::chrono::steady_clock::now();

    // coefficients and time
    cout << "Intercept: " << weightsVector[0] << endl;
    cout << "Sex Coefficiant: " << weightsVector[1] << endl;

    // source: https://www.techiedelight.com/measure-elapsed-time-program-chrono-library/
    cout << "Time for Algorithm to Compute: " << chrono::duration_cast<chrono::seconds>(end - start).count() << " seconds" << endl;

    cout << "\nUsing Test Data to Predict: " << endl;
    // predict

    vector<vector<double>> dataMatrixTest(pClassTestSet.size()); // test data matrix
    for (int i = 0; i < pClassTestSet.size(); i++)
    {
        // initialize and add values
        dataMatrixTest[i] = vector<double>(2);
        dataMatrixTest[i][0] = 1;
        dataMatrixTest[i][1] = sexTestSet[i];
    }

    // compute probability
    vector<double> probability = probTest(dataMatrixTest, weightsVector);

    // test metrics
    // ifelse(probs>0.5, 1, 0)
    vector<double> pred(probability.size());
    for (int i = 0; i < pred.size(); i++)
    {
        if (probability[i] > 0.5)
        {
            pred[i] = 1;
        }
        else
        {
            pred[i] = 0;
        }
    }

    // confusion matrix
    double TP = 0;
    double TN = 0;
    double FP = 0;
    double FN = 0;
    for (int i = 0; i < pred.size(); i++)
    {

        // survived
        if (survivedTestSet[i] == 1 && pred[i] == 1) // both say survived
        {
            TP++;
        }
        else if (survivedTestSet[i] == 1 && pred[i] == 0) // pred is wrong --> false neg bc of false death
        {
            FN++;
        }
        else if (survivedTestSet[i] == 0 && pred[i] == 0) // both say did not survive
        {
            TN++;
        }
        else if (survivedTestSet[i] == 0 && pred[i] == 1) // pred is wrong --> false pos bc of false survival
        {
            FP++;
        }
    }

    // print test metrics
    cout << "Accuracy: " << (TP + TN) / (TP + TN + FP + FN) << endl;
    cout << "Sensitivity: " << (TP) / (TP + FN) << endl;
    cout << "Specificity: " << (TN) / (TN + FP) << endl;

    return 0;
}