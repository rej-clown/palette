// Authored by rej.chev 

#if defined _palette_included
 #endinput
#endif
#define _palette_included

stock ArrayList palette;

// Передача своего объекта с палитрой
// Убежитесь, что текущей == null
#define SET_PALETTE(%0)             palette = %0

// Получение объекта палитры
// нечет - аббревиатуры
// чет - коды
#define GET_PALETTE()               palette

stock UserMessageType umType;
#define SET_USERMESSAGE_TYPE(%0)    umType = %0
#define GET_USERMESSAGE_TYPE()      umType

stock char palettePath[PLATFORM_MAX_PATH] = "configs/palette/";

// Установить свой путь до палитры цветов
#define SET_PALETTE_PATH(%0)        FormatEx(palettePath, sizeof(palettePath), "%s", %0);

// Текущий путь до палитры цветов
#define GET_PALETTE_PATH()          palettePath

#include "palette/parser.inc"
#include "palette/logic.inc"

/*  Запуск логики работы палитры: PARSE_PALETTE()
    Обязательно должно присутствовать в любом плагине

    Может быть помещено в события типа (на выбор): OnPluginStart(), OnMapStart(), OnConfigsExecuted() ...
*/
#define PARSE_PALETTE()             parse_palette(GET_PALETTE_PATH(), sizeof(GET_PALETTE_PATH()))
#define PARSE_PALETTE_EX(%0)        SET_PALETTE_PATH(%0); \
                                    PARSE_PALETTE()

// Замена абберивиатур цветов на коды
#define ACTIVATE_COLORS(%0,%1)      replace_colors_palette(%0,%1)

// Очистка строки от аббревиатур и кодов
#define CLEAN_COLORS(%0,%1)         replace_colors_palette(%0,%1, true)

stock void PrintToChat_palette(int recipient, const char[] p, any ...) {
    char text[512];

    SetGlobalTransTarget(recipient);
    VFormat(text, sizeof(text), p, 3);
    Format(text, sizeof(text), "%c %s", 1, text);

    ACTIVATE_COLORS(text, sizeof(text));

    ArrayList texts = new ArrayList(512, 0);
    texts.PushString(text);

    int rps[1];
    rps[0] = recipient;

    if(prepare_usermessage_palette(0, rps, 1, texts))
        delete texts;
}

stock void PrintToChatAll_palette(const char[] p, any ...) {
    int rpsCount;
    char text[512];
    int rps[MAXPLAYERS+1];
    ArrayList texts = new ArrayList(512, 0);

    for(int i; i <= MaxClients; i++) {
        if(!IsClientInGame(i) || (IsFakeClient(i) && !IsClientSourceTV(i)))
            continue;
        
        SetGlobalTransTarget(i);
        VFormat(text, sizeof(text), p, 2);
        Format(text, sizeof(text), "%c %s", 1, text);

        ACTIVATE_COLORS(text, sizeof(text));

        texts.PushString(text);
        rps[rpsCount++] = i;
    }

    if(prepare_usermessage_palette(0, rps, rpsCount, texts))
        delete texts;
}