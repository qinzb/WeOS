/**
 * @file sensorboards.h
 * @brief Top level include file for all the sensor boards supported by a platform
 * @author Ram Kumar {ram@ee.ucla.edu}
 */

#ifndef _SENSORBOARDS_H_
#define _SENSORBOARDS_H_

#if defined MICASB
#include <sensordrivers/micasb/micasb_sensortypes.h>

#elif defined MTS400SB
#include <sensordrivers/mts400/mts400_sensortypes.h>

#else
#include <sensordrivers/mts400/mts400_sensortypes.h>
//#warning "No valid sensor board defined for the Micaz configuration. Supported boards: MICASB, MTS400SB. Including MICASB by Default"
#endif

#endif //_SENSORBOARDS_H_
