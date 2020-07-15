


class EditorObjectPropertiesWindow: ScriptedWidgetEventHandler
{
	
	protected Widget m_Root;
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
			
	protected EditBoxWidget x_pos; protected EditBoxWidget y_pos; protected EditBoxWidget z_pos;
	protected EditBoxWidget x_rot; protected EditBoxWidget y_rot; protected EditBoxWidget z_rot;
	
	protected EditorObject m_Object;
			
	void ~EditorObjectPropertiesWindow()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarkerHandler::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		x_pos = m_Root.FindAnyWidget("pos_x");
		y_pos = m_Root.FindAnyWidget("pos_y");
		z_pos = m_Root.FindAnyWidget("pos_z");
		x_rot = m_Root.FindAnyWidget("pos_x0");
		y_rot = m_Root.FindAnyWidget("pos_y0");
		z_rot = m_Root.FindAnyWidget("pos_z0");
				
		m_ApplyButton 		= ButtonWidget.Cast(m_Root.FindAnyWidget("apply"));
		m_CancelButton 		= ButtonWidget.Cast(m_Root.FindAnyWidget("cancel"));
	}
	
	
	void SetObject(EditorObject target)
	{
		m_Object = target;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		vector position = m_Object.GetPosition();
		vector orientation = m_Object.GetOrientation();
		x_pos.SetText("" + position[0]); y_pos.SetText("" + position[1]); z_pos.SetText("" + position[2]);
		x_rot.SetText("" + orientation[0]); y_rot.SetText("" + orientation[1]); z_rot.SetText("" + orientation[2]);
	}
	
	void Update()
	{
		vector new_pos = { x_pos.GetText().ToFloat(), y_pos.GetText().ToFloat(), z_pos.GetText().ToFloat()};
		vector new_ori = { x_rot.GetText().ToFloat(), y_rot.GetText().ToFloat(), z_rot.GetText().ToFloat()};
		
		m_Object.SetPosition(new_pos);
		m_Object.SetOrientation(new_ori);
		vector position = m_Object.GetPosition();
		vector orientation = m_Object.GetOrientation();
		
		x_pos.SetText(position[0].ToString()); y_pos.SetText(position[1].ToString()); z_pos.SetText(position[2].ToString());
		x_rot.SetText(orientation[0].ToString()); y_rot.SetText(orientation[1].ToString()); z_rot.SetText(orientation[2].ToString());
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) return false;
		if (w == m_ApplyButton) {
			
			Print("Applied");
			return true;
		}
		if (w == m_CancelButton) {
			
			return true;
		}
		return false;
		
	}
	
	
	
	
	
	
	
	
}
