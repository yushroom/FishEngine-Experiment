#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Screen.hpp>

#include <FishEngine/Object.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/RectTransform.hpp>

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
	auto font = nvgCreateFont(vg, "icons", "/Users/yushroom/program/github/nanovg/example/entypo.ttf");
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
		m_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
		
		auto fontIcons = CreateFont(m_context, "icons", "/Users/yushroom/program/github/nanovg/example/entypo.ttf");
		auto fontNormal = nvgCreateFont(m_context, "sans", "/Users/yushroom/program/github/nanovg/example/Roboto-Regular.ttf");
		auto fontBold = nvgCreateFont(m_context, "sans-bold", "/Users/yushroom/program/github/nanovg/example/Roboto-Bold.ttf");
		auto fontEmoji = nvgCreateFont(m_context, "emoji", "/Users/yushroom/program/github/nanovg/example/NotoEmoji-Regular.ttf");
		nvgAddFallbackFontId(m_context, fontNormal, fontEmoji);
		nvgAddFallbackFontId(m_context, fontBold, fontEmoji);
	}
	
//	void UISystem::Start()
//	{
//		
//	}
	
	void UISystem::BeginDraw()
	{
		float pxRatio = Screen::pixelsPerPoint();
		int windowWidth = static_cast<int>( Screen::width() / pxRatio );
		int windowHeight = static_cast<int>( Screen::height() / pxRatio );
		nvgBeginFrame(m_context, windowWidth, windowHeight, pxRatio);
	}
	
	void UISystem::Update()
	{
		auto rts = Object::FindObjectsOfType<RectTransform>();
		RectTransform* rt = nullptr;
		for (auto r : rts)
		{
			auto rr = (RectTransform*)r;
			if (rr->gameObject()->name == "Button")
			{
				rt = rr;
				break;
			}
		}
		
//		rt->m_AnchorMin = rt->m_AnchorMax = Vector2(0.5, 0.0);
//		rt->m_AnchoredPosition = Vector2(0, 83);
//		rt->m_SizeDelta = Vector2(160, 60);
//		rt->m_Pivot = Vector2(0.5, 0);
		
		Vector2 parentRect{640, 1136};
		assert(rt->m_AnchorMin == rt->m_AnchorMax);
		Vector2 anchor;
		anchor.x = parentRect.x * rt->m_AnchorMin.x;
		anchor.y = parentRect.y * rt->m_AnchorMin.y;
//		auto anchor = parentRect * rt->m_AnchorMin;
		int width = rt->m_SizeDelta.x;
		int height = rt->m_SizeDelta.y;
		auto pivot = rt->m_Pivot;
		pivot.x *= width;
		pivot.y *= height;
		auto pivotP = anchor + rt->m_AnchoredPosition;
//		pos + pivot = pivotP
		Vector2 pos = pivotP - pivot;
//		printf("pos:(%f, %f)\n", pos.x, pos.y);
		
		drawButton(m_context, 0, "Rotate", pos.x/2, 1136/2 - pos.y/2 - height/2, width/2, height/2, nvgRGBA(255,255,255,255));
	}
	
	void UISystem::AfterDraw()
	{
		nvgEndFrame(m_context);
	}
	
	void UISystem::Clean()
	{
		nvgDeleteGL3(m_context);
	}
}
