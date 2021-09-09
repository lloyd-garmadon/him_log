/***********************************************************************
* FILENAME:
*       him_log.h
*
* DESCRIPTION:
*       Him Arduino Debug Log Routines.
*
* NOTES:
*       This library was written just for fun.
*
* AUTHOR:
*       Sven Himstedt
*
* COPYRIGHT: 
*       Sven Himstedt 2017, All rights reserved.
*
* LICENSE:
*       This library is free software.
*       You can redistribute it and/or modify it.
*       But give credit or you are an asshole.
*
* START DATE:
*       2017/11/18
*
* CHANGES:
*
***********************************************************************/

#ifndef _HIM_LOG_H_
#define _HIM_LOG_H_



#define HIM_LOG_ENABLE
#define HIM_LOG_DEBUG_ENABLE
#define HIM_LOG_TRACE_ENABLE
#define HIM_LOG_WARN_ENABLE
#define HIM_LOG_ERROR_ENABLE

#define HIM_LOG_BUFFER_ENABLE 
#define HIM_LOG_BUFFER_SIZE        128



#define HIM_LOG_CURSOR_HOME           "\033[H"               // Sets the cursor position where subsequent text will begin. If no row/column parameters are provided (ie. <ESC>[H), the cursor will move to the home position, at the upper left of the screen.
#define HIM_LOG_CURSOR_UP             "\033[A"               // Moves the cursor up
#define HIM_LOG_CURSOR_DOWN           "\033[B"               // Moves the cursor down
#define HIM_LOG_CURSOR_FORWARD        "\033[C"               // Moves the cursor forward 
#define HIM_LOG_CURSOR_BACKWARD       "\033[D"               // Moves the cursor backward 
#define HIM_LOG_CURSOR_POSITION(x,y)  "\033[" #y ";" #x "f"  // Force Cursor Position
#define HIM_LOG_CURSOR_UP_N(n)        "\033[" #n "A"         // Moves the cursor up by n rows
#define HIM_LOG_CURSOR_DOWN_N(n)      "\033[" #n "B"         // Moves the cursor down by n rows
#define HIM_LOG_CURSOR_FORWARD_N(n)   "\033[" #n "C"         // Moves the cursor forward by n columns
#define HIM_LOG_CURSOR_BACKWARD_N(n)  "\033[" #n "D"         // Moves the cursor backward by n columns

#define HIM_LOG_ERASE_EOL             "\033[K"          // Erases from the current cursor position to the end of the current line.
#define HIM_LOG_ERASE_SOL             "\033[1K"         // Erases from the current cursor position to the start of the current line.
#define HIM_LOG_ERASE_LINE            "\033[2K"         // Erases the entire current line.
#define HIM_LOG_ERASE_DOWN            "\033[J"          // Erases the screen from the current line down to the bottom of the screen.
#define HIM_LOG_ERASE_UP              "\033[1J"         // Erases the screen from the current line up to the top of the screen.
#define HIM_LOG_ERASE_SCREEN          "\033[2J"         // Erases the screen with the background colour and moves the cursor to home.

#define HIM_LOG_ATTR_RESET            "\033[0m"         // Reset all attributes
#define HIM_LOG_ATTR_BRIGHT           "\033[1m"         // Bright
#define HIM_LOG_ATTR_DIM              "\033[2m"         // Dim
#define HIM_LOG_ATTR_UNDERSCORE       "\033[4m"         // Underscore	
#define HIM_LOG_ATTR_BLINK            "\033[5m"         // Blink
#define HIM_LOG_ATTR_REVERSE          "\033[7m"         // Reverse
#define HIM_LOG_ATTR_HIDDEN           "\033[8m"         // Hidden
#define HIM_LOG_ATTR_FOREGND_BK       "\033[30m"        // Foreground Black
#define HIM_LOG_ATTR_FOREGND_RD       "\033[31m"        // Foreground Red
#define HIM_LOG_ATTR_FOREGND_GR       "\033[32m"        // Foreground Green
#define HIM_LOG_ATTR_FOREGND_YL       "\033[33m"        // Foreground Yellow
#define HIM_LOG_ATTR_FOREGND_BL       "\033[34m"        // Foreground Blue
#define HIM_LOG_ATTR_FOREGND_MA       "\033[35m"        // Foreground Magenta
#define HIM_LOG_ATTR_FOREGND_CY       "\033[36m"        // Foreground Cyan
#define HIM_LOG_ATTR_FOREGND_WT       "\033[37m"        // Foreground White
#define HIM_LOG_ATTR_BACKGND_BK       "\033[40m"        // Background Black
#define HIM_LOG_ATTR_BACKGND_RD       "\033[41m"        // Background Red
#define HIM_LOG_ATTR_BACKGND_GR       "\033[42m"        // Background Green
#define HIM_LOG_ATTR_BACKGND_YL       "\033[43m"        // Background Yellow
#define HIM_LOG_ATTR_BACKGND_BL       "\033[44m"        // Background Blue
#define HIM_LOG_ATTR_BACKGND_MA       "\033[45m"        // Background Magenta
#define HIM_LOG_ATTR_BACKGND_CY       "\033[46m"        // Background Cyan
#define HIM_LOG_ATTR_BACKGND_WT       "\033[47m"        // Background White



#include "Arduino.h"



