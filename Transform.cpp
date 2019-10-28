//
//  Transform.cpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Transform.hpp"

Transform::Transform(std::string name, Node* parent, glm::mat4 T){
    this->name = name;
    this->parent = parent;
    M = T;
}

Transform::~Transform(){
    for(std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it){
        delete(*it);
    }
}

void Transform::draw(glm::mat4 C){
    glm::mat4 M_new = C * M;
    for(std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it){
        (*it)->draw(M_new);
    }
}

void Transform::addChild(Node *input){
    children.push_back(input);
}
