class EditorInventoryFluidSlotController: ViewController
{
	EditorInventoryFluidSlot Parent;
	
	float Max;
	
	string Name;
	CarFluid FluidType;
	float Value;
	float CalculatedValue;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Value": {
				CalculatedValue = Math.Clamp(Math.Lerp(0, Max, Value), 0, Max);
				NotifyPropertyChanged("CalculatedValue", false);
				// Inform the gamers the fluid has changed
				Parent.OnValueChanged.Invoke(FluidType, CalculatedValue);
				break;
			}
			
			case "CalculatedValue": {
				Value = Math.InverseLerp(0, Max, CalculatedValue);
				NotifyPropertyChanged("Value");
				break;
			}
		}
	}
}