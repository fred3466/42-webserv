#!/bin/bash
#request_method=$(echo "$REQUEST_METHOD")

display_form() {
    #echo "<html>"
    #echo "<body>"
    echo "<h2>Enter a string to reverse :</h2>"
    echo "<form method='POST' action=''>"
    echo "String to reverse: <input type='text' name='STR_TO_REVERSE' value='azerty'><br>"
    echo "<input type='submit' value='Submit'>"
    echo "</form>"
    #echo "</body>"
    #echo "</html>"
}

reverse_string() {
    echo "$1" | rev
}

cat dir_list_pre.html

request_method=$REQUEST_METHOD
#echo "req=[$request_method]"

if [ "$request_method" = "GET" ]; then
    display_form
elif [ "$request_method" = "POST" ]; then
    #echo "POST_DATA=$POST_DATA"
    #echo "CONTENT_LENGTH=$CONTENT_LENGTH"
    read  -N $CONTENT_LENGTH POST_DATA
    STR_TO_REVERSE=$(echo "$POST_DATA" | sed 's/.*STR_TO_REVERSE=\([^&]*\).*/\1/' | sed 's/+/%20/g' | python3 -c "import sys, urllib.parse as ul; print(ul.unquote(sys.stdin.read()))")

    reversed_STR_TO_REVERSE=$(reverse_string "$STR_TO_REVERSE")

#cat conf/dir_list_pre.html

    echo "<h2>Reversed "$STR_TO_REVERSE" :</h2>"
    echo "<p>$reversed_STR_TO_REVERSE</p>"
	echo "<br>"
	echo "<hr>"
	echo "<form action='/index.html' method='get'>"
	echo "    <button type='submit'>Return to Home</button>"
	echo "</form>"
else
    echo "<h2>Error:</h2>"
    echo "<p>Invalid request method</p>"
fi

 cat dir_list_post.html