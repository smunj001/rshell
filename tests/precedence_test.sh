( echo hello && ls ) && exit
( echo hello && ls ) && ( ls || exit )
( pwd || ls ) || ( date && zyv) || ls -a #this is a comment
( echo A && ls )
date; ( garbage ); 
( echo A && ( echo B && echo C )); ( pwd || date && ( ls || xyz ));
test -d /home/ubuntu/workspace && echo hi || ls && ( echo A )
(;)
(&&)
(||)
((ls || ;) && (date || echo Hello))
