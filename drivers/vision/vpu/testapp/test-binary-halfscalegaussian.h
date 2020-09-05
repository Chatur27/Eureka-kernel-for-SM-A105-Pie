#define TASK_HALFSCALEGAUSSIAN_IN_INFO { \
{0, 0}, \
}

#define TASK_HALFSCALEGAUSSIAN_OT_INFO { \
{0, 3} \
}

#define TASK_HALFSCALEGAUSSIAN_IN {\
0x0000, 0x0000, 0x0101, 0x0101, 0x0202, 0x0202, 0x0303, 0x0303, 0x0404, 0x0404, 0x0505, 0x0505, 0x0606, 0x0606, 0x0707, 0x0707, \
0x0808, 0x0808, 0x0909, 0x0909, 0x0a0a, 0x0a0a, 0x0b0b, 0x0b0b, 0x0c0c, 0x0c0c, 0x0d0d, 0x0d0d, 0x0e0e, 0x0e0e, 0x0f0f, 0x0f0f, \
0x1010, 0x1010, 0x1111, 0x1111, 0x1212, 0x1212, 0x1313, 0x1313, 0x1414, 0x1414, 0x1515, 0x1515, 0x1616, 0x1616, 0x1717, 0x1717, \
0x1818, 0x1818, 0x1919, 0x1919, 0x1a1a, 0x1a1a, 0x1b1b, 0x1b1b, 0x1c1c, 0x1c1c, 0x1d1d, 0x1d1d, 0x1e1e, 0x1e1e, 0x1f1f, 0x1f1f, \
0x2020, 0x2020, 0x2121, 0x2121, 0x2222, 0x2222, 0x2323, 0x2323, 0x2424, 0x2424, 0x2525, 0x2525, 0x2626, 0x2626, 0x2727, 0x2727, \
0x2828, 0x2828, 0x2929, 0x2929, 0x2a2a, 0x2a2a, 0x2b2b, 0x2b2b, 0x2c2c, 0x2c2c, 0x2d2d, 0x2d2d, 0x2e2e, 0x2e2e, 0x2f2f, 0x2f2f, \
0x3030, 0x3030, 0x3131, 0x3131, 0x3232, 0x3232, 0x3333, 0x3333, 0x3434, 0x3434, 0x3535, 0x3535, 0x3636, 0x3636, 0x3737, 0x3737, \
0x3838, 0x3838, 0x3939, 0x3939, 0x3a3a, 0x3a3a, 0x3b3b, 0x3b3b, 0x3c3c, 0x3c3c, 0x3d3d, 0x3d3d, 0x3e3e, 0x3e3e, 0x3f3f, 0x3f3f, \
0x4040, 0x4040, 0x4141, 0x4141, 0x4242, 0x4242, 0x4343, 0x4343, 0x4444, 0x4444, 0x4545, 0x4545, 0x4646, 0x4646, 0x4747, 0x4747, \
0x4848, 0x4848, 0x4949, 0x4949, 0x4a4a, 0x4a4a, 0x4b4b, 0x4b4b, 0x4c4c, 0x4c4c, 0x4d4d, 0x4d4d, 0x4e4e, 0x4e4e, 0x4f4f, 0x4f4f, \
0x5050, 0x5050, 0x5151, 0x5151, 0x5252, 0x5252, 0x5353, 0x5353, 0x5454, 0x5454, 0x5555, 0x5555, 0x5656, 0x5656, 0x5757, 0x5757, \
0x5858, 0x5858, 0x5959, 0x5959, 0x5a5a, 0x5a5a, 0x5b5b, 0x5b5b, 0x5c5c, 0x5c5c, 0x5d5d, 0x5d5d, 0x5e5e, 0x5e5e, 0x5f5f, 0x5f5f, \
0x6060, 0x6060, 0x6161, 0x6161, 0x6262, 0x6262, 0x6363, 0x6363, 0x6464, 0x6464, 0x6565, 0x6565, 0x6666, 0x6666, 0x6767, 0x6767, \
0x6868, 0x6868, 0x6969, 0x6969, 0x6a6a, 0x6a6a, 0x6b6b, 0x6b6b, 0x6c6c, 0x6c6c, 0x6d6d, 0x6d6d, 0x6e6e, 0x6e6e, 0x6f6f, 0x6f6f, \
0x7070, 0x7070, 0x7171, 0x7171, 0x7272, 0x7272, 0x7373, 0x7373, 0x7474, 0x7474, 0x7575, 0x7575, 0x7676, 0x7676, 0x7777, 0x7777, \
0x7878, 0x7878, 0x7979, 0x7979, 0x7a7a, 0x7a7a, 0x7b7b, 0x7b7b, 0x7c7c, 0x7c7c, 0x7d7d, 0x7d7d, 0x7e7e, 0x7e7e, 0x7f7f, 0x7f7f, \
0x8080, 0x8080, 0x8181, 0x8181, 0x8282, 0x8282, 0x8383, 0x8383, 0x8484, 0x8484, 0x8585, 0x8585, 0x8686, 0x8686, 0x8787, 0x8787, \
0x8888, 0x8888, 0x8989, 0x8989, 0x8a8a, 0x8a8a, 0x8b8b, 0x8b8b, 0x8c8c, 0x8c8c, 0x8d8d, 0x8d8d, 0x8e8e, 0x8e8e, 0x8f8f, 0x8f8f, \
0x9090, 0x9090, 0x9191, 0x9191, 0x9292, 0x9292, 0x9393, 0x9393, 0x9494, 0x9494, 0x9595, 0x9595, 0x9696, 0x9696, 0x9797, 0x9797, \
0x9898, 0x9898, 0x9999, 0x9999, 0x9a9a, 0x9a9a, 0x9b9b, 0x9b9b, 0x9c9c, 0x9c9c, 0x9d9d, 0x9d9d, 0x9e9e, 0x9e9e, 0x9f9f, 0x9f9f, \
0xa0a0, 0xa0a0, 0xa1a1, 0xa1a1, 0xa2a2, 0xa2a2, 0xa3a3, 0xa3a3, 0xa4a4, 0xa4a4, 0xa5a5, 0xa5a5, 0xa6a6, 0xa6a6, 0xa7a7, 0xa7a7, \
0xa8a8, 0xa8a8, 0xa9a9, 0xa9a9, 0xaaaa, 0xaaaa, 0xabab, 0xabab, 0xacac, 0xacac, 0xadad, 0xadad, 0xaeae, 0xaeae, 0xafaf, 0xafaf, \
0xb0b0, 0xb0b0, 0xb1b1, 0xb1b1, 0xb2b2, 0xb2b2, 0xb3b3, 0xb3b3, 0xb4b4, 0xb4b4, 0xb5b5, 0xb5b5, 0xb6b6, 0xb6b6, 0xb7b7, 0xb7b7, \
0xb8b8, 0xb8b8, 0xb9b9, 0xb9b9, 0xbaba, 0xbaba, 0xbbbb, 0xbbbb, 0xbcbc, 0xbcbc, 0xbdbd, 0xbdbd, 0xbebe, 0xbebe, 0xbfbf, 0xbfbf, \
0xc0c0, 0xc0c0, 0xc1c1, 0xc1c1, 0xc2c2, 0xc2c2, 0xc3c3, 0xc3c3, 0xc4c4, 0xc4c4, 0xc5c5, 0xc5c5, 0xc6c6, 0xc6c6, 0xc7c7, 0xc7c7, \
0xc8c8, 0xc8c8, 0xc9c9, 0xc9c9, 0xcaca, 0xcaca, 0xcbcb, 0xcbcb, 0xcccc, 0xcccc, 0xcdcd, 0xcdcd, 0xcece, 0xcece, 0xcfcf, 0xcfcf, \
0xd0d0, 0xd0d0, 0xd1d1, 0xd1d1, 0xd2d2, 0xd2d2, 0xd3d3, 0xd3d3, 0xd4d4, 0xd4d4, 0xd5d5, 0xd5d5, 0xd6d6, 0xd6d6, 0xd7d7, 0xd7d7, \
0xd8d8, 0xd8d8, 0xd9d9, 0xd9d9, 0xdada, 0xdada, 0xdbdb, 0xdbdb, 0xdcdc, 0xdcdc, 0xdddd, 0xdddd, 0xdede, 0xdede, 0xdfdf, 0xdfdf, \
0xe0e0, 0xe0e0, 0xe1e1, 0xe1e1, 0xe2e2, 0xe2e2, 0xe3e3, 0xe3e3, 0xe4e4, 0xe4e4, 0xe5e5, 0xe5e5, 0xe6e6, 0xe6e6, 0xe7e7, 0xe7e7, \
0xe8e8, 0xe8e8, 0xe9e9, 0xe9e9, 0xeaea, 0xeaea, 0xebeb, 0xebeb, 0xecec, 0xecec, 0xeded, 0xeded, 0xeeee, 0xeeee, 0xefef, 0xefef, \
0xf0f0, 0xf0f0, 0xf1f1, 0xf1f1, 0xf2f2, 0xf2f2, 0xf3f3, 0xf3f3, 0xf4f4, 0xf4f4, 0xf5f5, 0xf5f5, 0xf6f6, 0xf6f6, 0xf7f7, 0xf7f7, \
0xf8f8, 0xf8f8, 0xf9f9, 0xf9f9, 0xfafa, 0xfafa, 0xfbfb, 0xfbfb, 0xfcfc, 0xfcfc, 0xfdfd, 0xfdfd, 0xfefe, 0xfefe, 0xffff, 0xffff, \
}

