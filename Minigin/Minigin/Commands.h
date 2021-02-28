#pragma once
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

	struct InputAction
	{
		InputAction(Command* pCommand, EventType eventType = EventType::pressed)
			: pCommand(pCommand), eventType(eventType) {}

		std::unique_ptr<Command> pCommand;
		EventType eventType;
	};


	// ---- Concrete Commands ----
	class DoDamage final : public Command
	{
	public:
		DoDamage();
		void Execute() override;

	private:
	};
}