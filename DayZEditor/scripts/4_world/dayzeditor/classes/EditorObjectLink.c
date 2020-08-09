

// Permanent memory link between an EditorObject and the session
class EditorObjectLink: ref Link<EditorObject>
{
	private string type_name;
	private vector transform[4];
	private EditorObjectFlags flags;
	
	void EditorObjectLink(EditorObject init)
	{
		Print("EditorObjectLink");		
		type_name = init.GetType();
		init.GetTransform(transform);
		flags = init.GetEditorFlags();
	}
	
	void ~EditorObjectLink()
	{
		Print("~EditorObjectLink");
	}
	
	void Create()
	{
		Print("EditorObjectLink::Create");
		if (IsNull()) {
			
			Init(EditorObject.Cast(GetGame().CreateObjectEx("EditorObject", transform[3], ECE_NONE)));		
			EditorObject editor_object = Ptr();
			
			editor_object.SetTransform(transform);
			editor_object.Init(type_name, flags);
			editor_object.Update();
			EditorEvents.ObjectCreateInvoke(null, editor_object);
			
			//editor_object.Select(false); todo fix
		} else {
			
			Print("EditorObjectLink::Create JUST CAME BACK AS NOT NULL");
			Print("IM NOT SURE WHY BUT YOU SHOULD PROBABLY REPORT IT TO A DEVELOPER SMILE :)");
		}
	}

	
	void Delete()
	{
		Print("EditorObjectLink::Delete");
		EditorObject editor_object = Ptr();
		GetGame().ObjectDelete(editor_object);
	}
}

