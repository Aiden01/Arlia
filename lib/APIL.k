`
	+----------+
	|          |
	|   APIL   |
	|	   |
	+----------+
`

define BYTE        1;
define WORD        2;
define DWORD       4;
define FWORD       6;
define QWORD       8;
define TWORD       10;
define DQWORD      16;
define QQWORD      32;
define ZWORD       64;

define object      instance structure;          ` an 'object' (keyword) is an instancied structure `
define rule        instance ret;                ` a 'rule' (keyword) is an instancied constant lambda function expression `
define as          : ;                          ` define a keyword to simplify type assignement `
proc   "_asm"(asm) extern asm;
define "breakpoint" _asm("int 3");

define nullchr     '\0';
define nullptr     0;

rule true : BYTE = 1;                           ` define 'true' as object `
rule false : BYTE = 0;                          ` define 'false' as object `

rule numeric : TWORD = (-0xFFFFFFFf to 0xFFFFFFFf); ` 'f' is 'float' format `
rule alphabetic : BYTE = (('a' to 'z') ^ ('A' to 'Z'));
rule signed : TWORD = (0f to 0xFFFFFFFf);
rule <what> exists = (&what != nullptr);

rule UpperCase = ('A' to 'Z');
rule LowerCase = ('a' to 'z');

func AddressOf(&obj); ` todo `

object <> char;
object <> string;
object <> integer;
object <> float;
object <> real;
object <> boolean;

object <typesize = BYTE> char {
    private rule AbstractCorrespondence : typesize = (-128c to 127c);
    private var value : AbstractCorrespondence = nullchr;
    private func SetValue(val : typesize);
    public func ToString() : string;
    public func ToInt() : integer;
    upon it --> SetValue(~it);
}
object <typesize = WORD> string {
    private rule AbstractCorrespondence[] : typesize = (-128c to 127c);
    public rule empty = nullchr;
    private var value[] : AbstractCorrespondence = nullchr;
    private func SetValue(val[] : typesize);
    private func IsIt(&obj) : boolean;
    public func ToChar(&str : string = ~it) : char;
    public func SubString(start : integer, length : integer, &str : string = ~it) : string;
    public func clear();
    public func ToUpper(&str : string = ~it) : string;
    public func ToLower(&str : string = ~it) : string;
    public func right(&str : string = ~it) : string;
    public func left(&str : string = ~it) : string;
    public func length() : signed;
    public func occurences(of : char) : signed;
    public func EndsWith(what : string) : boolean;
    public func StartsWith(what : string) : boolean;
    public func replace(&str : string = ~it, from : string, by : string) : string;
    public func find(what : char) : signed;
    public func IsNumeric() : boolean;
    public func IsSpace() : boolean;
    public func split(delim : char, &str : string = ~it)[] : string;
    public func back() : char;
    public func contains(what : string) : boolean;
    public func insert(what : string, where : signed, &str : string = ~it);
    public func pop(&str : string = ~it) : string;
    public func erase(start : signed, length : signed, &str : string = ~it) : string;
    upon it --> SetValue(~it);
}
object <typesize = DWORD> integer {
    private rule AbstractCorrespondence : TWORD = numeric;
    private var value : AbstractCorrespondence = nullptr;
    private func SetValue(val : typesize);
    public func ToString() : string;
    public func ToChar() : char;
    public ret MaxValue : typesize = 0xFFFFFFF;
    public ret MinValue : typesize = -0xFFFFFFF;
    upon it --> SetValue(~it);
}
object <typesize = QWORD> float : integer {
    public ret MaxValue : typesize = 0xFFFFFFFf;
    public ret MinValue : typesize = -0xFFFFFFFf;
}
object <typesize = TWORD> real : float;
object <typesize = BYTE> boolean {
    private rule AbstractCorrespondence : typesize = (0 || 1);
    private var value : AbstractCorrespondence = false;
    private func SetValue(val : typesize);
    public func ToString() : string;
    public func ToInt() : integer;
    upon it --> SetValue(~it);
}

` implementations `

enum LiteralType {
    Float,
    Char,
    Integer,
    Real,
    Boolean,
    String
}

` Set literal to suffix type `
func <type : LiteralType> LiteralSuffix(&literal : numeric) {
    match type {
        case (LiteralType.Float) literal = (literal)f;
        case (LiteralType.Char) literal = (literal)c;
        case (LiteralType.Integer) literal = (literal)i;
        case (LiteralType.Real) literal = (literal)r;
        case (LiteralType.Boolean) literal = (literal)b;
        case (LiteralType.String) literal = (literal)s;
    }
}

` boolean `

implement boolean.SetValue(val : typesize) {
    if (val isnt AbstractCorrespondence) throw "A Boolean value must be either true (= 1) or false (= 0), but not '" ^ val ^ "'.";
    it.value = val;
}
implement boolean.ToString() : string {
    match it.value {
        case (true) return "true";
        case (false) return "false";
    }
}
implement boolean.ToInt() : integer {
    match it.value {
        case (true) return 1;
        case (false) return 0;
    }
}

