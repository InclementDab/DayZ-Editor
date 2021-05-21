class EditorCameraTrackListItemController: Controller
{	
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
	
	//PanelColor
	Widget CameraTrackGroupWrapper;
	//OutlineColor
	Widget MainContainerWrapper;
	//CameraIconColor
	Widget CameraTrackOptionButton;
	// Abstract
	bool IsSelected;
	
	//Gorm add highlight on mouse enter
	void Select() 
	{
		IsSelected = true;
		CameraTrackGroupWrapper.SetColor(COLOR_BLUE);
		MainContainerWrapper.SetColor(COLOR_SALMON);
		CameraTrackOptionButton.SetColor(COLOR_BLUE);
	}
	
	void Highlight()
	{
		//CameraTrackGroupWrapper.SetColor(COLOR_BLUE_HIGHLIGHT);
		MainContainerWrapper.SetColor(COLOR_BLUE_HIGHLIGHT_OUTLINE);
		//CameraTrackOptionButton.SetColor(COLOR_BLUE_LIGHT);
	}
	
	void Deselect() 
	{	
		IsSelected = false;
		CameraTrackGroupWrapper.SetColor(COLOR_DEFAULT_PANEL);
		MainContainerWrapper.SetColor(COLOR_DEFAULT_OUTLINE);
		CameraTrackOptionButton.SetColor(COLOR_DEFAULT_ICON);
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (w.IsInherited(ButtonWidget) || w.IsInherited(EditBoxWidget)) {
			return super.OnMouseButtonUp(w, x, y, button);
		} 
		
		if (!IsSelected) {
			Select();
		} else {
			Deselect();
		}
		
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected) {
			Highlight();
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected) {
			Deselect();
		}
		
		return true;
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