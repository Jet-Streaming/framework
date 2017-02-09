@echo off

cd %~dp0

@rem gyp.exe all.gyp -Icommon.gypi -Iconfig.gypi --depth=. -f msvs -Gmsvs_version=2015

set configure_flags=--debug
set target_arch=x64
set TAG=

@rem Generate the VS project.
echo configure %configure_flags% --dest-cpu=%target_arch% --tag=%TAG% --without-ssl --without-inspector --without-perfctr --without-etw --shared
python configure %configure_flags% --dest-cpu=%target_arch% --tag=%TAG% --without-ssl --without-inspector --without-perfctr --without-etw --shared

@if errorlevel 1 @pause