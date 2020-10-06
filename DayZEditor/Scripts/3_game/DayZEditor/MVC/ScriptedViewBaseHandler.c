
class ScriptedViewBaseHandler : ScriptedWidgetEventHandler
{
	protected ScriptedViewBase m_ScriptedViewBase;

	void ScriptedViewBaseHandler(ScriptedViewBase viewBase)
	{
		m_ScriptedViewBase = viewBase;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBase.OnClick(w, x, y, button);
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		return m_ScriptedViewBase.OnModalResult(w, x, y, code, result);
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBase.OnDoubleClick(w, x, y, button);
	}

	override bool OnSelect(Widget w, int x, int y)
	{
		return m_ScriptedViewBase.OnSelect(w, x, y);
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		return m_ScriptedViewBase.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);
	}

	override bool OnFocus(Widget w, int x, int y)
	{
		return m_ScriptedViewBase.OnFocus(w, x, y);
	}

	override bool OnFocusLost(Widget w, int x, int y)
	{
		return m_ScriptedViewBase.OnFocusLost(w, x, y);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return m_ScriptedViewBase.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return m_ScriptedViewBase.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		return m_ScriptedViewBase.OnMouseWheel(w, x, y, wheel);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBase.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBase.OnMouseButtonUp(w, x, y, button);
	}

	override bool OnController(Widget w, int control, int value)
	{
		return m_ScriptedViewBase.OnController(w, control, value);
	}

	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		return m_ScriptedViewBase.OnKeyDown(w, x, y, key);
	}

	override bool OnKeyUp(Widget w, int x, int y, int key)
	{
		return m_ScriptedViewBase.OnKeyUp(w, x, y, key);
	}

	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		return m_ScriptedViewBase.OnKeyPress(w, x, y, key);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		return m_ScriptedViewBase.OnChange(w, x, y, finished);
	}

	override bool OnDrag(Widget w, int x, int y)
	{
		return m_ScriptedViewBase.OnDrag(w, x, y);
	}

	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBase.OnDragging(w, x, y, reciever);
	}

	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBase.OnDraggingOver(w, x, y, reciever);
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBase.OnDrop(w, x, y, reciever);
	}

	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBase.OnDropReceived(w, x, y, reciever);
	}

	override bool OnResize(Widget w, int x, int y)
	{
		return m_ScriptedViewBase.OnResize(w, x, y);
	}

	override bool OnChildAdd(Widget w, Widget child)
	{
		return m_ScriptedViewBase.OnChildAdd(w, child);
	}

	override bool OnChildRemove(Widget w, Widget child)
	{
		return m_ScriptedViewBase.OnChildRemove(w, child);
	}

	override bool OnUpdate(Widget w)
	{
		return m_ScriptedViewBase.OnUpdate(w);
	}
};