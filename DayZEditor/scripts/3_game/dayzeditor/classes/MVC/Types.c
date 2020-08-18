
class StringEvaluater
{
	int pos = -1;
	int ch;
	string value;
	
	float Parse(string _value)
	{
		pos = -1;
		value = _value;
		NextChar();
		float x = ParseExpression();
		if (pos < value.Length()) Error("Unexpected: " + ch);
		return x;
	}
	
	void NextChar() 
	{
		pos++;
		if (pos < value.Length()) {
			ch = value.Get(pos).Hash();
		} else {
			ch = -1;
		}
	}
	
	bool Eat(int charToEat) 
	{
	    while (ch == 32) NextChar();
	    if (ch == charToEat) {
	        NextChar();
	        return true;
	    }
	    return false;
	}
	
	float ParseExpression() 
	{
	    float x = ParseTerm();
	    while (!false) {
	        if      (Eat("+".Hash())) x += ParseTerm(); // addition
	        else if (Eat("-".Hash())) x -= ParseTerm(); // subtraction
	        else return x;
	    }
		
		return x;
	}
	
	float ParseTerm() 
	{
	    float x = ParseFactor();
	    while (true) {
	        if      (Eat("*".Hash())) x *= ParseFactor(); // multiplication
	        else if (Eat("/".Hash())) x /= ParseFactor(); // division
	        else return x;
	    }
		
		return x;
	}
	
	float ParseFactor() 
	{
	    if (Eat("+".Hash())) return ParseFactor(); // unary plus
	    if (Eat("-".Hash())) return -ParseFactor(); // unary minus
	
	    float x;
	    int startPos = pos;
	    if (Eat("(".Hash())) { // parentheses
	        x = ParseExpression();
	        Eat(")".Hash());
	    } else if ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) { // numbers
	        while ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) 
				NextChar();

	        x = (value.Substring(startPos, pos - startPos)).ToFloat();
	    } else if (ch >= "a".Hash() && ch <= "z".Hash()) { // functions
	        while (ch >= "a".Hash() && ch <= "z".Hash()) NextChar();
	        string fnc = value.Substring(startPos, pos - startPos);
	        x = ParseFactor();
	        if (fnc == "sqrt") x = Math.Sqrt(x);
	        else if (fnc == "sin") x = Math.Sin(x * Math.DEG2RAD);
	        else if (fnc == "cos") x = Math.Cos(x * Math.DEG2RAD);
	        else if (fnc == "tan") x = Math.Tan(x * Math.DEG2RAD);
	        else Error("Unknown function: " + fnc);
	    } else {
	        Error("Unexpected: " + ch);
	    }
	
	    if (Eat("^".Hash())) x = Math.Pow(x, ParseFactor()); // exponentiation
	
	    return x;
	}
}



// TextWidget
typedef string TextWidgetData;
class TextWidgetData: string {}

// EditBoxWidget
typedef string EditBoxWidgetData;
class EditBoxWidgetData: string {}

// EditBoxWidget
typedef string EditBoxWidgetDataF;
class EditBoxWidgetDataF: string 
{
	
	
	string GetValidString()
	{
		string final;
		for (int i = 0; i < value.Length(); i++) {
			string char = value.Get(i);
			int ascii = char.Hash();
			if ((ascii >= 45 && ascii <= 57) || ascii == 43 || ascii == 42 || ascii == 32) {
				final += char;
			}
		}

		return final;
	}
	
	
	float EvaluateExpression()
	{
		
	}
	
	 // Grammar:
	// expression = term | expression `+` term | expression `-` term
	// term = factor | term `*` factor | term `/` factor
	// factor = `+` factor | `-` factor | `(` expression `)`
	//        | number | functionName factor | factor `^` factor
	
	
}

// MultilineEditBoxWidget
typedef TStringArray MultilineEditBoxWidgetData;
class MultilineEditBoxWidgetData: TStringArray {}

// ButtonWidget
typedef bool ButtonWidgetData;
class ButtonWidgetData: bool {}

// CheckBoxWidget
typedef bool CheckBoxWidgetData;
class CheckBoxWidgetData: bool {}

// SliderWidget
typedef float SliderWidgetData;
class SliderWidgetData: float {}

// Interface POG
class IWidgetData
{
	private Class m_Parent;
	void IWidgetData(Class parent)
	{
		m_Parent = parent;
	}
	
	void Add(Widget w, Param params)
	{
		EditorLog.Trace("IWidgetData::Add");
		GetGame().GameScript.Call(m_Parent, "_AddData", new Param2<Widget, Param>(w, params));
	}
	
	void Remove(Widget w, Param params)
	{
		EditorLog.Trace("IWidgetData::Remove");
		GetGame().GameScript.Call(m_Parent, "_RemoveData", new Param2<Widget, Param>(w, params));
	}
	
	void Set(Widget w, Param params)
	{
		EditorLog.Trace("IWidgetData::Set");
		GetGame().GameScript.Call(m_Parent, "_SetData", new Param2<Widget, Param>(w, params));
	}
	
