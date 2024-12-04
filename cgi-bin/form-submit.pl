#!/usr/local/bin/perl
use URI::Escape;
#use strict;
#print "Content-Type: text/html;\n\n";

my $request_method = $ENV{'REQUEST_METHOD'};
my $form_info = $request_method eq "GET" ? $ENV{'QUERY_STRING'} : "";
my $size_of_form_information = $request_method eq "POST" ? $ENV{'CONTENT_LENGTH'} : 0;
my $buffer = '';

open(FH, "dir_list_pre.html")or die "Sorry!! couldn't open"; 
while(<FH>) { 
    print $_; 
} 
#close; 

#########################################################################

for( ; ; ) {
   #printf "This loop will run forever.\n";
   sleep(5);
}
#########################################################################
if ($request_method eq "POST") {
    read(STDIN, $buffer, $size_of_form_information); 
    my @pairs = split(/&/, $buffer); 
    
    print <<"POST";
<h1>Form Submitted</h1>
<hr>
POST

    foreach my $pair (@pairs) {
        my ($name, $value) = split(/=/, $pair); 
        print uri_unescape($name) . " = " . uri_unescape($value) . "<br>";
    }

    print <<"POST_BOTTOM";
<hr>
<form action="/index.html" method="get">
    <button type="submit">Return to Home</button>
</form>

</div>        
POST_BOTTOM

} else {
    print <<"FORM";
        <h1>When's your birthday?</h1>
        <hr>
        <form action="/cgi-perl/form-submit.pl" method="POST">
            Name: <input type="text" name="nom" value="fred" size="40"><br>
            Birthday (in the form of mm/dd/yy): <input type="text" name="birthday" value="03/03/1827" size="40"><br>
            <input type="submit" value="Submit Form!">
            <input type="reset" value="Clear Form">
        </form>
        <hr>
FORM
}
#######################################################
open(FH2, "dir_list_post.html")or die "Sorry!! couldn't open"; 
while(<FH2>) { 
    print $_; 
} 
close;
exit(0);


