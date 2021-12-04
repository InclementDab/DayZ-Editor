class EditorCameraTrackListItemController: ViewController
{	
	protected Object m_WorldObject;
	
	string Name;
	StringEvaluater pX, pY, pZ;
	StringEvaluater oX, oY, oZ;
	float Time;
	bool FlipOrientation;
	
	bool CollapseTab = false;
	Widget CollapsePanel;
	
	void EditorCameraTrackListItemController()
	{
		m_WorldObject = GetGame().CreateObject("CinematicCamera", vector.Zero, true);
	}
	
	void ~EditorCameraTrackListItemController()
	{
		GetGame().ObjectDelete(m_WorldObject);
	}
	
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
			
			case "pX":
			case "pY":
			case "pZ": {
				if (m_WorldObject) {
					m_WorldObject.SetPosition(GetPosition());
					m_WorldObject.Update();
				}
				break;
			}
			
			case "oX": 
			case "oY":
			case "oZ": {
				if (m_WorldObject) {
					m_WorldObject.SetOrientation(GetOrientation());
					m_WorldObject.Update();
				}
				break;
			}
		}
	}
	
	SerializedCameraTrack GetSerializedData()
	{
		return new SerializedCameraTrack(GetPosition(), GetOrientation(), Time, FlipOrientation, Name);
	}
}

class EditorCameraTrackListItem: ScriptViewTemplate<EditorCameraTrackListItemController>
{
	//PanelColor
	Widget CameraTrackGroupWrapper;
	//OutlineColor
	Widget MainContainerWrapper;
	//CameraIconColor
	Widget CameraTrackOptionButton;
	// Abstract
	bool IsSelected;
	
	protected Editor m_Editor = GetEditor();
	
	void EditorCameraTrackListItem(vector position, vector orientation, float time, string name, bool flip = false)
	{
		GetTemplateController().Name = name;
		SetData(position, orientation, time, flip);
	}
	
	void OnSelected() 
	{
		IsSelected = true;
		CameraTrackGroupWrapper.SetColor(m_Editor.Settings.SelectionColor);
		MainContainerWrapper.SetColor(m_Editor.Settings.HighlightColor);
		CameraTrackOptionButton.SetColor(m_Editor.Settings.MarkerPrimaryColor);
	}
		
	void OnDeselected() 
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
		
		if (!m_TemplateController.CollapseTab) {
			m_TemplateController.CollapseTab = true;
			m_TemplateController.NotifyPropertyChanged("CollapseTab");
		}
		
		if (!IsSelected && button == 0) {
			GetEditor().GetCameraTrackManager().SelectCameraTrack(this);
		} else {
			GetEditor().GetCameraTrackManager().DeselectCameraTrack(this);
		}
		
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected) {
			MainContainerWrapper.SetColor(COLOR_BLUE_HIGHLIGHT_OUTLINE);
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected) {
			MainContainerWrapper.SetColor(COLOR_DEFAULT_OUTLINE);
		}
		
		return true;
	}
	
	void OnSetExecute(ButtonCommandArgs args)
	{
		SetData(GetEditor().GetCamera().GetPosition(), GetEditor().GetCamera().GetOrientation(), GetTemplateController().Time, false);
	}
	
	void OnDeleteExecute(ButtonCommandArgs args)
	{
		GetEditor().GetCameraTrackManager().DeleteCameraTrack(this);
	}
	
	void OnFlyToExecute(ButtonCommandArgs args)
	{
		GetEditor().GetCamera().SetPosition(m_TemplateController.GetPosition());
		GetEditor().GetCamera().SetOrientation(m_TemplateController.GetOrientation());
	}
	
	void SetData(vector position, vector orientation, float time, bool flip)
	{
		m_TemplateController.pX = position[0].ToString();
		m_TemplateController.pY = position[1].ToString();
		m_TemplateController.pZ = position[2].ToString();
		m_TemplateController.oX = orientation[0].ToString();
		m_TemplateController.oY = orientation[1].ToString();
		m_TemplateController.oZ = orientation[2].ToString();
		m_TemplateController.Time = time;
		m_TemplateController.FlipOrientation = flip;
		m_TemplateController.NotifyPropertyChanged();
	}
		
	SerializedCameraTrack GetSerializedData()
	{
		return m_TemplateController.GetSerializedData();
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