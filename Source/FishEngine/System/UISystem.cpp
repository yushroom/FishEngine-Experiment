#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Screen.hpp>

#include <FishEngine/Object.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>

#if 0
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>


// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise
int isBlack(NVGcolor col)
{
	if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
	{
		return 1;
	}
	return 0;
}

static char* cpToUTF8(int cp, char* str)
{
	int n = 0;
	if (cp < 0x80) n = 1;
	else if (cp < 0x800) n = 2;
	else if (cp < 0x10000) n = 3;
	else if (cp < 0x200000) n = 4;
	else if (cp < 0x4000000) n = 5;
	else if (cp <= 0x7fffffff) n = 6;
	str[n] = '\0';
	switch (n) {
		case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
		case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
		case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
		case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
		case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
		case 1: str[0] = cp;
	}
	return str;
}

void drawButton(NVGcontext* vg, int preicon, const char* text, float x, float y, float w, float h, NVGcolor col)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = 4.0f;
	float tw = 0, iw = 0;
	
	bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,isBlack(col)?16:32), nvgRGBA(0,0,0,isBlack(col)?16:32));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
	if (!isBlack(col)) {
		nvgFillColor(vg, col);
		nvgFill(vg);
	}
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);
	
	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans-bold");
	tw = nvgTextBounds(vg, 0,0, text, NULL, NULL);
	if (preicon != 0) {
		nvgFontSize(vg, h*1.3f);
		nvgFontFace(vg, "icons");
		iw = nvgTextBounds(vg, 0,0, cpToUTF8(preicon,icon), NULL, NULL);
		iw += h*0.15f;
	}
	
	if (preicon != 0) {
		nvgFontSize(vg, h*1.3f);
		nvgFontFace(vg, "icons");
		nvgFillColor(vg, nvgRGBA(255,255,255,96));
		nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
		nvgText(vg, x+w*0.5f-tw*0.5f-iw*0.75f, y+h*0.5f, cpToUTF8(preicon,icon), NULL);
	}
	
	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
//	nvgFillColor(vg, nvgRGBA(0,0,0,160));
//	nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f-1,text, NULL);
//	nvgFillColor(vg, nvgRGBA(255,255,255,160));
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
	nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f,text, NULL);
}

int CreateFont(NVGcontext* vg, const char* name, const char* path)
{
#if FISHENGINE_PLATFORM_WINDOWS
	const std::string font_root = R"(D:\program\FishEngine-Experiment\Assets\Fonts\)";
#else
	const std::string font_root = R"(/Users/yushroom/program/FishEngine-Experiment/Assets/Fonts/)";
#endif
	auto font = nvgCreateFont(vg, name, (font_root+path).c_str());
	if (font == -1) {
		printf("Could not add font icons: %s", path);
		abort();
		return -1;
	}
	return font;
}


namespace FishEngine
{
	
	void UISystem::Init()
	{
		m_Context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
		
		auto fontIcons = CreateFont(m_Context, "icons", "entypo.ttf");
		auto fontNormal = CreateFont(m_Context, "sans", "Roboto-Regular.ttf");
		auto fontBold = CreateFont(m_Context, "sans-bold", "Roboto-Bold.ttf");
		auto fontEmoji = CreateFont(m_Context, "emoji", "NotoEmoji-Regular.ttf");
		nvgAddFallbackFontId(m_Context, fontNormal, fontEmoji);
		nvgAddFallbackFontId(m_Context, fontBold, fontEmoji);
	}
	
//	void UISystem::Start()
//	{
//		
//	}
	
	void UISystem::BeginDraw()
	{
		float pxRatio = Screen::GetPixelsPerPoint();
		int windowWidth = static_cast<int>( Screen::GetWidth() / pxRatio );
		int windowHeight = static_cast<int>( Screen::GetHeight() / pxRatio );
		nvgBeginFrame(m_Context, windowWidth, windowHeight, pxRatio);
	}
	
	RectTransform* GetRootUI()
	{
		auto scene = SceneManager::GetActiveScene();
		auto& rts = Object::FindObjectsOfType<RectTransform>();
		if (scene->GetRootCount() < rts.size())
		{
			for (auto t : scene->GetRootTransforms())
			{
				auto rt = t->GetGameObject()->GetComponent<RectTransform>();
				if (rt != nullptr)
				{
					return rt;
				}
			}
		}
		else
		{
			for (auto r : rts)
			{
				auto rr = dynamic_cast<RectTransform*>(r);
				if (rr->GetTransform()->GetParent() == nullptr)
				{
					return rr;
				}
			}
		}
		return nullptr;
	}
	
	void UISystem::Update()
	{
		RectTransform* root = GetRootUI();
		if (root == nullptr)
			return;
		root->m_Rect.m_XMin = 0;
		root->m_Rect.m_YMin = 0;
		root->m_Rect.m_Width = Screen::GetWidth();
		root->m_Rect.m_Height = Screen::GetHeight();
		root->Update();
		
		auto rts = Object::FindObjectsOfType<RectTransform>();
		RectTransform* rt = nullptr;
		for (auto r : rts)
		{
			auto rr = (RectTransform*)r;
			if (rr->GetGameObject()->GetName() == "Button")
			{
				rt = rr;
				break;
			}
		}
		assert(rt != nullptr);
		auto r = rt->m_Rect;

		int windowHeight = Screen::GetHeight()/ Screen::GetPixelsPerPoint();
		int p = Screen::GetPixelsPerPoint();
		drawButton(m_Context, 0, "Rotate", r.x()/p, windowHeight - r.y()/p - r.height()/p, r.width()/p, r.height()/p, nvgRGBA(255,255,255,255));
	}
	
	void UISystem::AfterDraw()
	{
		nvgEndFrame(m_Context);
	}
	
	void UISystem::Clean()
	{
		nvgDeleteGL3(m_Context);
	}
}

#else
namespace FishEngine
{

	void UISystem::Init()
	{
	}

	void UISystem::BeginDraw()
	{
	}

	void UISystem::Update()
	{
	}

	void UISystem::AfterDraw()
	{
	}

	void UISystem::Clean()
	{
	}
}


#endif