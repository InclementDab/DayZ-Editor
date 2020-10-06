
// Base class for all CommandArgs
class CommandArgs
{
	ScriptedViewBase Context;
};

class CommandArgsT<Class T>: CommandArgs
{
	T Source;
	
	void CommandArgsT(T source)
	{
		Source = source;
	}
};

// 0: Source Widget
// 1: Mouse Button
class ButtonCommandArgs: CommandArgsT<ButtonWidget>
{
	private int m_MouseButton;
	
	void ButtonCommandArgs(ButtonWidget source, int mouseButton)
	{
		m_MouseButton = mouseButton;
	}
	
	int GetMouseButton()
	{
		return m_MouseButton;
	}
	
	bool GetButtonState()
	{
		return Source.GetState();
	}
};


// 0: Source Widget
// 1: CheckBox State
class CheckBoxCommandArgs: CommandArgsT<CheckBoxWidget>
{	
	bool GetCheckBoxState()
	{
		return Source.IsChecked();
	}
};

// 0: Source Widget
// 1: Selected Item
class XComboBoxCommandArgs: Param2<XComboBoxWidget, int>
{
	XComboBoxWidget GetComboBoxWidget()
	{
		return param1;
	}
	
	int GetSelectedIndex()
	{
		return param2;
	}
};

// 0: Source Widget
// 1: Selected Widget
class WrapSpacerCommandArgs: Param2<WrapSpacerWidget, Widget>
{
	WrapSpacerWidget GetWrapSpacerWidget()
	{
		return param1;
	}
	
	Widget GetSelectedWidget()
	{
		return param2;
	}
};
