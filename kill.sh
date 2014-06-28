#!/bin/bash
	
	sudo stty echo
	sudo rmmod up_call_module
	VAL=`ps -all | grep rand_num | awk '{print($4)}'`
	kill $VAL
	
#script to kill the test_app application
