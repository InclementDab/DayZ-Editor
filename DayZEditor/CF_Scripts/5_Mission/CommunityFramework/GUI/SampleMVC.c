
static bool ShowedTutorialMessage;

// Check JM/CF/GUI/layouts/sample_mvc.layout
class SampleController: Controller
{	
	// Properties
	float slider_value = 25; // default value will be 25
	float progress_value = 75; // default value will be 75
	
	ButtonWidget text_list_add;
	ref ObservableCollection<string> observable_list_box = new ObservableCollection<string>("observable_list_box", this);

	
	void SampleController()
	{
		// This is just to get you into this file. Ignore it :)
		if (!ShowedTutorialMessage)
			Workbench.Dialog("Welcome to MVC!", "Check \"SampleController\" class for an example on how to use this utility!");
		
		ShowedTutorialMessage = true;
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "slider_value": {
				
				// sets progress_value to any value you want
				progress_value = Math.AbsFloat(slider_value - 100); 
				
				// MUST be called when changing a property, notifies the UI that something changed!
				NotifyPropertyChanged("progress_value"); 
				break;
			}
		}
	}
	
		
	// You also dont HAVE to use RelayCommands. normal Scripted Events work fine
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == text_list_add) {
			observable_list_box.Insert("Hello World! #" + observable_list_box.Count());
		}
		
		return false;
	}
}

// You can have sub controllers within windows for organization
// they wont interfere with each other
class SampleSubController: Controller
{
	// Note: Selected_Item on CheckBoxWidgets & ButtonWidgets binds the Text of a widget. 
	bool button_state = true;
	bool inverted_button_state = !button_state;
	
	// Dont use FindAnyWidget - Enfusion will automatically asign it if its the same name as the Widget name
	ButtonWidget button; 
	
	int observable_combo_selection;
	ref ObservableCollection<string> observable_combo_box = new ObservableCollection<string>("observable_combo_box", this);
	
	static const int COLOR_ON = COLOR_GREEN;
	static const int COLOR_OFF = COLOR_RED;
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		observable_combo_box.Insert("Item1");
		observable_combo_box.Insert("Item2");
		observable_combo_box.Insert("Item3");

		
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "button_state": {
				if (button_state) {
					button.SetColor(COLOR_ON);
				} else {
					button.SetColor(COLOR_OFF);
				}
				
				inverted_button_state = !button_state;
				
				// Calling false here will NOT call PropertyChanged again
				// Otherwise we'd have a stack overflow between button_state and inverted_button_state
				NotifyPropertyChanged("inverted_button_state", false); 
				break;
			}
			
			case "inverted_button_state": {
				
				button_state = !inverted_button_state;
				NotifyPropertyChanged("button_state");
				break;
			}
			
			
			case "observable_combo_selection": {
				Print("Combo Box Selection = " + observable_combo_box.Get(observable_combo_selection));
				break;
			}
		}
	}
}