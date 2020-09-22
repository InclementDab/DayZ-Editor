
class EditorTooltipController: Controller
{
	string ContentTitle;
	string ContentText;
	
	Object ContentItemData;
	
	void ~EditorTooltipController()
	{
		EditorLog.Trace("~EditorTooltipController");
#ifndef COMPONENT_SYSTEM
		if (ContentItemData) 
			GetWorkbenchGame().ObjectDelete(ContentItemData);
#endif
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "ContentItemData": {
				if (ContentItemData && ContentItemData.GetDisplayName() != string.Empty) {
					ContentTitle = ContentItemData.GetDisplayName();
					NotifyPropertyChanged("ContentTitle");
				}
				
				break;
			}
			
		}
	}
}

class EditorTooltip: ScriptView
{
	protected TextWidget HeaderLabel;
	
	void EditorToolTip(Widget parent = null)
	{
		EditorUIManager.CurrentTooltip = this;
	}
	
	void SetTitle(string title)
	{
		GetEditorTooltipController().ContentTitle = title;
		GetEditorTooltipController().NotifyPropertyChanged("ContentTitle");
		
		float w, h, lw, lh;
		HeaderLabel.GetScreenSize(w, h);
		m_LayoutRoot.GetScreenSize(lw, lh);
		m_LayoutRoot.SetSize(w + 80, lh);
	}
	
	void SetContent(string text)
	{
		GetEditorTooltipController().ContentText = text;
		GetEditorTooltipController().NotifyPropertyChanged("ContentText");
	}
	
	void SetContent(Object item)
	{
		GetEditorTooltipController().ContentItemData = item;
		GetEditorTooltipController().NotifyPropertyChanged("ContentItemData");
	}
		
	override typename GetControllerType() {
		return EditorTooltipController;
	}

	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
	
	protected EditorTooltipController GetEditorTooltipController() {
		return EditorTooltipController.Cast(GetController());
	}
}