

class TranslationWidget: BuildingBase
{
	private EditorObject m_ParentObject;
	
	void TranslationWidget()
	{
		Print("TranslationWidget");
		TStringArray textures = GetHiddenSelectionsTextures();
		SetObjectTexture(GetHiddenSelectionIndex("TranslateY"), textures[0]);
		SetObjectTexture(GetHiddenSelectionIndex("TranslateZ"), textures[0]);

	}
	
	
	void ~TranslationWidget()
	{
		Print("~TranslationWidget");
	}
	
	
	
	
	void SetEditorObject(EditorObject parent_object)
	{
		m_ParentObject = parent_object;
	}
	
	
	static TranslationWidget Create(EditorObject parent_object)
	{
		vector size = parent_object.GetSize();
		vector position = vector.Zero;
		position[1] = position[1] + size[1]/2;
		
		TranslationWidget widget = GetGame().CreateObjectEx("TranslationWidget", position, ECE_NONE);

		
		parent_object.AddChild(widget, -1);
		parent_object.Update();
		return widget;
		
	}
	
}