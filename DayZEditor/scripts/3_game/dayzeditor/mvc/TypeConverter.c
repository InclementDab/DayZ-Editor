


class TypeConverterHashMap
{
	private ref map<typename, typename> value;
	
	void TypeConverterHashMap()
	{
		EditorLog.Trace("TypeConverterHashMap");
		value = new map<typename, typename>();
	}
	
	void ~TypeConverterHashMap() { EditorLog.Trace("~TypeConverterHashMap"); }
	
	typename Get(typename conversion_type)
	{
		return value.Get(conversion_type);
	}
	
	void Remove(typename conversion_type)
	{
		value.Remove(conversion_type);
	}
	
	void Set(typename conversion_type, typename conversion_class)
	{
		EditorLog.Trace("TypeConverterHashMap::Set");
		
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.ErrorDialog(string.Format("RegisterTypeConversion: %1 must inherit from type TypeConversionTemplate", conversion_class));
			return;
		}
		
		value.Set(conversion_type, conversion_class);
	} 
	
	bool Insert(typename conversion_type, typename conversion_class)
	{
		EditorLog.Trace("TypeConverterHashMap::Insert");
		
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.ErrorDialog(string.Format("RegisterTypeConversion: %1 must inherit from type TypeConversionTemplate", conversion_class));
			return false;
		}
		
		return value.Insert(conversion_type, conversion_class);
	}
}

class TypeConverter
{			
	void TypeConverter() { EditorLog.Trace("TypeConverter"); }
	void ~TypeConverter() { EditorLog.Trace("~TypeConverter"); }
	typename GetType();
	
	bool GetBool();
	int GetInt();
	float GetFloat();
	string GetString();
	vector GetVector();
	Widget GetWidget();
	Param GetParam();
	
	void SetBool(bool value);
	void SetInt(int value);
	void SetFloat(float value);
	void SetString(string value);
	void SetVector(vector value);
	void SetWidget(Widget value);
	void SetParam(Param value);
	
	void SetToController(Class context, string name, int index);
	void GetFromController(Class context, string name, int index);
}


class TypeConversionTemplate<Class T>: TypeConverter
{
	protected T m_Value;
	
	void SetData(T value) {
		EditorLog.Trace("TypeConversionTemplate::SetData");
		m_Value = value;
	}
	
	T GetData() {
		EditorLog.Trace("TypeConversionTemplate::GetData");
		return m_Value;
	}
	
	override void SetParam(Param value) {
		m_Value = Param1<T>.Cast(value).param1;
	}
	
	override Param GetParam() {
		return new Param1<T>(m_Value);
	}
			
	override typename GetType() {
		return T;
	}
		
	override void SetToController(Class context, string name, int index) {
		EnScript.SetClassVar(context, name, index, m_Value);
	}
	
	override void GetFromController(Class context, string name, int index) {
		EnScript.GetClassVar(context, name, index, m_Value);
	}
}

class TypeConversionBool: TypeConversionTemplate<bool>
{
	override bool GetBool() {
		return m_Value;
	}
	
	override void SetBool(bool value) {
		m_Value = value;
	}
	
	override int GetInt() {
		return m_Value;
	}
	
	override void SetInt(int value) {
		m_Value = value;
	}
	
	override float GetFloat() {
		return m_Value;
	}
	
	override void SetFloat(float value) {
		m_Value = value;
	}
	
	override string GetString() {
		return m_Value.ToString();
	}
	
	override void SetString(string value) {
		m_Value = (value == "1");
	}
}

class TypeConversionInt: TypeConversionTemplate<int>
{
	override bool GetBool() {
		return m_Value;
	}
	
	override void SetBool(bool value) {
		m_Value = value;
	}
	
	override int GetInt() {
		return m_Value;
	}
	
	override void SetInt(int value) {
		m_Value = value;
	}
	
	override float GetFloat() {
		return m_Value;
	}
	
	override void SetFloat(float value) {
		m_Value = value;
	}
	
	override string GetString() {
		return string.ToString(m_Value, false, false, false);
	}
	
	override void SetString(string value) {
		m_Value = value.ToInt();
	}
}

class TypeConversionFloat: TypeConversionTemplate<float>
{
	override bool GetBool() {
		return m_Value;
	}
	
	override void SetBool(bool value) {
		m_Value = value;
	}
	
	override int GetInt() {
		return m_Value;
	}
	
	override void SetInt(int value) {
		m_Value = value;
	}
	
	override float GetFloat() {
		return m_Value;
	}
	
	override void SetFloat(float value) {
		m_Value = value;
	}
	
	override string GetString() {
		return string.ToString(m_Value, false, false, false);
	}
	
	override void SetString(string value) {
		m_Value = value.ToFloat();
	}
}

class TypeConversionString: TypeConversionTemplate<string>
{	
	override bool GetBool() {
		return string.ToString(m_Value, false, false, false) == "1";
	}
		
	override void SetBool(bool value) {
		m_Value = string.ToString(value, false, false, false);
	}
	
	override int GetInt() {
		return m_Value.ToInt();
	}
	
	override void SetInt(int value) {
		m_Value = string.ToString(value, false, false, false);
	}
	
	override float GetFloat() {
		return m_Value.ToFloat();
	}
	
	override void SetFloat(float value) {
		m_Value = string.ToString(value, false, false, false);
	}
	
	override vector GetVector() {
		return m_Value.ToVector();
	}
	
	override void SetVector(vector value) {
		m_Value = value.ToString(false);
	}
	
	override string GetString() {
		return m_Value;
	}
	
	override void SetString(string value) {
		m_Value = value;
	}
}

class TypeConversionVector: TypeConversionTemplate<vector>
{
	override vector GetVector() {
		return m_Value;
	}
	
	override void SetVector(vector value) {
		m_Value = value;
	}
	
	override string GetString() {
		return m_Value.ToString(false);
	}
	
	override void SetString(string value) {
		m_Value = value.ToVector();
	}
}

class TypeConversionWidget: TypeConversionTemplate<Widget>
{
	override void SetWidget(Widget value) {
		m_Value = value;
	}
	
	override Widget GetWidget() {
		return m_Value;
	}
}
