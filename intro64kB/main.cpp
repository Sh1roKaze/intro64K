/*
 *	Bachelors Thesis - 64kB intro
 * 
 *	Author: Jan Verny (xverny00)
*/

/* Windows headers */
#include <windows.h>        // Header File For Windows

/* OpenGL headers */
#include <glad\glad.h>
#include <gl\gl.h>          // Header File For The OpenGL32 Library

/* C/C++ standard headers */
#include <cstdio>

/* My own headers */
#include "types.h"
#include "shaders.h"
#include "renderer.h"
#include "meshes.h"
#include "lsystem.h"
#include "calendar.h"


HDC         hDC = NULL;       // Private GDI Device Context
HGLRC       hRC = NULL;       // Permanent Rendering Context
extern HWND hWnd = NULL;      // Holds Our Window Handle
HINSTANCE   hInstance;        // Holds The Instance Of The Application

bool    keys[256];
bool    active = TRUE;
bool    fullscreen = TRUE;    

GLsizei window_width = 1280;
GLsizei window_height = 720;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);   // Declaration For WndProc

int InitGL(GLvoid)                                      // All Setup For OpenGL Goes Here
{
	/* Black Background */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	/* Depth Buffer Setup */
	glClearDepth(1.0f);	  
	/* Enables Depth Testing */
	glEnable(GL_DEPTH_TEST);	
	/* The Type Of Depth Testing To Do */
	glDepthFunc(GL_LEQUAL);	   
	/* Enables blending using alpha channels */
	glEnable(GL_BLEND);	   
	/*Specify blending function */
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)                             // Properly Kill The Window
{
	if (fullscreen) {
		ChangeDisplaySettings(NULL, 0);                  // If So Switch Back To The Desktop
		ShowCursor(TRUE);                               // Show Mouse Pointer
	}

	if (hRC) {
		if (!wglMakeCurrent(NULL, NULL))                 // Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))                     // Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;                                       // Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))                    // Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;                                       // Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))                   // Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;                                      // Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))           // Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;                                 // Set hInstance To NULL
	}
}

/*
 * This Code Creates OpenGL Window.  Parameters Are:                   
 * title           - Title To Appear At The Top Of The Window              
 * width           - Width Of The GL Window Or Fullscreen Mode             
 * height          - Height Of The GL Window Or Fullscreen Mode            
 * bits            - Number Of Bits To Use For Color (8/16/24/32)          
 * fullscreenflag  - Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)   
 */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint      PixelFormat;            // Holds The Results After Searching For A Match
	WNDCLASS    wc;                     // Windows Class Structure
	DWORD       dwExStyle;              // Window Extended Style
	DWORD       dwStyle;                // Window Style
	RECT        WindowRect;             // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;            // Set Left Value To 0
	WindowRect.right = (long)width;       // Set Right Value To Requested Width
	WindowRect.top = (long)0;             // Set Top Value To 0
	WindowRect.bottom = (long)height;     // Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;          // Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);                // Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   // Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;                    // WndProc Handles Messages
	wc.cbClsExtra = 0;                                    // No Extra Window Data
	wc.cbWndExtra = 0;                                    // No Extra Window Data
	wc.hInstance = hInstance;                            // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);          // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
	wc.hbrBackground = NULL;                                 // No Background Required For GL
	wc.lpszMenuName = NULL;                                 // We Don't Want A Menu
	wc.lpszClassName = "OpenGL";                             // Set The Class Name

	if (!RegisterClass(&wc))                                    // Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                                           // Return FALSE
	}

	if (fullscreen)                                             // Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;                               // Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;                // Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;               // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;                 // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;       // Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;                                   // Return FALSE
			}
		}
	}

	if (fullscreen)                                             // Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;                              // Window Extended Style
		dwStyle = WS_POPUP;                                       // Windows Style
		ShowCursor(FALSE);                                      // Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;                            // Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,                          // Extended Style For The Window
		"OpenGL",                           // Class Name
		title,                              // Window Title
		dwStyle |                           // Defined Window Style
		WS_CLIPSIBLINGS |                   // Required Window Style
		WS_CLIPCHILDREN,                    // Required Window Style
		0, 0,                               // Window Position
		WindowRect.right - WindowRect.left,   // Calculate Window Width
		WindowRect.bottom - WindowRect.top,   // Calculate Window Height
		NULL,                               // No Parent Window
		NULL,                               // No Menu
		hInstance,                          // Instance
		NULL)))                             // Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	// pfd Tells Windows How We Want Things To Be
	static  PIXELFORMATDESCRIPTOR pfd =              
	{
		sizeof(PIXELFORMATDESCRIPTOR),                          // Size Of This Pixel Format Descriptor
		1,                                                      // Version Number
		PFD_DRAW_TO_WINDOW |                                    // Format Must Support Window
		PFD_SUPPORT_OPENGL |                                    // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                                       // Must Support Double Buffering
		(BYTE)PFD_TYPE_RGBA,                                    // Request An RGBA Format
		(BYTE)bits,                                             // Select Our Color Depth
		(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0,   // Color Bits Ignored
		(BYTE)0,                                                // No Alpha Buffer
		(BYTE)0,                                                // Shift Bit Ignored
		(BYTE)0,                                                // No Accumulation Buffer
		(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0,                     // Accumulation Bits Ignored
		(BYTE)16,                                               // 16Bit Z-Buffer (Depth Buffer)  
		(BYTE)0,                                                // No Stencil Buffer
		(BYTE)0,                                                // No Auxiliary Buffer
		(BYTE)PFD_MAIN_PLANE,                                   // Main Drawing Layer
		(BYTE)0,                                                // Reserved
		0, 0, 0                                                 // Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))                         // Did We Get A Device Context?
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) // Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))       // Are We Able To Set The Pixel Format?
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))               // Are We Able To Get A Rendering Context?
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))                    // Try To Activate The Rendering Context
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	if (gladLoadGL() == 0) 
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "GLAD init failure.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);                       // Show The Window
	SetForegroundWindow(hWnd);                      // Slightly Higher Priority
	SetFocus(hWnd);                                 // Sets Keyboard Focus To The Window

	if (!InitGL())                                  // Initialize Our Newly Created GL Window
	{
		KillGLWindow();                             // Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                               // Return FALSE
	}

	return TRUE;                                    // Success
}

