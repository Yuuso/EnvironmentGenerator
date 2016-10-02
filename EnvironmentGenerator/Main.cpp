
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Time.h>

#include "Environment.h"

#include <iostream>


static bool run = true;

spehs::Camera2D* camera;
spehs::BatchManager* batchManager;


void init()
{
	spehs::initialize("Environment Generator");
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);
	mainWindow->clearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
void uninit()
{
	delete batchManager;
	delete camera;
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

	std::cin.ignore();
	return 0;
}