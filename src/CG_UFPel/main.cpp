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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Funções de Alocação de Modelos
void allocate_sun(); // Sol
void allocate_planets(); // Planetas
void allocate_moons(); // Luas
void allocate_ship(); // Nave

// Funções de renderização de Modelos
void render_stars(Shader *ourShader, Model *stars); // Estrelas
void render_sun(Shader *ourShader); // Sol
void render_planets(Shader *ourShader); // Planetas
void render_moons(Shader *ourShader); // Luas
void render_ship(Shader *ourShader); // Nave

// Funções da Câmera
void up_vision(Shader *ourShader); // Modo 1
void pick_vision(Shader *ourShader); // Modo 2
void ship_vision(Shader *ourShader); // Modo 3
glm::vec3 distance_vision(); // Distância da visão do planeta
glm ::vec3 getMoonPosition(); // Posição da lua
float getMoonScale(); // Scale da lua
glm::vec3 shipPosition(); // Posição da nave
glm::vec3 camPosition(); // Posição da Câmera
void updateCameraOnShip(); // Atualiza a câmera da Nave
glm::vec3 rightFromShip(); // Retorna o Right da nave
glm::mat4 checkShip(); // Checa se a nave não está saindo do mundo

// Função do Botão
bool processButton(); // Apertou um botão
void freeButton(); // Largou o botão

// Função que inicializa as variaveis
void initialize();

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

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
    vector<tuple<Moon, Model>> moon;
    int qt;
}Moons;
Moons moons;

// Struct da visão dos planetas
typedef struct{
    int planet;
    int moon;
}Vision;
Vision vision;

// Struct da nave
typedef struct{
    vector<tuple<Model, glm::mat4>> ship;
    float scale;
    float speed;
}Ship;
Ship ship;

// Botão 
bool button;

// Modo de câmera
int mode;

