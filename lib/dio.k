`
    +-----------------------------------------------------------------------------------------------------------------+
    | dio is the standard import library.                                                                             |
    | It manages the display, as well as the input and output (IO), but also has utility and miscellaneous functions. |
    +-----------------------------------------------------------------------------------------------------------------+
`

import lib.primary;
import lib.array;
import lib.dictionary;
import lib.multidictionary;
import lib.list;
import lib.stack;

define newline "\n";
define space " ";

rule ASCII : array<char, 127> = (0 to 127);

namespace dio;

object display {
    private:
        func DisplayString(subject : string, newline : boolean) {
            extern PRINTF;
            if (newline) printf("%s\n", subject);
            else printf("%s", subject);
        }
        func DisplayChar(subject : char, newline : boolean) {
            extern PRINTF;
            if (newline) printf("%c\n", subject);
            else printf("%c", subject);
        }
        func DisplayInteger(subject : integer, newline : boolean) {
            extern PRINTF;
            if (newline) printf("%d\n", subject);
            else printf("%d", subject);
        }
        func DisplayFloat(subject : float, newline : boolean) {
            extern PRINTF;
            if (newline) printf("%f\n", subject);
            else printf("%f", subject);
        }
        func DisplayReal(subject : real, newline : boolean) {
            extern PRINTF;
            if (newline) printf("%lf\n", subject);
            else printf("%lf", subject);
        }
        func DisplayBoolean(subject : real, newline : boolean) {
            extern PRINTF;
            if (newline) {
                match subject {
                    case (true) printf("true\n", subject);
                    case (false) printf("false\n", subject);
                }
            }
            else {
                match subject {
                    case (true) printf("true", subject);
                    case (false) printf("false", subject);
                }
            }
        }
    static:
        func write(ToDisplay) {
            match TD --> ToDisplay {
                case (TD is string) DisplayString(ToDisplay);
                case (TD is char) DisplayChar(ToDisplay);
                case (TD is integer) DisplayInteger(ToDisplay);
                case (TD is float) DisplayFloat(ToDisplay);
                case (TD is real) DisplayReal(ToDisplay);
                case (TD is boolean) DisplayBoolean(ToDisplay);
                default throw "Inacceptable format: '" + ToDisplay.typename + "'";
            }
        }
        func writeln(ToDisplay) {
            match TD --> ToDisplay {
                case (TD is string) DisplayString(ToDisplay, true);
                case (TD is char) DisplayChar(ToDisplay, true);
                case (TD is integer) DisplayInteger(ToDisplay, true);
                case (TD is float) DisplayFloat(ToDisplay, true);
                case (TD is real) DisplayReal(ToDisplay, true);
                case (TD is boolean) DisplayBoolean(ToDisplay, true);
                default throw "Inacceptable format: '" + ToDisplay.typename + "'";
            }
        }
        func put(ToPut : char) {
            extern PUTCHAR;
            putchar(ToPut);
        }
} ` display `

object input {
    private:
        
    static:
        func ReadLine() : string {
            var buff : string;
            extern SCANF;
            scanf("%s", &buff);
            return buff;
        }
        func ReadChar() : char {
            extern GETCH;
            return getch();
        }
}
