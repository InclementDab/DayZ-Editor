class XMLDocument : XMLElement
{
    private XMLTag _currentTag;

    private ref XMLReader _reader;

    override ref XMLTag CreateTag( string name )
    {
        if ( _currentTag )
        {
            _currentTag = _currentTag.CreateTag( name );
        } else
        {
            _currentTag = super.CreateTag( name );
        }

        return _currentTag;
    }

    ref XMLDocument CopyDocument()
    {
        ref XMLDocument document = new XMLDocument( NULL );

        for ( int i = 0; i < _tags.Count(); ++i )
        {
            document._tags.Insert( _tags[i].Copy( this ) );
        }

        return document;
    }

    private bool ReadAttribute()
    {
        if ( !_currentTag )
        {
            Error( "Attempting to read attribute when not in tag!" );
            return false;
        }

        bool isQuoted;
        string name = _reader.GetWord();
        string eq = _reader.GetCharacter();
        string value = _reader.GetQuotedWord( isQuoted );

        if ( eq != "=" )
        {
            Error( "Expected '=' for attribute, got: " + eq );
            return false;
        }

        XMLAttribute attrib = _currentTag.CreateAttribute( name );
        attrib.SetValue( value );

        return true;
    }

    private bool PopTag()
    {
        if ( !_currentTag )
            return true;

        if ( _currentTag.GetParent() )
            _currentTag = _currentTag.GetParent().GetParent();
        else
            _currentTag = NULL;

        return true;
    }

    private bool ReadTag()
    {
        string c = _reader.SkipWhitespace();
        string tagName = "";

        if ( c == "<" )
        {
            c = _reader.GetCharacter();
            if ( c == "/" )
            {
                tagName = _reader.GetWord();

                if ( _currentTag == NULL )
                {
                    Error( "Unexpected closing tag: " + tagName );
                }

                c = _reader.GetCharacter();
                if ( c != ">" )
                {
                    Error( "Expected '>' for closing tag, got: " + c );
                }

                PopTag();

                return true;
            }

            if ( c != "?" )
                _reader.BackChar();

            tagName = _reader.GetWord();

            CreateTag( tagName );

            c = _reader.SkipWhitespace();
            while ( c != "/" && c != "?" && c != ">" )
            {
                c = _reader.BackChar();
                
                if ( !ReadAttribute() )
                    return false;

                c = _reader.SkipWhitespace();
            }

            if ( c == "/" || c == "?" )
            {
                string expected = c;
                c = _reader.GetCharacter();
                if ( c != ">" )
                {
                    Error( "Expected '" + expected + "' for inline closing tag, got: " + c );
                }

                PopTag();
            } else if ( c == ">" )
            {

            } else
            {
                Error( "Expected '>' for opening element within tag, got: " + c );
            }

            return true;
        }

        // could just be non-tagged content.
        
        return true;
    }

    bool Read( XMLReader reader )
    {
        _reader = reader;

        while ( !_reader.EOF() )
        {
            if ( !ReadTag() )
            {
                delete _reader;
                return false;
            }
        }

        delete _reader;
        return true;
    }

    void Save( string path )
    {
        ThreadWrite( path );
		//GetGame().GameScript.Call( this, "ThreadWrite", path );
    }

    private void ThreadWrite( string path )
    {
        Print( "ThreadWrite" );
        FileHandle handle = OpenFile( path, FileMode.WRITE );

        Print( handle );
		if ( handle != 0 )
		{
			OnWrite( handle, 0 );
			
			CloseFile( handle );
		}
    }
};