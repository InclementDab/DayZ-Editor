modded class NetworkParticleBase
{
	override void PropertyChanged(string property_name)
	{
		super.PropertyChanged(property_name);
		
		// Forcing the parameters to save when i change something.... MIGHT LAG :)
		if (GetEditor().GetEditorObject(this)) {
			//Write(GetEditor().GetEditorObject(this).CreateSerializedData().Parameters);
		}
	}
}