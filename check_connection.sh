((count = 10))                            # Maximum number to try.
#while [[ $count -ne 0 ]] ; do
while [[ 1 ]] ; do
    ping -I eth1 -c 1 8.8.8.8                      # Try once.
    rc=$?
    if [[ $rc -eq 0 ]] ; then
    (
			echo "ping is succesfull"
			curl --data "ethernet=1" http://192.168.10.112/post.php
		)
		else
		(
			echo "time out"
			curl --data "ethernet=0" http://192.168.10.112/post.php
		)
    fi
    ((count = count - 1))                  # So we don't go forever.
sleep 5
done

