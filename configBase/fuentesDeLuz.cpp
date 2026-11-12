//Proyecto CGIH parte 1
#include <iostream>
#include <cmath>

#include <vector>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Phys
#include <btBulletDynamicsCommon.h>


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();


GLfloat current_animation_time{};
void DoAnimation(GLfloat time, std::vector<int>& keyposes, std::vector<Model>& Models, const Shader& shader, const bool condition);


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

float player_rotation {135.0f};
bool bIsFirstPerson{ false }; 
bool bIsRunning{ false }; 

// Positions of the point lights
//Arreglo de luces puntuales
glm::vec3 pointLightPositions[] = {
	glm::vec3(3.4f, 4.0f, 10.0f),
	glm::vec3(2.0f, 0.0f, 2.0f),
	glm::vec3(-2.0f, 0.0f, -2.0f),
	glm::vec3(2.0f, 0.0f, -2.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);


//Anim
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimDog = false;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
glm::vec3 dogPos(0.0f, 0.5f, 3.0f);
float dogRot = 0.0f;
float rotate_dog = 0.0f;
bool step = false;
bool rotated{ false };

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

GLfloat TotaldeltaTimes{ 0.0f };



int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	//------------------------------------------------------------------------------------------------------------------------------------
	// 1. Configuración de colisiones predeterminada
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	// 2. Despachador de colisiones (maneja los algoritmos de choque)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// 3. Broadphase (optimiza qué objetos pueden chocar antes de calcularlos con precisión)
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	// 4. Solucionador de restricciones (gravedad, fuerzas, articulaciones)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// 5. El Mundo Físico Dinámico
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	// Configurar la gravedad (eje Y hacia abajo)
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	// --- Crear el Suelo Estático (Caja delimitadora) ---
	btCollisionShape* groundShape = new btBoxShape(btVector3(15.0f, 0.5f, 25.0f));
	//btCollisionShape* groundShape = new btBoxShape(btVector3(50.0f, 1.0f, 50.0f));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -1.9f, 0)); // Posición en Y

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	// Al pasarle 0.0f de masa, Bullet sabe que el objeto NO se mueve (es estático)
	btRigidBody::btRigidBodyConstructionInfo groundInfo(0.0f, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundBody = new btRigidBody(groundInfo);
	dynamicsWorld->addRigidBody(groundBody);

	// --- Crear el Balón Dinámico (Esfera de colisión) ---
	btCollisionShape* sphereShape = new btSphereShape(0.5f); // Radio de 1.0
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 20.0f, 0)); // Empieza cayendo desde 20 unidades arriba

	btScalar mass = 1.0f;
	btVector3 localInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(mass, localInertia); // Calcula cómo debe rodar/rebotar

	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo sphereInfo(mass, sphereMotionState, sphereShape, localInertia);
	btRigidBody* sphereBody = new btRigidBody(sphereInfo);
	dynamicsWorld->addRigidBody(sphereBody);


	//CHARRRRRRRRRRRRRRRR
	// --- INICIALIZACIÓN (Antes del while) ---