#ifdef HIM_LOG_ENABLE
    class HimLogging
    {
    public:
        HimLogging();
        ~HimLogging();

        void log(bool buffered_output, const __FlashStringHelper* format, ...);
        void log(bool buffered_output, const char* format, ...);
        void logv(bool buffered_output, bool progmem_format, const void* format, va_list args);

#ifdef HIM_LOG_BUFFER_ENABLE
        void setFlushMinInterval(unsigned long log_interval);
        void setFlushMinThreshold(unsigned int log_threshold /*percent*/ );
        void setFlushMaxTime(unsigned long max_time);
        void setFlushMaxBytes(unsigned int max_bytes);

        void flush(bool flush_now = false);

        unsigned int getFilled();
#endif

    private:
        char * value2string(int value, int * p_size, int base);
        char * value2string(long value, int * p_size, int base);
        char * value2string(unsigned int value, int * p_size, int base);
        char * value2string(unsigned long value, int * p_size, int base);
        char * value2string_base(unsigned long value, int * p_size, int base, bool sign);
        char * value2string_hex(unsigned long value, int * p_size);
        char * value2string_bin(unsigned long value, int * p_size);

        void write(bool buffered_output, char value);

    private:
        #define HIM_DIGIT_BUFFERSIZE 32
        char m_digit_buffer[HIM_DIGIT_BUFFERSIZE + 1];
        int  m_digit_buffer_size;
        char m_digit_table[16];

#ifdef HIM_LOG_BUFFER_ENABLE
        char m_log_buffer[HIM_LOG_BUFFER_SIZE];
        unsigned int m_log_buffer_size;
        char * m_log_buffer_begin_pointer;
        char * m_log_buffer_end_pointer;
        char * m_log_write_pointer;
        char * m_log_read_pointer;
        bool m_log_buffer_overflow;
        unsigned int  m_log_buffer_almost_full;

        unsigned long m_flush_min_interval;
        unsigned int  m_flush_min_threshold;
        unsigned long m_flush_max_time;
        unsigned int  m_flush_max_bytes;

        unsigned long m_last_flush_time;
#endif
    };

    extern HimLogging HimLog;

    // log macros
    #ifndef HIM_LOG_ENABLE
    #  define him_log_init(baudrate)
    #  define him_logd(format, ...)
    #  define him_logb(format, ...)
    #else 
    #  define him_log_init(baudrate)  Serial.begin(baudrate)
    #  define him_logd(format, ...)   HimLog.log(false, F(format), ##__VA_ARGS__)
    #  define him_logd_pos(x,y)       HimLog.log(false, "\033[%d;%df", x, y)
    #  ifdef HIM_LOG_BUFFER_ENABLE
    #    define him_logb(format, ...)   HimLog.log(true, F(format), ##__VA_ARGS__)
    #    define him_logb_pos(x,y)       HimLog.log(true, "\033[%d;%df", x, y)
    #  else 
    #    define him_logb(format, ...)
    #    define him_logb_pos(x,y)
    #  endif
    #endif


    #ifndef HIM_LOG_DEBUG_ENABLE
    #  define him_logd_debug(format, ...)
    #  define him_logb_debug(format, ...)
    #else 
    #  define him_logd_debug(format, ...) him_logd(format, ##__VA_ARGS__)
    #  define him_logb_debug(format, ...) him_logb(format, ##__VA_ARGS__)
    #endif

    #ifndef HIM_LOG_TRACE_ENABLE
    #  define him_logd_trace(format,...)
    #  define him_logb_trace(format,...)
    #else
    #  define him_logd_trace(format,...) him_logd(format, ##__VA_ARGS__)
    #  define him_logb_trace(format,...) him_logb(format, ##__VA_ARGS__)
    #endif

    #ifndef HIM_LOG_WARN_ENABLE
    #  define him_logd_warn(format,...)
    #  define him_logb_warn(format,...)
    #else
    #  define him_logd_warn(format,...) him_logd(format, ##__VA_ARGS__)
    #  define him_logb_warn(format,...) him_logb(format, ##__VA_ARGS__)
    #endif

    #ifndef HIM_LOG_ERROR_ENABLE
    #  define him_logd_error(format,...)
    #  define him_logb_error(format,...)
    #else
    #  define him_logd_error(format,...) him_logd(format, ##__VA_ARGS__)
    #  define him_logb_error(format,...) him_logb(format, ##__VA_ARGS__)
    #endif

    #ifdef HIM_LOG_BUFFER_ENABLE
    #  define him_log_set_flush_min_interval(x)  HimLog.setFlushMinInterval(x)
    #  define him_log_set_flush_min_threshold(x) HimLog.setFlushMinThreshold(x)
    #  define him_log_set_flush_max_time(x)      HimLog.setFlushMaxTime(x);
    #  define him_log_set_flush_max_bytes(x)     HimLog.setFlushMaxBytes(x);
    #  define him_log_flush(x) HimLog.flush(x)
    #else
    #  define him_log_set_flush_min_interval(x) 
    #  define him_log_set_flush_min_threshold(x)
    #  define him_log_set_flush_max_time(x) 
    #  define him_log_set_flush_max_bytes(x)
    #  define him_log_flush(x) 0
    #endif

#else

    // disable all log macros
    #define him_log_init(baudrate)
    #define him_logd(format, ...)
    #define him_logb(format, ...)
    #define him_logd_debug(format, ...)
    #define him_logb_debug(format, ...)
    #define him_logd_trace(format, ...)
    #define him_logb_trace(format, ...)
    #define him_logd_warn(format, ...)
    #define him_logb_warn(format, ...)
    #define him_logd_error(format, ...)
    #define him_logb_error(format, ...)

    #define him_log_set_flush_min_interval(x) 
    #define him_log_set_flush_min_threshold(x)
    #define him_log_set_flush_max_time(x) 
    #define him_log_set_flush_max_bytes(x)
    #define him_log_flush(x) 0
    
#endif


#endif /* _HIM_LOG_H_ */
