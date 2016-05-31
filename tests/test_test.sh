test
test -d /home/ubuntu/workspace
test -f /home/ubuntu/workspace
test -e /home/ubuntu/workspace
test  /home/ubuntu/workspace
test -d /home/ubuntu/workspace && echo hi || ls
test -f /home/ubuntu/workspace && exit
test -e /home/ubuntu/workspace || ls && exit
test  /home/ubuntu/workspace && ls; #this is a comment
[ -e /home/ubuntu/workspace || ls && exit]
[ -d /home/ubuntu/workspace ]
[ -f /home/ubuntu/workspace ]
[ -e /home/ubuntu/workspace ]
