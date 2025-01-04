class EditorLoginCallback : RestCallbackBase
{
	protected ref ScriptCaller m_OnPayloadSuccess;

	void EditorLoginCallback(ScriptCaller on_payload_success)
	{
		m_OnPayloadSuccess = on_payload_success;
	}

	override void OnSuccess(string data, int dataSize)
	{
		super.OnSuccess(data, dataSize);

		Payload_EditorLoginResponse response = new Payload_EditorLoginResponse();
		string error;
		if (!JsonFileLoader<Payload_EditorLoginResponse>.LoadData(data, response, error))
		{
			Error(error);
			return;
		}

		if (m_OnPayloadSuccess)
		{
			m_OnPayloadSuccess.Invoke(response);
		}
	}
}

class Payload_EditorLoginResponse : Managed
{
	string Message;
	int CurrentLoginCount;
	string Token;
	ref array<ref Payload_ServerShowcase> Showcases = { };

	// These are strings because we are likely to reach int.MAX soon
	string GlobalItemsPlaced;
    string GlobalItemsDeleted;
    string GlobalCharactersControlled;
    string GlobalCharactersEdited;
}