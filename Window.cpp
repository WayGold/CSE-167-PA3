//
//  Window.cpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Window.hpp"

int Window::width;
int Window::height;
int Window::event;
int Window::flag_n = 1;
int Window::mode = 1;
const char* Window::windowTitle = "CSE 167 PA3";

// Objects to display.
Transform* Window::root;
Transform* Window::leftArm;
Transform* Window::rightArm;
Transform* Window::leftLeg;
Transform* Window::rightLeg;
Transform* Window::tohead;
Transform* Window::tobody;

Geometry* Window::body;
Geometry* Window::limb;
Geometry* Window::head;
Geometry* Window::eyeball;
Geometry* Window::antenna;
Geometry* Window::sphere;


glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

glm::vec3 Window::lastPoint;
glm::vec3 Window::curPos;
glm::vec3 Window::rotAxis;

glm::vec3 lightPos(1.0f, 1.0f, 8.0f);

GLfloat Window::angle;

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.
GLuint Window::light_position;
GLuint Window::light_linear;
GLuint Window::viewPos;
GLuint Window::light_color;
GLuint Window::obj_color;
GLuint Window::material_diffuse;
GLuint Window::material_shininess;
GLuint Window::material_specular;
GLuint Window::flag;

bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!program)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    // Activate the shader program.
    glUseProgram(program);
    // Get the locations of uniform variables.
    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    modelLoc = glGetUniformLocation(program, "model");
    viewPos = glGetUniformLocation(program, "viewPos");
    light_position = glGetUniformLocation(program, "lightPos");
    light_linear = glGetUniformLocation(program, "light_linear");
    light_color = glGetUniformLocation(program, "lightColor");
    obj_color = glGetUniformLocation(program, "objectColor");
    flag = glGetUniformLocation(program, "flag");
    
    event = 0;
    
    return true;
}

