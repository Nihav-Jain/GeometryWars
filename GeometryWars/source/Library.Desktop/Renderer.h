#pragma once
#include <vector>
#include <map>

namespace Library {
	class Shader;
	class Buffer;
	class RenderDevice;
	class Renderable;
	class PostProcessing;
	class FrameBuffer;

	/**
	* The core render module
	*/
	class Renderer
	{
	public:
		// TODO: Remove this !!!!!!!!
		static Renderer * GetInstance(RenderDevice * device = nullptr) {
			if (sInstance == nullptr)
				sInstance = new Renderer(device);
			return  sInstance;
		};


		Renderer(RenderDevice * device);
		~Renderer();

		Renderer(const Renderer &) = delete;
		Renderer & operator=(const Renderer &) = delete;

		void AddPostPostProcessing(PostProcessing * postProcessing, std::uint32_t layerId = 100U);

		// TODO: Should the factory create the object from the XML accordingly or should we hava a way for the renderer create the obj 
		void AddRenderable(Renderable *, std::uint32_t layerId = 100U);
		void RemoveRenderable(Renderable *, std::uint32_t layerId = 100U);

		void Update();

	private:

		// TODO: Remove this !!!!!!!!
		static Renderer * sInstance;

		bool mInited;
		FrameBuffer * mDefaultFrameBuffer;
		Shader * mShader;
		Buffer * mBuffer;
		RenderDevice * mDevice;

		void Init();

		void CreateNewLayer(std::uint32_t layerId);

		struct Layer {
			std::vector<Renderable*> Objects;
			std::vector<PostProcessing*> PostProcessings;
			FrameBuffer * TargetFrameBuffer;
			FrameBuffer * PostProcessingFrameBuffer;
		};

		std::map<std::uint32_t, Layer> mLayers;
	};

}