// Função Main
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System Simulator", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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

    // Modelo das Estrelas
    Model stars(FileSystem::getPath("resources/objects/Stars/stars.obj"));

    // Inicializa as variaveis
    initialize();
    
    // render loop
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
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // Decide em qual modo de câmera está
        switch(mode){
            case 1: up_vision(&ourShader);   break;
            case 2: pick_vision(&ourShader); break;
            case 3: ship_vision(&ourShader); break;
        }

        // Chama as renderizações
        render_stars(&ourShader, &stars);
        render_sun(&ourShader);
        render_planets(&ourShader);
        render_moons(&ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}//main

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window){
    // Sair do programa
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
        return;
    }

    // Troca do tipo de câmera
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        mode = 1;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        mode = 2;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        mode = 3;
        return;
    }

    // Comandos do modo 1
    if (mode == 1){

    }//if

    // Comandos do modo 2
    if (mode == 2){

        // Easter Egg - Terra Plana
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
            if(processButton())
                return;

            Planet::plane = true;
        }//if

        // Easter Egg - Desativa Terra Plana
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            if(processButton())
                return;

            Planet::plane = false;
        }//if


        // Troca de Planetas
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            if(processButton())
                return;

            vision.moon = -1;
            vision.planet--;
            if (vision.planet < 0)
                vision.planet = 7;

            return;
        }//if
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            if(processButton())
                return;

            vision.moon = -1;
            vision.planet++;
            if (vision.planet > 7)
                vision.planet = 0;

            return;
        }//if

        // Se tiver luas, tem como trocar de planeta
        if (get<0>(planets.planet[vision.planet]).getMoons() > 0){
            //Troca de luas
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
                if(processButton())
                    return;

                vision.moon++;
                if (vision.moon >= (int) get<0>(planets.planet[vision.planet]).getMoons())
                    vision.moon = -1;

                return;
            }//if
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
                if(processButton())
                    return;

                vision.moon--;
                if (vision.moon == -2)
                    vision.moon = get<0>(planets.planet[vision.planet]).getMoons() -1;
                else if(vision.moon < 0)
                    vision.moon = -1;

                return;
            }//if
        }//if

    }//if

    // Comandos do modo 3
    if (mode == 3){

        // Movimentação da Nave
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            get<1>(ship.ship[0]) = glm::rotate(get<1>(ship.ship[0]), glm::radians(90.0f) * -deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
            updateCameraOnShip();
        }//if
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            get<1>(ship.ship[0]) = glm::rotate(get<1>(ship.ship[0]), glm::radians(90.0f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
            updateCameraOnShip();
        }//if
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            get<1>(ship.ship[0]) = glm::rotate(get<1>(ship.ship[0]), glm::radians(90.0f) * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
            updateCameraOnShip();
        }//if
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            get<1>(ship.ship[0]) = glm::rotate(get<1>(ship.ship[0]), glm::radians(90.0f) * -deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
            updateCameraOnShip();
        }//if
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            get<1>(ship.ship[0]) = glm::rotate(get<1>(ship.ship[0]), glm::radians(90.0f) * -deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
            updateCameraOnShip();
        }//if
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            get<1>(ship.ship[0]) = glm::rotate(get<1>(ship.ship[0]), glm::radians(90.0f) * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
            updateCameraOnShip();
        }//if

        // Velocidade da Nave
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            if(processButton())
                return;

            ship.speed++;
            if(ship.speed > 10)
                ship.speed = 10;
            return;
        }//if
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            if(processButton())
                return;

            ship.speed--;
            if(ship.speed < 3)
                ship.speed = 3;
            return;
        }//if
    }//if

    freeButton();
}//processInput

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}//framebuffer_size_callback

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    
    camera.ProcessMouseScroll(yoffset);
}//scroll_callback

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
    Planet planet_mercury("Mercury", 4879, 1, 0.5, 0.5);
    planets.planet.push_back(tuple<Planet, Model>(planet_mercury, mercury));
    planets.qt++;

    // Venus
    Model venus(FileSystem::getPath("resources/objects/Planets/venus/venus.obj"));
    Planet planet_venus("Venus", 12103, 2, -1, 0.75);
    planets.planet.push_back(tuple<Planet, Model>(planet_venus, venus));
    planets.qt++;

    // Earth
    Model earth(FileSystem::getPath("resources/objects/Planets/earth/earth.obj"));
    Planet planet_earth("Earth", 12756, 3, 1.5, 1);
    planet_earth.setMoons(1);
    planets.planet.push_back(tuple<Planet, Model>(planet_earth, earth));
    planets.qt++;

    // Mars
    Model mars(FileSystem::getPath("resources/objects/Planets/mars/mars.obj"));
    Planet planet_mars("Mars", 6792, 4, 2, 1.25);
    planets.planet.push_back(tuple<Planet, Model>(planet_mars, mars));
    planets.qt++;

    // Jupiter
    Model jupiter(FileSystem::getPath("resources/objects/Planets/jupiter/jupiter.obj"));
    Planet planet_jupiter("Jupiter", 142984, 5, 2.5, 2.75);
    planet_jupiter.setMoons(4);
    planets.planet.push_back(tuple<Planet, Model>(planet_jupiter, jupiter));
    planets.qt++;

    // Saturn
    Model saturn(FileSystem::getPath("resources/objects/Planets/saturn/saturn.obj"));
    Planet planet_saturn("Saturn", 120573, 6, 3, 5.0);
    planet_saturn.setMoons(1);
    planets.planet.push_back(tuple<Planet, Model>(planet_saturn, saturn));
    planets.qt++;

    // Uranus
    Model uranus(FileSystem::getPath("resources/objects/Planets/uranus/uranus.obj"));
    Planet planet_uranus("Uranus", 51118, 7, 3.5, 6.25);
    planet_uranus.setMoons(4);
    planets.planet.push_back(tuple<Planet, Model>(planet_uranus, uranus));
    planets.qt++;

    // Neptune
    Model neptune(FileSystem::getPath("resources/objects/Planets/neptune/neptune.obj"));
    Planet planet_neptune("Neptune", 49528, 8, 4, 7.25);
    planet_neptune.setMoons(1);
    planets.planet.push_back(tuple<Planet, Model>(planet_neptune, neptune));
    planets.qt++;
}// allocate_planets

