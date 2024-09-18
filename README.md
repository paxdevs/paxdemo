# Demo project for PAX devices

## Prerequisites
Install docker for the build environment.

## Building
Run `docker-build.sh` to build. The output file will be at `build/libpaxdemo.so`

## Uploading the program

Before uploading the program, you may wish to backup the original libosal.so file like so:
```
python3 prolin-xcb-client/client.py ACM0 pull /data/app/MAINAPP/lib/libosal.so libosal-original.so
```

Then, run these commands to upload the required files:
```
python3 prolin-xcb-client/client.py ACM0 push build/libpaxtest.so /data/app/MAINAPP/lib/libosal.so
python3 prolin-xcb-client/client.py ACM0 push video.frames /data/app/MAINAPP/lib/video.frames
```

Either restart the device of press X on the main menu to execute the program.

