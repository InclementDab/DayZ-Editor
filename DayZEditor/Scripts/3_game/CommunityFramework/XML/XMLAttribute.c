class XMLAttribute : Managed
{
    private string _name;
    private string _value;

    private XMLTag _parentTag;

    void XMLAttribute( ref XMLTag parent, string name )
    {
        _parentTag = parent;
        _name = name;
        _value = "";
    }

    ref XMLAttribute Copy( ref XMLTag parent = NULL )
    {
        ref XMLAttribute element = new XMLAttribute( parent, _name );

        element._value = _value;

        return element;
    }

    string GetName()
    {
        return _name;
    }

    void SetValue( string value )
    {
        _value = value;
    }

    void SetValue( bool value )
    {
        if ( value )
        {
            _value = "true";
        } else
        {
            _value = "false";
        }
    }

    void SetValue( int value )
    {
        _value = "" + value;
    }

    void SetValue( float value )
    {
        _value = "" + value;
    }

    void SetValue( vector value )
    {
        _value = "" + value[0] + " " + value[1] + " " + value[2];
    }

    string GetValue()
    {
        return _value;
    }

    string ValueAsString()
    {
        return _value;
    }

    bool ValueAsBool()
    {
        if ( _value == "true" )
            return true;

        return false;
    }

    int ValueAsInt()
    {
        return _value.ToInt();
    }

    float ValueAsFloat()
    {
        return _value.ToFloat();
    }

    vector ValueAsVector()
    {
        array<string> tokens = new array<string>;
        _value.Split( " ", tokens );

        return Vector( tokens[0].ToFloat(), tokens[1].ToFloat(), tokens[2].ToFloat() );
    }

    XMLTag GetParent()
    {
        return _parentTag;
    }

    void Debug( int level = 0 )
    {
        string indent = CF_Indent( level );
        Print( indent + " name=" + _name + " value=" + _value );
    }

    void OnWrite( FileHandle handle, int depth )
    {
        FPrint( handle, _name );
        FPrint( handle, "=\"" );
        FPrint( handle, _value );
        FPrint( handle, "\" " );
    }
};