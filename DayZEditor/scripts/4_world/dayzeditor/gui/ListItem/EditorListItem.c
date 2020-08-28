
class WidgetData<Class T>
{
	static T GetFromWidget(Widget w)
	{
		T list_item;
		w.GetUserData(list_item);
		return list_item;
	}
}


class EditorListItem: EditorWidget
{
	private int m_NestIndex;
		
	protected ref EditorListItemController m_EditorListItemController;
	EditorListItemController GetController() { 
		return m_EditorListItemController; 
	}

	void EditorListItem() 
	{ 
		EditorLog.Trace("EditorListItem");

		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/items/EditorListItem.layout", null);		
		m_LayoutRoot.GetScript(m_EditorListItemController);
		m_LayoutRoot.SetUserData(this);
		
		m_EditorListItemController.SetListItem(this);
	}
	


	void ~EditorListItem() 
	{ 
		EditorLog.Trace("~EditorListItem"); 
		//m_LayoutRoot.Unlink();
	}
	
	
	void SetNestIndex(int index)
	{
		EditorLog.Trace("EditorListItem::SetNestIndex " + index);
		m_NestIndex = index;
		float x, y;
		m_EditorListItemController.EditorListItemFrame.GetSize(x, y);
		m_EditorListItemController.EditorListItemFrame.SetSize(290 - 15 * m_NestIndex, y);
		m_EditorListItemController.EditorListItemFrame.Update();
	}
	
	int GetNestIndex() {
		return m_NestIndex;
	}
	
		

	
	
		

	
}
