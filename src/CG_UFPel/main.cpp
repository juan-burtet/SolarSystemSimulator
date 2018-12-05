#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <solarsystem/sun.h>
#include <solarsystem/planet.h>
#include <solarsystem/moon.h>

#include <iostream>
#include <string>

// Original functions of the project
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// Funções de Alocação de Modelos
void allocate_sun(); // Sol
void allocate_planets(); // Planetas
void allocate_moons(); // Luas
void allocate_ship(); // Nave

// Funções de renderização de Modelos
void render_sun(Shader *ourShader); // Sol
void render_planets(Shader *ourShader); // Planetas
void render_moons(Shader *ourShader); // Luas
void redner_ship(); // Nave

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// struct do Sol
typedef struct{
    vector<tuple<Sun, Model>> sun;
    int qt;
}Stars;
Stars star;

// struct dos planetas
typedef struct{
    vector<tuple<Planet, Model>> planet;
    int qt;
}Planets;
Planets planets;

// struct das luas
typedef struct{
    vector<tuple<Moon, int>> moon;
    vector<Model> models;
    int qt_moons;
    int qt_models;
}Moons;
Moons moons;

// Struct da nave
typedef struct{
    vector<tuple<Model, glm::mat4>> ship;
    int qt;
}Ship;
Ship ship;

// Modo de câmera
int mode;


int main(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());

    Model falcon(FileSystem::getPath("resources/objects/Stars/stars.obj"));



    // Aloca o sol
    allocate_sun();
    // Aloca os planetas
    allocate_planets();

    
    mode = 1;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)){
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        switch(mode){
            case 1:break;
            case 2:break;
            case 3:break;
        }


        glm::mat4 matrix;
        matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
        //matrix = glm::rotate(matrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::scale(matrix, 100.0f * glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", matrix);
        falcon.Draw(ourShader);
        
        render_sun(&ourShader);
        render_planets(&ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        Planet::UA += 0.001;

        cout << Planet::UA << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        Planet::UA -= 0.001;
        if(Planet::UA <= 0.000000000001)
            Planet::UA = 0.000000000001;

        cout << Planet::UA << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        Planet::days += 0.001;

        cout << Planet::days << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        Planet::days -= 0.001;
        if(Planet::days <= 0.000000000001)
            Planet::days = 0.000000000001;

        cout << Planet::days << endl;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    
    camera.ProcessMouseScroll(yoffset);
}

// Aloca o Sol
void allocate_sun(){
    star.qt = 0;

    // Sol
    Model model_sun(FileSystem::getPath("resources/objects/Sun/sun.obj"));
    Sun sun("Sun", 150000); // 109 vezes o tamanho da Terra
    star.sun.push_back(tuple<Sun, Model>(sun, model_sun));
    star.qt++;
}//alocate_sun

// Aloca os planetas
void allocate_planets(){
    planets.qt = 0;

    // Mercury
    Model mercury(FileSystem::getPath("resources/objects/Planets/mercury/mercury.obj"));
    Planet planet_mercury("Mercury", 4879, 0.24, 58.646, 0.5);
    planets.planet.push_back(tuple<Planet, Model>(planet_mercury, mercury));
    planets.qt++;

    // Venus
    Model venus(FileSystem::getPath("resources/objects/Planets/venus/venus.obj"));
    Planet planet_venus("Venus", 12103, 0.61, -243.021, 0.75);
    planets.planet.push_back(tuple<Planet, Model>(planet_venus, venus));
    planets.qt++;

    // Earth
    Model earth(FileSystem::getPath("resources/objects/Planets/earth/earth.obj"));
    Planet planet_earth("Earth", 12756, 1, 1, 1);
    planets.planet.push_back(tuple<Planet, Model>(planet_earth, earth));
    planets.qt++;

    // Mars
    Model mars(FileSystem::getPath("resources/objects/Planets/mars/mars.obj"));
    Planet planet_mars("Mars", 6792, 1.88, 1.025, 1.25);
    planets.planet.push_back(tuple<Planet, Model>(planet_mars, mars));
    planets.qt++;

    // Jupiter
    Model jupiter(FileSystem::getPath("resources/objects/Planets/jupiter/jupiter.obj"));
    Planet planet_jupiter("Jupiter", 142984, 11.86, 4, 1.75);
    planets.planet.push_back(tuple<Planet, Model>(planet_jupiter, jupiter));
    planets.qt++;

    // Saturn
    Model saturn(FileSystem::getPath("resources/objects/Planets/saturn/saturn.obj"));
    Planet planet_saturn("Saturn", 120573, 29.45, 0.43, 2.6);
    planets.planet.push_back(tuple<Planet, Model>(planet_saturn, saturn));
    planets.qt++;

    // Uranus
    Model uranus(FileSystem::getPath("resources/objects/Planets/uranus/uranus.obj"));
    Planet planet_uranus("Uranus", 51118, 84.32, 0.71, 3.2);
    planets.planet.push_back(tuple<Planet, Model>(planet_uranus, uranus));
    planets.qt++;

    // Neptune
    Model neptune(FileSystem::getPath("resources/objects/Planets/neptune/neptune.obj"));
    Planet planet_neptune("Neptune", 49528, 164.79, 0.69, 3.6);
    planets.planet.push_back(tuple<Planet, Model>(planet_neptune, neptune));
    planets.qt++;
}// allocate_planets

void allocate_moons(){
    moons.qt_models = 0;
    moons.qt_moons = 0;


    // Terra
    //  - Lua ("Lua", 1/4 * Terra)

    // Marte
    //  - Fobos
    //  - Deimos

    // Jupiter
    //  - Ganimedes
    //  - Calisto
    //  - Io
    //  - Europa

    // Saturno
    //  - Titã

    // Urano
    //  - Titânia
    //  - Oberon
    //  - Umbriel
    //  - Ariel

    // Netuno
    //  - Tritão


}//allocate_moons


// Renderiza o Sol
void render_sun(Shader *ourShader){
    ourShader->setMat4("model", get<0>(star.sun[0]).render());
    get<1>(star.sun[0]).Draw(*ourShader);
}//render_sun

// Renderiza os Planetas
void render_planets(Shader *ourShader){
    for(int i = 0; i < planets.qt; i++){
        ourShader->setMat4("model", get<0>(planets.planet[i]).render(glfwGetTime()));
        get<1>(planets.planet[i]).Draw(*ourShader);
    }
}//render_planets