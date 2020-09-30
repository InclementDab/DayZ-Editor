class XMLTag : Managed
{
    private string _name;

    private autoptr map< string, ref XMLAttribute > _attributes;

    private ref XMLElement _element;

    private XMLElement _parentElement;

    void XMLTag( ref XMLElement parent, string name, bool isCopy = false )
    {
        _attributes = new map< string, ref XMLAttribute >;
        _parentElement = parent;
        _name = name;

        if ( !isCopy )
            _element = new XMLElement( this );
    }

    void ~XMLTag()
    {
        for ( int i = 0; i < _attributes.Count(); ++i )
        {
            delete _attributes.GetElement( i );
        }

        delete _attributes;

        delete _element;
    }

    ref XMLTag Copy( ref XMLElement parent = NULL )
    {
        ref XMLTag tag = new XMLTag( parent, _name );

        for ( int i = 0; i < _attributes.Count(); ++i )
        {
            ref XMLAttribute attrib = _attributes.GetElement( i ).Copy( tag );
            tag._attributes.Insert( attrib.GetName(), attrib );
        }

        tag._element = _element.Copy( tag );

        return tag;
    }

    string GetName()
    {
        return _name;
    }

    ref XMLTag CreateTag( string name )
    {
        return _element.CreateTag( name );
    }

    ref XMLAttribute CreateAttribute( string name )
    {
        XMLAttribute attrb = new XMLAttribute( this, name );

        _attributes.Insert( name, attrb );

        return attrb;
    }

    ref XMLAttribute GetAttribute( string name )
    {
        return _attributes.Get( name );
    }

    XMLElement GetContent()
    {
        return _element;
    }

    XMLElement GetParent()
    {
        return _parentElement;
    }

    void Debug( int level = 0 )
    {
        string indent = CF_Indent( level );

        Print( indent + "Tag:" );
        Print( indent + " name=" + _name );

        Print( indent + "Attributes: count=" + _attributes.Count() );
        for ( int i = 0; i < _attributes.Count(); ++i )
        {
            _attributes.GetElement( i ).Debug( level );
        }

        Print( indent + "Element:" );
        _element.Debug( level + 1 );
    }

    void OnWrite( FileHandle handle, int depth )
    {
        string indent = CF_XML_Indent( depth );

        FPrint( handle, indent );
        FPrint( handle, "<" );
        FPrint( handle, _name );

        if ( _attributes.Count() > 0 )
        {
            FPrint( handle, " " );
            
            for ( int i = 0; i < _attributes.Count(); ++i )
            {
                _attributes.GetElement( i ).OnWrite( handle, depth );
            }
        }

        if ( _element.Count() > 0 )
        {
            FPrint( handle, ">\n" );

            _element.OnWrite( handle, depth + 1 );

            FPrint( handle, indent );
            FPrint( handle, "<" );
            FPrint( handle, _name );
            FPrint( handle, " " );
        } else if ( _attributes.Count() == 0 )
        {
            FPrint( handle, " " );
        }

        FPrint( handle, "/>\n" );
    }
};