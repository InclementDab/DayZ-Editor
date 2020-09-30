
enum NotifyCollectionChangedAction {
	Insert,
	InsertAt,
	Remove,
	Replace,
	Move,
	Swap,
	Clear
};


class CollectionSwapArgs
{
	int StartIndex;
	int FinishIndex;
	void CollectionSwapArgs(int start, int finish) 
	{
		StartIndex = start; FinishIndex = finish;
	}
}

// Event Args for Collection Changed
// 0: Collection that was changed
// 1: Collection Changed Action
// 2: Collection Changed Index
// 3: Collection Changed Value
class CollectionChangedEventArgs
{
	Observable Source;
	NotifyCollectionChangedAction ChangedAction;	
	int ChangedIndex;
	Class ChangedValue;
	
	void CollectionChangedEventArgs(Observable source, NotifyCollectionChangedAction changed_action, int changed_index, Class changed_value)
	{
		Source = source; ChangedAction = changed_action; ChangedIndex = changed_index; ChangedValue = changed_value;
	}
}



// Event Args for Property Changed
// 0: Name of property changed
// 1: New property value
class PropertyChangedEventArgs: Param2<string, Class>
{
	string GetPropertyName() {
		return param1;
	}
	
	Class GetPropertyValue() {
		return param2;
	}
}

class CommandArgs
{
	ScriptedViewBase Context;
}

class CommandArgsT<Class T>: CommandArgs
{
	T Source;
	
	void CommandArgsT(T source)
	{
		Source = source;
	}
}

// 0: Source Widget
// 1: Mouse Button
class ButtonCommandArgs: CommandArgsT<ButtonWidget>
{
	private int m_MouseButton;
	
	void ButtonCommandArgs(ButtonWidget source, int mouse_button)
	{
		m_MouseButton = mouse_button;
	}
	
	int GetMouseButton() {
		return m_MouseButton;
	}
	
	bool GetButtonState() {
		return Source.GetState();
	}
}


// 0: Source Widget
// 1: CheckBox State
class CheckBoxCommandArgs: CommandArgsT<CheckBoxWidget>
{	
	bool GetCheckBoxState() {
		return Source.IsChecked();
	}
}

// 0: Source Widget
// 1: Selected Item
class XComboBoxCommandArgs: Param2<XComboBoxWidget, int>
{
	XComboBoxWidget GetComboBoxWidget() {
		return param1;
	}
	
	int GetSelectedIndex() {
		return param2;
	}
}

// 0: Source Widget
// 1: Selected Widget
class WrapSpacerCommandArgs: Param2<WrapSpacerWidget, Widget>
{
	WrapSpacerWidget GetWrapSpacerWidget() {
		return param1;
	}
	
	Widget GetSelectedWidget() {
		return param2;
	}
}

