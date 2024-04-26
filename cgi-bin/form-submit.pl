#!/usr/local/bin/perl
use URI::Escape;
use strict;

print "Content-type: text/html\n\n";

my $request_method = $ENV{'REQUEST_METHOD'};
my $form_info = $request_method eq "GET" ? $ENV{'QUERY_STRING'} : "";
my $size_of_form_information = $request_method eq "POST" ? $ENV{'CONTENT_LENGTH'} : 0;
my $buffer = '';

# Common style for both pages
my $style = <<"STYLE";
<style>
    body {
        font-family: Arial, sans-serif;
        background-color: #76ADEC;
        text-align: center;
        padding-top: 50px;
        color: white;
        background-size: cover;
        background-repeat: no-repeat;
        background-attachment: fixed;
    }
    .container {
        background: rgba(0, 0, 0, 0.5);
        padding: 2rem;
        border-radius: 16px;
        width: 50%;
        margin: auto;
    }
    input[type=text], button, input[type=submit], input[type=reset] {
        width: 90%;
        padding: 10px;
        margin-top: 10px;
        border-radius: 5px;
        border: 1px solid #ccc;
    }
    button, input[type=submit], input[type=reset] {
        background-color: #156CEF;
        color: white;
        cursor: pointer;
    }
    button:hover, input[type=submit]:hover, input[type=reset]:hover {
        background-color: #5e2dbb;
    }
</style>
STYLE

if ($request_method eq "POST") {
    read(STDIN, $buffer, $size_of_form_information); 
    my @pairs = split(/&/, $buffer); 
    
    print <<"TOP";
<html>
<head>
    <title>Form Submitted</title>
    $style
</head>
<body>
<div class="container">
<h1>Form Submitted</h1>
<hr>
TOP

    foreach my $pair (@pairs) {
        my ($name, $value) = split(/=/, $pair); 
        print uri_unescape($name) . " = " . uri_unescape($value) . "<br>";
    }

    print <<"BOTTOM";
<hr>
<form action="/index.html" method="get">
    <button type="submit">Return to Home</button>
</form>

</div>        
</body></html>
BOTTOM

} else {
    print <<"FORM";
    <html>
    <head>
        <title>When's your birthday?</title>
        $style
    </head>
    <body>
    <div class="container">
        <h1>When's your birthday?</h1>
        <hr>
        <form action="/cgi-perl/form-submit.pl" method="POST">
            Name: <input type="text" name="nom" value="fred" size="40"><br>
            Birthday (in the form of mm/dd/yy): <input type="text" name="birthday" value="03/03/1827" size="40"><br>
            <input type="submit" value="Submit Form!">
            <input type="reset" value="Clear Form">
        </form>
        <hr>
    </div>
    </body>
    </html>
FORM
}

exit(0);


# #!/usr/local/bin/perl
# use URI::Escape;

# $request_method = $ENV{'REQUEST_METHOD'};
# if ($request_method eq "GET") {
#           $form_info = $ENV{'QUERY_STRING'};
# } 
#  if ($request_method eq "POST") {
# #		print "Content-type: text/plain", "\n\n";
# print <<'TOP';		
# <html><head></head><body>
# TOP
# 	  $size_of_form_information = $ENV{'CONTENT_LENGTH'};
# 	  read (STDIN, $buffer, $size_of_form_information); 
# 	  @pairs = split(/&/, $buffer); 
# 		foreach $pair (@pairs)  
# 		{ 
# 		    ($name, $value) = split(/=/, $pair); 
# #		    $value =~ tr/+/ /; 
# #		    $value =~ s/%([a-fA-F0-9] [a-fA-F0-9])/pack("C", hex($1))/eg; 
# #		    $value =~ s/~!/ ~!/g; 
# #		    $FORM{$name} = $value; 
# 		print (uri_unescape($name)." = ".uri_unescape($value)." <br>");
# 		} 
# #	  read (STDIN, $form_info, $size_of_form_information);
# #	  ($field_name, $command) = split (/=/, $form_info);
	  
# #	print "Hello ".uri_unescape($name)."<br>";
# #	print (uri_unescape($field_name)." = ".uri_unescape($command)." <br>");
# print <<'BOTTOM';
# <a href="">RETOUR</a>		
# </body></html>
# BOTTOM
#  }else{
# 	print <<'END';
	
# 	<HTML>
# 	<HEAD><TITLE>When's your birthday?</TITLE></HEAD>
# 	<BODY>
# 	<H1>When's your birthday?</H1>
# 	<HR>
# 	<FORM ACTION="/cgi-perl/form-submit.pl" METHOD="POST">
# 	Nom: <INPUT TYPE="text" value="fred" NAME="nom" SIZE=40>
# 	<P>
# 	Birthday (in the form of mm/dd/yy): <INPUT TYPE="text" value="03/03/1827" NAME="birthday" SIZE=40>
# 	<P>
# 	<INPUT TYPE="submit" VALUE="Submit Form!">
# 	<INPUT TYPE="reset"  VALUE="Clear Form">
# 	</FORM>
# 	<HR>
# 	</BODY>
# 	</HTML>
# END
# }



# exit (0);
