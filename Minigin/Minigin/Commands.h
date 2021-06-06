#pragma once
#include <functional>
#include <memory>

namespace dae
{
	enum class EventType
	{
		released,
		pressed,
		down
	};

	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	// ---- Concrete Commands ----
	class ExecuteFunction final : public Command
	{
	public:
		ExecuteFunction(std::function<void()> callback);

		void Execute() override;

	private:
		std::function<void()> m_Callback;
	};

	// Null command
	class DoNothing final : public Command
	{
	public:
		virtual void Execute() override;
	};

	struct InputAction
	{
		InputAction() : InputAction(new DoNothing(), EventType::released) {}
		InputAction(Command* pCommand, EventType eventType = EventType::pressed)
			: pCommand(pCommand), eventType(eventType) {}

		std::unique_ptr<Command> pCommand;
		EventType eventType;
	};
}