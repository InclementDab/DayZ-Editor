


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
		Widget frame = m_LayoutRoot.FindAnyWidget("EditorListItemFrame");
		frame.GetSize(x, y);
		frame.SetSize(290 - 15 * m_NestIndex, y);
	}
	
	int GetNestIndex() {
		return m_NestIndex;
	}
	
		

	
	
		

	
}
