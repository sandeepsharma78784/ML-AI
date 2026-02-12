#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h> // OpenMP हेडर

int main() {
    std::string filename = "data.txt";
    std::vector<std::string> lines;
    std::string line;

    // 1. फाइल को पहले लोड करें या कुल पंक्तियाँ गिनें
    std::ifstream file(filename);
    if (!file.is_open()) return 1;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    int total_lines = lines.size();
    
    // 2. समानांतर क्षेत्र (Parallel Region) शुरू करें
    #pragma omp parallel for
    for (int i = 0; i < total_lines; ++i) {
        // प्रत्येक थ्रेड यहाँ अपना काम कर रहा है
        #pragma omp critical
        {
            // std::cout को थ्रेड-सेफ बनाने के लिए critical सेक्शन
            std::cout << "Thread " << omp_get_thread_num() 
                      << " reading line " << i << ": " << lines[i] << std::endl;
        }
    }

    return 0;
}
