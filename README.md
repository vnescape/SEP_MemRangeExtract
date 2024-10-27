# SEP_MemRangeExtract
A tool to extract the physical memory regions of a decrypted Apple Secure Enclave Processor image


# Usage
`Usage: ./SEP_MemRangeExtract <filepath to decrypted Apple SEP image>`
# Example
```
./SEP_MemRangeExtract sep-firmware.d63.RELEASE.im4p.dec
Read struct: ctx=nubs, name=dram, start=0x23df00000, size=0x20000, unk=0x6
Read struct: ctx=nubs, name=dram, start=0x23df00000, size=0x20000, unk=0x6
Read struct: ctx=nubs, name=dram, start=0x23df00000, size=0x20000, unk=0x6
Read struct: ctx=ctr0, name=dram, start=0x23d050000, size=0x4000, unk=0x6
Read struct: ctx=ctr1, name=dram, start=0x23d054000, size=0x4000, unk=0x6
Read struct: ctx=scr , name=dram, start=0x23d2d0000, size=0x8000, unk=0x6
Read struct: ctx=pln0, name=dram, start=0x200000000, size=0x8000, unk=0x6
Read struct: ctx=tnpr, name=dram, start=0x200100000, size=0x4000, unk=0x6
Read struct: ctx=npgr, name=dram, start=0x200140000, size=0x4000, unk=0x6
Read struct: ctx=PMSC, name=XPRT, start=0x23d2dc000, size=0x4000, unk=0x6
Read struct: ctx=MSCR, name=XPRT, start=0x23d2b8000, size=0x4000, unk=0x6
Read struct: ctx=AXI2, name=EP0D, start=0x25f000000, size=0x4000, unk=0x6
Read struct: ctx=FUSE, name=XPRT, start=0x23d2c8000, size=0x10000, unk=0x6
Read struct: ctx=MISC, name=XPRT, start=0x23d2c4000, size=0x4000, unk=0x6
Read struct: ctx=BASE, name=PMGR, start=0x25d000000, size=0x40000, unk=0x6
Read struct: ctx=REGS, name=TRNG, start=0x25d180000, size=0x10000, unk=0x6
Read struct: ctx=BASE, name=KEY , start=0x25d1c0000, size=0x10000, unk=0x6
Read struct: ctx=FCFG, name=KEY , start=0x25d440000, size=0x10000, unk=0x6
Read struct: ctx=BASE, name=MONI, start=0x25d3c0000, size=0x40000, unk=0x6
Read struct: ctx=THRM, name=MONI, start=0x25d400000, size=0x10000, unk=0x6
Read struct: ctx=BASE, name=EISP, start=0x25c800000, size=0x280000, unk=0x16
Read struct: ctx=HMAC, name=EISP, start=0x25caa0000, size=0x4000, unk=0x16
Read struct: ctx=ANE , name=EISP, start=0x26a000000, size=0x1f00000, unk=0x16
Read struct: ctx=ANEM, name=EISP, start=0x26b878000, size=0x4000, unk=0x16
Read struct: ctx=ANET, name=EISP, start=0x26b8b8000, size=0x4000, unk=0x16
Read struct: ctx=ASPT, name=EISP, start=0x25d008000, size=0x4000, unk=0x16
Read struct: ctx=PMSS, name=EISP, start=0x23b750000, size=0x4000, unk=0x16
Read struct: ctx=MBOX, name=AKF , start=0x25e400000, size=0x8000, unk=0x6
Read struct: ctx=TUNA, name=AKF , start=0x25e000000, size=0x200000, unk=0x6
```