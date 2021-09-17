#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct vector2 { // 3d vector struture
    float X, Y;
    float mag;
    inline vector2( void ) {}
    inline vector2( const float x, const float y) { X = x; Y = y; }
};

int main() {
    // get info
    std::string inName, outName;
    float deviation;

    std::cout << "Enter .csv file name" << std::endl;
    std::cin >> inName;

    std::cout << "Enter output file name (followed by .csv)" << std::endl;
    std::cin >> outName;

    std::cout << "\nIngesting data" << std::endl;

    std::string line;
    std::vector<vector2> points;
    std::ifstream inFile(inName);

    if ( inFile.is_open() ){
        while ( getline (inFile,line) ){
            std::stringstream ss(line);
            std::vector<std::string> v;

            while ( ss.good() ) {
                std::string substr;
                getline(ss, substr, ',');
                v.push_back(substr);
            }

            if ( v.at(0) != "x" ) {
                vector2 curr;
                curr.X = stof(v.at(0));
                curr.Y = stof(v.at(1));

                points.push_back(curr);
            }
        }
    }

    <  
    for (auto i: points) {
        std::cout << i.X << "," << i.Y << std::endl;
    }



    return 0;
}