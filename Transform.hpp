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
    glm::mat4 M;
    
public:
    Transform(std::string name, Node* parent, glm::mat4 T);
    ~Transform();
    
    void addChild(Node* input);
    
    void draw(glm::mat4 C);
    void update();
};

#endif /* Transform_hpp */
