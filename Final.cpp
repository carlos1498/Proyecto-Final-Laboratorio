/*---------------------------------------------------------*/
/* ----------------  /Practica 10: animacion  keyframes               -----------*/
/*-----------------    2022-2   ---------------------------*/
/*------------- Alumno:   MARTINEZ oRTIZ cARLOS dANIEL                  ---------------*/
/*------------- No. Cuenta  314000170                ---------------*/

//Practica 9: animacion 25/04/2022
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, -1.0f, 0.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movx = 0.0f,
		movz = 0.0f,
	rotvehiculo = 0.0f;
//animation boat
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
	recorrido5 = false,
	recorrido6= false;
float giroLlantas = 0.0f;
float miVar = 0.0F;
float luzambx, luzamby, luzambz = 0.0f;
float luzdifx, luzdify, luzdifz = 0.0f;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		rotRodDer = 0.0f,
		giroMonito = 0.0f,
		movBrazoizq=0.0f,
		movBrazoDer = 0.0f,
		rotCabeza = 0.0f;
//jeep variables
float	JposX = 0.0f,
JposY = 0.0f,
JposZ = 0.0f,
rotLlantas = 0.0f,
giroJeep = 0.0f;
//increase jeep
float	incXJ = 0.0f,
incYJ = 0.0f,
incZJ = 0.0f,
rotLlantasIncJ = 0.0f,
giroJeepInc = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		rotRodDerInc = 0.0f,
		giroMonitoInc = 0.0f,
		movBrazoizqInc=0.0f,
		movBrazoDerInc = 0.0f,
		rotCabezaInc = 0.0f;
bool estado = false;

#define MAX_FRAMES 100
int i_max_steps = 60;//cuadros intemedios para las diferentes trancisiones, entre mas cuadros es mas lenta la animacion pero mas detallada
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames, no son las mismas variables de manipulacion y dibujo, estas son varaibles para guardar los keyframes
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float rotRodDer;
	float giroMonito;
	float movBrazoizq;
	float movBrazoDer;
	float rotCabeza;
	//attributes jeep
	float JposX;
	float	JposY;
	float	JposZ;
	float	rotLlantas;
	float	giroJeep;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 100;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	//lado derecho variables del arreglo
	KeyFrame[FrameIndex].posX = posX;//lado izuierdo las varaibles de dibujo
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].movBrazoizq = movBrazoizq;
	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer;
	KeyFrame[FrameIndex].rotCabeza = rotCabeza;
	//save frame jeep
	KeyFrame[FrameIndex].JposX = JposX;//lado izuierdo las varaibles de dibujo
	KeyFrame[FrameIndex].JposY = JposY;
	KeyFrame[FrameIndex].JposZ = JposZ;
	KeyFrame[FrameIndex].rotLlantas = rotLlantas;
	KeyFrame[FrameIndex].giroJeep = giroJeep;
	FrameIndex++;

	std::cout << "POSX = " << posX << std::endl;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	rotRodDer = KeyFrame[0].rotRodDer;
	giroMonito = KeyFrame[0].giroMonito;
	movBrazoizq = KeyFrame[0].movBrazoizq;
	movBrazoDer = KeyFrame[0].movBrazoDer;
	rotCabeza = KeyFrame[0].rotCabeza;
	//reset elements jeep
	JposX = KeyFrame[0].JposX;
	JposY = KeyFrame[0].JposY;
	JposZ = KeyFrame[0].JposZ;
	rotLlantas = KeyFrame[0].rotLlantas;
	giroJeep= KeyFrame[0].giroJeep;
}

void interpolation(void)
{		//playindex +1 es el cuadro final
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	rotRodDerInc = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	//se calculan los incrementos: valor final de la propiedad - valor inicial de la propiedad sobre el numero de pasos
	movBrazoizqInc = (KeyFrame[playIndex + 1].movBrazoizq - KeyFrame[playIndex].movBrazoizq) / i_max_steps;
	movBrazoDerInc = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;
	rotCabezaInc = (KeyFrame[playIndex + 1].rotCabeza - KeyFrame[playIndex].rotCabeza) / i_max_steps;
	//interpolaction elements jeep
	incXJ = (KeyFrame[playIndex + 1].JposX - KeyFrame[playIndex].JposX) / i_max_steps;
	incYJ = (KeyFrame[playIndex + 1].JposY - KeyFrame[playIndex].JposY) / i_max_steps;
	incZJ = (KeyFrame[playIndex + 1].JposZ - KeyFrame[playIndex].JposZ) / i_max_steps;
	rotLlantasIncJ = (KeyFrame[playIndex + 1].rotLlantas - KeyFrame[playIndex].rotLlantas) / i_max_steps;
	giroJeepInc = (KeyFrame[playIndex + 1].giroJeep - KeyFrame[playIndex].giroJeep) / i_max_steps;
}