` char `

implement char.SetValue(val : typesize) {
    if (val isnt AbstractCorrespondence) throw "A char value must be either -128 and 127, but not '" ^ val ^ "'.";
    it.value = val;
}

implement char.ToString() : string {
    return LiteralSuffix<LiteralType.String>(it.value);
}

implement char.ToInt() : integer {
    return LiteralSuffix<LiteralType.Integer>(it.value);
}

` integer `

implement integer.SetValue(val : typesize) {
    if (val isnt AbstractCorrespondence) throw "An integer value can't be equal to '" ^ val ^ "'.";
    it.value = val;
}

implement integer.ToString() : string {
    return LiteralSuffix<LiteralType.String>(it.value);
}

implement integer.ToChar() : char {
    return LiteralSuffix<LiteralType.Char>(it.value);
}

` string `

implement IsIt(&obj) : boolean {
    return AddressOf(obj) == AddressOf(it);
}

implement string.SetValue(val : typesize) {
    if (val isnt AbstractCorrespondence) throw "A string value can't be equal to '" ^ val ^ "'.";
    it.value = val;
}

implement string.ToChar(&str : string = ~it) : char {
    if (!IsIt(str)) {
        if (str.length == 1) return str[0];
        else throw "'" ^ str ^ "' can't be returned as char type";
    }
    else {
        if (~it.length == 1) ~it = ~it[0];
        else throw "'" ^ str ^ "' can't be returned as char type";
    }
    return ~it;
}

implement string.SubString(start : integer, length : integer, &str : string = ~it) : string {
    if (length < start) throw "Length can't be lesser than stating index.";
    if (!IsIt(str)) {
        return (str[start] to str[length]);
    }
    else {
        ~it = (~it[start] to ~it[length]);
    }
    return ~it;
}

implement string.clear() {
    ~it = nullchr;
}

implement string.ToUpper(&str : string = ~it) : string {
    if (!IsIt(str)) {
        return (
            chr = str[0] to str[str.length()]
            | chr is LowerCase
        );
    }
    else {
        ~it = (
            chr = str[0] to str[str.length()]
            | chr is LowerCase
        );
    }
    return ~it;
}

implement string.ToLower(&str : string = ~it) : string {
    if (!IsIt(str)) {
        return (
            chr = str[0] to str[str.length()]
            | chr is UpperCase
        );
    }
    else {
        ~it = (
            chr = str[0] to str[str.length()]
            | chr is UpperCase
        );
    }
    return ~it;
}

implement string.right(&str : string = ~it) : string {
    if (!IsIt(str)) return (str[str.length()] to str[0]);
    else ~it = (~it[str.length()] to ~it[0]);
    return ~it;
}

implement string.left(&str : string = ~it) : string {
    return str;
}

implement string.length() : integer {
    var result = 0;
    for each (chr in ~it) ++result;
    return result;
}

implement string.occurences(of : char) : integer {
    var counter = 0;
    for each (chr in ~it)
        if (chr == of) ++counter;
    return counter;
}

implement string.EndsWith(what : string) : boolean {
    return ((~it[(~it.length() - what.length())] to ~it.back()) == what);
}

implement string.StartsWith(what : string) : boolean {
    return ((~it[0] to what.length()) == what);
}

implement string.replace(&str : string = ~it, from : string, by : string) : string {
    ` todo `
}

implement string.find(what : char) : signed {
    var result : signed = 0;
    for each (chr in ~it)
        if (chr == what) ++result;
    return result;
}

implement string.IsNumeric() : boolean {
    var result = true;
    for each (chr in ~it) {
        if (
            chr != '0'
            && chr != '1'
            && chr != '2'
            && chr != '3'
            && chr != '4'
            && chr != '5'
            && chr != '6'
            && chr != '7'
            && chr != '8'
            && chr != '9'
        ) result = false;
    }
    return result;
}

implement string.IsSpace() : boolean {
    var tmp : string;
    tmp = ~it.replace(" ", "");
    tmp = tmp.replace("\t", "");
    return tmp == nullchr;
}

implement string.split(delim : char, &str : string = ~it)[] : string {
    var result[] : string;
    ` todo `
}

implement string.back() : char {
    return ~it[~it.length()];
}

implement string.contains(what : string) : boolean {
    ` todo `
}

implement string.insert(what : string, where : signed, &str : string = ~it) {
    ` todo `
}

implement string.pop(&str : string) : string {
    if (!IsIt(str))
        return (str[0] to str[str.length() - 1]);
    else
        ~it = (str[0] to str[str.length() - 1]);
    return ~it;
}

implement string.erase(start : signed, length : signed, &str : string = ~it) : string {
    ` todo `
}

` others `

rule string[] = (-127 to 127);
rule char = (-127 to 127);
rule integer = numeric;
rule float = numeric;
rule real = numeric;
rule boolean = (0 to 1);
