
#include "Environment.h"

#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Time.h>

#include <iostream>


extern int64_t sectorAllocations;
extern int64_t sectorDeallocations;


static bool run = true;

spehs::Camera2D* camera;
spehs::BatchManager* batchManager;
Environment* environment;
spehs::WorldPosition location(0, 0);

void init()
{
	spehs::initialize("Environment Generator");
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);
	mainWindow->clearColor(0.0f, 0.0f, 0.0f, 1.0f);

	environment = new Environment(0, &location);
}
void uninit()
{
	delete batchManager;
	delete camera;
	delete environment;
	spehs::uninitialize();
}
bool input()
{
	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

#pragma region CAMERA MOVEMENT
	static float cameraMoveSpeed;
	static float cameraZoomSpeed = 0.01f;
	cameraMoveSpeed = 1000.0f * spehs::getDeltaTime().asSeconds;
	if (inputManager->isKeyDown(KEYBOARD_LEFT))
	{
		camera->translate(glm::vec2(-cameraMoveSpeed, 0.0f));
	}
	if (inputManager->isKeyDown(KEYBOARD_RIGHT))
	{
		camera->translate(glm::vec2(cameraMoveSpeed, 0.0f));
	}
	if (inputManager->isKeyDown(KEYBOARD_UP))
	{
		camera->translate(glm::vec2(0.0f, cameraMoveSpeed));
	}
	if (inputManager->isKeyDown(KEYBOARD_DOWN))
	{
		camera->translate(glm::vec2(0.0f, -cameraMoveSpeed));
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEDOWN))
	{
		camera->scale = camera->scale - cameraZoomSpeed * camera->scale;
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEUP))
	{
		camera->scale = camera->scale + cameraZoomSpeed * camera->scale;
	}
	if (inputManager->isKeyDown(KEYBOARD_END))
	{
		camera->scale = 1.0f;
	}
#pragma endregion

	return true;
}
bool update()
{
	if (!input())
		return false;
	camera->update();
	location.setPosition(camera->position.x, camera->position.y);
	environment->update();
	spehs::console::update();

	return true;
}
void render()
{
	mainWindow->renderBegin();
	batchManager->render();
	spehs::console::render();
	spehs::drawFPS();
	mainWindow->renderEnd();
}

int main(char argc, char** argv)
{
	init();
	while (run)
	{
		spehs::beginFPS();
		run = update();
		render();
		spehs::endFPS();
	}
	uninit();

	std::cout << "Sector Allocations: " << sectorAllocations - sectorDeallocations << "/" << sectorAllocations << std::endl;
	std::cin.ignore();

	return 0;
}