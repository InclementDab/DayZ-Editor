modded class ActionManagerClient
{
	protected override void FindContextualUserActions(int pCurrentCommandID)
	{
		if (!GetEditor() || !GetEditor().IsActive()) {
			super.FindContextualUserActions(pCurrentCommandID);
		}
	}
}