void animate(void)
{
	/*lightPosition.x = 100.0f * cos(miVar);
	lightPosition.z = 250.0f * sin(miVar);*/
	miVar += 0.1f;
	
		
		/*luzambx += 0.1f;
		luzamby += 0.1f;
		luzambz += 0.1f;*/
	
		luzdifx += 100.0f * cos(miVar);
		luzdify += 100.0f * cos(miVar);
		luzdifz += 100.0f * cos(miVar);
	
	
		
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			rotRodDer += rotRodDerInc;
			giroMonito += giroMonitoInc;
			//variable de dibujo mas incremento que acabo de calcular
			movBrazoizq += movBrazoizqInc;
			movBrazoDer += movBrazoDerInc;
			rotCabeza += rotCabezaInc;
			i_curr_steps++;
			//draw animation jeep
			JposX += incXJ;
			JposY += incYJ;
			JposZ += incZJ;
			rotLlantas += rotLlantasIncJ;
			giroJeep += giroJeepInc;
		}
	}

	//Vehículo
	if (animacion)
	{
		if (recorrido1)
		{
			if (movz > 400.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
				rotvehiculo = 90.0f;

			}
			else
			{
				
				movz += 3.0f;
			}
		}


		if (recorrido2)
		{
			if (movx > 400.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
				rotvehiculo = 180.0f;

			}
			else
			{
				
				movx += 3.0f;
			}
		}

		if (recorrido3)
		{
			if (movz < 0.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
				rotvehiculo = 315.0f;//sumanos los 270 grados que teniamos de antes mas 45 grados para dar la impresíon de que el coche esta en diagonal con la cabeza de frente

			}
			else
			{
				movz -= 3.0f;

			}
		}

		if (recorrido4)
		{
			if (movx < 0.0f)
			{
				recorrido4 = false;
				recorrido2 = true;//activar recorrido dos para volver ciclica la animación
				rotvehiculo = 90.0f;

			}
			else
			{
				
				movx -= 3.0f;
				movz += 3.0f;
			}
		}



	}
}

