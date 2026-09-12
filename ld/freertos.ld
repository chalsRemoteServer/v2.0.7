/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-ft32")
OUTPUT_ARCH(ft32)
SEARCH_DIR("/opt/ft32/ft32-elf/lib");
/* Allow the command line to override the memory region sizes.  */
__PMSIZE    = DEFINED(__PMSIZE)    ? __PMSIZE    : 256K;
__RAMSIZE   = DEFINED(__RAMSIZE)   ? __RAMSIZE   : 64K;
__DLOGSIZE  = DEFINED(__DLOGSIZE)  ? __DLOGSIZE  : 0x1000;
__APPFLASH  = __PMSIZE - __DLOGSIZE;
__DLOGORIG  = __APPFLASH;

MEMORY
{
  flash_app (rx)   : ORIGIN = 0x0,        LENGTH = __APPFLASH
  dlog      (rx)   : ORIGIN = __DLOGORIG, LENGTH = __DLOGSIZE
  ram       (rw!x) : ORIGIN = 0x800000,   LENGTH = __RAMSIZE
}

SECTIONS
{
  .text :
  {
    *(.crt0*)
    *(.text*)
    *(.strings)
    *(._pm*)
    *(.init)
    *(.fini)
    . = ALIGN(4);
     _etext = . ;
  }  > flash_app
  
  .tors :
  {
    ___ctors = . ;
    *(.ctors)
    ___ctors_end = . ;
    ___dtors = . ;
    *(.dtors)
    . = ALIGN(4);
    ___dtors_end = . ;
  } > ram
  .data	  :
  {
  	. = ALIGN(4);
    __data = .;
    *(.data)
	*(.data*)
	*(.rodata)
    *(.rodata*)
  . = ALIGN(4);
     _edata = . ;
  }  > ram AT>flash_app
  .bss  SIZEOF(.data) + ADDR(.data) (NOLOAD) :
  {
     _bss_start = . ;
    *(.bss)
    *(COMMON)
     _end = . ;
  }  > ram
   __data_load_start = LOADADDR(.data);
   __data_load_end = __data_load_start + SIZEOF(.data);

    .dlog (NOLOAD) :
  {
    . = ALIGN(4);
    __dlog_partition = .;
    . = . + __DLOGSIZE;
  } > dlog
     
   
  .stab 0 (NOLOAD) :
  {
    *(.stab)
  }
  .stabstr 0 (NOLOAD) :
  {
    *(.stabstr)
  }
}