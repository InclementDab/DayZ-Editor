

class BrushBase: Building
{

	void BrushBase()
	{
		Print("BrushBase");
	}
	
	
	void SetTexture(string texture = "\DayZEditor\Editor\data\BrushBase.paa")
	{
		Print("BrushBase::SetTexture");
		SetObjectTexture(GetHiddenSelectionIndex("BrushBase"), texture);
		Update();
	}

}