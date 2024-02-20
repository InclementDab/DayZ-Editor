class NetworkLightBase: SerializedBuilding
{
	void PropertyChanged(Class source, string property_name)
	{
		// Forcing the parameters to save when i change something.... MIGHT LAG :)
		if (GetEditor().GetEditorObject(this)) {
			Write(GetEditor().GetEditorObject(this).GetData().Parameters);
		}
	}
}