#![feature(core)]
#![feature(core_str_ext)]
#![feature(libc)]
#![feature(no_std)]
#![feature(asm)]

#![no_std]

extern crate libc;

#[macro_use]
mod macros;
mod raw;

pub enum file {}
pub enum loff_t {}

#[no_mangle]
pub fn rdrand_read(f: *mut file,
                      buf: *mut libc::c_char,
                      len: usize,
                      off: *mut loff_t) -> isize {
  unsafe {
    for i in 0..len/4 {
      asm!("rdrand $0"
        : "=r"(*(buf.offset((i as isize)*4) as *mut i32))
        : : : "volatile"
       );
    }
    if len%4 != 0 {
      let x: i32;
      asm!("rdrand $0"
        : "=r"(x)
        : : : "volatile"
      );
      let offs = (len as isize) - (len as isize)%4;
      *buf.offset(offs+0) = (x >> 0) as i8;
      *buf.offset(offs+1) = (x >> 8) as i8;
      *buf.offset(offs+2) = (x >> 16) as i8;
      *buf.offset(offs+3) = (x >> 24) as i8;
    }
  }
  return (len as isize);
}

