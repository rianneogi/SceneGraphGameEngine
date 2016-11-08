//Scene Graph and Component based game engine

#include "Game.h"

#define USE_VSYNC
//#define USE_MSAA

Game* gGame = NULL;
SDL_Window* gWindow = NULL;
SDL_GLContext gContext;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int MAX_FPS = 60;

bool initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		_getch();
	}

	//Initialize DevIL
	ilInit();
	ilClearColour(255, 255, 255, 000);

	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		printf("Error initializing DevIL! %s\n", iluErrorString(ilError));
		_getch();
	}

	gWindow = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		_getch();
	}

	//SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);*/
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Anti-aliasing
#ifdef USE_MSAA
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
#endif

	gContext = SDL_GL_CreateContext(gWindow);
	if (gContext == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		_getch();
	}
	SDL_GL_MakeCurrent(gWindow, gContext);
	
	int x;
	SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &x);
	printf("BUFFERS: %d\n", x);

#ifdef USE_MSAA
	glEnable(GL_MULTISAMPLE);
#endif

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("GL error when initializing SDL %s\n", gluErrorString(error));
		_getch();
		return false;
	}

	return true;
}

bool initGL()
{
	const GLubyte* glVersion(glGetString(GL_VERSION));
	printf("OpenGL max version supported: %s\n", glVersion);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		return false;
	}
	
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) //catch this error to ignore it
	{
		printf("GL error when initializing GLEW %s\n", gluErrorString(error));
	}

	//Make sure OpenGL 3.0 is supported
	if (!GLEW_VERSION_3_1)
	{
		printf("OpenGL 3.1 not supported!\n");
		//return false;
	}

	if (!GLEW_ARB_framebuffer_object)
	{
		printf("framebuffer objects not supported\n");
		_getch();
	}

	//Set the viewport
	glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Initialize clear color
	glClearColor(0, 0, 0, 1);

	//Enable texturing
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE_3D);
	//glEnable(GL_TEXTURE_CUBE_MAP);

	//Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glDepthFunc(GL_LESS);

	//Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		_getch();
		return false;
	}

	return true;
}

void mainLoop()
{
	bool running = true;
	SDL_Event e;

	//sf::Clock FPSTimer;
	//Uint32 time = SDL_GetTicks();
	Timer MainTimer;
	Timer FPSTimer;
	unsigned int frameCount = 0;
	while (running)
	{
		Uint32 deltaTime = MainTimer.getElaspedTime();
		MainTimer.restart();
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				// end the program
				running = false;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
			}
			//else if (e.type == SDL_WINDOWEVENT)
			//{
			//	// adjust the viewport when the window is resized
			//	SCREEN_WIDTH = event.size.width;
			//	SCREEN_HEIGHT = event.size.height;
			//	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			//}
			gGame->handleEvent(e, deltaTime);
			//break;
		}
		gGame->update(deltaTime);
		//gTimer.restart();

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gGame->render(gWindow);
		SDL_GL_SwapWindow(gWindow);

		frameCount++;
		Uint32 t = FPSTimer.getElaspedTime();
		if (t >= 1000)
		{
			FPSTimer.restart();
			printf("FPS: %d %f\n", frameCount, 1.0/frameCount);
			frameCount = 0;
		}

		//VSYNC will handle this
		/*while (frameCount*(1000.f / MAX_FPS) > t)
		{
			t = FPSTimer.getElaspedTime();
		}*/
	}
}

int main(int argc, char* args[])
{
	if (!initSDL())
	{
		printf("ERROR initializing SDL\n");
		_getch();
	}

	if (!initGL())
	{
		printf("ERROR initializing Opengl\n");
		_getch();
	}

	//Use Vsync
#ifdef USE_VSYNC
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
#endif

	gGame = new Game();

	if (!gGame->init())
	{
		printf("ERROR initializing game\n");
		_getch();
	}

	int x, y;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &x);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &y);
	printf("Opengl Version: %d %d\n", x, y);
	

	mainLoop();

	delete gGame;
	SDL_GL_DeleteContext(gContext);

	return 0;
}