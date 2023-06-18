
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream> 
#include <math.h>
#include <sstream> 
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

//https://www.youtube.com/watch?v=O1F3_KUA7PY for forward selection

void forwardSelection();//(std::vector<int>);
void backwardSelection(std::vector<int>);
double accuracyTest(std::vector<int>);
int num_features;
std::vector<std::vector<long double>> data;
std::vector<long double> vector_feature;
void normalize();

//small.txt: (given example) 
//  forward: {5,3} : 92%
//  backward: {2, 4, 5, 7, 10} : 82%

//big.txt (given example)
//  forward: {27,1} : 95.5%
//  backward: {2, 3, 4, 5, 8, 9, 10, 11, 
//              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
//              22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
//              32, 33, 34, 35, 36, 37, 38, 39, 40} : 72.2%

int main()
{
    std::vector<int> vec;

    srand((unsigned)time(NULL));
    int choice;

    std::string filename;


    std::cout << "Welcome to Orlean Lazaro's Feature Selection Algorithm" << std::endl;
    std::cout << "Type in the name of the file to test: " << std::endl;
    std::cin >> filename;

    std::cout << "Type the number of algorithm you would like to use: " << std::endl;
    std::cout << "1: Forward Selection\n";
    std::cout << "2: Backward Elimination\n";
    std::cin >> choice;

    //https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    //link for reading files
    std::ifstream file;
    file.open(filename.c_str());
    std::string str, str_val; 
    

    getline(file, str);
    std::stringstream sstream(str);
    while (sstream >> str_val) {
        vector_feature.push_back(stold(str_val));
        data.push_back(vector_feature);
        vector_feature.clear();
    }

    while (getline(file, str)) {
        std::stringstream sstream(str);
        for (int i = 0; i < data.size(); i++) {
            if (sstream >> str_val) {
                data.at(i).push_back(stold(str_val)); //std::cout << str_val << " ";
            }
        }
    }
    num_features = data.size();
    std::cout << "This dataset has " << num_features - 1 <<
            " (not including the class attribute), with " << data[0].size() << "instances" << std::endl;
    
    std::cout << "Please wait while I normalize the data ...";
    normalize();
    std::cout << " Done!" << std::endl;
    
    if (choice == 1) { forwardSelection(); }
    else if (choice == 2) { backwardSelection(vec); }
    else { std::cout << "Invalid input" << std::endl; }
}

void forwardSelection() {
    double max_acc = -1;
    std::vector<int> best; //stores the best vector
    std::vector<int> index_Best;


    for (int i = 1; i < num_features; i++) {
        std::vector<int> temp;
        std::vector<int> index; //like feaure 1, feature 3, ...
        double best_local = 0.0, index_choice;
        for (int k = 1; k < num_features; k++) {
            std::vector<int> temp_best = best;
            bool recorded = false;
            if (temp_best.size() < 1) { temp_best.push_back(k); recorded = true; }
            else {
                bool isIn = false;
                for (int x = 0; x < temp_best.size(); x++) {
                    if (temp_best[x] == k) { isIn = true; /*std::cout << "temp_best[x]: " << temp_best[x] << " and k: " << k << std::endl;*/ }
                }
                if (!isIn) { temp_best.push_back(k); recorded = true; }
            }


            if (recorded) {
                double holder = accuracyTest(temp_best);
                temp.push_back(holder);

                std::cout << "Using feature(s) {";// << k + 1;
                for (int x = 0; x < temp_best.size(); x++) {
                    std::cout << temp_best[x];
                    if (temp_best.size() - x > 1) { std::cout << ", "; }
                }
                std::cout << "} accuracy is " << holder /*temp[k]*/ << "%" << std::endl;
                if (best_local < holder) { best_local = holder; index_choice = k; }
            }
        }
        if (/*temp[index_choice] */ best_local < max_acc) {
            std::cout << "Warning, accuracy has decreased!" << std::endl;
            break;
        }
        std::cout << "Feature set {" << index_choice << "} was best, accuracy is " << best_local /*temp[index_choice]*/ << "%" << std::endl;
        max_acc = best_local;//temp[index_choice];
        best.push_back(index_choice);


    }
    std::cout << "Finished search!! Best feature susbset is {";
    for (int j = 0; j < best.size(); j++) {
        std::cout << best[j];
        if (best.size() - 1 > j) {
            std::cout << ", ";
        }
        else {
            std::cout << "}";
        }
    }
    std::cout << " which has the accuracy of " << max_acc << "%" << std::endl;
}

