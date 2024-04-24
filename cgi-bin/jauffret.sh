#!/bin/sh -f

echo "Content-type: text/html"
echo ""

echo '<html>'
echo '<head>'
echo '<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">'
echo '<title>TestLog</title>'
echo '<body>'
echo '<h1>TestLog</h1>'
echo '<hr>'
env
ls -la
echo '</body>'
echo '</html>'
exit 0