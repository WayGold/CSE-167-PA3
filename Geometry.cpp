//
//  Geometry.cpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Geometry.hpp"

void Geometry::loadModel(std::string objFilename){
    std::ifstream objFile(objFilename); // The obj file we are reading.

    // Check whether the file can be opened.
    if (objFile.is_open())
    {
        std::string line; // A line in the file.

        // Read lines from the file.
        while (std::getline(objFile, line))
        {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;
            
            // Read the first word of the line.
            std::string label;
            ss >> label;

            // If the line is about vertex (starting with a "v").
            if (label == "v"){
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                
                // Process the point. For example, you can save it to a.
                points.push_back(point);
            }
            
            // Load triangles
            if(label == "f"){
                glm::ivec3 triangle;
                
                std::string delimiter = "//";
                std::string line_1, line_2, line_3;
                
                ss >> line_1 >> line_2 >> line_3;
                
                triangle.x = std::stoi(line_1.substr(0, line_1.find(delimiter))) - 1;
                triangle.y = std::stoi(line_2.substr(0, line_2.find(delimiter))) - 1;
                triangle.z = std::stoi(line_3.substr(0, line_3.find(delimiter))) - 1;
                //std::cerr << "Reading in: "<< triangle.x << " " << triangle.y << " " << triangle.z << std::endl;
                triangles.push_back(triangle);
            }
            
            // Load vector normals
            if(label == "vn"){
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
        }
    }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }

    objFile.close();
}

void Geometry::setModelMatrix(glm::mat4 M){
    
}
