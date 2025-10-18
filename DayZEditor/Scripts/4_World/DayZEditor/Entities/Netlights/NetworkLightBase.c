modded class NetworkLightBase
{
	override void PropertyChanged(Class source, string property_name)
	{
		super.PropertyChanged(source, property_name);
		
		EditorObject editor_object = GetEditor().GetEditorObject(this);
		if (editor_object) {
			EditorObjectData data = editor_object.GetData();
			Write(data.Parameters);
			Read(data.Parameters);
		}
	}
}