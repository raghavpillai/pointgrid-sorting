#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath> // float absolute

struct vector2 { // 3d vector struture
    float X, Y;
    inline vector2( void ) {}
    inline vector2( const float x, const float y) { X = x; Y = y; }
};

float returnPosition(vector2 A) { return (A.X) + (A.Y); }
float returnMagnitude(vector2 A, vector2 B) { return std::abs(A.X - B.X) + std::abs(A.Y - B.Y); }

std::vector<vector2> positionSort(std::vector<vector2> points) { // Sort coordinates by position, initial rough sort
    std::vector<vector2> sortedPoints;
    float lowestVectorPosition;
    for (auto i : points) {
        float lowestPosition = 100000000;

        for(int v=0; v < points.size(); v++){
            if ( returnPosition(points.at(v)) < lowestPosition) {
                lowestPosition = returnPosition(points.at(v));
                lowestVectorPosition = v;
            }
        }
        vector2 newVector;
        newVector.X = points.at(lowestVectorPosition).X;
        newVector.Y = points.at(lowestVectorPosition).Y;

        sortedPoints.push_back(newVector);

        points.erase(points.begin() + lowestVectorPosition);
    }

    return sortedPoints;
}

std::vector<vector2> magnitudeSort(std::vector<vector2> points) { // Requires the lowest point to be on position 0
    std::vector<vector2> sortedPoints;
    float lowestMagPosition;

    vector2 last;
    last.X = points.at(0).X;
    last.Y = points.at(0).Y;
    sortedPoints.push_back(last);

    points.erase(points.begin());
    
    while (points.size() != 0) {
        vector2 closestPoint;
        float lowestMagnitude = 100000000;

        for(int v=0; v < points.size(); v++){
            
            std::cout << "MAGNITUDE FROM: (" << last.X << ", " << last.Y << ") TO (" << points.at(v).X << ", " << points.at(v).Y << ") - MAG:" << returnMagnitude(last,points.at(v)) << std::endl;

            if ( !(last.X == points.at(v).X && last.Y == points.at(v).Y)
            && returnMagnitude(last,points.at(v)) < lowestMagnitude) {
                lowestMagnitude = returnMagnitude(last,points.at(v));
                closestPoint = points.at(v);
                lowestMagPosition = v;
            }
        }

        vector2 newVector;
        newVector.X = closestPoint.X;
        newVector.Y = closestPoint.Y;
        sortedPoints.push_back(newVector);

        std::cout << "CLOSEST: (" << last.X << ", " << last.Y << ") TO (" << newVector.X << ", " << newVector.Y << ") - MAG: " << returnMagnitude(last,newVector) << std::endl;

        last.X = points.at(lowestMagPosition).X;
        last.Y = points.at(lowestMagPosition).Y;

        points.erase(points.begin() + lowestMagPosition);
    }

    for (auto i: sortedPoints){
        std::cout << i.X << "," << i.Y << std::endl;
    }

    return sortedPoints;
}

void exportPoints( std::vector<vector2> points, std::string outputFileName ) { // Export points to file
    std::ofstream outFile(outputFileName); // New file (overwrite if already exists)
    outFile << "x,y" << std::endl;

    for ( auto i: points ){
        outFile << i.X << "," << i.Y << "," << std::endl;
    }
    outFile.close();

   //std::cout << "Exported " << points.size() << " points" << std::endl;
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

    std::vector<vector2> newPoints = positionSort(points);
    std::vector<vector2> sortedPoints = magnitudeSort(newPoints);

    exportPoints(sortedPoints,"output.csv");

    return 0;
}