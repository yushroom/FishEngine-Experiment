#pragma once

#include <FishGUI/Widget.hpp>
#include <FishGUI/Render/FrameBuffer.hpp>


class SceneViewApp;

class GLWidget : public FishGUI::IMWidget
{
public:
	GLWidget(const char* name, SceneViewApp* app);

	void Init();

	virtual void DrawImpl() override;
	void DrawScene();
	void Resize(int width, int height);


private:
	SceneViewApp* m_app = nullptr;
	FishGUI::FrameBuffer m_framebuffer;
};

