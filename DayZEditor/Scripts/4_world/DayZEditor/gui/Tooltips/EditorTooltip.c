
class EditorTooltip: ScriptView
{
	protected TextWidget HeaderLabel;
	protected EditorTooltipController m_EditorTooltipController;
		
	void SetTitle(string title)
	{
		m_EditorTooltipController = EditorTooltipController.Cast(GetController());
		m_EditorTooltipController.ContentTitle = title;
		m_EditorTooltipController.NotifyPropertyChanged("ContentTitle");
		
		float w, h, lw, lh;
		HeaderLabel.GetScreenSize(w, h);
		m_LayoutRoot.GetScreenSize(lw, lh);
		m_LayoutRoot.SetSize(w + 80, lh);
	}
	
	void SetContent(string text)
	{
		m_EditorTooltipController = EditorTooltipController.Cast(GetController());
		m_EditorTooltipController.ContentText = text;
		m_EditorTooltipController.NotifyPropertyChanged("ContentText");
	}
	
	void SetContent(Object item)
	{
		m_EditorTooltipController = EditorTooltipController.Cast(GetController());
		m_EditorTooltipController.ContentItemData = item;
		m_EditorTooltipController.NotifyPropertyChanged("ContentItemData");
	}
		
	override typename GetControllerType() {
		return EditorTooltipController;
	}

	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
}