// Radio de 0.5, Altura de la sección cilíndrica de 1.0 (Altura total = 2.0 unidades)
	btCollisionShape* playerShape = new btCapsuleShape(0.5f, 1.0f);

	btScalar playerMass = 75.0f;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(playerMass, playerInertia);

	btTransform playerStartTransform;
	playerStartTransform.setIdentity();
	playerStartTransform.setOrigin(btVector3(0, 2.0f, 0)); // Posición inicial del jugador
	btDefaultMotionState* playerMotionState = new btDefaultMotionState(playerStartTransform);

	btRigidBody::btRigidBodyConstructionInfo playerInfo(playerMass, playerMotionState, playerShape, playerInertia);
	btRigidBody* playerBody = new btRigidBody(playerInfo);

	// ¡CRÍTICO! Bloquear ejes de rotación X y Z. Solo permitimos giro sobre Y.
	playerBody->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	// Quitar fricción pura para no quedarse pegado a las paredes
	playerBody->setFriction(0.0f);

	dynamicsWorld->addRigidBody(playerBody);


	//-------------------------------------------------------------------------------------------------------------------------------------


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}



	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//Model Dog((char*)"Models/RedDog.obj");
	Model Dog((char*)"Models/ball.obj");
	Model Piso((char*)"Models/sand.obj");
	Model PisoPlaya((char*)"Models/sand.obj");

	Model SkyBox((char*)"Models/skysphere.obj");

	//Model chamaca((char*)"Models/MeinGott.glb");
	Model chamaca((char*)"Models/Player/PlayerIdle1.obj");
	
	
	//Model PRun1((char*)"Models/Player/PlayerRun1.obj");
	//Model PRun2((char*)"Models/Player/PlayerRun2.obj");
	//Model PRun3((char*)"Models/Player/PlayerRun3.obj");
	//Model PRun4((char*)"Models/Player/PlayerRun4.obj");
	//Model PRun5((char*)"Models/Player/PlayerRun5.obj");
	//Model PRun6((char*)"Models/Player/PlayerRun6.obj");
	//
	std::vector<Model> RunModels{/* PRun1,PRun2,PRun3,PRun4,PRun5,PRun6,PRun6*/};
	std::vector<int> Run_keyposes{/*0, 2, 8, 11, 13 , 18 ,20 */};

	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");

	//Model chamaca((char*)"Models/SK_HenoHeno06.fbx");

	Model Gradas((char*)"Models/Stands/Stands.obj");
	Model CanchaVolley((char*)"Models/Volleyball_Net/Voleyball_Court.obj");
	Model Gimnasio((char*)"Models/Gymnasium/gymnasium.obj");
	Model Salvavidas((char*)"Models/Lifeguard/lifeguard.obj");
	Model Silla1((char*)"Models/Beach_Chairs/chair1.obj");
	Model Silla2((char*)"Models/Beach_Chairs/chair2.obj");
	Model Sombrilla((char*)"Models/Umbrella/umbrella.obj");
	Model Edificio((char*)"Models/Edificio/Edificio.obj");

	Model Sea((char*)"Models/sea.obj");
	Model Mountein((char*)"Models/mounteindew.obj");



	

	// Al chile no se Bravo administración
	// --- Cargar Colisión Estática con Bullet Physics ---
	// Parámetros: Ruta, Posición (X,Y,Z), Rotación (X,Y,Z), Escala (X,Y,Z)
	btRigidBody* GradasCollision{ LoadRigidCollition("Models/Stands/Stands.obj", glm::vec3(0.0f, -0.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.3f, 1.3f, 1.3f)) };
	if (GradasCollision) {
		dynamicsWorld->addRigidBody(GradasCollision);
	}



	//Cargas de Colisiones Personalizadas
	btRigidBody* PlayaCollision{ LoadRigidCollition("Models/sand.obj", glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f)) };
	//ruta del modelo de colision, traslaciones, rotaciones y escalas para la colision
	if (PlayaCollision) { dynamicsWorld->addRigidBody(PlayaCollision); }


	btRigidBody* MounteinCollision{ LoadRigidCollition("Models/mounteindew.obj", glm::vec3(-120.0f, -1.0f, -20.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f)) };
	if (MounteinCollision) { dynamicsWorld->addRigidBody(MounteinCollision); }


	btRigidBody* SeaCollision{ LoadRigidCollition("Models/sea.obj", glm::vec3(0.0f,-3.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f)) };
	if (SeaCollision) { dynamicsWorld->addRigidBody(SeaCollision); }




	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.21f, 10000.0f); //Renderizado a objetos mas lejanos
	//glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		TotaldeltaTimes += deltaTime;
		current_animation_time += deltaTime;


		glfwPollEvents();
		DoMovement();
		Animation();


		//----------------------------------------------------------------------------------------------------




		//MOVEMENT
		// 1. Obtener la velocidad física actual (vital para preservar la caída de la gravedad en Y)
		btVector3 currentVelocity = playerBody->getLinearVelocity();
		btVector3 targetVelocity(0.0f, currentVelocity.getY(), 0.0f);

		float speed = 7.0f; // Ajusta la velocidad de caminata
		//float speed = 5.0f; // Ajusta la velocidad de caminata

		glm::vec3 moveDir(0.0f);

		// 2. Extraer los vectores direccionales de tu cámara
		glm::vec3 camFront = camera.GetFront();
		camFront.y = 0.0f; // Anular inclinación vertical
		camFront = glm::normalize(camFront);
		glm::vec3 camRight = glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f)));

		// 3. Leer las teclas directamente del arreglo que ya tienes configurado
		if (keys[GLFW_KEY_W]) moveDir += camFront;
		if (keys[GLFW_KEY_S]) moveDir -= camFront;
		if (keys[GLFW_KEY_A]) moveDir -= camRight;
		if (keys[GLFW_KEY_D]) moveDir += camRight;

		// 4. Aplicar el movimiento si hay teclas presionadas
		if (glm::length(moveDir) > 0.0f) {
			moveDir = glm::normalize(moveDir);
			targetVelocity.setX(moveDir.x * speed);
			targetVelocity.setZ(moveDir.z * speed);

			// Despertar el cuerpo: Bullet desactiva objetos quietos para ahorrar CPU. 
			// Si no lo despiertas, el jugador se congelará tras unos segundos inactivo.
			playerBody->activate(true);
		}


		// 4.5 Lógica de Salto con Raycast
		btVector3 startPos = playerBody->getWorldTransform().getOrigin();
		// Disparamos un rayo hacia abajo. 
		// La cápsula mide 2.0 de alto (1.0 de centro a pies) + 0.1 extra de margen para detectar el suelo
		btVector3 endPos = startPos - btVector3(0.0f, 1.4f, 0.0f);

		btCollisionWorld::ClosestRayResultCallback rayCallback(startPos, endPos);
		dynamicsWorld->rayTest(startPos, endPos, rayCallback);

		bool isFalling = !rayCallback.hasHit();

		if (keys[GLFW_KEY_SPACE] && !isFalling/*abs(currentVelocity.getY()) < 0.01f*/) {
			targetVelocity.setY(7.5f); // Fuerza/Altura del salto

			// "Consumimos" el input artificialmente para que no salga volando 
			// como un jetpack si mantienes la barra espaciadora presionada.
			keys[GLFW_KEY_SPACE] = false;
		}



		// 5. Inyectar la velocidad final al colisionador
		playerBody->setLinearVelocity(targetVelocity);




		dynamicsWorld->stepSimulation(deltaTime, 10);
		// 2. Obtener la nueva transformación (posición/rotación) del balón

		btTransform trans;


		sphereBody->getMotionState()->getWorldTransform(trans);

		// PRUEBA SIN GRÁFICOS: Descomenta esto para ver cómo los números caen en la consola
		// std::cout << "Altura Y del balon: " << trans.getOrigin().getY() << std::endl;

		// 3. Convertir la física directamente a matriz de GLM
		float matrix[16];
		trans.getOpenGLMatrix(matrix);
		glm::mat4 model = glm::make_mat4(matrix);




		playerBody->getMotionState()->getWorldTransform(trans);
		btVector3 pos = trans.getOrigin();

		// Calcular una posición para la cámara (ej. 3 metros atrás y 2 metros arriba del personaje)
		glm::vec3 offset = bIsFirstPerson ? glm::vec3(0.0f, 0.8f, 0.0f) : (camFront * -2.5f) + glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 offset = (camFront * -2.5f) + glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 targetCameraPos = glm::vec3(pos.x(), pos.y(), pos.z()) + offset;

		// Tu clase Camera asume tener un método o propiedad para asignar la posición.
		// Si tu variable interna de cámara es privada y no tiene setter, tendrás que agregarle 
		// un método SetPosition(glm::vec3 newPos) en Camera.h.
		camera.SetPosition(targetCameraPos);











		//----------------------------------------------------------------------------------------------------


		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);






		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);  //Direction  de
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), SunLight.x, SunLight.y, SunLight.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);


		//Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		/*glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);*/

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);




		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.1f, 0.01f, 0.01f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.2f, 3.3f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.5f, 0.5f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.0f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.1f, 0.1f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 5.2f, 2.6f, 2.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.7f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.5f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 6.1f, 0.3f, 2.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 3.9f, 0.9f, 4.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.6f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.7f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.1f, 0.1f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.1f, 0.1f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//glm::mat4 model(1);


		//Carga de modelo 








		// 1. Configurar vista y matrices generales
		//view = camera.GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// 2. Dibujar el Piso (Estático)
		glm::mat4 modelPiso = glm::mat4(1.0f);
		// La física del suelo la pusiste en Y = -1.0f, así que bajamos el modelo visual para que coincidan
		modelPiso = glm::translate(modelPiso, glm::vec3(0.0f, /*-1.0f*/0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiso));
		Piso.Draw(lightingShader);
		//PisoPlaya.Draw(lightingShader);
		SkyBox.Draw(lightingShader);



		//btTransform trans;
		playerBody->getMotionState()->getWorldTransform(trans);
		//float matrix[16];
		trans.getOpenGLMatrix(matrix);
		glm::mat4 PlayerModel = glm::make_mat4(matrix);

		// Compensación visual: Bajamos el modelo la mitad de la altura total de la cápsula (1.0 unidad) 
		// para que los pies del modelo visual toquen el fondo del colisionador invisible.

		if (glm::length(moveDir) > 0.2f) {
			player_rotation = atan2f(moveDir.x, moveDir.z);
			bIsRunning = true;
		}
		else {
			bIsRunning = false;
		}

		PlayerModel = glm::rotate(PlayerModel, player_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		//PlayerModel = glm::rotate(PlayerModel, 135.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		PlayerModel = glm::translate(PlayerModel, glm::vec3(0.0f, -1.0f, 0.0f));
		PlayerModel = glm::scale(PlayerModel, glm::vec3(0.21f, 0.21f, 0.21f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(PlayerModel));


		if (bIsRunning && !bIsFirstPerson) { //no dibujar al personaje corriendo en primera persona
			//DoAnimation(1.0f,Run_keyposes,RunModels,lightingShader,bIsRunning);
		}
		else  {
			chamaca.Draw(lightingShader);
		}
		
		//// (Opcional: Dibujar tu cubo del VAO estático en el origen)
		//glm::mat4 modelCubo = glm::mat4(1.0f);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCubo));
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);


		//Body
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp

		glm::mat4 modelDog = glm::mat4(1.0f);
		
		modelTemp = modelDog = glm::translate(modelDog, dogPos);
		modelTemp = modelDog = glm::rotate(modelDog, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		DogBody.Draw(lightingShader);
		//Head
		modelDog = modelTemp;
		modelDog = glm::translate(modelDog, glm::vec3(0.0f, 0.093f, 0.208f));
		modelDog = glm::rotate(modelDog, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		HeadDog.Draw(lightingShader);
		//Tail 
		modelDog = modelTemp;
		modelDog = glm::translate(modelDog, glm::vec3(0.0f, 0.026f, -0.288f));
		modelDog = glm::rotate(modelDog, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		modelDog = modelTemp;
		modelDog = glm::translate(modelDog, glm::vec3(0.112f, -0.044f, 0.074f));
		modelDog = glm::rotate(modelDog, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		modelDog = modelTemp;
		modelDog = glm::translate(modelDog, glm::vec3(-0.111f, -0.055f, 0.074f));
		modelDog = glm::rotate(modelDog, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		modelDog = modelTemp;
		modelDog = glm::translate(modelDog, glm::vec3(0.082f, -0.046, -0.218));
		modelDog = glm::rotate(modelDog, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		modelDog = modelTemp;
		modelDog = glm::translate(modelDog, glm::vec3(-0.083f, -0.057f, -0.231f));
		modelDog = glm::rotate(modelDog, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		B_RightLeg.Draw(lightingShader);





		// Sea (sea.obj)
		glm::mat4 modelSea = glm::mat4(1.0f);
		modelSea = glm::translate(modelSea, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSea));
		Sea.Draw(lightingShader);

		//Mountein (mounteindew.obj)
		glm::mat4 modelMountein = glm::mat4(1.0f);
		modelMountein = glm::translate(modelMountein, glm::vec3(100.0f, -2.0f, 120.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMountein));
		Mountein.Draw(lightingShader);

		// Gradas (Stands.obj)
		glm::mat4 modelGradas = glm::mat4(1.0f);
		modelGradas = glm::translate(modelGradas, glm::vec3(7.50f, 0.05f, 15.0f));
		modelGradas = glm::rotate(modelGradas, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGradas = glm::scale(modelGradas, glm::vec3(1.75f, 1.75f, 1.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGradas));
		Gradas.Draw(lightingShader);
		
		// Gradas2 (Stands.obj)
		glm::mat4 modelGradas2 = glm::mat4(1.0f);
		modelGradas2 = glm::translate(modelGradas2, glm::vec3(4.5f, 0.05f, 27.5f));
		modelGradas2 = glm::rotate(modelGradas2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGradas2 = glm::scale(modelGradas2, glm::vec3(1.75f, 1.75f, 1.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGradas2));
		Gradas.Draw(lightingShader);

		// Cancha (Voleyball_Court.obj)
		glm::mat4 modelCancha = glm::mat4(1.0f);
		modelCancha = glm::translate(modelCancha, glm::	vec3(0.0f, 0.003f, 15.0f));
		modelCancha = glm::scale(modelCancha, glm::vec3(0.6f, 1.0f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCancha));
		CanchaVolley.Draw(lightingShader);


		// Gimnasio (gymnasium.obj)
		glm::mat4 modelGimnasio = glm::mat4(1.0f);
		modelGimnasio = glm::translate(modelGimnasio, glm::vec3(0.0f, 0.005f, 15.0f));
		modelGimnasio = glm::rotate(modelGimnasio, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGimnasio = glm::scale(modelGimnasio, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGimnasio));
		Gimnasio.Draw(lightingShader);

		// Salvavidas (lifeguard.obj)
		glm::mat4 modelSalvavidas = glm::mat4(1.0f);
		modelSalvavidas = glm::translate(modelSalvavidas, glm::vec3(-10.0f, -1.25f, -15.0f));
		//modelSalvavidas = glm::rotate(modelSalvavidas, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSalvavidas = glm::scale(modelSalvavidas, glm::vec3(1.25f, 1.25f, 1.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSalvavidas));
		Salvavidas.Draw(lightingShader);

		// Sombrilla (umbrella.obj)
		glm::mat4 modelSombrilla = glm::mat4(1.0f);
		modelSombrilla = glm::translate(modelSombrilla, glm::vec3(8.0f, -1.75f, -15.0f));
		modelSombrilla = glm::scale(modelSombrilla, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSombrilla));
		Sombrilla.Draw(lightingShader);

		// Silla1 (chair1.obj) Café
		glm::mat4 modelSilla1 = glm::mat4(1.0f);
		modelSilla1 = glm::translate(modelSilla1, glm::vec3(10.0f, -1.75f, -15.0f));
		modelSilla1 = glm::rotate(modelSilla1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSilla1 = glm::scale(modelSilla1, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla1));
		Silla1.Draw(lightingShader);

		// Silla2 (chair2.obj) Azul
		glm::mat4 modelSilla2 = glm::mat4(1.0f);
		modelSilla2 = glm::translate(modelSilla2, glm::vec3(6.0f, -1.50f, -15.0f));
		modelSilla2 = glm::rotate(modelSilla2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSilla2 = glm::scale(modelSilla2, glm::vec3(1.25f, 1.25f, 1.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla2));
		Silla2.Draw(lightingShader);

		
		modelSombrilla = glm::mat4(1.0f);
		modelSombrilla = glm::translate(modelSombrilla, glm::vec3(-2.0f, -1.5f, -15.0f));
		modelSombrilla = glm::scale(modelSombrilla, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSombrilla));
		Sombrilla.Draw(lightingShader);

		modelSilla1 = glm::mat4(1.0f);
		modelSilla1 = glm::translate(modelSilla1, glm::vec3(0.0f, -1.25f, -15.0f));
		modelSilla1 = glm::rotate(modelSilla1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSilla1 = glm::scale(modelSilla1, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla1));
		Silla1.Draw(lightingShader);

		modelSilla2 = glm::mat4(1.0f);
		modelSilla2 = glm::translate(modelSilla2, glm::vec3(2.0f, -1.25f, -15.0f));
		modelSilla2 = glm::rotate(modelSilla2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSilla2 = glm::rotate(modelSilla2, glm::radians(05.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelSilla2 = glm::scale(modelSilla2, glm::vec3(1.25f, 1.25f, 1.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla2));
		Silla2.Draw(lightingShader);

		// Edificio (Edificio.obj)
		glm::mat4 modelE = glm::mat4(1.0f);
		modelE = glm::translate(modelE, glm::vec3(-15.0f, 0.05f, 35.0f));
		modelE = glm::rotate(modelE, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelE = glm::scale(modelE, glm::vec3(0.035f, 0.035f, 0.035f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelE));
		Edificio.Draw(lightingShader);



		// 3. Dibujar el Balón/Dog (Dinámico)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// ¡AQUÍ ESTÁ LA MAGIA! Pasamos la matriz 'model' calculada por Bullet Physics directamente al shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Dog.Draw(lightingShader);

		glDisable(GL_BLEND);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


		/*
		//      view = camera.GetViewMatrix();
		//model = glm::mat4(1);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Piso.Draw(lightingShader);


		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);



		//model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa -----------------------------------------------
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
	 //   Dog.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa ---------------------------------------------
		//glBindVertexArray(0);
		*/

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	/*if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}*/

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_P])
	{
		AnimDog = !AnimDog;

	}

	//if (keys[GLFW_KEY_SPACE])
	//{
	//	active = !active;
	//	if (active)
	//	{
	//		Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
	//	}
	//	else
	//	{
	//		Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
	//	}
	//}
	if (keys[GLFW_KEY_1])
	{
		bIsFirstPerson = !bIsFirstPerson;
	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


void Animation() {
	if (AnimBall)
	{
		rotBall += 0.4f;
		//printf("%f", rotBall);
	}

	float quantity{ 0.5f };


	if (AnimDog) {
		

		dogPos.z += 0.005f;


		if (!step) {
			RLegs -= quantity;
			FLegs -= quantity;
			head -= quantity;
			tail -= quantity;
			if (RLegs <= -15.0f) { step = !step; }
		}
		else {
			RLegs += quantity;
			FLegs += quantity;
			head += quantity;
			tail += quantity;
			if (RLegs >= 15.0f) { step = !step; }
		}
	}
}

void DoAnimation(GLfloat time, std::vector<int>& keyposes, std::vector<Model>& Models, const Shader& shader, const bool condition) {

	GLfloat choppedtime{ time / keyposes.back()};

	if (!condition || current_animation_time > choppedtime*keyposes.back()) { 
		current_animation_time = 0; 
		return;
	}

	for (int i{ 0 }; i < keyposes.size() -1; i++) {
		if (current_animation_time >= choppedtime * keyposes[i] && current_animation_time <= choppedtime * keyposes[i + 1]) {
			Models[i].Draw(shader);
		}
	}

}