// Aloca as luas
void allocate_moons(){
    moons.qt = 0;

    // Luas da Terra
    Model moon_model(FileSystem::getPath("resources/objects/Moons/Earth/Moon/moon.obj"));
    Moon moon("Moon", 12756/4, 1, 0.5, 0.07, &get<0>(planets.planet[2]));
    moons.moon.push_back(tuple<Moon, Model>(moon, moon_model));
    moons.qt++;

    //--------------------------------------------

    // Luas de Jupiter
    Model io_model(FileSystem::getPath("resources/objects/Moons/Jupiter/Io/io.obj"));
    Moon io("Io", 142984/7, 1, 0.5, 0.6, &get<0>(planets.planet[4]));
    moons.moon.push_back(tuple<Moon, Model>(io, io_model));
    moons.qt++;

    Model europa_model(FileSystem::getPath("resources/objects/Moons/Jupiter/Europa/europa.obj"));
    Moon europa("Europa", 142984/7.5, 2, 1, 0.85, &get<0>(planets.planet[4]));
    moons.moon.push_back(tuple<Moon, Model>(europa, europa_model));
    moons.qt++;

    Model ganymede_model(FileSystem::getPath("resources/objects/Moons/Jupiter/Ganymede/ganymede.obj"));
    Moon ganymede("Ganymede", 142984/5, 3, 1.5, 1.1, &get<0>(planets.planet[4]));
    moons.moon.push_back(tuple<Moon, Model>(ganymede, ganymede_model));
    moons.qt++;

    Model callisto_model(FileSystem::getPath("resources/objects/Moons/Jupiter/Callisto/callisto.obj"));
    Moon callisto("Callisto", 142984/6, 4, 2, 1.35, &get<0>(planets.planet[4]));
    moons.moon.push_back(tuple<Moon, Model>(callisto, callisto_model));
    moons.qt++;

    //-----------------------------------------------

    // Lua de Saturno
    Model titan_model(FileSystem::getPath("resources/objects/Moons/Saturn/Titan/titan.obj"));
    Moon titan("Titan", 120573/4, 1, 0.5, 0.6, &get<0>(planets.planet[5]));
    moons.moon.push_back(tuple<Moon, Model>(titan, titan_model));
    moons.qt++;

    //-----------------------------------------------

    // Luas de Urano
    Model ariel_model(FileSystem::getPath("resources/objects/Moons/Uranus/Ariel/ariel.obj"));
    Moon ariel("Ariel", 51118/5, 1, 0.5, 0.3, &get<0>(planets.planet[6]));
    moons.moon.push_back(tuple<Moon, Model>(ariel, ariel_model));
    moons.qt++;

    Model umbriel_model(FileSystem::getPath("resources/objects/Moons/Uranus/Umbriel/umbriel.obj"));
    Moon umbriel("Umbriel", 51118/5, 2, 1.0, 0.4, &get<0>(planets.planet[6]));
    moons.moon.push_back(tuple<Moon, Model>(umbriel, umbriel_model));
    moons.qt++;

    Model titania_model(FileSystem::getPath("resources/objects/Moons/Uranus/Titania/titania.obj"));
    Moon titania("Titania", 51118/5, 3, 1.5, 0.5, &get<0>(planets.planet[6]));
    moons.moon.push_back(tuple<Moon, Model>(titania, titania_model));
    moons.qt++;

    Model oberon_model(FileSystem::getPath("resources/objects/Moons/Uranus/Oberon/oberon.obj"));
    Moon oberon("Oberon", 51118/5, 4, 2, 0.6, &get<0>(planets.planet[6]));
    moons.moon.push_back(tuple<Moon, Model>(oberon, oberon_model));
    moons.qt++;

    //------------------------------------------------

    // Luas de Netuno
    Model triton_model(FileSystem::getPath("resources/objects/Moons/Neptune/Triton/triton.obj"));
    Moon triton("Triton", 49528/4, 1, 0.5, 0.3, &get<0>(planets.planet[7]));
    moons.moon.push_back(tuple<Moon, Model>(triton, triton_model));
    moons.qt++;
}//allocate_moons

