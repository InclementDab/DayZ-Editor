
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