all:
	cc -g -o rangeSencer range_sencer.c ibeacon.c -lbluetooth -lwiringPi
clean:
	rm -f rangeSencer
