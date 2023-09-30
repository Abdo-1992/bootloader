/******************************************************************************
 *
 * project.ld - Linker configuration file for project.
 *
 * Copyright (c) 2013-2017 Texas Instruments Incorporated.  All rights reserved.
 * Software License Agreement
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the  
 *   distribution.
 * 
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * This is part of revision 2.1.4.178 of the Tiva Firmware Development Package.
 *
 *****************************************************************************/
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))    /*This directive ensures that the contents of the .isr_vector section (from input files) are kept in the output, even if they appear to be unused*/
        *(.text*)               /*This collects all .text sections from the input files and concatenates them*/
        *(.rodata*)             /*This collects all .rodata (read-only data) sections from the input files and concatenates them.*/
        . = ALIGN(4);           /*This aligns the current location counter to a 4-byte boundary. This ensures that the following data and instructions are placed at addresses divisible by 4*/
    } > FLASH

    .init : AT(ADDR(.text) + SIZEOF(.text))
    {
        *(.init)
        . = ALIGN(4);
    } > FLASH

    .init_array :
    {
        __init_array_flash_start = LOADADDR(.init_array);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    } > FLASH

    .data : AT(ADDR(.init_array) + SIZEOF(.init_array))
    {
        _data = .;
        _ldata = LOADADDR (.data);
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM


}
