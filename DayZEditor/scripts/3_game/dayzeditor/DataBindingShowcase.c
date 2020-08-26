//Print(__LINE__);
//Print(__FILE__); // useful shit // tools.pak?????


class DataBindingShowcaseController: Controller
{
	
	//EquationEvaluater WindowText = "(10 / 2) + 7";
	bool WindowText;
	float WindowSlider = 10;
	
	bool EvaluateEquation;
	
	bool WindowTextToggleValue;
	
	//ref ObservableCollection<string> WindowListWidgets;
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
	
		/*
		WindowListWidgets = new ObservableCollection<string>("WindowListWidgets", this);
		WindowListWidgets.Insert("Test1");
		WindowListWidgets.Insert("Test2");
		WindowListWidgets.Remove(1);
		WindowListWidgets.Insert("Test3");*/
		
		WindowText = true;
				
	}
	
	override void PropertyChanged(string property_name)
	{
		Print(property_name);
		Print(WindowText);
		if (property_name == "WindowTextToggleValue") {
			WindowText = !WindowText;
			NotifyPropertyChanged("WindowText");
		}
		
		/*
		if (property_name == "EvaluateEquation") {
			WindowText.Evaluate();
			NotifyPropertyChanged("WindowText");
		}*/
	}
	

}


// Example for defining a custom type
modded class MVC
{
	override void RegisterConversionTemplates(out TypeConverterHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(EquationEvaluater, TypeConversionString);
		

	}
}



typedef string EquationEvaluater;
class EquationEvaluater: string
{
	void Evaluate()
	{
		StringEvaluater evaluater();
		value = evaluater.Parse(value).ToString();
	}
}


