//
//  Transform.hpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include "Node.hpp"

class Transform : public Node{

private:
    glm::mat4 T;
    std::list<Node*> children;
    
public:
    Transform(std::string name, glm::mat4 T);
    ~Transform();
    
    void addChild(Node* input);
    
    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
};

#endif /* Transform_hpp */
