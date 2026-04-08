/***********************************************************************
* FILENAME:
*       him_log_config.h
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

#ifndef _HIM_LOG_CONFIG_H_
#define _HIM_LOG_CONFIG_H_



#define HIM_LOG_ENABLE          // general enable flag for comment functionality - comment out to disable

#define HIM_LOG_DEBUG_ENABLE    // enable flag for debug macros - comment out to disable
#define HIM_LOG_INFO_ENABLE     // enable flag for info macros - comment out to disable
#define HIM_LOG_WARN_ENABLE     // enable flag for warn macros - comment out to disable
#define HIM_LOG_ERROR_ENABLE    // enable flag for error macros - comment out to disable

#define HIM_LOG_BUFFER_ENABLE   // enable flag for buffered logging - comment out to disable
#define HIM_LOG_BUFFER_SIZE     128



#endif /* _HIM_LOG_CONFIG_H_ */
