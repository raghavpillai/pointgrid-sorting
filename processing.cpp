#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath> // float absolute

struct vector3 { // 3d vector struture
    float X, Y, Z;
    inline vector3( void ) {}
    inline vector3( const float x, const float y, const float z) { X = x; Y = y; Z = z; }
};

std::vector<vector3> positionSort(std::vector<vector3> points);
std::vector<vector3> magnitudeSort(std::vector<vector3> points);
std::vector<vector3> ingest( std::string inFileName );
void exportPoints( std::vector<vector3> points, std::string outputFileName );

float returnPosition(vector3 A) { return (A.X) + (A.Y) + (A.Z); }
float returnMagnitude(vector3 A, vector3 B) { return std::abs(A.X - B.X) + std::abs(A.Y - B.Y) + std::abs(A.Z - B.Z); }

std::vector<vector3> positionSort(std::vector<vector3> points) { // Sort coordinates by position, initial rough sort
    std::vector<vector3> sortedPoints;
    float lowestVectorPosition;
    for (auto i : points) {
        float lowestPosition = 100000000;

        for(int v=0; v < points.size(); v++){
            if ( returnPosition(points.at(v)) < lowestPosition) {
                lowestPosition = returnPosition(points.at(v));
                lowestVectorPosition = v;
            }
        }
        vector3 newVector;
        newVector.X = points.at(lowestVectorPosition).X;
        newVector.Y = points.at(lowestVectorPosition).Y;
        newVector.Z = points.at(lowestVectorPosition).Z;

        sortedPoints.push_back(newVector);

        points.erase(points.begin() + lowestVectorPosition);
    }

    std::cout << "Position sorted " << sortedPoints.size() << " points" << std::endl;
    return sortedPoints;
}

std::vector<vector3> magnitudeSort(std::vector<vector3> points) { // Requires the lowest point to be on position 0
    std::vector<vector3> sortedPoints;
    float lowestMagPosition;

    vector3 last;
    last.X = points.at(0).X;
    last.Y = points.at(0).Y;
    last.Z = points.at(0).Z;
    sortedPoints.push_back(last);

    points.erase(points.begin());
    
    while (points.size() != 0) {
        vector3 closestPoint;
        float lowestMagnitude = 100000000;

        for(int v=0; v < points.size(); v++){
            if ( !(last.X == points.at(v).X && last.Y == points.at(v).Y && last.Z == points.at(v).Z)
            && returnMagnitude(last,points.at(v)) < lowestMagnitude) {
                lowestMagnitude = returnMagnitude(last,points.at(v));
                closestPoint = points.at(v);
                lowestMagPosition = v;
            }
            //std::cout << "MAGNITUDE FROM: (" << last.X << ", " << last.Y << ", " << last.Z << ") TO (" << points.at(v).X << ", " << points.at(v).Y << ", " << points.at(v).Z << ") - MAG:" << returnMagnitude(last,points.at(v)) << std::endl;
        }

        vector3 newVector;
        newVector.X = closestPoint.X;
        newVector.Y = closestPoint.Y;
        newVector.Z = closestPoint.Z;
        sortedPoints.push_back(newVector);

        //std::cout << "CLOSEST: (" << last.X << ", " << last.Y << ", " << last.Z << ") TO (" << newVector.X << ", " << newVector.Y << ", " << newVector.Z << ") - MAG: " << returnMagnitude(last,newVector) << std::endl;

        last.X = points.at(lowestMagPosition).X;
        last.Y = points.at(lowestMagPosition).Y;
        last.Z = points.at(lowestMagPosition).Z;

        points.erase(points.begin() + lowestMagPosition);
    }

    std::cout << "Magnitude sorted " << sortedPoints.size() << " points" << std::endl;
    return sortedPoints;
}

std::vector<vector3> ingest( std::string inFileName ) {
    std::string line;
    std::vector<vector3> points;
    std::ifstream inFile(inFileName);

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
                vector3 curr;
                curr.X = stof(v.at(0));
                curr.Y = stof(v.at(1));
                curr.Z = stof(v.at(2));

                points.push_back(curr);
            }
        }
    }

    std::cout << "Ingested " << points.size() << " points" << std::endl;
    return points;
}

void exportPoints( std::vector<vector3> points, std::string outputFileName ) { // Export points to file
    std::ofstream outFile(outputFileName); // New file (overwrite if already exists)
    outFile << "x,y,z" << std::endl;

    for ( auto i: points ){
        outFile << i.X << "," << i.Y << "," << i.Z << std::endl;
    }
    outFile.close();

   std::cout << "Exported " << points.size() << " points" << std::endl;
}

int main() {
    // get info
    std::string inName, outName;

    /*std::cout << "Enter .csv file name" << std::endl;
    std::cin >> inName;

    std::cout << "Enter output file name (followed by .csv)" << std::endl;
    std::cin >> outName;*/

    std::cout << "\nIngesting data" << std::endl;
    std::vector<vector3> inPoints = ingest("input.csv"); // Ingest

    std::vector<vector3> newPoints = positionSort(inPoints);
    std::vector<vector3> sortedPoints = magnitudeSort(newPoints);

    exportPoints(sortedPoints,"output.csv");

    return 0;
}