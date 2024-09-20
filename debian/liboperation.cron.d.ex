#
# Regular cron jobs for the liboperation package
#
0 4	* * *	root	[ -x /usr/bin/liboperation_maintenance ] && /usr/bin/liboperation_maintenance
