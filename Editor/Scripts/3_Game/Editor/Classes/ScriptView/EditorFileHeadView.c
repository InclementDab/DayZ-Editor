class EditorFileHeadView: ScriptView
{	
	SpacerWidget TextSpacer;
	TextWidget Text;
	
	void EditorFileHeadView(string text)
	{
		Text.SetText(text);
		
		float w, h;
		TextSpacer.GetScreenSize(w, h);
		Print(w);
		
		Text.GetScreenSize(w, h);
		Print(w);
		
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\FileHead.layout";
	}
}