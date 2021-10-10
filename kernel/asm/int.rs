#![feature(asm)]

static mut CLI_COUNT: u32 = 1;

#[no_mangle]
pub fn cli() {
    unsafe {
        asm!("cli");
        CLI_COUNT += 1;
    }
}

#[no_mangle]
pub fn sti() {
    unsafe {
        CLI_COUNT -= 1;
        asm!("sti");
    }
}
