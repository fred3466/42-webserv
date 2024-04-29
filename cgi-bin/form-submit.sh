#!/bin/bash

display_form() {
    echo "<html>"
    echo "<body>"
    echo "<h2>Enter your name:</h2>"
    echo "<form method='POST' action=''>"
    echo "Name: <input type='text' name='username'><br>"
    echo "<input type='submit' value='Submit'>"
    echo "</form>"
    echo "</body>"
    echo "</html>"
}

reverse_string() {
    echo "$1" | rev
}

request_method=$(echo "$REQUEST_METHOD")

if [ "$request_method" = "GET" ]; then
    display_form
elif [ "$request_method" = "POST" ]; then
    read POST_DATA

    username=$(echo "$POST_DATA" | sed 's/.*username=\([^&]*\).*/\1/' | sed 's/+/%20/g' | python3 -c "import sys, urllib.parse as ul; print(ul.unquote(sys.stdin.read()))")

    reversed_username=$(reverse_string "$username")

    echo "<html>"
    echo "<body>"
    echo "<h2>Reversed name:</h2>"
    echo "<p>$reversed_username</p>"
	echo "<br>"
	env
    echo "</body>"
    echo "</html>"
else
    echo "<html>"
    echo "<body>"
    echo "<h2>Error:</h2>"
    echo "<p>Invalid request method</p>"
    echo "</body>"
    echo "</html>"
fi
