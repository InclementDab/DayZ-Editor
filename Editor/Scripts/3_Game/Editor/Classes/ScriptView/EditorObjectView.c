class EditorObjectView: ScriptView
{
	protected EditorObject m_EditorObject;
	
	ImageWidget MarkerImage, MarkerOutline;
	
	void EditorObjectView(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		
		array<string> icon_split = {};
		m_EditorObject.GetIcon().Split(":", icon_split);
		if (icon_split.Count() > 2) {
			Print(string.Format("set:solid image:%1", icon_split[2]));
			MarkerImage.LoadImageFile(0, string.Format("set:solid image:%1", icon_split[2]));
			MarkerOutline.LoadImageFile(0, string.Format("set:thin image:%1", icon_split[2]));
		} else {
			MarkerImage.LoadImageFile(0, m_EditorObject.GetIcon());
			MarkerOutline.LoadImageFile(0, m_EditorObject.GetIcon());
		}
		
		MarkerImage.SetImage(0);
		MarkerOutline.SetImage(0);
	}
	
	override void Update(float dt)
	{
		vector base_position = m_EditorObject.GetBasePoint();
		base_position = GetGame().GetScreenPos(base_position);
		
		if (base_position[2] > 0) {
			m_LayoutRoot.SetPos(base_position[0] - 12.5, base_position[1] - 12.5);
			m_LayoutRoot.Show(true);
		} else {
			m_LayoutRoot.Show(false);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(MarkerImage, WidgetAnimatorProperty.COLOR_A, 1.0, 100);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.Animate(MarkerImage, WidgetAnimatorProperty.COLOR_A, 150.0 / 255.0, 100);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		m_EditorObject.SetSelected(!m_EditorObject.IsSelected());
		return super.OnClick(w, x, y, button);
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Marker.layout";
	}
}