#define TASK_HALFSCALEGAUSSIAN_OT { \
0x0808, 0x0909, 0x0a0a, 0x0b0b, 0x0c0c, 0x0d0d, 0x0e0e, 0x0b0f, 0x1818, 0x1919, 0x1a1a, 0x1b1b, 0x1c1c, 0x1d1d, 0x1e1e, 0x171f, \
0x2828, 0x2929, 0x2a2a, 0x2b2b, 0x2c2c, 0x2d2d, 0x2e2e, 0x232f, 0x3838, 0x3939, 0x3a3a, 0x3b3b, 0x3c3c, 0x3d3d, 0x3e3e, 0x2f3f, \
0x4848, 0x4949, 0x4a4a, 0x4b4b, 0x4c4c, 0x4d4d, 0x4e4e, 0x3b4f, 0x5858, 0x5959, 0x5a5a, 0x5b5b, 0x5c5c, 0x5d5d, 0x5e5e, 0x475f, \
0x6868, 0x6969, 0x6a6a, 0x6b6b, 0x6c6c, 0x6d6d, 0x6e6e, 0x536f, 0x7878, 0x7979, 0x7a7a, 0x7b7b, 0x7c7c, 0x7d7d, 0x7e7e, 0x5f7f, \
0x8888, 0x8989, 0x8a8a, 0x8b8b, 0x8c8c, 0x8d8d, 0x8e8e, 0x6b8f, 0x9898, 0x9999, 0x9a9a, 0x9b9b, 0x9c9c, 0x9d9d, 0x9e9e, 0x779f, \
0xa8a8, 0xa9a9, 0xaaaa, 0xabab, 0xacac, 0xadad, 0xaeae, 0x83af, 0xb8b8, 0xb9b9, 0xbaba, 0xbbbb, 0xbcbc, 0xbdbd, 0xbebe, 0x8fbf, \
0xc8c8, 0xc9c9, 0xcaca, 0xcbcb, 0xcccc, 0xcdcd, 0xcece, 0x9bcf, 0xd8d8, 0xd9d9, 0xdada, 0xdbdb, 0xdcdc, 0xdddd, 0xdede, 0xa7df, \
0xe8e8, 0xe9e9, 0xeaea, 0xebeb, 0xecec, 0xeded, 0xeeee, 0xb3ef, 0xb8b8, 0xb8b8, 0xb9b9, 0xbaba, 0xbbbb, 0xbbbb, 0xbcbc, 0x8dbd, \
}
