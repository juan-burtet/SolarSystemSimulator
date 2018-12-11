# SolarSystemSimulator

## TODO

- [x] Todos os planetas rotacionando em volta do Sol;
- [x] Luas dos Planetas rotacionando em volta dos seus respectivos planetas;
- [ ] Podendo modificar a velocidade que o Mundo se movimenta:
	- [ ] Diminuir a velocidade;
	- [ ] Aumentar a velocidade;
	- [ ] Pausar todos os movimentos do mundo;
- [x] Multiplas câmeras:
	- [x] Câmera com a visão geral do Sistema Solar, podendo ver todos os planetas e suas luas em movimento;
	- [x] Câmera pode dar foco em um planeta especifico, observando tal planeta (ou lua) enquanto se movimenta com o planeta;
	- [x] Câmera que acompanha uma nave espacial
- [x] Nave espacial pode ser controlada pelo usuário para se movimentar pelo Sistema;
- [ ] Desenho dos movimentos dos Planetas (e suas luas);
- [ ] Nome de todos os objetos do Universo(Sol, Planetas, Luas, etc...)


## Compilação (Em Inglês)

### Windows building
All relevant libraries are found in /libs and all DLLs found in /dlls (pre-)compiled for Windows. 
The CMake script knows where to find the libraries so just run CMake script and generate project of choice.
Note that you still have to manually copy the required .DLL files from the /dlls folder to your binary folder for the binaries to run.

Keep in mind the supplied libraries were generated with a specific compiler version which may or may not work on your system (generating a large batch of link errors). In that case it's advised to build the libraries yourself from the source.

### Linux building
First make sure you have CMake, Git, and GCC by typing as root (sudo) `apt-get install g++ cmake git` and then get the required packages:
Using root (sudo) and type `apt-get install libsoil-dev libglm-dev libassimp-dev libglew-dev libglfw3-dev libxinerama-dev libxcursor-dev  libxi-dev` .
Next, run CMake (preferably CMake-gui). The source directory is LearnOpenGL and specify the build directory as LearnOpenGL/build. Creating the build directory within LearnOpenGL is important for linking to the resource files (it also will be ignored by Git). Hit configure and specify your compiler files (Unix Makefiles are recommended), resolve any missing directories or libraries, and then hit generate. Navigate to the build directory (`cd LearnOpenGL/build`) and type `make` in the terminal. This should generate the executables in the respective chapter folders.

Note that CodeBlocks or other IDEs may have issues running the programs due to problems finding the shader and resource files, however it should still be able to generate the exectuables. To work around this problem it is possible to set an environment variable to tell the tutorials where the resource files can be found. The environment variable is named LOGL_ROOT_PATH and may be set to the path to the root of the LearnOpenGL directory tree. For example:

    `export LOGL_ROOT_PATH=/home/user/tutorials/LearnOpenGL`

Running `ls $LOGL_ROOT_PATH` should list, among other things, this README file and the resources direcory.

### Mac OS X building
Building on Mac OS X is fairly simple (thanks [@hyperknot](https://github.com/hyperknot)):
```
brew install cmake assimp glm glfw
mkdir build
cd build
cmake ../.
make -j8
```
