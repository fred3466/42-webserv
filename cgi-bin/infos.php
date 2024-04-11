#!/usr/bin/php
<?php



// Affiche toutes les informations, comme le ferait INFO_ALL
<<<<<<< HEAD
phpinfo();
$arr = get_defined_vars();
print_r($arr);
=======
// phpinfo();
>>>>>>> branch 'main' of git@github.com:fred3466/42-webserv.git

<<<<<<< HEAD
$a = print_r(var_dump($GLOBALS),1);
echo '<pre>';
echo htmlspecialchars($a);
echo '</pre>';
=======
 $arr = get_defined_vars();
 print_r($arr);
>>>>>>> branch 'main' of git@github.com:fred3466/42-webserv.git

$a = print_r(var_dump($GLOBALS),1);
echo '<pre>';
echo htmlspecialchars($a);
echo '</pre>';

// // Affiche uniquement le module d'information.
// // phpinfo(8) fournirait les mêmes informations.
// //phpinfo(INFO_MODULES);

<<<<<<< HEAD
=======
echo "Path: " . $_ENV['Path'];

>>>>>>> branch 'main' of git@github.com:fred3466/42-webserv.git
foreach ($_ENV as $k=>$v)
  echo $k . " => " . $v . "<br>";
?>

