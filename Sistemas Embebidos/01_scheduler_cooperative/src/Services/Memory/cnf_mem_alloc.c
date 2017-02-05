/****************************************************************************************************/
/**
\file       cnf_mem_alloc.c
\brief      Configuration of Dynamic Memory Allocation services
\author     Abraham Tezmol
\version    1.0
\date       21/09/2013
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Dynamic Memory allocation configuration definitions */
#include    "cnf_mem_alloc.h"

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/

/* Dynamic memory allocation configuration for FAR memory */
const tPagedRAMpages_config PagedRamPages_config[] =
{
    {RAM_F0, (uint8_t *)0x20450000, (uint8_t *)0x20450FFF},
    {RAM_F1, (uint8_t *)0x20451000, (uint8_t *)0x20451FFF},
    {RAM_F2, (uint8_t *)0x20452000, (uint8_t *)0x20452FFF},
    {RAM_F3, (uint8_t *)0x20453000, (uint8_t *)0x20453FFF},
    {RAM_F4, (uint8_t *)0x20454000, (uint8_t *)0x20454FFF},
    {RAM_F5, (uint8_t *)0x20455000, (uint8_t *)0x20455FFF},
    {RAM_F6, (uint8_t *)0x20456000, (uint8_t *)0x20456FFF},
    {RAM_F7, (uint8_t *)0x20457000, (uint8_t *)0x20457FFF},
    {RAM_F8, (uint8_t *)0x20458000, (uint8_t *)0x20458FFF},
    {RAM_F9, (uint8_t *)0x20459000, (uint8_t *)0x20459FFF},
    {RAM_FA, (uint8_t *)0x2045A000, (uint8_t *)0x2045AFFF},
    {RAM_FB, (uint8_t *)0x2045B000, (uint8_t *)0x2045BFFF},
    {RAM_FC, (uint8_t *)0x2045C000, (uint8_t *)0x2045CFFF},
    {RAM_FD, (uint8_t *)0x2045D000, (uint8_t *)0x2045DFFF},
    {RAM_FE, (uint8_t *)0x2045E000, (uint8_t *)0x2045EFFF},
    {RAM_FF, (uint8_t *)0x2045F000, (uint8_t *)0x2045FFFF}
};

const tMemAlloc_config  DynamicMemAlloc_config[] =
{
    {
        sizeof(PagedRamPages_config)/sizeof(PagedRamPages_config[0]),    /* number of configured RAM pages */
        &PagedRamPages_config[0]                                         /* RAM pages configuration structure */
    }
};
/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide private FUNCTIONS
*****************************************************************************************************/


/*****************************************************************************************************
* Code of public FUNCTIONS
*****************************************************************************************************/


/***************************************************************************************************/
