class EditorVersionCallback: RestCallbackBase
{
	protected ScriptCaller m_OnPayloadSuccess;

	void EditorVersionCallback(ScriptCaller on_payload_success)
	{
		m_OnPayloadSuccess = on_payload_success;
	}

    override void OnSuccess(string data, int dataSize)
	{
		super.OnSuccess(data, dataSize);

		Payload_EditorVersionResponse response = new Payload_EditorVersionResponse();
		string error;
		if (!JsonFileLoader<Payload_EditorVersionResponse>.LoadData(data, response, error)) {
			Error(error);
			return;
		}

		if (m_OnPayloadSuccess.IsValid()) {
			m_OnPayloadSuccess.Invoke(response);
		}
	}
}

class Payload_EditorVersionResponse: Managed
{
    string Version;
}