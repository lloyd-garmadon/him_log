#include <him_log.h>
#include "Arduino.h"

void setup() {
  // opens serial port, sets data rate to 57600 bps
  him_log_init(9600);
}


int section_current = 1;
int section_next = 1;
const int section_max = 12;


void loop() {

    unsigned long start_1, stop_1, time_1;
    unsigned long start_2, stop_2, time_2;
    unsigned long start_3, stop_3, time_3;
    unsigned long start_4, stop_4, time_4;
    unsigned long start_5, stop_5, time_5;

    if( section_current == 1) {
        Serial.println( F("Section 1 - Introduction") );
        Serial.println( F("========================") );
        Serial.println( F("") );
        Serial.println( F("A common way to debug Arduino Sketches is to write out status and debug") );
        Serial.println( F("informations via the buildin serial port. The provided default functionality is") );
        Serial.println( F("AFAIK the 'Serial' library and its methods. Almost all logging needs are covered") );
        Serial.println( F("by the Serial functions but they are not comfortable to use. Especially when") );
        Serial.println( F("tying to dump complex strings including variables in certain formats.") );
        Serial.println( F("") );
        Serial.println( F("The basic requirements for a logging library are") );
        Serial.println( F(" - printf-like function using a intuitive format literal and variadic parameters") );
        Serial.println( F(" - all new log functions shall be wrapped into macros to disable logs by the") );
        Serial.println( F("   preprocessor during compile time ") );
        Serial.println( F(" - the new library shall still use the serial port under the hood and use the") );
        Serial.println( F("   'Serial.begin()' to initialize.") );
        Serial.println( F("   Direct call to the 'Serial' methods are still possible.") );
        Serial.println( F("") );
        Serial.println( F("First simple macros are:") );
        him_logd(         "him_logd(...)              - direct logging via serial port\n");
        him_logd_debug(   "him_logd_debug(...)    - enabled via HIM_LOG_DEBUG_ENABLE in the header file\n");
        him_logd_info(    "him_logd_info(...)    - enabled via HIM_LOG_INFO_ENABLE in the header file\n");
        him_logd_warn(    "him_logd_warn(...)    - enabled via HIM_LOG_WARN_ENABLE in the header file\n");
        him_logd_error(   "him_logd_error(...)    - enabled via HIM_LOG_ERROR_ENABLE in the header file\n");
        him_logd(         "\n");
        Serial.println( F("Please inspect the source code of this example in parallel") );
        Serial.println( F("when stepping through the following sections") );
        Serial.println( F("") );

    } else if( section_current == 2) {

        int test = 8879;

        him_logd("Section %d - Usage example\n", section_current);
        him_logd("=========================\n");
        him_logd("\n");

        Serial.println( F("Serial.print example:") );
        Serial.print( F("This in an ") );
        Serial.print(sizeof(test));
        Serial.print( F("-byte Integer: 0x") );
        Serial.print(test, HEX);
        Serial.print( F("/") );
        Serial.print(test);
        Serial.println( F("- displayed in hex and decimal format") );
        Serial.println( F("") );

        him_logd("him_logd example\n");
        him_logd("This in an %d-byte Integer: 0x%04x/%d - displayed in hex and decimal format\n", sizeof(test), test, test );

        him_logd("\n");

    } else if( section_current == 3) {

        him_logd("Section %d - Format Specifiers\n", section_current);
        him_logd("=============================\n");
        him_logd("\n");
        him_logd("Like the well known printf function the according him_log functions expects a\n");
        him_logd("format literal and variadic paramters as arguments. The format specifiers are\n");
        him_logd("not full printf compatible but very similar.\n");
        him_logd("General rules:\n");
        him_logd(" - Format specifiers starting with a %% character.\n");
        him_logd(" - A %% followed by another %% character will write a single %% to the stream.\n");
        him_logd(" - Width specifiers can be added between %% character and format specifier.\n");
        him_logd("\n");
        him_logd("Available format specifiers are:\n");
        him_logd("  'c' -  char\n");
        him_logd("  's' -  string\n");
        him_logd("  'd' -  int - in decimal output format\n");
        him_logd("  'D' -  long - in decimal output format\n");
        him_logd("  'u' -  unsigned int - in decimal output format\n");
        him_logd("  'U' -  unsigned long - in decimal output format\n");
        him_logd("  'x' -  unsigned int - in hex output format\n");
        him_logd("  'X' -  unsigned long - in hex output format\n");
        him_logd("  'b' -  unsigned int - in binary output format\n");
        him_logd("  'B' -  unsigned long - in binary output format\n");
        him_logd("  'p' -  pointer - output format $xxxxxxxx\n");
        him_logd("  't' -  time - output format decimal [xxxxxxxxxx]\n");
        him_logd("  'T' -  time - output format hex [$xxxxxxxx]\n");
        him_logd("Example:\n");

        unsigned int test_int = 0x01ff;
        unsigned long test_long = 0x0001ffff;

        him_logd("  %t test %%d   : %d\n",   test_int);
        him_logd("  %t test %%7d  : %7d\n",  test_int);
        him_logd("  %t test %%07d : %07d\n", test_int);
        him_logd("  %t test %%7D  : %7D\n",  test_long);
        him_logd("  %t test %%2x  : %2x\n",  test_int);
        him_logd("  %t test %%8x  : %8x\n",  test_int);
        him_logd("  %t test 0x%%08x : 0x%08x\n", test_int);
        him_logd("  %t test 0x%%08X : 0x%08X\n", test_long);

        him_logd("\n");

    } else if( section_current == 4) {

        him_logd("Section %d - Buffered Logging\n", section_current);
        him_logd("============================\n");
        him_logd("\n");
        him_logd("The him_logd() macro directly maps to the Serial.write() function of the\n");
        him_logd("serial port. Therefore the messages length, the baudrate and the internal\n");
        him_logd("buffer states are determining the time till the log function finishes.\n");
        him_logd("The debug logging can significantly influence the timing of your application.\n");
        him_logd("\n");
        him_logd("Alternatively the log messages may be written to a internal buffer to reduce\n");
        him_logd("the log time and can be dumped later on.\n");
        him_logd("Since most Arduino Sketches are designed like 'do something', 'delay', and      \n");
        him_logd("'loop again'. The idle time during the 'delay' time can be used to send the data\n");
        him_logd("out through the serial port.\n");
        him_logd("\n");
        him_logd("According to the 'him_logd' direct logging buffered logging functions 'him_logb'\n");
        him_logd("can be used. The log buffer must flushed by the 'him_log_flush()' function.\n");
        him_logd("\n");
        him_logd("Buffered log macros are:\n");
        him_logb(       "him_logb(...)            - logs to internal buffer\n");
        him_log_flush(true);
        him_logb_debug( "him_logb_debug(...)  - enabled via HIM_LOG_DEBUG_ENABLE in the header file\n");
        him_log_flush(true);
        him_logb_info(  "him_logb_info(...)  - enabled via HIM_LOG_INFO_ENABLE in the header file\n");
        him_log_flush(true);
        him_logb_warn(  "him_logb_warn(...)  - enabled via HIM_LOG_WARN_ENABLE in the header file\n");
        him_log_flush(true);
        him_logb_error( "him_logb_error(...)  - enabled via HIM_LOG_ERROR_ENABLE in the header file\n");
        him_log_flush(true);

        him_logd("\n");
        him_logd("The execution time for the log function itself can significantly accelerated by\n");
        him_logd("a buffered logging. The time to send the the characters via the serial port can\n");
        him_logd("not be accelerated of course. But this way the time consumpting flush function\n");
        him_logd("can be located in a non timing critical part of the Arduino Sketch.\n");
        him_logd("Please have a look at the timing examples.\n");

        him_logd("\n");

    } else if( section_current == 5) {

        him_logd("Section %d - Buffer overflow\n", section_current);
        him_logd("===========================\n");
        him_logd("\n");
        him_logd("The size of the internal logbuffer is determined at compile time. intensive buffered\n");
        him_logd("logging may cause a buffer overflow. The default size is 128 characters.\n");
        him_logd("\n");
        him_logd("Fill up the buffer with 124 characters\n");
        him_logb("0123456789");
        him_logb("abcdefghijklmnopqrstuvwxyz");
        him_logb("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        him_logb("0123456789");
        him_logb("abcdefghijklmnopqrstuvwxyz");
        him_logb("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

        him_logd("Fill up the buffer with additional 10 characters\n");
        him_logd("This will provoke a buffer overflow\n");
        him_logb("xxxxxxxxxx");

        him_logd("\n");
        him_logd("Flush logbuffer:\n");
        him_log_flush(true);
        him_logd("\n");

        him_logd("\n");
        him_logd("The first charcters of the buffer are overwritten\n");
        him_logd("The overflow is marked with **OF**\n");

        him_logd("\n");

    } else if( section_current == 6) {

        unsigned long test_long;

        him_logd("Section %d\n", section_current);
        him_logd("Timing example for direct him_logd() macro\n");
        him_logd("==========================================\n");
        him_logd("\n");

        test_long = 0x0001ffff;
        start_1 = micros();
        him_logd("1st direct log: %08D\n", test_long );
        stop_1 = micros();

        test_long += 100;
        start_2 = micros();
        him_logd("2nd direct log: %08D\n", test_long );
        stop_2 = micros();

        test_long += 100;
        start_3 = micros();
        him_logd("3rd direct log: 0x%08X\n", test_long );
        stop_3 = micros();

        test_long += 100;
        start_4 = micros();
        him_logd("4th direct log: 0x%08X\n", test_long );
        stop_4 = micros();

        time_1 = stop_1 - start_1;
        time_2 = stop_2 - start_2;
        time_3 = stop_3 - start_3;
        time_4 = stop_4 - start_4;
        him_logd("\n");
        him_logd("1st direct logging took %D.%03D ms\n", time_1/1000, time_1%1000 );
        him_logd("2nd direct logging took %D.%03D ms\n", time_2/1000, time_2%1000 );
        him_logd("3rd direct logging took %D.%03D ms\n", time_3/1000, time_3%1000 );
        him_logd("4th direct logging took %D.%03D ms\n", time_4/1000, time_4%1000 );

        him_logd("\n");

    } else if( section_current == 7) {

        unsigned long test_long;

        him_logd("Section %d\n", section_current);
        him_logd("Timing example for buffered him_logb() macro\n");
        him_logd("============================================\n");
        him_logd("\n");

        test_long = 0x0001ffff;
        start_1 = micros();
        him_logb("1st buffered log: %08D\n", test_long );
        stop_1 = micros();

        test_long += 100;
        start_2 = micros();
        him_logb("2nd buffered log: %08D\n", test_long );
        stop_2 = micros();

        test_long += 100;
        start_3 = micros();
        him_logb("3rd buffered log: 0x%08X\n", test_long );
        stop_3 = micros();

        test_long += 100;
        start_4 = micros();
        him_logb("4th buffered log: 0x%08X\n", test_long );
        stop_4 = micros();

        start_5 = micros();
        him_log_flush(true);
        stop_5 = micros();

        time_1 = stop_1 - start_1;
        time_2 = stop_2 - start_2;
        time_3 = stop_3 - start_3;
        time_4 = stop_4 - start_4;
        time_5 = stop_5 - start_5;
        him_logd("\n");
        him_logd("1st buffered logging took %D.%03D ms\n", time_1/1000, time_1%1000 );
        him_logd("2nd buffered logging took %D.%03D ms\n", time_2/1000, time_2%1000 );
        him_logd("3rd buffered logging took %D.%03D ms\n", time_3/1000, time_3%1000 );
        him_logd("4th buffered logging took %D.%03D ms\n", time_4/1000, time_4%1000 );
        him_logd("Flush the log buffer took %D.%03D ms\n", time_5/1000, time_5%1000 );

        him_logd("\n");

    } else if( section_current == 8) {

        him_logd("Section %d - Flushing\n", section_current);
        him_logd("====================\n");
        him_logd("\n");
        him_logd("The him_log_flush() can be called every time. The function itself decides if \n");
        him_logd("the internal log buffer needs to be flushed via the serial port. Since the      \n");
        him_logd("execution time may take much time or to prevent a buffer overflow the behaviour \n");
        him_logd("can be modified in serveral ways. The him_log_flush() itself can be forced to\n");
        him_logd("flush the log buffer immediately. A flush interval and log buffer thresholds can\n");
        him_logd("set up via some helper functions. The maximal number of bytes to be dumpes and  \n");
        him_logd("maximal time per him_flush() call can be set as well. But be careful - some  \n");
        him_logd("combinations may prevent the buffer to be flushed empty or let the buffer tend  \n");
        him_logd("overflow. However just play a little with the following setting functions to    \n");
        him_logd("find your optimal log buffer flush strategies.\n");
        him_logd("  him_log_set_flush_min_interval() \n");
        him_logd("  him_log_set_flush_min_threshold()\n");
        him_logd("  him_log_set_flush_max_time()     \n");
        him_logd("  him_log_set_flush_max_bytes()    \n");
        him_logd("  him_log_flush()\n");

        him_logd("\n");

    } else if (section_current == 9) {

        him_logd("Section %d - Flushing example\n", section_current);
        him_logd("============================\n");
        him_logd("\n");

        him_logd("Fill up the buffer with 80 characters\n");
        him_logb("0123456789");
        him_logb("0123456789");
        him_logb("0123456789");
        him_logb("0123456789");
        him_logb("0123456789");
        him_logb("0123456789");
        him_logb("0123456789");
        him_logb("0123456789");
        him_logd("flush logbuffer:\n");
        start_1 = micros();
        him_log_flush(true);
        stop_1 = micros();
        time_1 = stop_1 - start_1;
        him_logd("\n");
        him_logd("Flushing whole buffered logging took %D.%03D ms\n", time_1/1000, time_1%1000 );
        him_logd("\n\n");

        him_logd("Fill up the buffer with 80 characters\n");
        him_logb("0123456789012345678901234567890123456789\n");
        him_logb("0123456789012345678901234567890123456789\n");
        him_logd("Set a maximal flush time to 5 ms\n");
        him_log_set_flush_max_time(5000);
        him_logd("flush logbuffer:\n");
        start_1 = micros();
        him_log_flush(true);
        stop_1 = micros();
        time_1 = stop_1 - start_1;
        him_logd("\n");

        him_logd("\n");
        him_logd("1st flush of the log buffer took %D.%03D ms\n", time_1/1000, time_1%1000 );
        him_logd("It is obviously not empty. So flush again.\n");
        him_logd("\n");

        him_logd("flush logbuffer again:\n");
        start_1 = micros();
        him_log_flush(true);
        stop_1 = micros();
        time_1 = stop_1 - start_1;
        him_logd("\n");

        him_logd("\n");
        him_logd("2st flush of the log buffer took %D.%03D ms\n", time_1/1000, time_1%1000 );
        him_logd("It is obviously not empty. So flush again.\n");
        him_logd("\n");

        him_logd("flush logbuffer again:\n");
        start_1 = micros();
        him_log_flush(true);
        stop_1 = micros();
        time_1 = stop_1 - start_1;
        him_logd("\n");

        him_logd("\n");
        him_logd("3nd flush of the log buffer took %D.%03D ms\n", time_1/1000, time_1%1000 );
        him_logd(".....\n");

        him_logd("\n");

    } else if( section_current == 10) {

        him_logd("Section %d - F Macro\n", section_current);
        him_logd("===================\n");
        him_logd("\n");
        him_logd("Data located within the Arduinos program memory (Flash) can not accessed within\n");
        him_logd("the Arduino Sketch without copying it to the RAM. To avoid copy operation\n");
        him_logd("during runtime a string literal is stored in progmen and transfered to RAM\n");
        him_logd("before the program starts. So every string literal statically occupies Flash\n");
        him_logd("and additional memory space in RAM.\n");
        him_logd("To prevent spending rare RAM memory for string literals it can be left in the\n");
        him_logd("program Flash using the F() macro. Handling progmem string literals must be\n");
        him_logd("implemented in the according him_log functions. Every him_logd and him_logb macro\n");
        him_logd("automatically applies the F() macro to the format string literal.\n");
        him_logd("\n");
        him_logd("This also means that these macros always expect a format string literal!\n");
        him_logd("\n");
        him_logd("This restriction is usefull in most cases, but to remove this restriction\n");
        him_logd("analog macros exist that do not have this restriction\n");
        him_logd("\n");
        him_logd("For completness, these are the variable macros\n");
        him_logd("  him_logvd(...)\n");
        him_logd("  him_logvd_debug(...)\n");
        him_logd("  him_logvd_info(...)\n");
        him_logd("  him_logvd_warn(...)\n");
        him_logd("  him_logvd_error(...)\n");
        him_logd("  him_logvb(...)\n");
        him_logd("  him_logvb_debug(...)\n");
        him_logd("  him_logvb_info(...)\n");
        him_logd("  him_logvb_warn(...)\n");
        him_logd("  him_logvb_error(...)\n");
        him_logd("\n");
        him_logd("Example:\n");

        int variable = 999;
        const char * const_array = "const char *  : %d\n";
        char char_array[24];
        char_array[0] = 'c';
        char_array[1] = 'h';
        char_array[2] = 'a';
        char_array[3] = 'r';
        char_array[4] = '[';
        char_array[5] = ']';
        char_array[6] = ' ';
        char_array[7] = ' ';
        char_array[8] = ' ';
        char_array[9] = ' ';
        char_array[10] = ' ';
        char_array[11] = ' ';
        char_array[12] = ' ';
        char_array[13] = ' ';
        char_array[14] = ':';
        char_array[15] = ' ';
        char_array[16] = '%';
        char_array[17] = 'd';
        char_array[18] = '\n';
        char_array[19] = 0;

        him_logd("  format string is a "); him_logvd( F("flash literal : %d\n"), variable);
        him_logd("  format string is a "); him_logvd( const_array, variable);
        him_logd("  format string is a "); him_logvd( char_array, variable);
        him_logd("\n");

        him_logd("Please have a look at the according define in the sourcecode.\n");
        him_logd("\n");

    } else if (section_current == 11) {

        him_logd("Section %d - Implementation caveats\n", section_current);
        him_logd("==================================\n");
        him_logd("The him_log library internally instanciates a global static object\n");
        him_logd("which implements the functionality.\n");
        him_logd("\n");
        him_logd("The function interface is encapsulated in just some simple defines\n");
        him_logd("as described in the previous sections\n");
        him_logd("\n");
        him_logd("Also regard the him_log_config.h header. You can also modify the size \n");
        him_logd("of the internal log buffer to tweak compile size. \n");
        him_logd("You may also enabled/diabled certain log macros or may disable bufferd\n");
        him_logd("logging completely at compile time - when not needed.\n");
        him_logd("\n");

    } else if (section_current == 12) {

        him_logd("That's all Folks\n");
        him_logd("================\n");
        him_logd("\n");
        him_logd("Have fun using the him_log library.\n");
        him_logd(";-)\n");
        him_logd("\n");
        him_logd("Bests\n");
        him_logd("\n");
        him_logd("Sven\n");
        him_logd("\n");
        him_logd("\n");

        section_current++;
    }

    
    if(section_current < section_max) {
        him_logd("Press ENTER to continue");

        while(section_current == section_next) {
            while(Serial.available() > 0) {
                if (Serial.read() == 10) {       
                    section_next++;
                    break;
                }
            }
            delay(100);
        }
        section_current = section_next;

        him_logd("\n\n\n");
    }

}
