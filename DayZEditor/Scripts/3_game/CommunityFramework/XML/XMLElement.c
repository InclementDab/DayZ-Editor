class XMLElement : Managed
{
    protected autoptr array< ref XMLTag > _tags;

    protected XMLTag _parentTag;

    protected string _data;
    
    void XMLElement( ref XMLTag parent = NULL )
    {
        _tags = new array< ref XMLTag >;
        _parentTag = parent;
    }

    void ~XMLElement()
    {
        for ( int i = 0; i < _tags.Count(); ++i )
        {
            delete _tags[i];
        }

        delete _tags;
    }

    ref XMLElement Copy( ref XMLTag parent = NULL )
    {
        ref XMLElement element = new XMLElement( parent );

        for ( int i = 0; i < _tags.Count(); ++i )
        {
            element._tags.Insert( _tags[i].Copy( element ) );
        }

        element._data = "" + _data;

        return element;
    }

    ref XMLTag CreateTag( string name )
    {
        ref XMLTag tag = new XMLTag( this, name );

        _tags.Insert( tag );

        return tag;
    }
    
    int Count()
    {
        return _tags.Count();
    }

    ref XMLTag Get( int index )
    {
        return _tags[index];
    }

    void Remove( ref XMLTag tag )
    {
        int index = _tags.Find( tag );
        if ( index >= 0 )
            _tags.Remove( index );
    }

    array< XMLTag > Get( string type )
    {
        array< XMLTag > tags = new array< XMLTag >;

        for ( int i = 0; i < _tags.Count(); ++i )
        {
            if ( _tags[i].GetName() == type )
            {
                tags.Insert( _tags[i] );
            }
        }

        return tags;
    }

    void SetContent( string data )
    {
        _data = data;
    }

    string GetContent()
    {
        return _data;
    }

    XMLTag GetParent()
    {
        return _parentTag;
    }

    void Debug( int level = 0 )
    {
        string indent = CF_Indent( level );

        Print( indent + "Tags: count=" + _tags.Count() );
        for ( int i = 0; i < _tags.Count(); ++i )
        {
            _tags[i].Debug( level + 1 );
        }
    }

    void OnWrite( FileHandle handle, int depth )
    {
        for ( int i = 0; i < _tags.Count(); ++i )
        {
            _tags[i].OnWrite( handle, depth );
        }
    }
};