//void playMusic() {
//	//Play Background music
//	PlaySound(TEXT("resources/Music/selva.wav"), NULL, SND_LOOP | SND_ASYNC);
//}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/left2.jpg",
		"resources/skybox/back2.jpg",
		"resources/skybox/top2.jpg",
		"resources/skybox/bottom2.jpg",
		"resources/skybox/right2.jpg",
		"resources/skybox/front2.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/pisopro/piscocalle2.obj");
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel_LSVC.obj");
	Model casaVieja("resources/objects/casa/OldHouse.obj");
	//Model cubo("resources/objects/cubo/cube02.obj");
	Model casaDoll("resources/objects/casa/DollHouse.obj");
	Model casabrujas("resources/objects/casabrujas/casa2.obj");
	Model jeep("resources/objects/jeep/jeep.obj");
	Model jeepllanta("resources/objects/jeep/jeepllanta.obj");
	Model entrada("resources/objects/puertajura/puerta.obj");
	Model Trex("resources/objects/TRex/tREX.obj");
	Model Helipuerto("resources/objects/Helipuerto/Helipuerto.obj");
	Model cabania("resources/objects/cabana/cabana2.obj");
	Model edificio("resources/objects/edificio_principal/ediprinc2.obj");
	Model tienda("resources/objects/GiftShop/giftshop2.obj");
	Model box("resources/skybox/skybox.obj");
	Model Hcuerpo("resources/objects/helicoptero/Hcuerpo.obj");
	Model helice("resources/objects/helicoptero/helice.obj");
	Model hcola("resources/objects/helicoptero/helicecola.obj");
	Model barco("resources/objects/barco/barco.obj");
	ModelAnim animacionPersonaje("resources/objects/Man/Man.dae");

	animacionPersonaje.initShaders(animShader.ID);

	//ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	//ninja.initShaders(animShader.ID);

	//ModelAnim wrestler("resources/objects/descarga/TurnLeft45Degrees.dae");//cargar animacion mixamo
	//wrestler.initShaders(animShader.ID);
	

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		//animation helicopter	
		KeyFrame[0].posX = 0;
		KeyFrame[0].posY = 0;
		KeyFrame[0].posZ = 0;
		KeyFrame[0].rotRodIzq = 0;
		KeyFrame[0].giroMonito = 0;
		KeyFrame[1].rotRodIzq = 270.0f;
		KeyFrame[2].posX = -600.0f;
		KeyFrame[2].posY = 800.0;
		KeyFrame[2].posZ = 0.0f;
		KeyFrame[2].rotRodIzq = 0.0f;
		KeyFrame[2].giroMonito = 180.0f;
		//KeyFrame[3].rotRodIzq = 360.0f;
		KeyFrame[3].posX = -660.0f;
		KeyFrame[3].posY = 600.0;
		KeyFrame[3].posZ = 500.0f;
		KeyFrame[3].rotRodIzq = 0.0f;
		KeyFrame[3].giroMonito = 90.0f;
		KeyFrame[3].rotRodIzq = 360.0f;
		KeyFrame[4].posX = -800.0f;
		KeyFrame[4].posY = 300.0;
		KeyFrame[4].posZ = 800.0f;
		KeyFrame[4].rotRodIzq = 360.0f;
		KeyFrame[4].giroMonito =270.0f;
		KeyFrame[5].posX = -300.0f;
		KeyFrame[5].posY = 600.0;
		KeyFrame[5].posZ = 800.0f;
		KeyFrame[5].rotRodIzq = 360.0f;
		KeyFrame[5].giroMonito = 90.0f;
		//---animation Jeep------------------------------------
		KeyFrame[0].JposX = 0;//lateral mov
		KeyFrame[0].JposY = 0;
		KeyFrame[0].JposZ = 0;
		KeyFrame[0].rotLlantas = 0;
		KeyFrame[1].JposX = 0.0f;
		KeyFrame[1].JposY = 0.0f;
		KeyFrame[1].JposZ = 200.0f;
		KeyFrame[1].rotLlantas = 0.0f;
		KeyFrame[1].giroJeep = 0.0f;
		KeyFrame[2].JposX = 50.0f;
		KeyFrame[2].JposY = 0.0;
		KeyFrame[2].JposZ = 200.0f;
		KeyFrame[2].rotLlantas = 0.0f;
		KeyFrame[2].giroJeep = 90.0f;
		KeyFrame[3].JposX = 150.0f;
		KeyFrame[3].JposY = 0.0f;
		KeyFrame[3].JposZ = 200.0f;
		KeyFrame[3].rotLlantas = 360.0f;
		KeyFrame[3].giroJeep = 135.0f;
		KeyFrame[4].JposX = 200.0f;
		KeyFrame[4].JposY = 0.0f;
		KeyFrame[4].JposZ = 200.0f;
		KeyFrame[4].rotLlantas = 360.0f;
		KeyFrame[4].giroJeep = 180.0f;
		KeyFrame[5].giroJeep = 180.0f;
		KeyFrame[5].JposX = 200.0f;
		KeyFrame[5].JposY = 0.0f;
		KeyFrame[5].JposZ = -800.0f;
		KeyFrame[5].rotLlantas = 360.0f;
		KeyFrame[6].JposX = 400.0f;
		KeyFrame[6].JposY = 0.0f;
		KeyFrame[6].JposZ = -1000.0f;
		KeyFrame[6].rotLlantas = 360.0f;
		KeyFrame[6].giroJeep = 35.0f;
		KeyFrame[7].giroJeep = 0.0f;
		KeyFrame[7].JposX = 400.0f;
		KeyFrame[7].JposY = 0.0f;
		KeyFrame[7].JposZ = -1200.0f;
		KeyFrame[7].rotLlantas = 360.0f;
		
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//fuentes de luz
		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.4, 0.4f, 0.4f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.0008f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.005f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		//3
		staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.005f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);
		///
		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);
		//animacion mixamo luchador
		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 100.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.125f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		animacionPersonaje.Draw(animShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 150.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.125f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		animacionPersonaje.Draw(animShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		//staticShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		//tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		//model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		//model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der trasera

		//model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq trase
		// -----------------------------------------------------------------
		
		// ------------------------------------------------------------
		// Entrada
		// -----------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(550.0f, 70.0f,-850.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		entrada.Draw(staticShader);
		///cabana1
		/// ----------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 00.0f, -340.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		///-----------------------------------------------
		/// cabana2
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 00.0f, -110.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		///-----------------------------------------------
		/// cabana3
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 00.0f, 130.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		///-----------------------------------------------
		/// cabana4
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 00.0f, 400.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		///-----------------------------------------------
		/// cabana5
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(300.0f, 00.0f, 400.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		///-----------------------------------------------
		/// cabana6
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(300.0f, 00.0f, 130.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		///-----------------------------------------------
		/// cabana7
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(300.0f, 00.0f, -120.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		/// cabana8
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(300.0f, 00.0f, -340.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.50f));
		staticShader.setMat4("model", model);
		cabania.Draw(staticShader);
		/// edifcioprin
		/// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-340.0f, 15.0f, -1300.0f));
		model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.50f));
		staticShader.setMat4("model", model);
		edificio.Draw(staticShader);
		///-----------------------------------------------------------
		/// Tienda1
		/// --------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-340.0f, -5.0f, -640.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(20.50f));
		staticShader.setMat4("model", model);
		tienda.Draw(staticShader);
		///-----------------------------------------------------------
		/// Tienda2
		/// --------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(35.0f, -5.0f, -840.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(20.50f));
		staticShader.setMat4("model", model);
		tienda.Draw(staticShader);
		///-------------------------------------------------------
		///-------------------------------------------------------
		//Helipuerto
		// --------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(450.0f, 20.0f, -1350.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.50f));
		staticShader.setMat4("model", model);
		Helipuerto.Draw(staticShader);
		//--------------------------------------------------------
		// barco
		// ------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f+movx, 10.0f, 800.0f+movz));
		model = glm::rotate(model, glm::radians(rotvehiculo), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(7.50f));
		staticShader.setMat4("model", model);
		barco.Draw(staticShader);
		//--------------------------------------------------------
		// jeep
		// ---------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f + JposX, 18.0f + JposY, 0.0 + JposZ));
		model = glm::scale(model, glm::vec3(2.50f));
		tmp = model = glm::rotate(model, glm::radians(giroJeep), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		jeep.Draw(staticShader);
		/////----------------------------------------------------------------------------------------
		/////jeep llanata delantera  izquierda
		///// ---------------------------------------------------------------------------------------
		model = glm::translate(tmp, glm::vec3(7.0f, -4.3f, 5.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		jeepllanta.Draw(staticShader);
		/////----------------------------------------------------------------------------------------
		/////jeep llanata delantera  derecha
		///// ---------------------------------------------------------------------------------------
		model = glm::translate(tmp, glm::vec3(-8.0f, -4.3f, 5.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		jeepllanta.Draw(staticShader);
		/////----------------------------------------------------------------------------------------
		/////jeep llanata trasera  izquierda
		///// ---------------------------------------------------------------------------------------
		model = glm::translate(tmp, glm::vec3(7.0f, -4.3f, -13.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		jeepllanta.Draw(staticShader);
		/////----------------------------------------------------------------------------------------
		/////jeep llanata trasera  izquierda
		///// ---------------------------------------------------------------------------------------
		model = glm::translate(tmp, glm::vec3(-8.0f, -4.3f, -13.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		jeepllanta.Draw(staticShader);
		////----------------------------------------------------------------------------
		////Helicoptero
		//// ----------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(450.0f+posX, 120.0f+posY, -1350.0f+posZ));
		model = glm::scale(model, glm::vec3(2.50f));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		Hcuerpo.Draw(staticShader);
		/////-------------------------------------------------------------------------------
		/////helice
		///// ------------------------------------------------------------------------------
		model = glm::translate(tmp, glm::vec3(0.0f, 3.0f, 0.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		helice.Draw(staticShader);
		/////------------------------------------------------------------------------------------
		/////helice cola
		///// -------------------------------------------------------------------------------
		model = glm::translate(tmp, glm::vec3(2.0f, 0.1f, 45.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		hcola.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		//staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		////Pierna Der
		//model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		////Pie Der
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		//staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		////Pierna Izq
		//model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		//model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		////Pie Iz
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		//staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		////Brazo derecho
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		//model = glm::rotate(model, glm::radians(-movBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		////Brazo izquierdo
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		//model = glm::rotate(model, glm::radians(-movBrazoizq), glm::vec3(0.0f, 0.0f, 1.0f));
		//staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		////Cabeza
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-rotCabeza), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		//staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		posY--;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		posY++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.y++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.y--;
	/*if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		playMusic();*/

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacion ^= true;
		/*if (movAuto_z >= 200 ) {
			movAuto_z = 0.0f;
		}*/
	}
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		movBrazoizq ++;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		movBrazoizq --;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		movBrazoDer++;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		movBrazoDer--;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodDer+=3.0f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodDer -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		rotCabeza += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		rotCabeza -= 3.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}