// Aloca a nave
void allocate_ship(){
    glm::mat4 matrix;

    // Inicializa a parte da nave
    Model model_ship(FileSystem::getPath("resources/objects/Falcon/Millennium_Falcon.obj"));
    matrix = glm::translate(matrix, glm::vec3(0.0f, -0.02f, 2.9f));
    matrix = glm::rotate(matrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::scale(matrix, 0.0001f * glm::vec3(1.0f, 1.0f, 1.0f));
    ship.ship.push_back(tuple<Model,glm::mat4>(model_ship, matrix));
    ship.scale = 1/0.0001f;
}//allocate_ship

// Renderiza as estrelas
void render_stars(Shader *ourShader, Model *stars){
    glm::mat4 matrix;
    matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    if(mode == 1)
        matrix = glm::rotate(matrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, 30.0f * glm::vec3(1.0f, 1.0f, 1.0f));
    ourShader->setMat4("model", matrix);
    stars->Draw(*ourShader);
}//render_stars

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

// Renderiza as luas
void render_moons(Shader *ourShader){
    for(int i = 0; i < moons.qt; i++){
        ourShader->setMat4("model", get<0>(moons.moon[i]).render(glfwGetTime()));
        get<1>(moons.moon[i]).Draw(*ourShader);
    }
}//render_moons

// Renderiza a nave
void render_ship(Shader *ourShader){
    ourShader->setMat4("model", get<1>(ship.ship[0]));
    get<0>(ship.ship[0]).Draw(*ourShader);
}//render_ship

// Utiliza a câmera com visão total do sistema solar
void up_vision(Shader *ourShader){
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
    camera.Position = glm::vec3(0.0f, 20.0f, 0.0f);
    camera.Front = glm::normalize(glm::vec3(0.0f, -3.0f, 0.0f));
    camera.Up = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    camera.Right = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = camera.GetViewMatrix();
    
    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);    
}//up_vision

// Utiliza a câmera com visão dos objetos
void pick_vision(Shader *ourShader){
    // Posição do objeto visualizado
    glm::vec3 objPos = get<0>(planets.planet[vision.planet]).getPosition();

    if(vision.moon >= 0)
        objPos = getMoonPosition();

    // Criação da matriz de projeção
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);

    // Posição da câmera
    camera.Position = distance_vision() + objPos;
    camera.Front    = glm::normalize(objPos - camera.Position);
    camera.Right    = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
    camera.Up       = glm::normalize(glm::cross(camera.Right, camera.Front));
    
    // View
    glm::mat4 view = camera.GetViewMatrix();
    
    // Atualiza o Shader
    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);    
}//pick_vision

// Utiliza a câmera da nave
void ship_vision(Shader *ourShader){
    
    // Movimenta a nave
    get<1>(ship.ship[0]) = checkShip();

    // Atualiza a câmera
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
    updateCameraOnShip();
    glm::mat4 view = camera.GetViewMatrix();
    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);
    render_ship(ourShader);
}//ship_vision

// Retorna a distância para visualizar o objeto
glm::vec3 distance_vision(){
    glm::vec3 x;
    float scale;
    if(vision.moon < 0)
        scale = get<0>(planets.planet[vision.planet]).getScale();
    else
        scale = getMoonScale();

    float aux = (1.5f * scale)/142984;
    x = glm::vec3(-aux, aux, 0.0f);
    return x;
}//distance_vision

