#!/usr/bin/php-cgi
<?php
// Explicitly turn off output buffering
while (@ob_end_flush());    

// Simple CGI script to return the request method and query parameters
header("Content-Type: text/plain");
echo "Hello, CGI World!\n";
echo "Request Method: " . $_SERVER['REQUEST_METHOD'] . "\n";
if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    echo "Query Parameters: \n";
    print_r($_GET);
} elseif ($_SERVER['REQUEST_METHOD'] == 'POST') {
    echo "Post Parameters: \n";
    print_r($_POST);
}

// Flush (send) the output buffer and turn off output buffering
ob_end_flush();
?>
