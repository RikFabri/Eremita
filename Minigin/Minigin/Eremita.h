#pragma once

struct SDL_Window;
namespace dae
{
	class Eremita
	{
	public:
		void Initialize();
		void Cleanup();
		void Run();
	private:
		SDL_Window* m_Window{};
	};
}