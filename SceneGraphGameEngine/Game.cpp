#include "Game.h"
#include "time.h"

float gSpeed = 0.25f;
float gMouseSpeed = 0.0075f;

Game::Game()
{
	mDayTexture = NULL;
	mSkybox = NULL;
	mTerrain = NULL;
	mNightTexture = NULL;
}

Game::~Game()
{
	for (int i = 0;i < gTileTextures.size();i++)
	{
		if (gTileTextures[i] != NULL)
			delete gTileTextures[i];
	}
	for (int i = 0;i < mMeshes.size();i++)
	{
		if (mMeshes[i] != NULL)
			delete mMeshes[i];
	}
	for (int i = 0;i < mTextures.size();i++)
	{
		if (mTextures[i] != NULL)
			delete mTextures[i];
	}
	for (int i = 0;i < mShaders.size();i++)
	{
		if (mShaders[i] != NULL)
			delete mShaders[i];
	}
	if (mTerrain != NULL)
		delete mTerrain;
	if (gTerrain3DTexture != NULL)
		delete gTerrain3DTexture;
	if (mDayTexture != NULL)
		delete mDayTexture;
}

bool Game::init()
{
	srand(time(0));
	loadTextures();

	mMeshes.push_back(new Mesh("Resources//Models//cube2.obj"));
	mMeshes.push_back(new Mesh("Resources//Models//alduin.obj"));
	mMeshes.push_back(new Mesh("Resources//Models//tree.obj"));

	mTextures.push_back(new Texture("Resources//Textures//alduin.jpg"));

	mDayTexture = new CubeMapTexture();
	mDayTexture->load("Resources//Textures//skybox//right.png", "Resources//Textures//skybox//left.png", "Resources//Textures//skybox//top.png",
		"Resources//Textures//skybox//bottom.png",  "Resources//Textures//skybox//back.png", "Resources//Textures//skybox//front.png");

	mSkybox = new SkyBox(mMeshes[0], mDayTexture);
	mCamera.setPosition(glm::vec3(0, 0, 0));

	//m.loadFromFile("Resources//Models//uvsphere_highres.obj");
	//t.loadFromFile("Resources//Textures//sky.png");
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_ambient", "#define MULTI_TEXTURE\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_directional", "#define MULTI_TEXTURE\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_point", "#define MULTI_TEXTURE\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_ambient"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_directional"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_point"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//skybox"));
	
	mShaders[1]->bind();
	mShaders[1]->setUniformVec3f("gLight.dir", glm::vec3(-1, -1, 1));
	mShaders[1]->setUniformFloat("gLight.intensity", 0.5);
	mShaders[1]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));
	
	mShaders[2]->bind();
	mShaders[2]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	mShaders[2]->setUniformFloat("gLight.intensity", 500);
	mShaders[2]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	mShaders[4]->bind();
	mShaders[4]->setUniformVec3f("gLight.dir", glm::vec3(-1, -1, 1));
	mShaders[4]->setUniformFloat("gLight.intensity", 0.5);
	mShaders[4]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	mShaders[5]->bind();
	mShaders[5]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	mShaders[5]->setUniformFloat("gLight.intensity", 500);
	mShaders[5]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	//mShaders[3]->bind();
	//mShaders[3]->setUniformVec3f("gSkyColor", glm::vec3(0, 0, 1));

	mTerrain = new Terrain();
	mTerrain->generate();

	for (int i = 0;i < 50;i++)
	{
		float x = rand() % 64;
		float z = rand() % 64;
		float y = mTerrain->getHeight(x, z);
		mEntities.push_back(glm::translate(glm::mat4(1.0), glm::vec3(x, y, z)));
	}

	mShadowFBO.createFBO(SCREEN_WIDTH, SCREEN_HEIGHT);
	mShadowFBO.createDepthBufferAttachment();

	return true;
}

