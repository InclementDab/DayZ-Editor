class EditorInventoryFluidSlot: ScriptViewTemplate<EditorInventoryFluidSlotController>
{
	ref ScriptInvoker OnValueChanged = new ScriptInvoker();
	
	SliderWidget FluidSlider;
	
	void EditorInventoryFluidSlot(CarFluid fluid, float max, float current)
	{
		m_TemplateController.Max = max;
		m_TemplateController.Parent = this;
		
		m_TemplateController.Name = typename.EnumToString(CarFluid, fluid);
		m_TemplateController.NotifyPropertyChanged("Name");
		
		m_TemplateController.FluidType = fluid;
		m_TemplateController.NotifyPropertyChanged("FluidType");
		
		// 0..1 due to GetFluidFraction()
		m_TemplateController.Value = current;
		m_TemplateController.NotifyPropertyChanged("Value");
		
		FluidSlider.SetColor(GetFluidColor(fluid));
	}
	
	static int GetFluidColor(CarFluid fluid)
	{
		switch (fluid) {
			case (CarFluid.FUEL): 		return COLOR_YELLOW;
			case (CarFluid.BRAKE): 		return COLOR_SALMON;
			case (CarFluid.OIL):		return COLOR_BLACK;
			case (CarFluid.COOLANT): 	return COLOR_BLUE;
		}
		
		return -1;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\items\\EditorInventoryFluids.layout";
	}
}