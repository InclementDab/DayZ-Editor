

class BrushBase: ItemBase
{

	void BrushBase()
	{
		Print("BrushBase");
	}
	
	
	void SetTexture(string texture = "dayzeditor\\editor\\data\\brushdelete.paa")
	{
		Print("BrushBase::SetTexture");
		SetObjectTexture(GetHiddenSelectionIndex("BrushBase"), texture);
		Update();
	}

}