#include "GLWidget.hpp"

#include <FishEngine/Debug.hpp>
#include <FishGUI/FishGUI.hpp>
#include "SceneViewApp.hpp"

GLWidget::GLWidget(const char* name) : FishGUI::IMWidget(name)
{
}

void GLWidget::DrawImpl()
{
	auto tex = m_app->m_framebuffer.GetColorTexture();
	FishGUI::Image(tex, m_rect, true);
}

void GLWidget::DrawScene()
{

}

void GLWidget::Resize(int width, int height)
{
	LogInfo(FishEngine::Format("GLWidget::Resize w={} h={}", width, height));
	m_app->Resize(width, height);
}