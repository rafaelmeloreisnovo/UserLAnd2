ENTRY(_start)

MEMORY
{
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
  .text.boot : {
    KEEP(*(.text.boot))
  } > RAM

  .text : {
    *(.text*)
  } > RAM

  /DISCARD/ : {
    *(.comment*)
    *(.note*)
  }
}
