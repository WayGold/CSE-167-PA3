//
//  Geometry.hpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <stdio.h>
#include "Node.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

class Geometry : public Node {
    
private:
    std::vector<glm::vec3> points;
    std::vector<glm::ivec3> triangles;
    std::vector<glm::vec3> normals;
    
public:
    Geometry();
    void setModelMatrix(glm::mat4 M);
    void loadModel(std::string objFilename);
    void draw(glm::mat4 C);
    void update();
};

#endif /* Geometry_hpp */
