class EditorShowcaseResponse: RestCallbackBase
{
	protected ref ScriptCaller m_OnPayloadSuccess;
	int Index;	

	void EditorShowcaseResponse(ScriptCaller on_payload_success)
	{
		m_OnPayloadSuccess = on_payload_success;
	}
	
	override void OnSuccess(string data, int dataSize)
	{
		super.OnSuccess(data, dataSize);
	}
	
	override void OnFileCreated(string fileName, int dataSize)
	{
		super.OnFileCreated(fileName, dataSize);
		
		if (m_OnPayloadSuccess) {
			m_OnPayloadSuccess.Invoke(fileName, Index);
		}
	}
}

class EditorSupporterCallback: RestCallbackBase
{
	protected ref ScriptCaller m_OnPayloadSuccess;

	void EditorSupporterCallback(ScriptCaller on_payload_success)
	{
		m_OnPayloadSuccess = on_payload_success;
	}

	override void OnSuccess(string data, int dataSize)
	{
		super.OnSuccess(data, dataSize);

		Payload_EditorSupporterResponse response = new Payload_EditorSupporterResponse();
		string error;
		if (!JsonFileLoader<Payload_EditorSupporterResponse>.LoadData(data, response, error)) {
			Error(error);
			return;
		}

		if (m_OnPayloadSuccess) {
			m_OnPayloadSuccess.Invoke(response);
		}
	}
}

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

class Payload_EditorSupporter: Managed
{
	string DisplayName;
	int Level;
}

class Payload_EditorSupporterResponse: Managed
{
	ref array<ref Payload_EditorSupporter> Supporters = {};
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
	string GlobalTimePlayed;
}