// Retorna a posição da lua selecionada
glm::vec3 getMoonPosition(){
    glm::vec3 x;

    switch(vision.planet){
        case 2: x = get<0>(moons.moon[0]).getPosition();                break;
        case 4: x = get<0>(moons.moon[1 + vision.moon]).getPosition();  break;
        case 5: x = get<0>(moons.moon[5]).getPosition();                break;
        case 6: x = get<0>(moons.moon[6 + vision.moon]).getPosition();  break;
        case 7: x = get<0>(moons.moon[10]).getPosition();               break;
    }

    return x;
}//getMoonPosition

// Retorna o scale da lua
float getMoonScale(){
    float x;

    switch(vision.planet){
        case 2: x = get<0>(moons.moon[0]).getScale();                break;
        case 4: x = get<0>(moons.moon[1 + vision.moon]).getScale();  break;
        case 5: x = get<0>(moons.moon[5]).getScale();                break;
        case 6: x = get<0>(moons.moon[6 + vision.moon]).getScale();  break;
        case 7: x = get<0>(moons.moon[10]).getScale();               break;
    }

    return x;
}//getMoonScale

// Retorna a posição da nave
glm::vec3 shipPosition(){
    glm::mat4 matrix = get<1>(ship.ship[0]);
    glm::vec3 position;

    position.x = matrix[3][0];
    position.y = matrix[3][1];
    position.z = matrix[3][2];

    return position; 
}//shipPosition

// Retorna a posição da câmera
glm::vec3 camPosition(){
    glm::mat4 aux;
    glm::vec3 dist;

    aux = glm::translate(get<1>(ship.ship[0]), ship.scale * glm::vec3(0.0f, 0.020f, -0.11f));
    dist.x = aux[3][0];
    dist.y = aux[3][1];
    dist.z = aux[3][2];

    return dist;
}//camPosition

// Retorna o right da nave
glm::vec3 rightFromShip(){
    glm::mat4 aux;
    glm::vec3 dist;

    aux = glm::translate(get<1>(ship.ship[0]), ship.scale * glm::vec3(1.0f, 0.0f, 0.0f));
    dist.x = aux[3][0];
    dist.y = aux[3][1];
    dist.z = aux[3][2];

    return shipPosition() - dist;
}//rightFromShip

// Atualiza a câmera da nave
void updateCameraOnShip(){
    camera.Position = camPosition();
    camera.Front    = glm::normalize(shipPosition() - camera.Position);
    camera.Right    = glm::normalize(rightFromShip());
    camera.Up       = glm::normalize(glm::cross(camera.Right, camera.Front));
}//updateCameraOnShip

// Verifica se a nave pode se movimentar
glm::mat4 checkShip(){
    glm::vec3 x;
    glm::mat4 aux;
    
    // Usados pra movimentar a nave
    x = glm::vec3(0.0f, 0.0f, 1000.0f);

    // Movimenta a nave
    aux = glm::translate(get<1>(ship.ship[0]), deltaTime * (x * ship.speed));

    // posição da nave
    x.x = aux[3][0];
    x.y = aux[3][1];
    x.z = aux[3][2];

    // Se tiver muito a cima ou muito abaixo, não pode movimentar
    if(abs(x.y) > 2.0f)
        return get<1>(ship.ship[0]);

    // Se tiver uma distância muito grande do centro, não se movimenta
    if(glm::length(x) > 8.0f)
        return get<1>(ship.ship[0]);

    // pode se movimentar
    return aux;
}//checkShip

// Aperta o botão
bool processButton(){
    if(button)
        return true;

    button = true;
    return false;
}//processButton

// Abre o botão
void freeButton(){
    // botão não apertado
    button = false;
}//freeButton

// Inicializa as variaveis
void initialize(){
    // Aloca o sol
    allocate_sun();
    // Aloca os planetas
    allocate_planets();
    // Aloca as luas
    allocate_moons();
    // Aloca a nave
    allocate_ship();

    // Inicializa os valores da struct Vision
    vision.planet = 0;
    vision.moon = -1;

    // Modo da camêra
    mode = 1;

    // Botão não apertado
    button = false;

    // Velocidade da nave
    ship.speed = 3;
}//initialize