#!/usr/local/bin/perl
use URI::Escape;

$request_method = $ENV{'REQUEST_METHOD'};
if ($request_method eq "GET") {
          $form_info = $ENV{'QUERY_STRING'};
} 
 if ($request_method eq "POST") {
#		print "Content-type: text/plain", "\n\n";
print <<'TOP';		
<html><head></head><body>
TOP
	  $size_of_form_information = $ENV{'CONTENT_LENGTH'};
	  read (STDIN, $buffer, $size_of_form_information); 
	  @pairs = split(/&/, $buffer); 
		foreach $pair (@pairs)  
		{ 
		    ($name, $value) = split(/=/, $pair); 
#		    $value =~ tr/+/ /; 
#		    $value =~ s/%([a-fA-F0-9] [a-fA-F0-9])/pack("C", hex($1))/eg; 
#		    $value =~ s/~!/ ~!/g; 
#		    $FORM{$name} = $value; 
		print (uri_unescape($name)." = ".uri_unescape($value)." <br>");
		} 
#	  read (STDIN, $form_info, $size_of_form_information);
#	  ($field_name, $command) = split (/=/, $form_info);
	  
#	print "Hello ".uri_unescape($name)."<br>";
#	print (uri_unescape($field_name)." = ".uri_unescape($command)." <br>");
print <<'BOTTOM';
<a href="">RETOUR</a>		
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
	Nom: <INPUT TYPE="text" value="fred" NAME="nom" SIZE=40>
	<P>
	Birthday (in the form of mm/dd/yy): <INPUT TYPE="text" value="03/03/1827" NAME="birthday" SIZE=40>
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