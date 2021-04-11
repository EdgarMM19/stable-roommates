#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

//https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c

std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

std::mt19937 gen(seed);
std::uniform_int_distribution<int> distrib(0, int((1LL<<32)-1));

int randomNumber(int n){
    return rand()%n;
}

vector<int> generateRandomPermutation(int len, bool startAtOne){
    //https://en.wikipedia.org/wiki/Fisher–Yates_shuffle
    vector<int> v(len);
    for(int i = 0; i < len; ++i) v[i] = i + startAtOne;
    for(int i = len-1; i >= 1; --i){
        int j = randomNumber(i+1);
        swap(v[i], v[j]);
    }
    return v;
}

