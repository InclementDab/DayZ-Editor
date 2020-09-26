


static void ClearWidgetChildren(out ref Widget w)
{
	
	Widget child = w.GetChildren();
	while (child != null) {
		w.RemoveChild(child);
		child = child.GetSibling();
	}	
}

