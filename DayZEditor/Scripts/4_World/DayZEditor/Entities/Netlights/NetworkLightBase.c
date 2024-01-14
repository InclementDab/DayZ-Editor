modded class NetworkLightBase
{
	override void PropertyChanged(Class source, string property_name)
	{
		super.PropertyChanged(source, property_name);
		
		// Forcing the parameters to save when i change something.... MIGHT LAG :)
		if (GetEditor().GetEditorObject(this)) {
			Write(GetEditor().GetEditorObject(this).CreateSerializedData().Parameters);
		}
	}
}