#pragma once

#include "Texture2D.hpp"

namespace FishEngine
{
	class ColorBuffer : public Texture2D
	{
	public:
		static ColorBuffer* Create(int width, int height, TextureFormat format = TextureFormat::RGBA32);
		virtual void Resize(int newWidth, int newHeight) override;
	protected:
		//TextureFormat m_format;
	};

//class LayeredColorBuffer : public ColorBuffer
//{
//public:
//    static std::shared_ptr<LayeredColorBuffer> Create(const int width, const int height, const int layers, TextureFormat format = TextureFormat::RGBA32);
//    //virtual void Resize(const int newWidth, const int newHeight) override;
//protected:
//    int m_layers;
//};


	class DepthBuffer : public Texture2D
	{
	public:
		static DepthBuffer* Create(int width, int height);
		virtual void Resize(int newWidth, int newHeight) override;
		bool m_useStencil = true;
	};


	class LayeredDepthBuffer : public DepthBuffer
	{
	public:
		static LayeredDepthBuffer* Create(int width, int height, int depth, bool useStencil = true);
		virtual void Resize(int newWidth, int newHeight) override;
	protected:
		int m_depth;
	};
}