LRESULT CALLBACK WndProc(HWND    hWnd,           // Handle For This Window
	UINT    uMsg,           // Message For This Window
	WPARAM  wParam,         // Additional Message Information
	LPARAM  lParam)         // Additional Message Information
{
	switch (uMsg)                                   // Check For Windows Messages
	{
	case WM_ACTIVATE:                           // Watch For Window Activate Message
	{
		// Check Minimization State
		if (!HIWORD(wParam)) {
			active = TRUE;                        // Program Is Active
		} else {
			active = FALSE;                       // Program Is No Longer Active
		}
		return 0;                               // Return To The Message Loop
	}

	case WM_SYSCOMMAND:                         // Intercept System Commands
	{
		switch (wParam)                         // Check System Calls
		{
		case SC_SCREENSAVE:                 // Screensaver Trying To Start?
		case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
			return 0;                           // Prevent From Happening
		}
		break;                                  // Exit
	}

	case WM_CLOSE:                              // Did We Receive A Close Message?
	{
		PostQuitMessage(0);                     // Send A Quit Message
		return 0;                               // Jump Back
	}

	case WM_KEYDOWN:                            // Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;                    // If So, Mark It As TRUE
		return 0;                               // Jump Back
	}

	case WM_KEYUP:                              // Has A Key Been Released?
	{
		keys[wParam] = FALSE;                   // If So, Mark It As FALSE
		return 0;                               // Jump Back
	}

	case WM_SIZE:                               // Resize The OpenGL Window
	{
		window_width = LOWORD(lParam);
		window_height = HIWORD(lParam);
		return 0;                               // Jump Back
	}
	}

	/* Pass All Unhandled Messages To DefWindowProc */
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE   hInstance,          // Instance
	HINSTANCE   hPrevInstance,      // Previous Instance
	LPSTR       lpCmdLine,          // Command Line Parameters
	int         nCmdShow)           // Window Show State
{

	/* Windows Message Structure */ 
	MSG     msg;  

	/* Ask The User Which Screen Mode They Prefer */
	if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) 
	{
		fullscreen = FALSE;
	}

	/* Create Our OpenGL Window */
	if (!CreateGLWindow("64kb graphic intro", window_width, window_height, 32, fullscreen)) 
	{
		return 0;
	}

	/* Set lastWidth and lastHeight */
	int lastWidth = window_width;
	int lastHeight = window_height;

	/* Renderer init */
	Camera *mainCamera = new Camera();
	mainCamera->direction = Vec3(0.f, 0.f, 1.f);
	mainCamera->up = Vec3(0.f, 1.f, 0.f);

	Renderer* renderer = new Renderer();
	renderer->UseCamera(mainCamera);
	renderer->UpdateProjectionMatrix(60.f, (float)window_width / (float)window_height, 0.1f, 1000.f);

	/* Compile GLSL library shader library  */
	GLuint libSh = shaderCreate(library_shader, GL_VERTEX_SHADER);
	if (libSh == 0)
		MessageBox(NULL, "Library shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	
	/* Render to texture */
	Vec3 *quadVertices = genQuadVertices(1.f, 1.f);
	Vec3 *milkyWayVertices = genQuadVertices(1.f, 1.f);
	Vec3ui quadIndices[2] = { Vec3ui(0, 1, 2), Vec3ui(2, 1, 3) };

	Renderable *quad = new Renderable(MESH_STATIC);
	quad->createVBO(4, quadVertices);
	quad->createEAB(2, quadIndices);

	GLuint qvShader = shaderCreate(vert_shader_texture_rendering, GL_VERTEX_SHADER);
	if (qvShader == 0)
		MessageBox(NULL, "TexRender quad vert shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint qfShader = shaderCreate(frag_shader_perlin, GL_FRAGMENT_SHADER);
	if (qfShader == 0)
		MessageBox(NULL, "TexRender Mars frag shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint earthqfS = shaderCreate(frag_shader_earth, GL_FRAGMENT_SHADER);
	if (earthqfS == 0)
		MessageBox(NULL, "TexRender Earth frag shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint mwqfS = shaderCreate(frag_shader_milky_way, GL_FRAGMENT_SHADER);
	if (mwqfS == 0)
		MessageBox(NULL, "TexRender MW frag shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint qsfShader = shaderCreate(frag_shader_dot_noise, GL_FRAGMENT_SHADER);
	if (qfShader == 0)
		MessageBox(NULL, "TexRender Sky frag shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint sun_Sh = shaderCreate(frag_shader_sun, GL_FRAGMENT_SHADER);
	if (sun_Sh == 0)
		MessageBox(NULL, "TexRender Sun frag shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint spaceshipSh = shaderCreate(frag_shader_spaceship, GL_FRAGMENT_SHADER);
	if (spaceshipSh == 0)
		MessageBox(NULL, "TexRender spaceship frag shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint cloudSh = shaderCreate(frag_sh_clouds, GL_FRAGMENT_SHADER);
	if (cloudSh == 0)
		MessageBox(NULL, "TexRender spaceship frag shader compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	Renderable *quadMW = new Renderable(MESH_STATIC);
	quadMW->createVBO(4, milkyWayVertices);
	quadMW->createEAB(2, quadIndices);
	quadMW->createProgram(qvShader, mwqfS);

	quad->createProgram(qvShader, qfShader);
	renderer->AddToRenderQueue(quad);
	GLuint marsTex = renderer->RenderToTexture(2048, 2048);
	
	quad->createProgram(qvShader, earthqfS);
	renderer->AddToRenderQueue(quad);
	GLuint earthTex = renderer->RenderToTexture(2048, 2048);
	
	quad->createProgram(qvShader, qsfShader);
	renderer->AddToRenderQueue(quad);
	renderer->AddToRenderQueue(quadMW);
	GLuint skyTex = renderer->RenderToTexture(2048, 2048);

	quad->createProgram(qvShader, sun_Sh);
	renderer->AddToRenderQueue(quad);
	GLuint sunTex = renderer->RenderToTexture(2048, 2048);

	quad->createProgram(qvShader, cloudSh);
	renderer->AddToRenderQueue(quad);
	GLuint cloudsTex = renderer->RenderToTexture(2048, 2048);

	quad->createProgram(qvShader, spaceshipSh);
	renderer->AddToRenderQueue(quad);
	GLuint spaceshipTex = renderer->RenderToTexture(2048, 2048);

	glDeleteShader(qvShader);
	glDeleteShader(qfShader);
	glDeleteShader(earthqfS);
	glDeleteShader(qsfShader);
	glDeleteShader(sun_Sh);
	glDeleteShader(mwqfS);

	delete quadVertices;
	delete milkyWayVertices;

	delete quad;
	delete quadMW;
	/*Finished render to texture */


	/* Generate sphere vertices and idices */
	Vec3 *verts = NULL;
	/* Sphere indices */
	Vec3ui *indx = NULL;
	Vec2 *coords = NULL;
	int parallels = 255;
	int meridians = 255;
	genSphere(&verts, &indx, parallels, meridians);
	sphereTexCoords(&coords, parallels, meridians);
	int points = sphereVertices(parallels, meridians);
	int indxLength = sphereIndices(parallels, meridians); // 2 * meridians + (parallels - 1) * meridians * 2

	GLuint fragmentShader = shaderCreate(frag_shader_phong, GL_FRAGMENT_SHADER);
	if (fragmentShader == 0)
		MessageBox(NULL, "Phong frag sh compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	/* Mars Renderable */
	Renderable *mars = new Renderable(MESH_STATIC);
	GLuint vertexShader = shaderCreate(vert_shader_planet, GL_VERTEX_SHADER);
	if (vertexShader == 0)
		MessageBox(NULL, "MESH_STATIC vert sh compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	mars->scale = 5.f;
	mars->updateModelMatrix();
	mars->createVBO(points, verts, coords);
	mars->createEAB(indxLength, indx);
	mars->createProgram(vertexShader, fragmentShader);
	mars->useTexture(marsTex);

	/* Sky renderable */
	Renderable *sky = new Renderable(MESH_STATIC);
	GLuint vertexSky = shaderCreate(vert_shader_skybox, GL_VERTEX_SHADER);
	if (vertexShader == 0)
		MessageBox(NULL, "Skybox vert sh compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	GLuint fragmentSky = shaderCreate(frag_shader_skybox, GL_FRAGMENT_SHADER);
	if (fragmentShader == 0)
		MessageBox(NULL, "Skybox frag sh compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	sky->scale = 900.f;
	sky->updateModelMatrix();
	sky->createVBO(points, verts, coords);
	sky->createEAB(indxLength, indx);
	sky->createProgram(vertexSky, fragmentSky);
	sky->useTexture(skyTex);

	/* Earth Renderable */
	Renderable *earth = new Renderable(MESH_STATIC);
	earth->scale = 2.5f;
	earth->updateModelMatrix();
	earth->createVBO(points, verts, coords);
	earth->createEAB(indxLength, indx);
	earth->createProgram(vertexShader, fragmentShader);
	earth->useTexture(earthTex);
	/* Earth clouds Renderable */
	Renderable *clouds = new Renderable(MESH_STATIC);
	clouds->scale = 2.7f;
	clouds->updateModelMatrix();
	clouds->createVBO(points, verts, coords);
	clouds->createEAB(indxLength, indx);
	clouds->createProgram(vertexSky, fragmentShader);
	clouds->useTexture(cloudsTex);

	delete verts;
	delete indx;
	
	/* Sun Renderable */
	Renderable *sun = new Renderable(MESH_STATIC);
	Vec3 *vertsSun = genQuadVertices(1.f, 1.f);
	Vec2 *quadCoords = new Vec2[4];
	quadCoords[0] = Vec2(0.f, 0.f);
	quadCoords[1] = Vec2(0.f, 1.f);
	quadCoords[2] = Vec2(1.f, 0.f);
	quadCoords[3] = Vec2(1.f, 1.f);
	sun->scale = 8.f;
	sun->updateModelMatrix();
	sun->createVBO(4, vertsSun, quadCoords);
	sun->createEAB(2, quadIndices);
	sun->createProgram(vertexSky, fragmentSky);
	sun->useTexture(sunTex);



	int curr_seed = 1193; //orig seed 1566, 1193, 1561
	GLuint boxVSh = shaderCreate(vert_shader_normals, GL_VERTEX_SHADER);
	if (boxVSh == 0)
		MessageBox(NULL, "Box vert sh compile failure", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	Renderable *interactive = new Renderable(MESH_STATIC_NORMALS);
	interactive->scale = 0.5f;
	interactive->updateModelMatrix();
	LSystem *lsys = new LSystem(curr_seed); 
	Mesh *ship = lsys->GetSpaceship();
	interactive->createVBO(ship->vertexCount, ship->vertices);
	interactive->createEAB(ship->indexCount, ship->indices);
	interactive->createProgram(boxVSh, fragmentShader);
	interactive->useTexture(spaceshipTex);
	delete ship;
	delete lsys;


	/* 32 Renderables, random space ships */
	unsigned int seeds[16] = { 
		22187, 10623, 11300, 29432,
		23613, 27897, 10623, 20519,
		28255, 11297, 4388, 2611,
		12526, 2367, 32026, 20309 };
	Renderable **shipsArray = new Renderable*[32];
	for (unsigned int i = 0; i < 32; i++)
	{
		shipsArray[i] = new Renderable(MESH_STATIC_NORMALS);
		shipsArray[i]->scale = 0.11f;
		shipsArray[i]->updateModelMatrix();
		LSystem *help = new LSystem(seeds[i < 16 ? i : i - 16]);
		Mesh *m = help->GetSpaceship();
		shipsArray[i]->createVBO(m->vertexCount, m->vertices, m->texCoords);
		shipsArray[i]->createEAB(m->indexCount, m->indices);
		shipsArray[i]->createProgram(boxVSh, fragmentShader);
		shipsArray[i]->useTexture(spaceshipTex);
		delete m;
		delete help;
	}


	/* Calendar Init */
	Calendar *mainCalendar = new Calendar();
	
	/* scene1 */
	Scene *scene1 = new Scene(0.f, 25000.f);
	mainCalendar->AddScene(scene1);

	scene1->AddRenderable(sky);
	scene1->AddRenderable(mars);
	scene1->AddRenderable(sun);
	scene1->AddRenderable(interactive);
	scene1->AddRenderable(shipsArray[0]);
	scene1->AddRenderable(shipsArray[1]);

	/* scene1 set up locations */
	Event *tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, M_PI * 0.15f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::MoveLightTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 50.f, -50.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = interactive;
	tEvent->vector = Vec3(-2.2f, 6.f, -5.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = interactive;
	tEvent->vector = Vec3(M_PI / 2, M_PI/2, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = shipsArray[0];
	tEvent->vector = Vec3(0.5f, 9.5f, -6.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::SetScale;
	tEvent->renderable = shipsArray[0];
	tEvent->vector = Vec3(0.5f, 0.f, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = shipsArray[0];
	tEvent->vector = Vec3(M_PI / 2, M_PI / 2, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = shipsArray[1];
	tEvent->vector = Vec3(4.f, 8.f, -7.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::SetScale;
	tEvent->renderable = shipsArray[1];
	tEvent->vector = Vec3(0.5f, 0.f, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = shipsArray[1];
	tEvent->vector = Vec3(M_PI / 2, M_PI / 2, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = mars;
	tEvent->vector = Vec3(0.f, 5.5f, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = mars;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = sun;
	tEvent->vector = Vec3(0.f, 80.f, -100.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = sky;
	tEvent->vector = Vec3(0.f, M_PI / 8.f, M_PI/4 * 0.2f);
	scene1->AddEvent(tEvent);

	/* scene1 move and rotate sun and sky */
	tEvent = new Event(0.f, 10000.f);
	tEvent->EventFunction = EventFunctions::MoveBy;
	tEvent->renderable = sun;
	tEvent->vector = Vec3(0.f, -80.f, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, 10000.f);
	tEvent->EventFunction = EventFunctions::MoveLightBy;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, -50.f, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(0.f, 10000.f);
	tEvent->EventFunction = EventFunctions::RotateBy;
	tEvent->renderable = sky;
	tEvent->vector = Vec3(0.f, 0.f, M_PI * 0.15f);
	scene1->AddEvent(tEvent);

	/* scene1 adjust camera */
	tEvent = new Event(5000.f, 15000.f);
	tEvent->EventFunction = EventFunctions::RotateBy;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, - M_PI * 0.15f);
	scene1->AddEvent(tEvent);

	/* scene1 move and rotate the spaceship */
	tEvent = new Event(5000.f, 20000.f);
	tEvent->EventFunction = EventFunctions::MoveBy;
	tEvent->renderable = interactive;
	tEvent->vector = Vec3(0.f, -8.5f, -1.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(12000.f, 23000.f);
	tEvent->EventFunction = EventFunctions::RotateBy;
	tEvent->renderable = interactive;
	tEvent->vector = Vec3(0.f, M_PI, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(6000.f, 21000.f);
	tEvent->EventFunction = EventFunctions::MoveBy;
	tEvent->renderable = shipsArray[0];
	tEvent->vector = Vec3(0.f, -8.5f, -1.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(13000.f, 24000.f);
	tEvent->EventFunction = EventFunctions::RotateBy;
	tEvent->renderable = shipsArray[0];
	tEvent->vector = Vec3(0.f, M_PI, 0.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(7000.f, 22000.f);
	tEvent->EventFunction = EventFunctions::MoveBy;
	tEvent->renderable = shipsArray[1];
	tEvent->vector = Vec3(0.f, -8.5f, -1.f);
	scene1->AddEvent(tEvent);

	tEvent = new Event(14000.f, 25000.f);
	tEvent->EventFunction = EventFunctions::RotateBy;
	tEvent->renderable = shipsArray[1];
	tEvent->vector = Vec3(0.f, M_PI, 0.f);
	scene1->AddEvent(tEvent);

	/* scene1 start playing music */
	tEvent = new Event(8000.f, 0.f);
	tEvent->EventFunction = EventFunctions::PlayMusic;
	scene1->AddEvent(tEvent);
	

	/* scene2  */
	Scene *scene2 = new Scene(25000.f, 40000.f);
	mainCalendar->AddScene(scene2);

	scene2->AddRenderable(sky);
	scene2->AddRenderable(mars);
	for (unsigned int i = 0; i < 32; i++)
		scene2->AddRenderable(shipsArray[i]);

	/* scene2 set up positions */
	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveLightTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(-80.f, -50.f, 70.f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(0.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = sky;
	tEvent->vector = Vec3(M_PI / 8 * 1.5f, M_PI / -4.f, (M_PI / 4.f) * 1.f);
	scene2->AddEvent(tEvent);
	
	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = mars;
	tEvent->vector = Vec3(10.f, 5.f, -25.f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = mars;
	tEvent->vector = Vec3(M_PI / 2, 0.f, M_PI / 8 * -0.35f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = interactive;
	tEvent->vector = Vec3(-10.f, -14.f, -30.f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = interactive;
	tEvent->vector = Vec3(M_PI / 2, (M_PI / 2) * 3.f, 0.f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::SetScale;
	tEvent->renderable = shipsArray[0];
	tEvent->vector = Vec3(0.11f, 0.f, 0.f);
	scene2->AddEvent(tEvent);

	tEvent = new Event(25000.f, 0.f);
	tEvent->EventFunction = EventFunctions::SetScale;
	tEvent->renderable = shipsArray[1];
	tEvent->vector = Vec3(0.11f, 0.f, 0.f);
	scene2->AddEvent(tEvent);

	

	float x = 0;
	/* scene2 ships behind mars mowing in front of it */
	for (unsigned int i = 0; i < 16; i++)
	{
		x += i > 8 ? 0.4f : -0.4f;
		float z = i % 2 ? 0.35f : -0.35f;
		float o = (float)rand() / (float)RAND_MAX - 0.5;


		tEvent = new Event(25000.f, 0.f);
		tEvent->EventFunction = EventFunctions::MoveTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(15 + x, 2 + z + o, -50 + (i * 0.5));
		scene2->AddEvent(tEvent);

		tEvent = new Event(25000.f, 0.f);
		tEvent->EventFunction = EventFunctions::RotateTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(M_PI / 2, M_PI / 2, 0.f);
		scene2->AddEvent(tEvent);

		tEvent = new Event(25001.f, 40000.f);
		tEvent->EventFunction = EventFunctions::MoveBy;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(-10, -4, 13);
		scene2->AddEvent(tEvent);
	}

	/* scene2 ships moving towards camera */
	x = 0;
	for (unsigned int i = 16; i < 24; i++)
	{
		x += i > 8 ? 0.8f : -0.8f;
		float z = i % 2 ? 0.35f : -0.35f;

		tEvent = new Event(25000.f, 0.f);
		tEvent->EventFunction = EventFunctions::MoveTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(0 - x, 10 + z, -40);
		scene2->AddEvent(tEvent);

		tEvent = new Event(25000.f, 0.f);
		tEvent->EventFunction = EventFunctions::RotateTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(M_PI / 2, (M_PI / 2) * 1.15, 0.f);
		scene2->AddEvent(tEvent);

		tEvent = new Event(25001.f, 40000.f);
		tEvent->EventFunction = EventFunctions::MoveBy;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(0, -10, 30);
		scene2->AddEvent(tEvent);

		tEvent = new Event(30000.f, 36000.f);
		tEvent->EventFunction = EventFunctions::RotateBy;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(0.f, M_PI * -0.12, 0.f);
		scene2->AddEvent(tEvent);
	}
	
	/* scene2 ships in the right moving left  */
	x = 0;
	for (unsigned int i = 24; i < 32; i++)
	{
		x += i > 8 ? 0.8f : -0.8f;
		float z = i % 2 ? 0.35f : -0.35f;

		tEvent = new Event(25000.f, 0.f);
		tEvent->EventFunction = EventFunctions::MoveTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(-6 - x, 6 + z, -20.f);
		scene2->AddEvent(tEvent);

		tEvent = new Event(25000.f, 0.f);
		tEvent->EventFunction = EventFunctions::RotateTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(M_PI / 2, (M_PI / 2) * 0.65f, 0.f);
		scene2->AddEvent(tEvent);

		tEvent = new Event(25001.f, 40000.f);
		tEvent->EventFunction = EventFunctions::MoveBy;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(20.f, -5.f, 20.f);
		scene2->AddEvent(tEvent);
	}


	/* scene 3 */
	Scene *scene3 = new Scene(40000.f, 48000.f);
	mainCalendar->AddScene(scene3);

	scene3->AddRenderable(sky);
	for (unsigned int i = 0; i < 32; i++)
		scene3->AddRenderable(shipsArray[i]);

	/* scene3 set up position */
	tEvent = new Event(40000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene3->AddEvent(tEvent);

	tEvent = new Event(40000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene3->AddEvent(tEvent);

	tEvent = new Event(40000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveLightTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(-10.f, -100.f, 0.f);
	scene3->AddEvent(tEvent);

	tEvent = new Event(40000.f, -1.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = sky;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene3->AddEvent(tEvent);

	x = 0;
	/* scene3 ships set up */
	for (unsigned int i = 0; i < 32; i++)
	{
		x += i < 8  || i > 24 ? 1.3f : -1.3f;
		x += i % 2 ? 4.5 : -4.5;
		float z = i % 2 ? 0.35f : -0.35f;
		z += (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.8f;
		
		tEvent = new Event(40000.f, 0.f);
		tEvent->EventFunction = EventFunctions::MoveTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(15 + x, 0 + z, -15 + ((32 - i) * 0.5));
		scene3->AddEvent(tEvent);

		tEvent = new Event(40000.f, 0.f);
		tEvent->EventFunction = EventFunctions::RotateTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(0.f, M_PI, 0.f);
		scene3->AddEvent(tEvent);

		
		tEvent = new Event(40001.f, 51000.f);
		tEvent->EventFunction = EventFunctions::MoveBy;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(-35 + ((32 - i) * 0.4), 0.f, 3.f);
		scene3->AddEvent(tEvent);	
	}


	/* scene4 */
	Scene *scene4 = new Scene(48000.f, 70000.f);
	mainCalendar->AddScene(scene4);
	
	scene4->AddRenderable(sky);
	scene4->AddRenderable(earth);
	scene4->AddRenderable(clouds);
	scene4->AddRenderable(sun);
	for (int i = 0; i < 32; i++)
		scene4->AddRenderable(shipsArray[i]);

	/* scene4 set up locations */
	tEvent = new Event(48000.f, 0);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(20.f, 0.f, -35.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, M_PI / -2.f, 0.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveLightTo;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(60.f, -20.f, 20.f);
	scene4->AddEvent(tEvent);

	/* scene4 sky setup */
	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = sky;
	tEvent->vector = Vec3(0.f, 0.f, (M_PI /2) * 0.85f);
	scene4->AddEvent(tEvent);

	/* scene4 sun setup */
	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = sun;
	tEvent->vector = Vec3(240.f, -80.f, -80.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = sun;
	tEvent->vector = Vec3(0.f, (M_PI/4), 0.f);
	scene4->AddEvent(tEvent);

	/* scene4 earth setup */
	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = earth;
	tEvent->vector = Vec3(18.f, 0.f, 5.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = earth;
	tEvent->vector = Vec3((M_PI / 2) * 1.1f, 0.f, 0.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::MoveTo;
	tEvent->renderable = clouds;
	tEvent->vector = Vec3(18.f, 0.f, 5.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(48000.f, 0.f);
	tEvent->EventFunction = EventFunctions::RotateTo;
	tEvent->renderable = clouds;
	tEvent->vector = Vec3(0.f, 0.f, 0.f);
	scene4->AddEvent(tEvent);

	/* scene4 spaceships movement */
	x = 0;
	float z = 0;
	for (int i = 0; i < 32; i++)
	{
		float help = (float)rand() / (float)RAND_MAX;
		float y = i % 2 ? -2 : 2;
		y = y * help;

		z += i < 16 ? 0.5 : -0.5;

		tEvent = new Event(48000.f, 0.f);
		tEvent->EventFunction = EventFunctions::MoveTo;
		tEvent->renderable = shipsArray[i];
		tEvent->vector = Vec3(i * 1.05f, y + 0.5, 25 + z);
		scene4->AddEvent(tEvent);


		if (i < 16)
		{
			tEvent = new Event(48000.f, 0.f);
			tEvent->EventFunction = EventFunctions::RotateTo;
			tEvent->renderable = shipsArray[i];
			tEvent->vector = Vec3(M_PI / 2.f, (M_PI / 2.f) * 3 * 1.05, 0.f);
			scene4->AddEvent(tEvent);

			tEvent = new Event(48000.f, 72000.f);
			tEvent->EventFunction = EventFunctions::MoveBy;
			tEvent->renderable = shipsArray[i];
			tEvent->vector = Vec3(3.5f, 0.f, -20.f);
			scene4->AddEvent(tEvent);
		}
		else
		{
			tEvent = new Event(48000.f, 0.f);
			tEvent->EventFunction = EventFunctions::RotateTo;
			tEvent->renderable = shipsArray[i];
			tEvent->vector = Vec3(M_PI / 2.f, (M_PI / 2.f) * 3 * 0.95, 0);
			scene4->AddEvent(tEvent);

			tEvent = new Event(48000.f, 72000.f);
			tEvent->EventFunction = EventFunctions::MoveBy;
			tEvent->renderable = shipsArray[i];
			tEvent->vector = Vec3(-3.5f, 0.f, -20.f);
			scene4->AddEvent(tEvent);
		}
	}

	/* scene4 camera movement */
	tEvent = new Event(48000.f, 55000.f);
	tEvent->EventFunction = EventFunctions::MoveBy;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(-42.f, 0.f, 0.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(54000.f, 70000.f);
	tEvent->EventFunction = EventFunctions::MoveBy;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, 0.f, 19.f);
	scene4->AddEvent(tEvent);

	tEvent = new Event(48000.f, 62000.f);
	tEvent->EventFunction = EventFunctions::RotateBy;
	tEvent->camera = mainCamera;
	tEvent->vector = Vec3(0.f, (M_PI / 2.f) * 1.2f, 0.f);
	scene4->AddEvent(tEvent);



	/* Render loop */
	RenderableList *rListA = NULL;
	RenderableList *rListB = NULL;
	mainCalendar->Reset();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			/* Message handling */
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/* Program Active? */
			if (active)                             
			{
				/* Pressed escape */
				if (keys[VK_ESCAPE]) 
					break;
				
				/* Pressed space */
				if (keys[VK_SPACE])
				{
					mainCalendar->Reset();
					renderer->UpdateEverything();
 					keys[VK_SPACE] = false;
				}

				/* Window size changed */
				if ((lastWidth - window_width != 0) || (lastHeight - window_height != 0)) 
				{
					glViewport(0, 0, window_width, window_height);
					renderer->UpdateProjectionMatrix(60.f, (float)window_width / (float)window_height, 0.1f, 1000.f);
					lastWidth = window_width;
					lastHeight = window_height;
				}

				/* Update Calendar */
				mainCalendar->Tick();

				/* Fill render queue and render */
				rListA = mainCalendar->GetRenderables();
				if (rListA != rListB) 
				{
					renderer->UpdateEverything();
					rListB = rListA;
				}

				renderer->AddToRenderQueue(rListA);
				renderer->Render();

				/* Swap buffers (Double Buffering) */ 
				SwapBuffers(hDC);
			}
		}
	}

	/*Shutdown */
	KillGLWindow();
	return (msg.wParam);
}