	void Reload(Widget w)
	{
		EditorLog.Trace("IWidgetData::Reload");
		GetGame().GameScript.Call(m_Parent, "_ReloadData", new Param1<Widget>(w));
	}
	
	void Clear(Widget w)
	{
		EditorLog.Trace("IWidgetData::Clear");
		GetGame().GameScript.Call(m_Parent, "_ClearData", new Param1<Widget>(w));
	}
}


// TextListboxWidget
typedef Param2<string, Class> TStringClassPair;
class TextListboxWidgetData: ObservableCollection<TStringClassPair>
{
	
	// Param1: Index [int]
	// Param2: TStringClassPair [Param2<string, Class>]
	void AddData(Widget w, Param params)
	{
		TStringClassPair insert_params = TStringClassPair.Cast(Param2<int, TStringClassPair>.Cast(params).param2);
		TextListboxWidget.Cast(w).AddItem(insert_params.param1, insert_params.param2, 0);
	}
	
	
	void RemoveData(Widget w, Param params)
	{
		TextListboxWidget.Cast(w).RemoveRow(Param2<int, TStringClassPair>.Cast(params).param1);
	}
	
	void SetData(Widget w, Param params)
	{
		Param2<int, TStringClassPair> collection_params = Param2<int, TStringClassPair>.Cast(params);
		TStringClassPair insert_params = TStringClassPair.Cast(collection_params.param2);
		TextListboxWidget.Cast(w).SetItem(collection_params.param1, insert_params.param1, insert_params.param2, 0);
	}
	
	void ReloadData(Widget w)
	{
		ClearData(w);
		for (int i = 0; i < Count(); i++) {
			TStringClassPair data = Get(i);
			TextListboxWidget.Cast(w).AddItem(data.param1, data.param2, 0);
		}
	}


	void ClearData(Widget w)
	{
		TextListboxWidget.Cast(w).ClearItems();
	}
	
	void _AddData(Param2<Widget, Param> params) { AddData(params.param1, params.param2); }
	void _RemoveData(Param2<Widget, Param> params) { RemoveData(params.param1, params.param2); }	
	void _SetData(Param2<Widget, Param> params) { SetData(params.param1, params.param2); }
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

// WrapSpacerWidget
class WrapSpacerWidgetData: ObservableCollection<Widget>
{
	// Param1: Index
	// Param2: Widget
	void AddData(Widget w, Param params)
	{
		w.AddChild(Param2<int, Widget>.Cast(params).param2);
	}
	
	void RemoveData(Widget w, Param params)
	{
		w.RemoveChild(Param2<int, Widget>.Cast(params).param2);
	}
	
	
	void SetData(Widget w, Param params)
	{
		WrapSpacerWidget widget = WrapSpacerWidget.Cast(w);
		Param2<int, Widget> collection_params = Param2<int, Widget>.Cast(params);
		
		widget.RemoveChild(this[collection_params.param1]);
		widget.Update();
		widget.AddChildAfter(collection_params.param2, this[collection_params.param1 - 1]);
		widget.Update();
	}
	
	void ReloadData(Widget w)
	{	
		ClearData(w);
		for (int i = 0; i < Count(); i++) {
			w.AddChild(Get(i));
		}
	}
	
	void ClearData(Widget w)
	{
		Widget child = w.GetChildren();
		while (child != null) {
			w.RemoveChild(child);
			child = child.GetSibling();
		}
	}
	
	void _AddData(Param2<Widget, Param> params) { AddData(params.param1, params.param2); }
	void _RemoveData(Param2<Widget, Param> params) { RemoveData(params.param1, params.param2); }	
	void _SetData(Param2<Widget, Param> params) { SetData(params.param1, params.param2); }
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

// XComboBoxWidget
class XComboBoxWidgetData: ObservableCollection<string> 
{
	// Param1: index
	// Param2: String
	void AddData(Widget w, Param params)
	{
		XComboBoxWidget.Cast(w).AddItem(Param2<int, string>.Cast(params).param2);
	}
	
	void RemoveData(Widget w, Param params)
	{		
		XComboBoxWidget.Cast(w).RemoveItem(Param2<int, string>.Cast(params).param1);
	}
	
	void SetData(Widget w, Param params)
	{
		
	}
	
	void ReloadData(Widget w)
	{
		ClearData(w);
		for (int i = 0; i < Count(); i++) 
			XComboBoxWidget.Cast(w).AddItem(Get(i));
	}
	
	void ClearData(Widget w)
	{
		XComboBoxWidget.Cast(w).ClearAll();
	}
	
	void _AddData(Param2<Widget, Param> params) { AddData(params.param1, params.param2); }
	void _RemoveData(Param2<Widget, Param> params) { RemoveData(params.param1, params.param2); }	
	void _SetData(Param2<Widget, Param> params) { SetData(params.param1, params.param2); }
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

