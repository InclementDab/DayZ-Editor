
class DropdownElementPrefabController: Controller
{
	string Text;
}

class DropdownElementPrefab: ScriptViewTemplate<DropdownElementPrefabController>
{
	void DropdownElementPrefab(string text)
	{
		Debug_Logging = true;
		m_TemplateController.Text = text;
		m_TemplateController.NotifyPropertyChanged("Text");
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownElementPrefab.layout";
	}
}


class DropdownListPrefab: PrefabBase<ObservableCollection<ref ScriptedViewBase>>
{
	ref ObservableCollection<ref ScriptedViewBase> collection_ref;
			
	private WrapSpacerWidget DropdownWrapper;
	
	void DropdownListPrefab(string caption, Controller binding_context, string binding_name, ObservableCollection<ref ScriptedViewBase> default_value = DEFAULT_VALUE)
	{
		Debug_Logging = true;
		collection_ref = new ObservableCollection<ref ScriptedViewBase>("Value", m_PrefabBaseController);
		m_PrefabBaseController.Value = collection_ref;
	}
	
	void ~DropdownListPrefab()
	{
		Print("~DropdownListPrefab");
		delete collection_ref;
	}
	
		
	void InsertItem(string item_text)
	{
		InsertItem(new DropdownElementPrefab(item_text));
	}
	
	void InsertItem(DropdownElementPrefab element)
	{
		collection_ref.Insert(element);
		
		// Doing this because our buttons need to be set on TOP of the render
		float x, y, w, h, x1, y1;
		m_LayoutRoot.Update();
		m_LayoutRoot.GetScreenPos(x, y);
		m_LayoutRoot.GetScreenSize(w, h);
		element.GetLayoutRoot().GetScreenSize(x1, y1);
		/*
		element.GetLayoutRoot().SetPos(x, y + (y1 * (collection_ref.Count() + 1)));
		element.GetLayoutRoot().SetSize(w, h);*/
	}
	
	void Toggle()
	{
		for (int i = 0; i < collection_ref.Count(); i++) {
			collection_ref[i].GetLayoutRoot().Show(collection_ref[i].GetLayoutRoot().IsVisible());
		}
	}
	
	bool DropdownPrefabExecute(ButtonCommandArgs args)
	{
		Toggle();
		return true;
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		//EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.Value);
		//m_BindingContext.PropertyChanged(m_BindingName);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownPrefab.layout";
	}
	
}