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
	mMeshes.push_back(new Mesh("Resources//Models//plane.obj"));

	mWater = new Water(mMeshes[3]);

	mTextures.push_back(new Texture("Resources//Textures//alduin.jpg"));
	mTextures.push_back(new Texture("Resources//Textures//alduin_n.jpg"));
	mTextures.push_back(new Texture("Resources//Textures//Water//waterDUDV.png"));
	
	mDayTexture = new CubeMapTexture();
	mDayTexture->load("Resources//Textures//skybox//right.png", "Resources//Textures//skybox//left.png", "Resources//Textures//skybox//top.png",
		"Resources//Textures//skybox//bottom.png",  "Resources//Textures//skybox//back.png", "Resources//Textures//skybox//front.png");

	mSkybox = new SkyBox(mMeshes[0], mDayTexture);
	mCamera.setPosition(glm::vec3(0, 0, 0));

	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_ambient"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_directional"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_point"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_ambient", "#define MULTI_TEXTURE\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_directional", "#define MULTI_TEXTURE\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_point", "#define MULTI_TEXTURE\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//skybox"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_ambient", "#define NORMAL_MAP\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_directional", "#define NORMAL_MAP\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//forward_point", "#define NORMAL_MAP\n"));
	mShaders.push_back(new ShaderProgram("Resources//Shaders//water"));
	
	mShaders[1]->bind();
	mShaders[1]->setUniformVec3f("gLight.dir", glm::vec3(-1, -1, 1));
	mShaders[1]->setUniformFloat("gLight.intensity", 1);
	mShaders[1]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));
	
	mShaders[2]->bind();
	mShaders[2]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	mShaders[2]->setUniformFloat("gLight.intensity", 500);
	mShaders[2]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	mShaders[4]->bind();
	mShaders[4]->setUniformVec3f("gLight.dir", glm::vec3(-1, -1, 1));
	mShaders[4]->setUniformFloat("gLight.intensity", 1);
	mShaders[4]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	mShaders[5]->bind();
	mShaders[5]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	mShaders[5]->setUniformFloat("gLight.intensity", 500);
	mShaders[5]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	mShaders[8]->bind();
	mShaders[8]->setUniformVec3f("gLight.dir", glm::vec3(-1, -1, 1));
	mShaders[8]->setUniformFloat("gLight.intensity", 1);
	mShaders[8]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	mShaders[9]->bind();
	mShaders[9]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	mShaders[9]->setUniformFloat("gLight.intensity", 500);
	mShaders[9]->setUniformVec3f("gLight.color", glm::vec3(1, 1, 1));

	mShaders[10]->bind();
	mShaders[10]->setUniformVec3f("gColor", glm::vec3(0.5, 0.5, 1.0));
	mShaders[10]->setUniformFloat("gWaveStrength", mWater->mWaveStrength);
	mShaders[10]->setUniformFloat("gWaveHeight", mWater->mWaveHeight);
	mShaders[10]->setUniformFloat("gSpecularFactor", mWater->mSpecularFactor);
	mShaders[10]->setUniformFloat("gShineDamper", mWater->mShineDamper);
	mShaders[10]->setUniformFloat("gScreenNear", mCamera.mScreenNear);
	mShaders[10]->setUniformFloat("gScreenFar", mCamera.mScreenFar);
	mShaders[10]->setTextureLocation("gReflectionTexture", 0);
	mShaders[10]->setTextureLocation("gRefractionTexture", 1);
	mShaders[10]->setTextureLocation("gDuDv", 2);
	mShaders[10]->setTextureLocation("gDepthTexture", 3);

	/*GLuint t1Location = glGetUniformLocation(mShaders[10]->getProgramID(), "gReflectionTexture");
	GLuint t2Location = glGetUniformLocation(mShaders[10]->getProgramID(), "gRefractionTexture");
	GLuint t3Location = glGetUniformLocation(mShaders[10]->getProgramID(), "gDuDv");
	glUniform1i(t1Location, 0);
	glUniform1i(t2Location, 1);
	glUniform1i(t3Location, 2);*/

	//mShaders[3]->bind();
	//mShaders[3]->setUniformVec3f("gSkyColor", glm::vec3(0, 0, 1));

	mTerrain = new Terrain();
	mTerrain->generate();

	for (int i = 0;i < 50;i++)
	{
		float x = rand() % 320;
		float z = rand() % 320;
		float y = mTerrain->getHeight(x, z);
		mEntities.push_back(glm::translate(glm::mat4(1.0), glm::vec3(x, y, z)));
	}

	mShadowFBO.initFBO(SCREEN_WIDTH, SCREEN_HEIGHT, GL_NONE);
	mShadowFBO.createDepthBufferAttachment();
	
	return true;
}


void Game::renderSkybox(const glm::mat4& view, const glm::mat4& projection)
{
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	mShaders[6]->bind();
	mShaders[6]->setUniformMat4f("gModelMat", glm::translate(glm::mat4(1.0), mCamera.mPosition));
	mShaders[6]->setUniformMat4f("gViewMat", view);
	mShaders[6]->setUniformMat4f("gProjectionMat", projection);
	mSkybox->render();

	glCullFace(GL_BACK);
}

