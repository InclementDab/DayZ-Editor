static ref LayoutBindingManager g_LayoutBindingManager;

class LayoutBindingManager
{
	static int LBMLogLevel;
	
	private static void CheckLayoutBindingManager()
	{
		if (!g_LayoutBindingManager)
		{ 
			g_LayoutBindingManager = new LayoutBindingManager();
		}
	}
	
	protected static ref TypenameHashMap m_WidgetControllerHashMap;
	static WidgetController GetWidgetController(Widget data) 
	{
		CheckLayoutBindingManager();
	
		WidgetController widgetController = WidgetController.Cast(m_WidgetControllerHashMap.Get(data.Type()).Spawn());
		g_Script.Call(widgetController, "SetWidget", data);
		return widgetController;
	}	
	
	protected static ref TypeConversionHashMap m_TypeConverterHashMap;
	static TypeConverter GetTypeConversion(typename type) 
	{
		CheckLayoutBindingManager();
				
		return TypeConverter.Cast(m_TypeConverterHashMap[type].Spawn()); 
	}
	
	void LayoutBindingManager()
	{
		Log("LayoutBindingManager");
		
		if (!m_TypeConverterHashMap)
		{
			m_TypeConverterHashMap = new TypeConversionHashMap();
			RegisterConversionTemplates(m_TypeConverterHashMap);
		}
		
		if (!m_WidgetControllerHashMap)
		{
			m_WidgetControllerHashMap = new TypenameHashMap();
			RegisterWidgetwidget_controllers(m_WidgetControllerHashMap);
		}
	}
	
	void ~LayoutBindingManager() 
	{ 
		Log("~LayoutBindingManager"); 
	}
	
	// Override THIS to add your own Custom Conversion Templates
	// this determines how data is represented in the UI itself
	// i.e. you can assign a TextWidget to float, due to the TypeConversion's GetString()
	void RegisterConversionTemplates(out TypeConversionHashMap type_conversions)
	{
		Log("LayoutBindingManager::RegisterConversionTemplates");
		type_conversions.Insert(bool, TypeConversionBool);
		type_conversions.Insert(int, TypeConversionInt);
		type_conversions.Insert(float, TypeConversionFloat);
		type_conversions.Insert(string, TypeConversionString);
		type_conversions.Insert(vector, TypeConversionVector);
		type_conversions.Insert(Widget, TypeConversionWidget);
		type_conversions.Insert(Object, TypeConversionObject);
		type_conversions.Insert(Observable, TypeConversionObservable);
		type_conversions.Insert(ScriptedViewBase, TypeConversionScriptView);
	}
	
	// Override THIS to add your own Widget widget_controllers 
	// this determins how the Widget controls the data sent to it
	// Great for prefabs
	void RegisterWidgetwidget_controllers(out TypenameHashMap widget_controllers)
	{
		Log("LayoutBindingManager::RegisterWidgetwidget_controllers");
		
		widget_controllers.Insert(Widget, WidgetBaseController);
		widget_controllers.Insert(SpacerBaseWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(WrapSpacerWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(GridSpacerWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(ScrollWidget, SpacerBaseWidgetController);

		widget_controllers.Insert(ButtonWidget, ButtonWidgetController);
		widget_controllers.Insert(CheckBoxWidget, CheckBoxWidgetController);
		widget_controllers.Insert(EditBoxWidget, EditBoxWidgetController);
		widget_controllers.Insert(SliderWidget, SliderWidgetController);
		widget_controllers.Insert(ProgressBarWidget, ProgressBarController);
		widget_controllers.Insert(TextWidget, TextWidgetController);
		widget_controllers.Insert(MultilineEditBoxWidget, MultilineEditBoxWidgetController);
		widget_controllers.Insert(XComboBoxWidget, XComboBoxWidgetController);
		widget_controllers.Insert(ImageWidget, ImageWidgetController);
		widget_controllers.Insert(TextListboxWidget, TextListboxController);
		widget_controllers.Insert(RichTextWidget, TextWidgetController);
		widget_controllers.Insert(ItemPreviewWidget, ItemPreviewWidgetController);
	}	
	
	static void Trace(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (LBMLogLevel <= 0)
			PrintFormat("LayoutBindingManagerLog::Trace %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}
	
	static void Log(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (LBMLogLevel <= 1)
			PrintFormat("LayoutBindingManagerLog::Log %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Error(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		string msg = string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		//PrintFormat("LayoutBindingManagerLog::Error %1", msg);
		Error2("LayoutBindingManager Error", msg);
		
#ifdef COMPONENT_SYSTEM
		Workbench.Dialog("LayoutBindingManager Error", msg);
#endif
	}
};

// Workbench safe variant of GetGame()
// https://feedback.bistudio.com/T153287
DayZGame GetWorkbenchGame() 
{
	// dont want to call this while actually in game
#ifdef COMPONENT_SYSTEM 
	if (!g_Game) {
		g_Game = new DayZGame();
	}
#endif
	
	return g_Game;
}