void backwardSelection(std::vector<int> vec) {
    double max_acc = -1;
    std::vector<int> best; //stores the best vector (0, 1, 2, 3)
    std::vector<int> index_Best;

    std::cout << "Begin search. \nUsing feature(s) {";
    for (int i = 0; i < num_features; i++) {
        if (i > 0 && i < num_features - 1) { std::cout << i << ", "; }
        else if (i > 0 && i == num_features - 1) { std::cout << i; }

        if (i > 0) { best.push_back(i); } //push all values except for 0

    }
    max_acc = accuracyTest(best);

    std::cout << "} accuracy is " << max_acc << "%" << std::endl;
    for (int i = 0; i < num_features; i++) {
        std::vector<int> temp;
        std::vector<int> index;
        double best_local = 0, index_choice; // get the best nodes in the current itteration
        for (int k = 0; k < best.size(); k++) {
            std::vector<int> temp_best = best;
            temp_best.erase(temp_best.begin() + k);
            std::cout << "Using feature(s) {";
            for (int j = 0; j < temp_best.size(); j++) {
                if (j < temp_best.size() - 1) { std::cout << temp_best[j] << ", "; }
                else { std::cout << temp_best[j]; }
            }
            double hold_acc = accuracyTest(temp_best);
            std::cout << "} accuracy is " << hold_acc << "%" << std::endl;

            if (hold_acc > best_local) {
                best_local = hold_acc;
                index_choice = k;
            }
        }
        if (best_local > max_acc) {
            max_acc = best_local;
            best.erase(best.begin() + index_choice);
            std::cout << "Feature set {";
            for (int j = 0; j < best.size(); j++) {
                if (j < best.size() - 1) { std::cout << best[j] << ", "; }
                else { std::cout << best[j]; }
            }
            std::cout << "} was best, accuracy is " << max_acc << "%" << std::endl;
        }
        else {
            std::cout << "Warning, accuracy has decreased!" << std::endl;
            break;
        }
    }

    std::cout << "Finished sear!! Best feature susbset is {";
    for (int j = 0; j < best.size(); j++) {
        if (j < best.size() - 1) { std::cout << best[j] << ", "; }
        else { std::cout << best[j]; }
    }
    std::cout << "} which has the accuracy of " << max_acc << "%" << std::endl;
}

double accuracyTest(std::vector<int> vec) {
    int correct = 0;
    std::vector<int> feature_num = vec;
    for (int i = 0; i < data[0].size(); i++) {
        double near_dist = 10000;
        int near_location = 10000;
        int label;
        for (int k = 0; k < data[0].size(); k++) {
            if (i != k) {
                double dist = 0;
                for (int j = 0; j < feature_num.size(); j++) {
                    dist += pow((data[feature_num[j]][i] - data[feature_num[j]][k]), 2);
                }
                double tmp_dist = dist; 
                if (tmp_dist < near_dist) {
                    near_dist = tmp_dist;
                    near_location = k;
                    label = data[0][k];
                }
            }
        }
        if (data[0][i] == label) {
            correct++;
        }
    }
    double acc = ((double)correct / (double)data[0].size()) * 100.0;
    return acc;
}

void normalize() { //got the formula from piazza
    double norm;

    for (int i = 1; i < data.size(); i++) {
        double temp_val = 0;
        double mean;
        for (int k = 0; k < data[i].size(); k++) {
            temp_val += data[i][k];
        }
        mean = temp_val / data[0].size();
        double std_val = 0;
        for (int x = 0; x < data[i].size(); x++) {
            std_val += pow((data[i][x] - mean), 2);
        }
        double stds = sqrt(std_val / data[0].size());

        for (int j = 0; j < data[i].size(); j++) {
            data[i][j] = (data[i][j] - mean) / stds;
        }
    }
}