void Game::renderScene(const glm::mat4& view, const glm::mat4& projection)
{
	/*GLint drawFboId = 0, readFboId = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);
	printf("Rendering with: %d %d\n", drawFboId, readFboId);*/

	glDepthMask(true);
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	renderSkybox(view, projection);

	glDepthFunc(GL_LESS);

	gTerrain3DTexture->bind();
	mTextures[0]->bind();

	mShaders[3]->bind();
	mShaders[3]->setUniformMat4f("M", glm::scale(glm::vec3(1, 1, 1)));
	mShaders[3]->setUniformMat4f("V", view);
	mShaders[3]->setUniformMat4f("P", projection);

	mTerrain->render();

	mShaders[0]->bind();
	mShaders[0]->setUniformMat4f("V", view);
	mShaders[0]->setUniformMat4f("P", projection);
	for (int i = 0;i < mEntities.size();i++)
	{
		mShaders[0]->setUniformMat4f("M", mEntities[i]);
		mMeshes[2]->render();
	}
	mMeshes[1]->render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);

	mShaders[4]->bind();
	mShaders[4]->setUniformMat4f("gModelMat", glm::scale(glm::vec3(1, 1, 1)));
	mShaders[4]->setUniformMat4f("gViewMat", view);
	mShaders[4]->setUniformMat4f("gProjectionMat", projection);
	mShaders[4]->setUniformVec3f("gEyePos", mCamera.mPosition);
	mTerrain->render();

	mShaders[1]->bind();
	mShaders[1]->setUniformMat4f("gViewMat", view);
	mShaders[1]->setUniformMat4f("gProjectionMat", projection);
	mShaders[1]->setUniformVec3f("gEyePos", mCamera.mPosition);
	for (int i = 0;i < mEntities.size();i++)
	{
		mShaders[1]->setUniformMat4f("gModelMat", mEntities[i]);
		mMeshes[2]->render();
	}
	mMeshes[1]->render();

	mShaders[5]->bind();
	mShaders[5]->setUniformMat4f("gModelMat", glm::scale(glm::vec3(1, 1, 1)));
	mShaders[5]->setUniformMat4f("gViewMat", view);
	mShaders[5]->setUniformMat4f("gProjectionMat", projection);
	mShaders[5]->setUniformVec3f("gEyePos", mCamera.mPosition);
	mShaders[5]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	mTerrain->render();

	mShaders[2]->bind();
	mShaders[2]->setUniformMat4f("gViewMat", view);
	mShaders[2]->setUniformMat4f("gProjectionMat", projection);
	mShaders[2]->setUniformVec3f("gEyePos", mCamera.mPosition);
	mShaders[2]->setUniformVec3f("gLight.pos", mCamera.mPosition);
	for (int i = 0;i < mEntities.size();i++)
	{
		mShaders[2]->setUniformMat4f("gModelMat", mEntities[i]);
		mMeshes[2]->render();
	}
	mMeshes[1]->render();

	glDepthMask(true);
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
}

void Game::render(SDL_Window* window)
{
	glm::mat4 model, view, projection, view2, projection2;
	mCamera.render(view2, projection2);

	mCamera.mPosition = glm::vec3(mCamera.mPosition.x, -mCamera.mPosition.y, mCamera.mPosition.z);
	mCamera.mVerticalAngle = -mCamera.mVerticalAngle;
	mCamera.render(view, projection);
	mWater->bindReflection();
	renderScene(view, projection);

	mCamera.mPosition = glm::vec3(mCamera.mPosition.x, -mCamera.mPosition.y, mCamera.mPosition.z);
	mCamera.mVerticalAngle = -mCamera.mVerticalAngle;
	mCamera.render(view, projection);
	mWater->bindRefraction();
	renderScene(view, projection);
	assert(view == view2 && projection == projection2);
	mWater->mRefractionFBO.unbind();
	
	renderScene(view, projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mWater->mReflectionFBO.mColorTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mWater->mRefractionFBO.mColorTexture);
	mTextures[2]->bind(GL_TEXTURE2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mWater->mRefractionFBO.mDepthTexture);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mShaders[10]->bind();
	mShaders[10]->setUniformMat4f("gModelMat", glm::translate(glm::scale(glm::vec3(160, 160, 160)), glm::vec3(1,0,1)));
	mShaders[10]->setUniformMat4f("gViewMat", view);
	mShaders[10]->setUniformMat4f("gProjectionMat", projection);
	mShaders[10]->setUniformFloat("gDuDvOffset", mWater->mDuDvOffset);
	mShaders[10]->setUniformVec3f("gEyePos", mCamera.mPosition);
	mShaders[10]->setUniformVec3f("gLightDir", glm::vec3(-1, -1, 1));
	mWater->render();
}

void Game::update(int deltaTime)
{
	mCamera.update();
	mWater->update(deltaTime);
}

void Game::handleEvent(SDL_Event e, int deltaTime)
{
	printf("deltatime: %d\n", deltaTime);
	//deltaTime = deltaTime / 1000;
	if (e.type == SDL_MOUSEMOTION)
	{
		Vector2i pos(e.motion.xrel, e.motion.yrel);

		mCamera.mHorizontalAngle -= gMouseSpeed * deltaTime * pos.x;
		mCamera.mVerticalAngle -= gMouseSpeed * deltaTime * pos.y;
		if (mCamera.mVerticalAngle > 3.14f / 2)
		{
			mCamera.mVerticalAngle = 3.14f / 2;
		}
		if (mCamera.mVerticalAngle < -3.14f / 2)
		{
			mCamera.mVerticalAngle = -3.14f / 2;
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


