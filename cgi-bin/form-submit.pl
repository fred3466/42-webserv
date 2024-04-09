#!/usr/local/bin/perl
use URI::Escape;

$request_method = $ENV{'REQUEST_METHOD'};
if ($request_method eq "GET") {
          $form_info = $ENV{'QUERY_STRING'};
} 
 if ($request_method eq "POST") {
	  $size_of_form_information = $ENV{'CONTENT_LENGTH'};
	  read (STDIN, $form_info, $size_of_form_information);
	  
	  ($field_name, $command) = split (/=/, $form_info);
		print "Content-type: text/plain", "\n\n";
print <<'TOP';		
<html><head></head><body>
TOP
		print (uri_unescape($field_name)." = ".uri_unescape($command)." <br>");
print <<'BOTTOM';		
</body></html>
BOTTOM
 }else{
	print <<'END';
	
	<HTML>
	<HEAD><TITLE>When's your birthday?</TITLE></HEAD>
	<BODY>
	<H1>When's your birthday?</H1>
	<HR>
	<FORM ACTION="/cgi-perl/form-submit.pl" METHOD="POST">
	Birthday (in the form of mm/dd/yy): <INPUT TYPE="text" value="03/03/1997" NAME="birthday" SIZE=40>
	<P>
	<INPUT TYPE="submit" VALUE="Submit Form!">
	<INPUT TYPE="reset"  VALUE="Clear Form">
	</FORM>
	<HR>
	</BODY>
	</HTML>
	
END
}



exit (0);