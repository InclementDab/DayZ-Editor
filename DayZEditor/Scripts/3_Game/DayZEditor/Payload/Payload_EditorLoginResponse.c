class EditorGenericCallback: RestCallbackBase
{
	ref ScriptCaller SuccessCallback;
	ref ScriptCaller FileCallback;
	
	ref Param Data;
	
	void EditorGenericCallback(ScriptCaller success = null, ScriptCaller file = null, Param data = null)
	{
		SuccessCallback = success;
		FileCallback = file;
		Data = data;
	}
	
	override void OnSuccess(string data, int dataSize)
	{
		super.OnSuccess(data, dataSize);

		if (SuccessCallback.IsValid()) {
			if (Data) {
				SuccessCallback.Invoke(data, dataSize, Data);
			} else {
				SuccessCallback.Invoke(data, dataSize);
			}
		}
	}
	
	override void OnFileCreated(string fileName, int dataSize)
	{
		super.OnFileCreated(fileName, dataSize);
		
		if (FileCallback.IsValid()) {
			if (Data) {
				FileCallback.Invoke(fileName, dataSize, Data);
			} else {
				FileCallback.Invoke(fileName, dataSize);
			}
		}
	}
}

class EditorLoginCallback : RestCallbackBase
{
	protected ScriptCaller m_OnPayloadSuccess;

	void EditorLoginCallback(ScriptCaller on_payload_success)
	{
		m_OnPayloadSuccess = on_payload_success;
	}

	override void OnError(int errorCode)
	{
		super.OnError(errorCode);
		
		GetDayZGame().LoginCache = null;
	}
	
	override void OnSuccess(string data, int dataSize)
	{
		super.OnSuccess(data, dataSize);

		Payload_EditorLoginResponse response = new Payload_EditorLoginResponse();
		string error;
		if (!JsonFileLoader<Payload_EditorLoginResponse>.LoadData(data, response, error)) {
			Error(error);
			return;
		}
		
		if (m_OnPayloadSuccess) {
			m_OnPayloadSuccess.Invoke(response);
		}
		
		GetDayZGame().LoginCache = response;
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
	string GlobalTimePlayed;
}