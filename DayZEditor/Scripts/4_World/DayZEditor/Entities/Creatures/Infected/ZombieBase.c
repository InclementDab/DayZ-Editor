modded class ZombieBase
{
	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished)) {
			return true;
		}

		return EditorObject.ShouldSuppressAICommands(this);
	}
}