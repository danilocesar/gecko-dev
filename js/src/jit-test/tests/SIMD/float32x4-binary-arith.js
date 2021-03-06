load(libdir + 'simd.js');

setJitCompilerOption("ion.warmup.trigger", 50);

function maxNum(x, y) {
    if (x != x)
        return y;
    if (y != y)
        return x;
    return Math.max(x, y);
}

function minNum(x, y) {
    if (x != x)
        return y;
    if (y != y)
        return x;
    return Math.min(x, y);
}

function f() {
    var f1 = SIMD.float32x4(1, 2, 3, 4);
    var f2 = SIMD.float32x4(4, 3, 2, 1);
    for (var i = 0; i < 150; i++) {
        assertEqX4(SIMD.float32x4.div(f1, f2), binaryX4((x, y) => x / y, f1, f2));
        assertEqX4(SIMD.float32x4.min(f1, f2), binaryX4(Math.min, f1, f2));
        assertEqX4(SIMD.float32x4.max(f1, f2), binaryX4(Math.max, f1, f2));
        assertEqX4(SIMD.float32x4.minNum(f1, f2), binaryX4(minNum, f1, f2));
        assertEqX4(SIMD.float32x4.maxNum(f1, f2), binaryX4(maxNum, f1, f2));
    }
}

f();
