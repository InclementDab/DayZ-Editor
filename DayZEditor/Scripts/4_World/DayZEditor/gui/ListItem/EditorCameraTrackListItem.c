class EditorCameraTrackListItemController: Controller
{	
	vector Position0;
	vector Position1;
	
	
	string Name;
	StringEvaluater pX, pY, pZ;
	StringEvaluater oX, oY, oZ;
	float Time;
	bool FlipOrientation;
	
	bool CollapseTab = false;
	Widget CollapsePanel;
	
	vector GetPosition()
	{
		return Vector(pX.Parse(), pY.Parse(), pZ.Parse());
	}
	
	vector GetOrientation()
	{
		return Vector(oX.Parse(), oY.Parse(), oZ.Parse());
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "CollapseTab": {
				CollapsePanel.Show(CollapseTab);
				break;
			}
		}
	}
}

class EditorCameraTrackListItem: ScriptViewTemplate<EditorCameraTrackListItemController>
{
	void EditorCameraTrackListItem(vector position, vector orientation, float time, string name)
	{
		GetTemplateController().Name = name;
		SetData(position, orientation, time);
	}
	
	void OnSetExecute(ButtonCommandArgs args)
	{
		SetData(GetEditor().GetCamera().GetPosition(), GetEditor().GetCamera().GetOrientation(), GetTemplateController().Time);
	}
	
	void OnDeleteExecute(ButtonCommandArgs args)
	{
		delete this;
	}
	
	void OnFlyToExecute(ButtonCommandArgs args)
	{
		GetEditor().GetCamera().SetPosition(m_TemplateController.GetPosition());
		GetEditor().GetCamera().SetOrientation(m_TemplateController.GetOrientation());
	}
	
	void SetData(vector position, vector orientation, float time)
	{
		m_TemplateController.pX = position[0].ToString();
		m_TemplateController.pY = position[1].ToString();
		m_TemplateController.pZ = position[2].ToString();
		m_TemplateController.oX = orientation[0].ToString();
		m_TemplateController.oY = orientation[1].ToString();
		m_TemplateController.oZ = orientation[2].ToString();
		m_TemplateController.Time = time;
		m_TemplateController.NotifyPropertyChanged();
	}
		
	EditorCameraTrackListItemController GetData()
	{
		return GetTemplateController();
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/cameratrack/CameraTrackGroup.layout";
	}
}