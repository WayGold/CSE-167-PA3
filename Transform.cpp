//
//  Transform.cpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Transform.hpp"

Transform::Transform(std::string name, glm::mat4 T){
    this->name = name;
    this->T = T;
}

Transform::~Transform(){
    for(std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it){
        if(*it != nullptr){
            //delete(*it);
        }
    }
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C, std::vector<glm::vec3> all_center, std::vector<glm::vec3> all_norm, int & r_count, bool flag){
    glm::mat4 M_new = C * T;
    glm::vec3 tmp = center;
    center = glm::vec3(C * glm::vec4(center, 1.0f));
    
    if(name == "root"){
        double ftox = glm::dot((center - all_center.at(0)), all_norm.at(0));
        double ntox = glm::dot((center - all_center.at(1)), all_norm.at(1));
        double ltox = glm::dot((center - all_center.at(2)), all_norm.at(2));
        double rtox = glm::dot((center - all_center.at(3)), all_norm.at(3));
        double utox = glm::dot((center - all_center.at(4)), all_norm.at(4));
        double dtox = glm::dot((center - all_center.at(5)), all_norm.at(5));
        
//        std::cerr << "Check robot" << " with center at: " << glm::to_string(center) << std::endl << "ftox: " << ftox << ". ntox: " << ntox << ". ltox: " << ltox << ". rtox: " << rtox << ". utox: " << utox << ". dtox: " << dtox << std::endl;
        center = tmp;
        
        if(ftox < -radius || ntox < -radius || ltox < -radius || rtox < -radius || utox < -radius || dtox < -radius){
//            std::cerr << "Not drawing!" << std::endl;
            if(flag)
                return;
        }
        
        r_count++;
        
    }
    
    for(std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it){
//        std::cerr << std::endl << "calling draw on: " << (*it)->getName() << std::endl;
        (*it)->draw(shaderProgram, M_new, all_center, all_norm, r_count, flag);
    }
}

void Transform::addChild(Node* input){
//    std::cerr << "Adding child: " << input->getName() << " to " << this->getName() << std::endl;
    input->setParent(this);
    children.push_back(input);
}

void Transform::update(glm::mat4 C){
    this->T = C * T;
}

void Transform::set_center(glm::vec3 point){
    center = point;
}

glm::vec3 Transform::get_center(){
    return center;
}

float Transform::get_radius(){
    return radius;
}
