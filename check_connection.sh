((count = 10))                            # Maximum number to try.
while [[ $count -ne 0 ]] ; do
    ping -I eth0 -c 1 8.8.8.8                      # Try once.
    rc=$?
    if [[ $rc -eq 0 ]] ; then
    (
			echo "ping is succesfull"
			curl --data "ethernet=1" http://10.42.0.1/post.php
		)
		else
		(
			echo "time out"
			curl --data "ethernet=0" http://10.42.0.1/post.php
		)
    fi
    ((count = count - 1))                  # So we don't go forever.
done

