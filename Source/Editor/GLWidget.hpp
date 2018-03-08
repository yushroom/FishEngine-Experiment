#pragma once

#include <FishGUI/Widget.hpp>


class SceneViewApp;

class GLWidget : public FishGUI::IMWidget
{
public:
	GLWidget(const char* name);
	virtual void DrawImpl() override;
	void DrawScene();
	void Resize(int width, int height);

	SceneViewApp* m_app = nullptr;
};

