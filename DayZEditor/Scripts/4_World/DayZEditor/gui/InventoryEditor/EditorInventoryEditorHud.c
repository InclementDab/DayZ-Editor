class EditorInventoryEditorCamera: Camera
{
	protected ref Timer m_LerpTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	
	void EditorInventoryEditorCamera()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(OnFrame);
	}
	
	void ~EditorInventoryEditorCamera()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(OnFrame);
	}
	
	void OnFrame()
	{
		LookAt(GetGame().GetPlayer().GetPosition() + "0 1 0");
	}
	
	void LerpToPosition(vector target_position, float animation_time)
	{
		vector current_pos = GetGame().GetCurrentCameraPosition();
		m_LerpTimer.Run(0.01, this, "RunPositionLerp", new Param4<vector, vector, float, int>(current_pos, target_position, animation_time, GetGame().GetTime()), true);
	}	
	
	private void RunPositionLerp(vector start, vector finish, float duration, int start_time)
	{
		float tfactor = (1 / duration) * (GetGame().GetTime() - start_time) / 1000;
		SetPosition(Math.SmoothLerpVector(start, finish, tfactor));
		Update();		
		
		if (tfactor >= 1) {
			m_LerpTimer.Stop();
		}
	}
}

class EditorInventoryEditorController: ViewController
{
	static const ref TStringArray RADIO_BUTTONS = {
		"ShoulderLeft",
		"ShoulderRight",
		"VestSlot",
		"ShirtSlot",
		"BeltSlot",
		"LegsSlot",
		"BackSlot",
		"HeadSlot",
		"FaceSlot",
		"EyeSlot",
		"GloveSlot",
		"FeetSlot",
		"ArmbandSlot"
	};
	
	bool ShoulderLeft;
	bool ShoulderRight;
	bool VestSlot;
	bool ShirtSlot;
	bool BeltSlot;
	bool LegsSlot;
	bool BackSlot;
	bool HeadSlot;
	bool FaceSlot;
	bool EyeSlot;
	bool GloveSlot;
	bool FeetSlot;
	bool ArmbandSlot;
	
	override void PropertyChanged(string property_name)
	{
		// Radio Button Logic
		foreach (string button: RADIO_BUTTONS) {
			if (button == property_name) {
				continue;
			}
			
			EnScript.SetClassVar(this, button, 0, false);
			NotifyPropertyChanged(button, false);
		}
	}
}

class EditorInventoryEditorHud: ScriptViewTemplate<EditorInventoryEditorController>
{
	protected EditorInventoryEditorCamera m_Camera;
	
	void EditorInventoryEditorHud()
	{
		m_Camera = EditorInventoryEditorCamera.Cast(GetGame().CreateObject("EditorInventoryEditorCamera", GetGame().GetPlayer().GetPosition()));
		
		vector pos = GetGame().GetPlayer().GetPosition();
		vector dir = GetGame().GetPlayer().GetDirection();
		
		vector target_pos = pos + "0 1.2 0" + (dir * 3.0) + (dir.Perpend() * 1.0);
		//m_Camera.LerpToPosition(target_pos, 1.0);
		m_Camera.SetPosition(target_pos);
		m_Camera.Update();
		
		m_Camera.SetActive(true);
	}
	
	void ~EditorInventoryEditorHud()
	{
		m_Camera.SetActive(false);
		GetGame().ObjectDelete(m_Camera);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/Inventory.layout";
	}
}