#include "StartMenu.h"
#include "GUILabel.h"
#include "GlutSession.h"

const int StartMenu::ZOOM_LEVEL = 3;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Construct a game window with given width, height, position and title. */
StartMenu::StartMenu(int w, int h, int x, int y, char *t)
	: GlutWindow(w, h, x, y, t)
{
	container.SetSize(GLVector2i(400, 400));
}

/** Destructor. */
StartMenu::~StartMenu()
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

void OnKeyPressed(int key, int x, int y)
{
	// If the F1 key has been pressed, toggle the fullscreen window mode
	if (key == 's') { GlutSession::Stop(); }
}


/** Call world and display to render themselves. */
void StartMenu::OnDisplay(void)
{

	// Create a new GUILabel and wrap it up in a shared_ptr
	gameTitle = make_shared<GUILabel>("Asteroids");
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	gameTitle->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the horizontal alignment of the label to GUI_HALIGN_MIDDLE
	gameTitle->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> title_component
		= static_pointer_cast<GUIComponent>(gameTitle);
	container.AddComponent(title_component, GLVector2f(0.5f, 0.5f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	startMessage = make_shared<GUILabel>("Press enter to start");
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	startMessage->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the horizontal alignment of the label to GUI_HALIGN_MIDDLE
	startMessage->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> message_component
		= static_pointer_cast<GUIComponent>(startMessage);
	container.AddComponent(message_component, GLVector2f(0.5f, 0.6f));

	// Clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT);
	// Render the world and display
	Render();
	// Show the backbuffer
	glutSwapBuffers();

	// Add a (transparent) border around the edge of the game display
	container.SetBorder(GLVector2i(10, 10));
}

/** Update world and display. */
void StartMenu::OnIdle(void)
{
	// Call parent to do any idle loop processing
	GlutWindow::OnIdle();
	// Calculate the time in milliseconds since the last update
	static int lasttime;
	int dt = glutGet(GLUT_ELAPSED_TIME) - lasttime;
	lasttime = glutGet(GLUT_ELAPSED_TIME);
	// Update the display
	Update(dt);
	// Request a redisplay
	glutPostRedisplay();
}

/** Reshape viewport, world and display. */
void  StartMenu::OnWindowReshaped(int w, int h)
{
	// Call parent to handle default window reshaping
	GlutWindow::OnWindowReshaped(w, h);
	// Reshape the viewport to cover the whole window
	glViewport(0, 0, w, h);
	// Update the display to match
	UpdateDisplaySize();
}

/** Render display by rendering top level container. */
void StartMenu::Render(void)
{
	// Update the projection matrix
	glMatrixMode(GL_PROJECTION);
	// Initialize the projection matrix to the identity matrix
	glLoadIdentity();
	// Set orthographic projection to include the world
	glOrtho(0, 400, 0, 400, -100, 100);
	// Switch to model mode ready for rendering
	glMatrixMode(GL_MODELVIEW);
	// Initialize the projection matrix to the identity matrix
	glLoadIdentity();

	container.Draw();
}

/** Update Display. */
void StartMenu::UpdateDisplaySize(void)
{
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
