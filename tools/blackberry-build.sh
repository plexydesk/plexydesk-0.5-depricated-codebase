export PATH=$QNX_HOST/usr/bin:$PATH
export CC="$QNX_HOST/usr/bin/qcc -V4.6.3,gcc_ntox86v7le "
export CFLAGS="-V4.6.3,gcc_ntox86v7le -g "
export CPP="$QNX_HOST/usr/bin/qcc -V4.6.3,gcc_ntox86v7le_cpp -E"
export LD="$QNX_HOST/usr/bin/ntoarmv7-ld "
export RANLIB="$QNX_HOST/usr/bin/ntoarmv7-ranlib "
export AR="$QNX_HOST/usr/bin/ntoarmv7-ar"
export LDFLAGS="-L$QNX_TARGET/armle-v7/usr/lib -L$QNX_TARGET/armle-v7/lib"
