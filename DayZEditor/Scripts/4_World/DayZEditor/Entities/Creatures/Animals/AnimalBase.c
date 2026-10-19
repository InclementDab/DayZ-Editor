modded class AnimalBase
{
	override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished)) {
			return true;
		}

		if (!EditorObject.ShouldSuppressAICommands(this)) {
			return false;
		}

		DayZAnimalInputController input_controller = GetInputController();
		if (!input_controller) {
			return false;
		}

		input_controller.OverrideTurnSpeed(true, 0);
		input_controller.OverrideMovementSpeed(true, 0);
		input_controller.OverrideAlertLevel(true, false, 0, 0);
		input_controller.OverrideBehaviourSlot(true, DayZAnimalBehaviourSlot.CALM_GRAZING);
		input_controller.OverrideBehaviourAction(true, DayZAnimalBehaviourAction.GRAZE_ON_SPOT_INPUT);

		if (!pCurrentCommandFinished && pCurrentCommandID == DayZAnimalConstants.COMMANDID_ATTACK) {
			SignalAIAttackEnded();
			StartCommand_Move();
		}

		return false;
	}

	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished)) {
			return true;
		}

		return EditorObject.ShouldSuppressAICommands(this);
	}
}