void Game::render(SDL_Window* window)
{
	glm::mat4 model, view, projection;
	mCamera.render(view, projection);

	glDepthMask(true);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	mShaders[6]->bind();
	mShaders[6]->setUniformMat4f("gModelMat", glm::translate(glm::mat4(1.0), mCamera.mPosition));
	mShaders[6]->setUniformMat4f("gViewMat", view);
	mShaders[6]->setUniformMat4f("gProjectionMat", projection);
	mSkybox->render();

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);

	gTerrain3DTexture->bind();
	mTextures[0]->bind();

	mShaders[0]->bind();
	mShaders[0]->setUniformMat4f("M", glm::scale(glm::vec3(1,1,1)));
	mShaders[0]->setUniformMat4f("V", view);
	mShaders[0]->setUniformMat4f("P", projection);
	
	mTerrain->render();

	mShaders[3]->bind();
	mShaders[3]->setUniformMat4f("V", view);
	mShaders[3]->setUniformMat4f("P", projection);
	for (int i = 0;i < mEntities.size();i++)
	{
		mShaders[3]->setUniformMat4f("M", mEntities[i]);
		mMeshes[2]->render();
	}
	//mMeshes[1]->render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);

	mShaders[1]->bind();
	mShaders[1]->setUniformMat4f("gModelMat", glm::scale(glm::vec3(1, 1, 1)));
	mShaders[1]->setUniformMat4f("gViewMat", view);
	mShaders[1]->setUniformMat4f("gProjectionMat", projection);
	mShaders[1]->setUniformVec3f("gEyePos", mCamera.mPosition);
	mTerrain->render();

	mShaders[4]->setUniformMat4f("gViewMat", view);
	mShaders[4]->setUniformMat4f("gProjectionMat", projection);
	mShaders[4]->setUniformVec3f("gEyePos", mCamera.mPosition);
	for (int i = 0;i < mEntities.size();i++)
	{
		mShaders[4]->setUniformMat4f("gModelMat", mEntities[i]);
		mMeshes[2]->render();
	}
	
	//mMeshes[1]->render();

	mShaders[2]->bind();
	mShaders[2]->setUniformMat4f("gModelMat", glm::scale(glm::vec3(1, 1, 1)));
	mShaders[2]->setUniformMat4f("gViewMat", view);
	mShaders[2]->setUniformMat4f("gProjectionMat", projection);
	mShaders[2]->setUniformVec3f("gEyePos", mCamera.mPosition);
	mShaders[2]->setUniformVec3f("gLight.pos", mCamera.mPosition);

	mShaders[5]->setUniformMat4f("gViewMat", view);
	mShaders[5]->setUniformMat4f("gProjectionMat", projection);
	mShaders[5]->setUniformVec3f("gEyePos", mCamera.mPosition);
	mShaders[5]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	mTerrain->render();
	for (int i = 0;i < mEntities.size();i++)
	{
		mShaders[5]->setUniformMat4f("gModelMat", mEntities[i]);
		mMeshes[2]->render();
	}
}

void Game::update(int deltaTime)
{
	mCamera.update();
}

void Game::handleEvent(SDL_Event e, int deltaTime)
{
	printf("deltatime: %d\n", deltaTime);
	//deltaTime = deltaTime / 1000;
	if (e.type == SDL_MOUSEMOTION)
	{
		Vector2i pos(e.motion.xrel, e.motion.yrel);

		mCamera.horizontalAngle -= gMouseSpeed * deltaTime * pos.x;
		mCamera.verticalAngle -= gMouseSpeed * deltaTime * pos.y;
		if (mCamera.verticalAngle > 3.14f / 2)
		{
			mCamera.verticalAngle = 3.14f / 2;
		}
		if (mCamera.verticalAngle < -3.14f / 2)
		{
			mCamera.verticalAngle = -3.14f / 2;
		}
		/*if (gCamera.horizontalAngle > 3.14f)
		{
		gCamera.horizontalAngle -= 2*3.14f;
		}
		if (gCamera.horizontalAngle < -3.14f)
		{
		gCamera.horizontalAngle += 2*3.14f;
		}*/
	}

	//glm::vec3 dir = glm::vec3(gCamera.direction.x, 0, gCamera.direction.z);
	glm::vec3 dir = mCamera.mDirection;

	// Movement
	if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_w)
		{
			mCamera.mPosition += dir * (deltaTime * gSpeed);
		}

		if (e.key.keysym.sym == SDLK_s)
		{
			mCamera.mPosition -= dir * (deltaTime * gSpeed);
		}

		if (e.key.keysym.sym == SDLK_a)
		{
			mCamera.mPosition -= mCamera.mRight * (deltaTime * gSpeed);
		}

		if (e.key.keysym.sym == SDLK_d)
		{
			mCamera.mPosition += mCamera.mRight * (deltaTime * gSpeed);
		}

		/*if (e.key.code == sf::Keyboard::E)
		{
		updateFrustum = !updateFrustum;
		}*/
		/*if (e.key.code == sf::Keyboard::F)
		{
		gOcclusionCulling = !gOcclusionCulling;
		}*/
	}

	/*if (e.type == sf::Event::MouseWheelMoved)
	{
	gCamera.FoV = std::max(30.f,std::min(60.f, gCamera.FoV-e.mouseWheel.delta));
	}*/
	//mCamera.update();
}


