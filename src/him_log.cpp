/***********************************************************************
* FILENAME:
*       him_log.cpp
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

#define HIM_LIBRARY_COMPILE

#include "Arduino.h"

#include "him_log.h"



#ifdef HIM_LOG_ENABLE

HimLogging::HimLogging()
{
#ifdef HIM_LOG_BUFFER_ENABLE
    m_log_buffer_overflow = false;

    m_log_buffer_size = HIM_LOG_BUFFER_SIZE;

    m_log_buffer_begin_pointer = m_log_buffer;
    m_log_buffer_end_pointer = m_log_buffer + (HIM_LOG_BUFFER_SIZE - 1);
    m_log_write_pointer = m_log_buffer;
    m_log_read_pointer = m_log_buffer;

    m_flush_min_interval = 0;
    m_flush_min_threshold = 0;
    m_flush_max_time = 0;
    m_flush_max_bytes = 0;

    m_log_buffer_almost_full = m_log_buffer_size * 90 / 100;

    m_last_flush_time = 0;
#endif

    memset(m_digit_buffer, 0, HIM_DIGIT_BUFFERSIZE + 1);
    m_digit_buffer_size = HIM_DIGIT_BUFFERSIZE;

    m_digit_table[0x00] = '0';
    m_digit_table[0x01] = '1';
    m_digit_table[0x02] = '2';
    m_digit_table[0x03] = '3';
    m_digit_table[0x04] = '4';
    m_digit_table[0x05] = '5';
    m_digit_table[0x06] = '6';
    m_digit_table[0x07] = '7';
    m_digit_table[0x08] = '8';
    m_digit_table[0x09] = '9';
    m_digit_table[0x0a] = 'a';
    m_digit_table[0x0b] = 'b';
    m_digit_table[0x0c] = 'c';
    m_digit_table[0x0d] = 'd';
    m_digit_table[0x0e] = 'e';
    m_digit_table[0x0f] = 'f';
}


HimLogging::~HimLogging()
{
}

void HimLogging::log(bool buffered_output, const __FlashStringHelper* format, ...)
{
    va_list args;
    va_start(args, format);

    logv(buffered_output, true, (const void*)format, args);

    va_end(args);
}

void HimLogging::log(bool buffered_output, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    logv(buffered_output, false, (const void*)format, args);

    va_end(args);
}

void HimLogging::logv(bool buffered_output, bool progmem_format, const void* format, va_list args)
{
    char format_char;

    int format_idx = 0;
    char * format_literal_ram = (char *)format;
    unsigned int format_literal_flash = (unsigned int)format;
    #define HIM_LOG_READ_FORMAT(index) (progmem_format ? pgm_read_byte_near(format_literal_flash + index) : format_literal_ram[index])

    char * format_string = NULL;
    int format_string_size = 0;
    int format_fillup_size = 0;
    char format_fillup_character = ' ';
    bool format_dollar = false;
    bool format_brackets = false;
    bool end = false;

    while((format_char = HIM_LOG_READ_FORMAT(format_idx)) != 0) {
        if(format_char == '\n') {
            write( buffered_output, '\r' );
            write( buffered_output, '\n' );
            format_idx++;
        } else if(format_char != '%') {
            write( buffered_output, format_char );
            format_idx++;
        } else {
            format_string = NULL;
            format_string_size = 0;
            format_fillup_size = 0;
            format_fillup_character = ' ';
            format_dollar = false;
            format_brackets = false;
            end = false;

            for(end = false, format_idx++; !end && ((format_char = HIM_LOG_READ_FORMAT(format_idx)) != 0 ); format_idx++) {
                switch (format_char) {
                //
                // modifiers
                //
                case '0':
                    if(format_fillup_size == 0) {
                        format_fillup_character = '0';
                    }
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    format_fillup_size *= 10;
                    format_fillup_size += (format_char - '0');
                    break;
                //
                // types
                //
                case '%':  // the percent character itself
                    m_digit_buffer[0] = '%'; 
                    m_digit_buffer[1] = 0; 
                    format_string = m_digit_buffer;
                    end = true;
                    break;
                case 'c':  // char
                    m_digit_buffer[0] = va_arg(args, unsigned int ); 
                    m_digit_buffer[1] = 0; 
                    format_string = m_digit_buffer;
                    end = true;
                    break;
                case 's':  // string
                    format_string = va_arg(args, char * );
                    end = true;
                    break;
                case 'd':  // long - in decimal format
                    format_string = value2string(va_arg(args, int ), &format_string_size, 10);
                    end = true;
                    break;
                case 'D':  // long - in decimal format
                    format_string = value2string(va_arg(args, long), &format_string_size, 10);
                    end = true;
                    break;
                case 'u':  // unsigned int - in decimal format
                    format_string = value2string(va_arg(args, unsigned int), &format_string_size, 10);
                    end = true;
                    break;
                case 'U':  // unsigned long - in decimal format
                    format_string = value2string(va_arg(args, unsigned long), &format_string_size, 10);
                    end = true;
                    break;
                case 'x':  // unsigned int - in hex format
                    format_string = value2string(va_arg(args, unsigned int), &format_string_size, 16);
                    end = true;
                    break;
                case 'X':  // unsigned long - in hex format
                    format_string = value2string(va_arg(args, unsigned long), &format_string_size, 16);
                    end = true;
                    break;
                case 'b':  // unsigned int - in binary format
                    format_string = value2string(va_arg(args, unsigned int), &format_string_size, 2);
                    end = true;
                    break;
                case 'B':  // unsigned long - in binary format
                    format_string = value2string(va_arg(args, unsigned long), &format_string_size, 2);
                    end = true;
                    break;
                case 'p':  // pointer
                    format_string = value2string(va_arg(args, unsigned int), &format_string_size, 16);
                    format_fillup_size = 8;
                    format_fillup_character = '0';
                    format_dollar = true;
                    end = true;
                    break;
                case 't':  // time decimal format
                    format_string = value2string(micros(), &format_string_size, 10);
                    format_fillup_size = 10;
                    format_fillup_character = '0';
                    format_brackets = true;
                    end = true;
                    break;
                case 'T':  // time hex format
                    format_string = value2string(micros(), &format_string_size, 16);
                    format_fillup_size = 8;
                    format_fillup_character = '0';
                    format_brackets = true;
                    format_dollar = true;
                    end = true;
                    break;
                default:
                    end = true;
                }
            }

            if( format_brackets ) {
                write( buffered_output, '[' );
            }
            if( format_dollar ) {
                write( buffered_output, '$' );
            }
            for(int i=format_fillup_size; i > format_string_size; i--) {
                write( buffered_output, format_fillup_character );
            }
            for(int i=0; format_string[i] != 0; i++) {
                write( buffered_output, format_string[i] );
            }
            if( format_brackets ) {
                write( buffered_output, ']' );
            }
        }
    }
}

inline char * HimLogging::value2string(unsigned int value, int * p_size, int base)
{
    switch(base) {
    case 2:
        return value2string_bin((unsigned long)value, p_size);
        break;
    case 16:
        return value2string_hex((unsigned long)value, p_size);
        break;
    default:
        return value2string_base((unsigned long)value, p_size, base, false);
    }
}

inline char * HimLogging::value2string(unsigned long value, int * p_size, int base)
{
    switch(base) {
    case 2:
        return value2string_bin((unsigned long)value, p_size);
        break;
    case 16:
        return value2string_hex((unsigned long)value, p_size);
        break;
    default:
        return value2string_base((unsigned long)value, p_size, base, false);
    }
}

inline char * HimLogging::value2string(int value, int * p_size, int base)
{
    switch(base) {
    case 2:
        return value2string_bin((unsigned long)value, p_size);
        break;
    case 16:
        return value2string_hex((unsigned long)value, p_size);
        break;
    default:
        bool sign = false;

        if(value < 0) {
            value = -value;
            sign = true;
        }

        return value2string_base((unsigned long)value, p_size, base, sign);
    }
}

inline char * HimLogging::value2string(long value, int * p_size, int base)
{
    switch(base) {
    case 2:
        return value2string_bin((unsigned long)value, p_size);
        break;
    case 16:
        return value2string_hex((unsigned long)value, p_size);
        break;
    default:
        bool sign = false;

        if(value < 0) {
            value = -value;
            sign = true;
        }

        return value2string_base((unsigned long)value, p_size, base, sign);
    }
}

char * HimLogging::value2string_base(unsigned long value, int * p_size, int base, bool sign)
{
    int index = m_digit_buffer_size;

    if(!value) {
        m_digit_buffer[--index] = '0';
    } else do {
        m_digit_buffer[--index] = m_digit_table[ value % base ];
        value /= base;
    } while (value);

    if(sign) {
        m_digit_buffer[--index] = '-';
    }

    if(p_size) *p_size = m_digit_buffer_size - index;

    return &m_digit_buffer[index];
}

char * HimLogging::value2string_hex(unsigned long value, int * p_size)
{
    int index = m_digit_buffer_size;

    if(!value) {
        m_digit_buffer[--index] = '0';
    } else do {
        m_digit_buffer[--index] = m_digit_table[ value & 0x0f ];
        value >>= 4;
    } while (value);

    if(p_size) *p_size = m_digit_buffer_size - index;

    return &m_digit_buffer[index];
}

char * HimLogging::value2string_bin(unsigned long value, int * p_size)
{
    int index = m_digit_buffer_size;

    if(!value) {
        m_digit_buffer[--index] = '0';
    } else do {
        m_digit_buffer[--index] = m_digit_table[ value & 0x01 ];
        value >>= 1;
    } while (value);

    if(p_size) *p_size = m_digit_buffer_size - index;

    return &m_digit_buffer[index];
}


inline void HimLogging::write(bool buffered_output, char value)
{
#ifdef HIM_LOG_BUFFER_ENABLE
    if(buffered_output) {
        *m_log_write_pointer = value;
        if(m_log_write_pointer == m_log_buffer_end_pointer) {
            m_log_write_pointer = m_log_buffer_begin_pointer;
        } else {
            m_log_write_pointer++;
        }

        if(m_log_write_pointer == m_log_read_pointer) {
            if(m_log_read_pointer == m_log_buffer_end_pointer) {
                m_log_read_pointer = m_log_buffer_begin_pointer;
            } else {
                m_log_read_pointer++;
            }
            m_log_buffer_overflow = true;
        }
    } else 
#endif
    {
        Serial.print( value );
    }
}

#ifdef HIM_LOG_BUFFER_ENABLE

void HimLogging::setFlushMinInterval(unsigned long min_interval)
{
    m_flush_min_interval = min_interval;
}

void HimLogging::setFlushMinThreshold(unsigned int min_threshold)
{
    m_flush_min_threshold = m_log_buffer_size * min_threshold / 100;
}

void HimLogging::setFlushMaxTime(unsigned long max_time)
{
    m_flush_max_time = max_time;
}

void HimLogging::setFlushMaxBytes(unsigned int max_bytes)
{
    m_flush_max_bytes = max_bytes;
}

unsigned int HimLogging::getFilled()
{
    if(m_log_write_pointer >= m_log_read_pointer) {
        return (m_log_write_pointer - m_log_read_pointer);
    } else {
        return (m_log_buffer_size - (m_log_read_pointer - m_log_write_pointer));
    }
}

void HimLogging::flush(bool flush_now)
{
    unsigned int count = getFilled();
    unsigned long time = micros();
    bool flush = flush_now;

    if(count > m_log_buffer_almost_full) {
        flush = true;
    }

    if((m_flush_min_threshold > 0) && (count > m_flush_min_threshold)) {
        flush = true;
    }

    if((m_flush_min_interval > 0) && ((micros() - m_last_flush_time) > m_flush_min_interval)) {
        flush = true;
    }

    if(m_log_buffer_overflow) {
        flush = true;
        Serial.println("** OF **");
    }

    flush = flush && count;
    for(count = 1; flush; count++) {

        Serial.print(*m_log_read_pointer);

        if(m_log_read_pointer == m_log_buffer_end_pointer) {
            m_log_read_pointer = m_log_buffer_begin_pointer;
        } else {
            m_log_read_pointer++;
        }

        if(m_log_read_pointer == m_log_write_pointer) {
            break;
        }

        if((m_flush_max_time > 0) && ((micros() - time) > m_flush_max_time)) {
            break;
        }

        if((m_flush_max_bytes > 0) && (count > m_flush_max_bytes)) {
            break;
        }
    }

    if(flush) {
        m_log_read_pointer = m_log_read_pointer;
        m_log_buffer_overflow = false;
        m_last_flush_time = micros();
    }
}
#endif

HimLogging HimLog = HimLogging();

#endif

