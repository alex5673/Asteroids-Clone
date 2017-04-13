#ifndef __STARTMENU_H__
#define __STARTMENU_H__

#include "GameUtil.h"
#include "GlutWindow.h"
#include "IKeyboardListener.h"
#include "GUIContainer.h"

class GUILabel;

class StartMenu : public GlutWindow
{
public:
	StartMenu(int w, int h, int x, int y, char* t);
	virtual~ StartMenu();

	virtual void OnDisplay(void);
	virtual void OnIdle(void);
	virtual void OnWindowReshaped(int w, int h);

	virtual void Update(int t) {}
	virtual void Render(void);

	virtual void UpdateDisplaySize(void);

	void Reshape(int w, int h)
	{
		container.SetSize(GLVector2i(w, h));
	}

	GUIContainer *GetContainer() { return &container; }

protected:
	static const int ZOOM_LEVEL;
	shared_ptr<GUILabel> gameTitle;
	shared_ptr<GUILabel> startMessage;

	GUIContainer container;
};

#endif 
