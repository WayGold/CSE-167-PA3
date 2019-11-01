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
int Window::r_count = 0;
double Window::counter = 0;
double Window::fov = 60;
double Window::fake_fov = fov;
double Window::near = 1.0;
double Window::far = 1000.0;
double Window::Hnear;
double Window::Hfar;
double Window::Wnear;
double Window::Wfar;
double Window::midDis, Window::Hmid, Window::Wmid;
bool Window::bbox = false;
bool Window::culling = true;
bool Window::culling_debug = true;
const char* Window::windowTitle = "CSE 167 PA3";

// Objects to display.
Transform* Window::root;
Transform* Window::god;
Transform* Window::leftArm;
Transform* Window::rightArm;
Transform* Window::leftLeg;
Transform* Window::rightLeg;
Transform* Window::tohead;
Transform* Window::tobody;
Transform* Window::sgleye;
Transform* Window::toCenter;

Transform* Window::paddUp;
Transform* Window::paddDown;
Transform* Window::paddDepth;
Transform* Window::paddLeft;
Transform* Window::paddRight;

std::vector<Transform*> Window::all_nodes;

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
glm::vec3 Window::fc, Window::ftl, Window::ftr, Window::fbl, Window::fbr, Window::nc, Window::ntl, Window::ntr, Window::nbl,             Window::nbr, Window::Nnormal, Window::Fnormal, Window::Lnormal, Window::Rnormal, Window::Unormal, Window::Dnormal,             Window::lc, Window::rc, Window::uc, Window::dc, Window::mc;

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
    glm::mat4 RAtrans = glm::translate(glm::vec3(3.0, 0.0, 1.0)) * glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)) * glm::mat4(1);
    glm::mat4 LAtrans = glm::translate(glm::vec3(-3.0, 0.0, 1.0)) * glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)) * glm::mat4(1);
    glm::mat4 HDtrans = glm::translate(glm::vec3(0.0, 2.6, 0.0)) * glm::scale(glm::vec3(0.2f, 0.2f, 0.2f)) * glm::mat4(1);
    glm::mat4 RLtrans = glm::translate(glm::vec3(1.0, -3.0, 1.0)) * glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)) * glm::mat4(1);
    glm::mat4 LLtrans = glm::translate(glm::vec3(-1.0, -3.0, 1.0)) * glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)) * glm::mat4(1);
    glm::mat4 Etrans = glm::translate(glm::vec3(0.0, 3.0, 9.0)) * glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)) * glm::mat4(1);
    
    glm::mat4 padd_Right = glm::translate(glm::vec3(25.0, 0.0, 0.0)) * glm::mat4(1);
    glm::mat4 padd_Left = glm::translate(glm::vec3(-25.0, 0.0, 0.0)) * glm::mat4(1);
    glm::mat4 padd_Up = glm::translate(glm::vec3(0.0, 0.0, 0.0)) * glm::mat4(1);
    glm::mat4 padd_Depth = glm::translate(glm::vec3(0.0, 0.0, -25.0)) * glm::mat4(1);
    
    //std::cerr << "Left arm transform matrix: " << glm::to_string(RAtrans) << std::endl;
    //Transform
    root = new Transform("root", glm::mat4(1));
    god = new Transform("god", glm::mat4(1));
    rightArm = new Transform("RightArm", RAtrans);
    leftArm = new Transform("LeftArm", LAtrans);
    tobody = new Transform("tobody", glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)));
    tohead = new Transform("tohead", HDtrans);
    leftLeg = new Transform("leftLeg", LLtrans);
    rightLeg = new Transform("rightLeg", RLtrans);
    sgleye = new Transform("eye", Etrans);
    
    
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
    body->setColor(glm::vec3(0.8f, 0.0f, 0.5f));
    
    limb->loadModel("limb_s.obj");
    limb->setColor(glm::vec3(0.3f, 0.3f, 0.3f));
    
    head->loadModel("head_s.obj");
    head->setColor(glm::vec3(0.8f, 0.0f, 0.5f));
    
    eyeball->loadModel("eyeball_s.obj");
    eyeball->setColor(glm::vec3(0.0f, 0.0f, 0.0f));
    
    antenna->loadModel("antenna_s.obj");
    antenna->setColor(glm::vec3(0.8f, 0.5f, 0.0f));
    
    // light src
    sphere->loadModel("sphere.obj");
    sphere->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    sphere->set_diffuse(glm::vec3(1.0f, 1.0f,  1.0f));
    sphere->set_specular(glm::vec3(1.0f, 1.0f,  1.0f));
    sphere->set_shininess(1.0f);
    sphere->scale(glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)));
    
    // building scene graph
    root->addChild(rightArm);
    root->addChild(leftArm);
    root->addChild(tobody);
    root->addChild(tohead);
    root->addChild(rightLeg);
    root->addChild(leftLeg);
    
    rightArm->addChild(limb);
    leftArm->addChild(limb);
    tobody->addChild(body);
    tohead->addChild(head);
    leftLeg->addChild(limb);
    rightLeg->addChild(limb);
    tohead->addChild(sgleye);
    sgleye->addChild(eyeball);
    
    // setting center and adding the bounding sphere
    root->set_center(body->calc_center());
    
    glm::mat4 Ctrans = glm::translate(body->calc_center());
    toCenter = new Transform("toCenter", Ctrans);
    root->addChild(toCenter);
    toCenter->addChild(sphere);
    
    /* START GENERATING 100 ROBOTS*/
    god->addChild(root);
    root->set_center(body->calc_center());
    
    // ptr to keep track of operation
    Transform* current = god;
    
    // loop to create 100 robots
    for(int i = 0; i < 10; i++){
        //set center ptr to current
        Transform* c_point = current;
        // copy 5 to the left
        for(int j = 0; j < 5; j++){
            Transform* tmp = new Transform(std::to_string(i), padd_Left);
            current->addChild(tmp);
            tmp->addChild(root);
            current = tmp;
        }
        // ptr back to center
        current = c_point;
        // copy 5 to the right
        for(int j = 0; j < 4; j++){
            Transform* tmp = new Transform(std::to_string(i), padd_Right);
            current->addChild(tmp);
            tmp->addChild(root);
            current = tmp;
        }
        // ptr back to center
        current = c_point;
        if(i == 9){
            break;
        }
        // move up to the next row
        Transform* tmp = new Transform(std::to_string(i), padd_Up * padd_Depth);
        current->addChild(tmp);
        tmp->addChild(root);
        current = tmp;
    }
    
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
    Window::projection = glm::perspective(glm::radians(fov),
                                          double(width) / (double)height, near, far);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.
    glm::mat4 trans = glm::mat4(1.0f);
    
    glm::mat4 ani_front = glm::rotate(trans, glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 inverse_front = glm::transpose(ani_front);
    glm::mat4 ani_back = glm::rotate(trans, glm::radians(-2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 inverse_back = glm::transpose(ani_back);
    
    //std::cerr << "Value of sin(counter): " << std::sin(counter) << std::endl;
    
    if(counter < 10){
        leftArm->update(ani_front);
        rightArm->update(ani_back);
        leftLeg->update(ani_back);
        rightLeg->update(ani_front);
    }
    if(counter >= 10 && counter < 20){
        leftArm->update(inverse_front);
        rightArm->update(inverse_back);
        leftLeg->update(inverse_back);
        rightLeg->update(inverse_front);
    }
    if(counter >= 20 && counter < 30){
        leftArm->update(ani_back);
        rightArm->update(ani_front);
        leftLeg->update(ani_front);
        rightLeg->update(ani_back);
    }
    if(counter >= 30){
        leftArm->update(inverse_back);
        rightArm->update(inverse_front);
        leftLeg->update(inverse_front);
        rightLeg->update(inverse_back);
        if(counter == 40) counter = 0;
    }
    counter++;
    //std::cerr << "counter value: "<< counter << std::endl;

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
    
    Hnear = 2 * tan(fov / 2) * near;
    Wnear = Hnear * (double(width) / (double)height);
    
    Hfar = 2 * tan(fov / 2) * far;
    Wfar = Hfar * (double(width) / (double)height);
    
    glm::vec3 d_vec = glm::normalize(center - eye);
    glm::vec3 right = glm::cross(up, d_vec);
    glm::vec3 left = -right;
    
    lc = rc = uc = dc = eye;
    fc = eye + d_vec * glm::vec3(far);
    nc = eye + d_vec * glm::vec3(near);
    
    Nnormal = d_vec;
    Fnormal = -d_vec;
    Rnormal = glm::cross(up, glm::normalize((nc + right * glm::vec3(Wnear / 2)) - eye));
    Lnormal = -glm::cross(up, glm::normalize((nc + left * glm::vec3(Wnear / 2)) - eye));
    Unormal = glm::cross(left, glm::normalize((nc + up * glm::vec3(Hnear / 2)) - eye));
    Dnormal = glm::cross(right, glm::normalize((nc + -up * glm::vec3(Hnear / 2)) - eye));
    
    std::vector<glm::vec3> all_center;
    std::vector<glm::vec3> all_norm;
    
    all_center.push_back(fc);
    all_center.push_back(nc);
    all_center.push_back(lc);
    all_center.push_back(rc);
    all_center.push_back(uc);
    all_center.push_back(dc);
    
    all_norm.push_back(Fnormal);
    all_norm.push_back(Nnormal);
    all_norm.push_back(Lnormal);
    all_norm.push_back(Rnormal);
    all_norm.push_back(Unormal);
    all_norm.push_back(Dnormal);
    
    // Render the object.
    god->draw(program, glm::mat4(1), all_center, all_norm, r_count, culling);
    std::cout << "Robots rendered: " << r_count << std::endl;
    r_count = 0;
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
                
        case GLFW_KEY_B:
            bbox = !bbox;
            break;
        
        case GLFW_KEY_C:
            culling = !culling;
            break;
                
        case GLFW_KEY_D:
            culling_debug = !culling_debug;
            break;
                
        default:
            break;
        }
    }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    
    // Scroll up
    if(yoffset > 0){
        // Not debug mod
        if(!culling_debug){
            if(fov + 1 > 175) return;
            fov = fov + 1;
            projection = glm::perspective(glm::radians(fov), double(width) / (double)height, near, far);
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }
        //debug mode
        else{
            fake_fov = fov;
            if(fake_fov + 1 > 175) return;
            fake_fov = fake_fov + 1;
            projection = glm::perspective(glm::radians(fake_fov), double(width) / (double)height, near, far);
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }
    }
    // Scroll down
    else{
        //Not debug
        if(!culling_debug){
            if(fov - 1 < 5) return;
            Window::fov = fov - 1;
            Window::projection = glm::perspective(glm::radians(fov), double(width) / (double)height, near, far);
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }
        //debug mode
        else{
            fake_fov = fov;
            if(fake_fov - 1 < 5) return;
            Window::fake_fov = fake_fov - 1;
            Window::projection = glm::perspective(glm::radians(fake_fov), double(width) / (double)height, near, far);
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }
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
    glm::vec3 camDirec = glm::normalize(center - eye);
    camDirec = glm::vec3(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis) * glm::vec4(camDirec.x, camDirec.y, camDirec.z, 0.0));
    view = glm::lookAt(eye, eye + camDirec, up);
    center = eye + camDirec;
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
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


