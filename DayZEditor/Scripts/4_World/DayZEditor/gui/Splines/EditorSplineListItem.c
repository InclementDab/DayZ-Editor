class EditorSplineListItem: ScriptView
{
	protected string m_Uuid;
	protected bool m_Selected;
	protected float m_ClickTick;
	protected ref ScriptCaller m_OnClicked;

	ImageWidget Icon;
	TextWidget Label, Detail;

	void EditorSplineListItem(EditorSplineData spline, ScriptCaller on_click)
	{
		m_Uuid = spline.Uuid;
		m_OnClicked = on_click;
		Label.SetText(spline.Name);

		if (spline.IsSpline()) {
			Symbols.BEZIER_CURVE.Load(Icon, 3);
		} else {
			Symbols.VECTOR_POLYGON.Load(Icon, 3);
		}

		Icon.SetColor(0xFFFF6FA8);

		string detail = string.Format("%1 points", spline.Count());
		if (spline.Generator.Type != EditorSplineGeneratorType.NONE) {
			detail = EditorSplineGeneratorSettings.GetTypeName(spline.Generator.Type) + "  |  " + detail;
		}

		if (spline.Generated.Count() > 0) {
			detail += "  |  placed";
		}

		Detail.SetText(detail);
		SetSelected(false);
	}

	void SetSelected(bool selected)
	{
		m_Selected = selected;
		if (m_Selected) {
			m_LayoutRoot.SetColor(0xFF007ACC);
		} else {
			m_LayoutRoot.SetColor(0xFF24282E);
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!m_Selected) {
			m_LayoutRoot.SetColor(0xFF343A42);
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!m_Selected) {
			m_LayoutRoot.SetColor(0xFF24282E);
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == 0 && m_OnClicked) {
			bool double_click = GetGame().GetTickTime() < m_ClickTick + 0.3;
			m_ClickTick = GetGame().GetTickTime();
			if (double_click) {
				m_ClickTick = 0;
			}

			m_OnClicked.Invoke(this, m_Uuid, double_click);
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	string GetUuid()
	{
		return m_Uuid;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/splines/SplineListItem.layout";
	}
}