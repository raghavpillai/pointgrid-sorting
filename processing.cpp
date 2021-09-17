#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct vector2 { // 3d vector struture
    long X, Y;
    inline vector2( void ) {}
    inline vector2( const long x, const long y) { X = x; Y = y; }
};

int returnMagnitude(vector2 A, vector2 B) {
    return abs(A.X - B.X) + abs(A.Y - B.Y);
}

std::vector<vector2> sortVector(std::vector<vector2> points) {
    std::vector<vector2> newPoints;
    
    for (auto i: points) {
        vector2 closestPoint;

        int lowestMagnitude = 100000000;
        for (auto v: points) {
            //std::cout << (i.X == v.X && i.Y == v.Y) << " SAME (" << i.X << "," << i.Y << "  -  " << v.X << "," << v.Y << ")" << std::endl;
            //std::cout << (returnMagnitude(i,v) < lowestMagnitude) << " LESS (" << i.X << "," << i.Y << "  -  " << v.X << "," << v.Y << ")" << std::endl;
            
            if ( !(i.X == v.X && i.Y == v.Y) && returnMagnitude(i,v) < lowestMagnitude) {
                lowestMagnitude = returnMagnitude(i,v);
                closestPoint = v;
            }
        }
        
        std::cout << "CLOSEST TO (" << i.X << "," << i.Y << "): (" << closestPoint.X << "," << closestPoint.Y << "), MAGNITUDE: " << lowestMagnitude << std::endl;
    }

    return newPoints;
}

int main() {
    // get info
    std::string inName, outName;
    float deviation;

    //std::cout << "Enter .csv file name" << std::endl;
    //std::cin >> inName;

    std::string line;
    std::vector<vector2> points;
    //std::ifstream inFile(inName);
    std::ifstream inFile("input.csv");

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

    sortVector(points);



    return 0;
}