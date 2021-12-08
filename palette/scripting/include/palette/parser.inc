stock SMCParser create_palette_parser()
{
    SMCParser parser;
    if(!(parser = new SMCParser()))
        return parser;

    parser.OnKeyValue = palette_parser_kv;
    parser.OnEnd = palette_parser_end;
    return parser;
}

stock SMCResult palette_parser_kv(SMCParser smc, const char[] sKey, const char[] sValue, bool bKey_Quotes, bool bValue_quotes)
{
    if(!sKey[0] || !sValue[0])
        return SMCParse_Continue;

    int iBuffer;

    char szBuffer[16];
    strcopy(szBuffer, sizeof(szBuffer), sValue);
    
    iBuffer = strlen(szBuffer);
    
    szBuffer[0] =   (szBuffer[0] == '#')
                        ? (iBuffer == 7) 
                            ? 7 
                            : 8
                        : StringToInt(szBuffer); 
    if(iBuffer < 3) 
        szBuffer[1] = 0;
    
    GET_PALETTE().PushString(sKey);
    GET_PALETTE().PushString(szBuffer);

    return SMCParse_Continue;
}

stock void palette_parser_end(SMCParser smc, bool halted, bool failed)
{
    delete smc;

    if(halted || failed)
        SetFailState("There was a problem reading the configuration file");
}