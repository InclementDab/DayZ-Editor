


class EditorObjectPropertiesDialog: UILinkedObject
{
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
			
	protected TextWidget x_pos;
	
	protected EditorObject m_Object;
			
	void ~EditorObjectPropertiesDialog()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	
	protected override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarkerHandler::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		x_pos = TextWidget.Cast(m_Root.FindAnyWidget("pos_x"));
				
		m_ApplyButton 		= ButtonWidget.Cast(m_Root.FindAnyWidget("SelectButton"));
		m_CancelButton 		= ButtonWidget.Cast(m_Root.FindAnyWidget("CloseButton"));
	}
	

	
	override void Update()
	{
		
		vector position = m_EditorObject.GetPosition();
		x_pos.SetText(position[0].ToString());
		
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) return false;
		
		if (w == m_ApplyButton) {
			
			Print("Applied");
			return true;
		}
		if (w == m_CancelButton) {
			
			m_Root.Show(false);
			m_Root.Update();
			return true;
		}
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y) 
	{
		// temp
		Print(w.GetName());
		if (w.GetName() == "pos_x") {
			GetEditor().GetEditorUI().SetCursor(EditorCursor.HORIZONTAL_SCROLL);
		} else {
			GetEditor().GetEditorUI().SetCursor();
		}
		
		return true;
	}
}
