#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath> // float absolute
#include <ctime> // UNIX epoch time

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
    std::time_t startTime = std::time(0);
    std::vector<vector3> sortedPoints; // sorted points to return
    float lowestVectorPosition; // global lowest vector position
    for (auto i : points) { // range loop
        float lowestPosition = 100000000; // default value

        for(int v=0; v < points.size(); v++){ // newb loop to index
            if ( returnPosition(points.at(v)) < lowestPosition) {
                lowestPosition = returnPosition(points.at(v)); // get new lowest position
                lowestVectorPosition = v; // lowest position for removing later
            }
        }
        vector3 newVector; // data duplication
        newVector.X = points.at(lowestVectorPosition).X;
        newVector.Y = points.at(lowestVectorPosition).Y;
        newVector.Z = points.at(lowestVectorPosition).Z;
        sortedPoints.push_back(newVector); // add as next lowest value

        points.erase(points.begin() + lowestVectorPosition); // delete from existing values
    }

    std::cout << "Position sorted " << sortedPoints.size() << " points in " << std::time(0) - startTime << " seconds" << std::endl;
    return sortedPoints;
}

std::vector<vector3> magnitudeSort(std::vector<vector3> points) { // Requires the lowest point to be on position 0
    std::time_t startTime = std::time(0); // start time of sort
    std::vector<vector3> sortedPoints; // to return
    float lowestMagPosition;

    vector3 last; // first value in vector
    last.X = points.at(0).X;
    last.Y = points.at(0).Y;
    last.Z = points.at(0).Z;
    sortedPoints.push_back(last);

    points.erase(points.begin()); // remove first 
    
    while (points.size() != 0) { // while no values to find
        vector3 closestPoint;
        float lowestMagnitude = 100000000; // big value as default

        for(int v=0; v < points.size(); v++){
            if ( !(last.X == points.at(v).X && last.Y == points.at(v).Y && last.Z == points.at(v).Z) // eliminate duplicate values
            && returnMagnitude(last,points.at(v)) < lowestMagnitude) { // lower than lowest current magnitude
                lowestMagnitude = returnMagnitude(last,points.at(v)); // set lowest mag to the new one
                closestPoint = points.at(v); // new closest point
                lowestMagPosition = v; // for indexing later
            }
            //std::cout << "MAGNITUDE FROM: (" << last.X << ", " << last.Y << ", " << last.Z << ") TO (" << points.at(v).X << ", " << points.at(v).Y << ", " << points.at(v).Z << ") - MAG:" << returnMagnitude(last,points.at(v)) << std::endl;
        }

        vector3 newVector; // new vector3 to put as next value
        newVector.X = closestPoint.X;
        newVector.Y = closestPoint.Y;
        newVector.Z = closestPoint.Z;
        sortedPoints.push_back(newVector);

        //std::cout << "CLOSEST: (" << last.X << ", " << last.Y << ", " << last.Z << ") TO (" << newVector.X << ", " << newVector.Y << ", " << newVector.Z << ") - MAG: " << returnMagnitude(last,newVector) << std::endl;

        last.X = points.at(lowestMagPosition).X; // duplicating data
        last.Y = points.at(lowestMagPosition).Y;
        last.Z = points.at(lowestMagPosition).Z;

        points.erase(points.begin() + lowestMagPosition); // erase the closest value from vector
    }

    std::cout << "Magnitude sorted " << sortedPoints.size() << " points in " << std::time(0) - startTime << " seconds" << std::endl;
    return sortedPoints;
}

std::vector<vector3> ingest( std::string inFileName ) {
    std::time_t startTime = std::time(0); // start time of ingest
    std::string line;
    std::vector<vector3> points;
    std::ifstream inFile(inFileName);

    if ( inFile.is_open() ){
        while ( getline (inFile,line) ){
            std::stringstream ss(line); // stringstream because lazy
            std::vector<std::string> v; // should switch to array soon because of memory usage

            while ( ss.good() ) {
                std::string substr;
                getline(ss, substr, ','); // break by comma
                v.push_back(substr); // push into vector for easy 1,2,3
            }

            if ( v.at(0) != "x" ) { // If not first line
                vector3 curr; // make new vector3 to add to point vector
                curr.X = stof(v.at(0));
                curr.Y = stof(v.at(1));
                curr.Z = stof(v.at(2));

                points.push_back(curr);
            }
        }
    }

    std::cout << "Ingested " << points.size() << " points in " << std::time(0) - startTime << " seconds" << std::endl;
    return points;
}

void exportPoints( std::vector<vector3> points, std::string outputFileName ) { // Export points to file
    std::time_t startTime = std::time(0);
    std::ofstream outFile(outputFileName); // New file (overwrite if already exists)
    outFile << "x,y,z" << std::endl;

    for ( auto i: points ){ // output each value
        outFile << i.X << "," << i.Y << "," << i.Z << std::endl;
    }
    outFile.close(); // close manually

   std::cout << "Exported " << points.size() << " points in " << std::time(0) - startTime << " seconds" << std::endl;
}

int main() {
    // get info
    std::string inName, outName;

    std::cout << "Enter .csv file name" << std::endl;
    std::cin >> inName;

    std::cout << "Enter output file name (followed by .csv)" << std::endl;
    std::cin >> outName;
    
    std::cout << "\nIngesting data" << std::endl;
    std::vector<vector3> inPoints = ingest("input2.csv"); // Ingest

    std::vector<vector3> newPoints = positionSort(inPoints); // Sort by position
    std::vector<vector3> sortedPoints = magnitudeSort(newPoints); // Sort by magnitude

    exportPoints(sortedPoints,"output.csv"); // Export returned points

    return 0;
}