bool Window::initializeObjects()
{
    glm::mat4 LAtrans = glm::translate(glm::vec3(2.0, 0.0, 0.0)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) * glm::mat4(1);
    
    std::cerr << "Left arm transform matrix: " << glm::to_string(LAtrans) << std::endl;
    //Transform
    root = new Transform("root", glm::mat4(1));
    leftArm = new Transform("leftArm", LAtrans);
    tobody = new Transform("tobody", glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)));
    
    //Geometry
    body = new Geometry("body");
    limb = new Geometry("limb");
    head = new Geometry("head");
    eyeball = new Geometry("eyeball");
    antenna = new Geometry("antenna");
    sphere = new Geometry("sphere");
    
    body->loadModel("body_s.obj");
    body->set_diffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    body->set_specular(glm::vec3(0.0f, 0.0f, 0.0f));
    body->set_shininess(0.0f);
    body->setColor(glm::vec3(0.8f, 0.5f, 0.0f));
    
    limb->loadModel("limb_s.obj");
    limb->setColor(glm::vec3(0.8f, 0.5f, 0.0f));
    
    head->loadModel("head_s.obj");
    head->setColor(glm::vec3(0.8f, 0.5f, 0.0f));
    
    eyeball->loadModel("eyeball_s.obj");
    eyeball->setColor(glm::vec3(0.8f, 0.5f, 0.0f));
    
    antenna->loadModel("antenna_s.obj");
    antenna->setColor(glm::vec3(0.8f, 0.5f, 0.0f));
    
    // light src
    sphere->loadModel("sphere.obj");
    sphere->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    sphere->set_diffuse(glm::vec3(1.0f, 1.0f,  1.0f));
    sphere->set_specular(glm::vec3(1.0f, 1.0f,  1.0f));
    sphere->set_shininess(1.0f);
    sphere->scale(glm::scale(glm::vec3(0.05f, 0.05f, 0.05f)));
    sphere->translate(glm::translate(lightPos));
    
    // building scene graph
    root->addChild(leftArm);
    root->addChild(tobody);
    
    leftArm->addChild(limb);
    tobody->addChild(body);
    
    root->addChild(sphere);
    //root->addChild(head);
    
    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.
    delete root;

    // Delete the shader program.
    glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);
    glfwSetScrollCallback(window, scroll_callback);
    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(60.0),
        double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.
    //root->update();
}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    // light properties
    glUniform3fv(light_position, 1, glm::value_ptr(lightPos));
    glUniform3fv(viewPos, 1, glm::value_ptr(eye));
    glUniform1f(light_linear, 0.09f);
    glUniform3fv(light_color, 1, glm::value_ptr(glm::vec3(0.0f, 0.5f, 0.9f)));
    glUniform1i(flag, flag_n);

    // Render the object.
    root->draw(program, glm::mat4(1));

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            // Close the window. This causes the program to also terminate.
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
                
        // Mode 1
        case GLFW_KEY_1:
            mode = 1;
            break;
                
        // Mode 2
        case GLFW_KEY_2:
            mode = 2;
            break;
        
        // Mode 3
        case GLFW_KEY_3:
            mode = 3;
            break;
        
        case GLFW_KEY_N:
            if(flag_n){
                glUniform1i(flag, 0);
                flag_n = 0;
            }
            else{
                glUniform1i(flag, 1);
                flag_n = 1;
            }
            
            break;
                
        default:
            break;
        }
    }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    
    glm::vec3 direction = glm::normalize(glm::vec3(0, 0, 0) - lightPos);
    
    if(yoffset > 0){
        sphere->translate(glm::translate(-direction));
        lightPos = sphere->getModel() * glm::vec4(0, 0, 0, 1);
    }
    else{
        sphere->translate(glm::translate(direction));
        lightPos = sphere->getModel() * glm::vec4(0, 0, 0, 1);
    }
}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos){
    // no action detected
    if(event == 0) return;
    // left press and hold
    if(event == 1){
        // Map the mouse position to a logical sphere location.
        // Keep it in the class variable lastPoint.
        glm::vec2 point = glm::vec2(xpos, ypos);
        
        // get world coord of first point click
        lastPoint = trackBallMapping(point);
        // set event to handle cursor moving
        event = 2;
        return;
    }
    
    // Get the current point in world coord
    curPos = trackBallMapping(glm::vec2(xpos, ypos));
    /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
    angle = glm::acos(std::min(1.0f, glm::dot(lastPoint, curPos)));
    /* Cross product to get the rotation axis, but it's still in camera coordinate */
    rotAxis  = glm::cross(lastPoint, curPos);
    
    // move camera
    if(mode == 1){
//        currentObj->rotate(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis));
        glm::vec3 camDirec = glm::normalize(eye - center);
        camDirec = glm::vec3(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis) * glm::vec4(camDirec.x, camDirec.y, camDirec.z, 0.0));
        //view = glm::lookAt(eye, , up);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    }
    else if(mode == 2){
        sphere->rotate(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis));
        glm::vec4 rotLight = glm::vec4(lightPos.x, lightPos.y, lightPos.z, 1.0f);
        rotLight = glm::rotate(glm::degrees(angle) * 0.05f, rotAxis) * rotLight;
        lightPos.x = rotLight.x;
        lightPos.y = rotLight.y;
        lightPos.z = rotLight.z;
    }
    
    glm::vec2 point = glm::vec2(xpos, ypos);
           
    // get world coord of first point click
    lastPoint = trackBallMapping(point);
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods){
    event = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
}

glm::vec3 Window::trackBallMapping(glm::vec2 point){
    glm::vec3 v;
    float d;
    
    v.x = (2.0f * point.x - width) / width;
    v.y = (height - 2.0f * point.y) / height;
    v.z = 0.0f;
    
    d = glm::length(v);
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrtf(1.001f - d * d);
    
    v = glm::normalize(v);
    return v;
}


