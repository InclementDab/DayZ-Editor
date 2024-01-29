class ObjectNodeView: ScriptView
{
	protected ObjectNode m_ObjectNode;
	
	ButtonWidget Button;
	ImageWidget Image, Outline;
	
	void ObjectNodeView(ObjectNode editor_object)
	{
		m_ObjectNode = editor_object;
		
		Symbols icon = m_ObjectNode.GetIcon();
		
		Image.LoadImageFile(0, icon.Solid());
		Image.SetImage(0);
		
		Outline.LoadImageFile(0, icon.Thin());
		Outline.SetImage(0);
	}
	
	void OnSelectionChange(TreeNode selectable)
	{
		//Image.SetColor(Ternary<int>.If(selectable.IsSelected(), ARGB(255, 7, 111, 255), ARGB(255, 255, 255, 255)));
	}
	
	override void Update(float dt)
	{
		vector base_position = m_ObjectNode.GetBasePoint();
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
		WidgetAnimator.Animate(Image, WidgetAnimatorProperty.COLOR_A, 1.0, 100);
		GetDayZGame().GetEditor().GetHud().SetCursor(Symbols.UP_DOWN_LEFT_RIGHT);
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.Animate(Image, WidgetAnimatorProperty.COLOR_A, 150.0 / 255.0, 100);
		GetDayZGame().GetEditor().GetHud().ClearCursor();
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (button) {
			case 0: {
				if (!m_ObjectNode) {
					return false;
				}
				
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					GetDayZGame().GetEditor().ClearSelections();
				}
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					GetDayZGame().GetEditor().ToggleSelect(m_ObjectNode);
				} else {
					GetDayZGame().GetEditor().Select(m_ObjectNode);
				}
				
				return true;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Marker.layout";
	}
}