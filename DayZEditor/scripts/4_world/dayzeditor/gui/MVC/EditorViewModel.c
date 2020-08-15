
class EditorUIViewModel: Managed
{
	
	string DebugText1;
	string DebugText2;
	string DebugText3;
	
	
}


class EditorViewProperty<Class T1, Class T2> 
{
	protected T1 m_Widget;
	protected T2 m_Value;
	
	void EditorViewProperty(T1 w)
	{
		EditorLog.Trace("EditorViewModelProperty");
		Print(w);
	}
	
	static void SetValue(Widget w, T2 value)
	{
		Print("SetValue");
		Print(value);
	}
	
	static T2 GetValue(Widget w)
	{
		T2 value;
		switch (w.Type()) {
			
			case MultilineTextWidget:
			case MultilineEditBoxWidget: {
				
				break;
			}
			
			
		}
		
		return value;
	}
}
