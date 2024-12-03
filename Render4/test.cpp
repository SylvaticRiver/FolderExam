
#include"Mesh.h"
#include "Object.h"
#include "Shape.h"
#include <list>
#include <array>
#include "BSplineSurface.h"
#include "SOSIConverter.h"
#include "PhysicsObject.h"

int windowWidth = 1280;
int windowHeight = 720;

double cursorX = 0;
double cursorY = 0;

GLFWwindow* window;

bool cursorEnabled = false;
bool wireframe = false;
bool showLights = true;
bool dotRender = false;
bool tpSphere = false;
int selectedSphere = 1;

float lastTick = 0;
float tickDelta;

//Press R to render dots / vertices
//press V to render wireframe mode

//noen kommentarer er norsk mens andre er engelsk. dokumentasjon fra forelesningsnotater er norsk, mens forklaring er er engelsk siden jeg ikke kan bruke de siste 3 bokstavene i alfabetet

void processInput(GLFWwindow* window, Camera& camera) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
		cout << "I have been closed" << endl;
	}
	camera.Inputs(window, cursorEnabled);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	cout << "width = " << width << " height = " << height << endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		cursorEnabled = !cursorEnabled;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		wireframe = !wireframe;
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		showLights = !showLights;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		dotRender = !dotRender;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		tpSphere = true;
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		selectedSphere = 1;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		selectedSphere = 2;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		selectedSphere = 3;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		selectedSphere = 4;
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		selectedSphere = 5;
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		selectedSphere = 6;
	}
	if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
		selectedSphere = 7;
	}
	if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
		selectedSphere = 8;
	}
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		selectedSphere = 9;
	}
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && cursorEnabled) {
		glfwGetCursorPos(window, &cursorX, &cursorY);

		cout << "mouse X = " << cursorX << " mouse y = " << cursorY << endl;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Hello again", NULL, NULL);
	if (window == NULL) {
		cout << "Window was never created" << endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(window, mouseCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to init GLAD" << endl;
		glfwTerminate();
	}

	SOSIConverter converter("SOSIData/Basisdata_3403_Hamar_25832_N50Hoyde_SOSI.sos");

	converter.createAndWriteCustomFile("SOSIData/HeightData.chd");
	converter.renderHeight("SOSIData/HeightData.chd", 24);

	Texture textures[]
	{
		Texture("empty_keg.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("empty_keg_spec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	Shader shaderProgram("Resources/Vertexshader.vert", "Resources/FragmentShader.frag");
	Shape sph(Shape::SPHERE);
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	Mesh sMesh(sph.getVerts(), sph.getInds(), tex);
	Mesh height(converter.vertexes, converter.indices, tex);

	Shader lightShader("Resources/LightVert.vert", "Resources/LightFrag.frag");
	Shape lig(Shape::CUBE);
	Mesh lightMesh(lig.getVerts(), lig.getInds(), tex);

	LightObject light(lightMesh, glm::vec3(250.0f, 225.0f, 100.0f), glm::vec3(1.0f, 1.0f, 1.0f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1000.0f, lightShader, shaderProgram);

	PhysicsObject* sphere = new PhysicsObject(sMesh, glm::vec3(250.0f, 75.0f, 75.0f), glm::vec3(1.0f, 1.0f, 1.0f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, true, 1.0f);
	PhysicsObject* sphere2 = new PhysicsObject(sMesh, glm::vec3(250.0f, 200.0f, 75.0f), glm::vec3(1.0f, 1.0f, 1.0f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, true, 2.0f);

	vector<PhysicsObject*> spheres = vector<PhysicsObject*>{ sphere, sphere2 };

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			spheres.push_back(new PhysicsObject(sMesh, glm::vec3(100.0f + (20 * i), 200.0f, 50.0f + (20 * j)), glm::vec3(1.0f, 1.0f, 1.0f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, true, i * 0.2 + (j / 2)));
		}
	}

	glEnable(GL_DEPTH_TEST);

	Camera camera(windowWidth, windowHeight, glm::vec3(70.1f, 50.2f, 100.0));
	camera.orientation = glm::rotate(camera.orientation, glm::radians(-160.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	camera.orientation = glm::rotate(camera.orientation, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera.orientation = glm::vec3(0.88f, -0.44f, -0.12f);

	BSplineSurface surface = BSplineSurface();
	Mesh spline(surface.verts, surface.ind, tex);

	//BSplineSurface
	//I couldn't manbage to get it within the camera view, so you might have to look around a little
	//Press V to enable wireframe mode
	Object plane2(spline, glm::vec3(50.0f, 100.0f, -100.0f), glm::vec3(20.0f, 20.0f, 20.0f), Rotator{ glm::vec3(0, 1, 0), 0 }, shaderProgram);


	//DotCloud
	//press R to enable dot rendering
	Object plane(height, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), Rotator{ glm::vec3(0, 1, 0), 0 }, shaderProgram);

	vector<Triangle> planeTris = converter.triangles;

	while (!glfwWindowShouldClose(window))
	{
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		if (cursorEnabled) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		glClearColor(0.533, 0.996, 1, 0.78);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "cameraMatrix");
		camera.UpdateMatrix(45.0f, 0.1f, 1000.0f);
		if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
			processInput(window, camera);
		}

		for (int i = 0; i < planeTris.size(); i++) {
			glm::vec3 barycentricCoordinates;
			glm::vec3 P0;
			glm::vec3 P1;
			glm::vec3 P2;
			Triangle tris = planeTris[i];
			P0 = tris.verticies[0].pos;
			P1 = tris.verticies[1].pos;
			P2 = tris.verticies[2].pos;

			for (PhysicsObject* ball : spheres) {
				barycentricCoordinates = ball->barycentricCoordinates(P0, P1, P2);

				bool b1 = barycentricCoordinates.x >= -1e-6f && barycentricCoordinates.x <= 1 + 1e-6f;
				bool b2 = barycentricCoordinates.y >= -1e-6f && barycentricCoordinates.y <= 1 + 1e-6f;
				bool b3 = barycentricCoordinates.z >= -1e-6f && barycentricCoordinates.z <= 1 + 1e-6f;
				bool b4 = barycentricCoordinates.x + barycentricCoordinates.y + barycentricCoordinates.z <= 1 + 1e-6f;

				if (b1 && b2 && b3 && b4) {
					float sphereHeight = ball->getBarHeight(tris, barycentricCoordinates) + (ball->size.y / 2);

					if (ball->currPos.y < sphereHeight) {
						ball->setPos(glm::vec3(ball->currPos[0], sphereHeight, ball->currPos[2]));
						ball->rollOnsurface(tris);
					}
				}
			}
		}

		if (sphere->isCollidingBall(sphere2)) {
			sphere->onBallCollision(sphere2);
		}

		if (tpSphere) {
			PhysicsObject* selected = spheres[min(selectedSphere - 1, (int)spheres.size())];
			selected->velocity *= 0;
			selected->setPos(camera.position);
			tpSphere = false;
		}

		plane.Render(shaderProgram, camera, dotRender);
		plane2.Render(shaderProgram, camera, dotRender);

		for (PhysicsObject* ball : spheres) {
			ball->Render(shaderProgram, camera, dotRender);
		}

		if (showLights) {
			light.Render(lightShader, camera, dotRender);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	lightShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
