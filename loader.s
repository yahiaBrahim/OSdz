extern kmain


MULTIBOOT_PAGE_ALIGN   equ 1<<0 ; 00000000 00000000 00000000 00000001
MULTIBOOT_MEMORY_INFO  equ 1<<1 ; 00000000 00000000 00000000 00000010



MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO

;Flags = 00000000 00000000 00000000 00000011
; According to the multiboot header : bit 0 & bit 1 are set
; that is mean : 
; 1) boot modules loaded along with the operating system must be aligned on page (4KB).
; 2)  information on available memory must be included.

MULTIBOOT_CHECKSUM     equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
; Checksum to confirm that the others fields did not corrupted.



section .text
align 4 ; Aligned by 4 bytes

multiboot:
  
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM



global start


start:
  cli
  mov esp, $kernel_stack
  push ebx       ; Pointer to Multiboot header 
  push eax       ; MAGIC Value.
  call kmain
  jmp $
  hlt


section .bss
   resb 1*1024*1024 ; 10KB of memory reserved
kernel_stack: