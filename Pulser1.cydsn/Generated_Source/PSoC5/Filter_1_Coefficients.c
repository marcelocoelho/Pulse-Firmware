#include "Filter_1.h"
#include "Filter_1_PVT.h"


/*******************************************************************************
* ChannelA filter coefficients.
* Filter Type is: Biquad
*******************************************************************************/

/* Number of Biquad sections are: 8 */

uint8 ChannelABiquadCoefficients[] = 
{
 /* Coefficients of Section 0 */
 0x48, 0x97, 0x07, 0x00, /* Section(0)_A0, 0.118608474731445 */

 0x70, 0xD1, 0xF0, 0x00, /* Section(0)_A1, -0.237216949462891 */

 0x48, 0x97, 0x07, 0x00, /* Section(0)_A2, 0.118608474731445 */

 0x4F, 0xEC, 0x41, 0x00, /* Section(0)_B1, -1.03004813194275 */

 0xC7, 0x88, 0xDD, 0x00, /* Section(0)_B2, 0.538526773452759 */

 /* Coefficients of Section 1 */
 0x25, 0x67, 0x1E, 0x00, /* Section(1)_A0, 0.475045442581177 */

 0xB6, 0x31, 0xC3, 0x00, /* Section(1)_A1, -0.950090885162354 */

 0x25, 0x67, 0x1E, 0x00, /* Section(1)_A2, 0.475045442581177 */

 0x8D, 0x12, 0x4E, 0x00, /* Section(1)_B1, -1.21988224983215 */

 0x8D, 0x4B, 0xD7, 0x00, /* Section(1)_B2, 0.636013746261597 */

 /* Coefficients of Section 2 */
 0x11, 0xE9, 0x07, 0x00, /* Section(2)_A0, 0.123600244522095 */

 0x22, 0xD2, 0x0F, 0x00, /* Section(2)_A1, 0.247200489044189 */

 0x11, 0xE9, 0x07, 0x00, /* Section(2)_A2, 0.123600244522095 */

 0x22, 0xC8, 0x35, 0x00, /* Section(2)_B1, -0.840340137481689 */

 0xF5, 0x12, 0xE0, 0x00, /* Section(2)_B2, 0.49884295463562 */

 /* Coefficients of Section 3 */
 0x25, 0xED, 0x08, 0x00, /* Section(3)_A0, 0.139474153518677 */

 0x4A, 0xDA, 0x11, 0x00, /* Section(3)_A1, 0.278948307037354 */

 0x25, 0xED, 0x08, 0x00, /* Section(3)_A2, 0.139474153518677 */

 0x93, 0x06, 0x2C, 0x00, /* Section(3)_B1, -0.687901258468628 */

 0x10, 0x71, 0xDD, 0x00, /* Section(3)_B2, 0.539974212646484 */

 /* Coefficients of Section 4 */
 0x99, 0x0A, 0x1C, 0x00, /* Section(4)_A0, 0.438146829605103 */

 0xCD, 0xEA, 0xC7, 0x00, /* Section(4)_A1, -0.876293897628784 */

 0x99, 0x0A, 0x1C, 0x00, /* Section(4)_A2, 0.438146829605103 */

 0x7D, 0x09, 0x59, 0x00, /* Section(4)_B1, -1.39120411872864 */

 0x5C, 0x0D, 0xCF, 0x00, /* Section(4)_B2, 0.764809608459473 */

 /* Coefficients of Section 5 */
 0xC8, 0x83, 0x0A, 0x00, /* Section(5)_A0, 0.16429328918457 */

 0x8F, 0x07, 0x15, 0x00, /* Section(5)_A1, 0.328586339950562 */

 0xC8, 0x83, 0x0A, 0x00, /* Section(5)_A2, 0.16429328918457 */

 0x08, 0x51, 0x26, 0x00, /* Section(5)_B1, -0.598695755004883 */

 0xAB, 0x84, 0xD5, 0x00, /* Section(5)_B2, 0.663777589797974 */

 /* Coefficients of Section 6 */
 0x57, 0x60, 0x08, 0x00, /* Section(6)_A0, 0.130880117416382 */

 0xAF, 0xC0, 0x10, 0x00, /* Section(6)_A1, 0.261760473251343 */

 0x57, 0x60, 0x08, 0x00, /* Section(6)_A2, 0.130880117416382 */

 0x6B, 0x9D, 0x25, 0x00, /* Section(6)_B1, -0.587733030319214 */

 0x76, 0x79, 0xC8, 0x00, /* Section(6)_B2, 0.867586612701416 */

 /* Coefficients of Section 7 */
 0x2C, 0xE4, 0x29, 0x00, /* Section(7)_A0, 2.61820602416992 */

 0xA8, 0x37, 0xAC, 0x00, /* Section(7)_A1, -5.23641204833984 */

 0x2C, 0xE4, 0x29, 0x00, /* Section(7)_A2, 2.61820602416992 */

 0xD4, 0xE8, 0x62, 0x00, /* Section(7)_B1, -1.54546070098877 */

 0xE9, 0x68, 0xC5, 0x00, /* Section(7)_B2, 0.915471792221069 */
};

